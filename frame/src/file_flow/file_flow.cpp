/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   file_flow.cpp
 * Author: chengyeke
 * 
 * Created on 2017年8月21日, 下午4:03
 */

#include <iostream>

#include "file_flow.h"

#define BLOCK_SIZE 100

struct tflow_id {
    DWORD dwSize;

    void change_endian() {
        CHANGE_ENDIAN(dwSize);
    }
};

file_flow::file_flow(const char* file_name_, const char* file_path, bool reuse_) {
    count = 0;
    FPOS_SET(content_size, 0);
    fp_id_file = NULL;
    fp_content_file = NULL;
    comm_phase_no = 0;
    last_read_id = -1;
    FPOS_SET(last_read_offset, 0);
    open_file(file_name_, file_path, reuse_);
    INIT_CRITICAL_VAR(critical_var);

}

file_flow::file_flow(int flow_id_, const char* file_path_, bool reuse_) {
    char flow_name[20];
    sprintf(flow_name, "%08x", flow_id_);
    new (this)file_flow(flow_name, file_path_, reuse_);
}

file_flow::~file_flow() {
    DELETE_CRITICAL_VAR(critical_var);
    close_file();
}

int file_flow::get_count() {
    return count;
}

const int* file_flow::get_count_ptr() {
    return &count;
}

fpos_t file_flow::get_content_size() {
    return content_size;
}

int file_flow::append(void* object_, int length_) {
    ENTER_CRITICAL(critical_var);
    fsetpos(fp_content_file, &content_size);
    tflow_id flowid;
    flowid.dwSize = length_;
    flowid.change_endian();

    if (fwrite(&flowid, sizeof (flowid), 1, fp_content_file) != 1)
        assert(false && "Can not write content file for CFlow");

    if (fwrite(object_, 1, length_, fp_content_file) != (size_t) length_)
        assert(false && "Can not write content file for CFlow");
    fflush(fp_content_file);

    FPOS_SET(content_size, FPOS_GET(content_size) + length_ + sizeof (flowid));
    count++;

    if (count % BLOCK_SIZE == 0) {
        block_offset.push_back(content_size);
        fseek(fp_id_file, 0, SEEK_END);

        UF_INT8 dwOffset = FPOS_GET(content_size);
        CHANGE_ENDIAN(dwOffset);
        if (fwrite(&dwOffset, sizeof (dwOffset), 1, fp_id_file) != 1)
            assert(false && "Can not write id file for CFlow");

        fflush(fp_id_file);
    }
    LEAVE_CRITICAL(critical_var);
    return count - 1;
}

fpos_t file_flow::get_offset(int id_) {
    fpos_t offset;
    FPOS_SET(offset, 0);
    if (last_read_id + 1 == id_) {
        offset = last_read_offset;
    } else {
        tflow_id flowid;
        int i = id_ / BLOCK_SIZE;
        offset = block_offset[i];
        for (i *= BLOCK_SIZE; i < id_; i++) {
            fsetpos(fp_content_file, &offset);
            if (fread(&flowid, sizeof (flowid), 1, fp_content_file) != 1)
                assert(false && "Can not read content file for CFlow");
            flowid.change_endian();
            FPOS_SET(offset, FPOS_GET(offset) + flowid.dwSize + sizeof (flowid));
        }
    }
    return offset;
}

int file_flow::get(int id_, void* object_, int length_) {
    ENTER_CRITICAL(critical_var);
    fpos_t offset = get_offset(id_);
    tflow_id flowid;
    fsetpos(fp_content_file,&offset);
    if(fread(&flowid,sizeof(flowid),1,fp_content_file) != 1)
        assert(false&&("Can not read content file for CFlow"));
    flowid.change_endian();
    
    if(flowid.dwSize > length_)
        assert(false&&("Can not read content file for CFlow"));
    
    if(fread(object_,1,flowid.dwSize, fp_content_file) != (size_t)flowid.dwSize)
        assert(false&&("Can not read content file for CFlow"));
    
    last_read_id = id_;
    FPOS_SET(last_read_offset,FPOS_GET(offset)+ flowid.dwSize + sizeof(flowid));
    LEAVE_CRITICAL(critical_var);
    
    return flowid.dwSize;
}

int file_flow::get(int id_, zmq::message_t* msg_) {
    ENTER_CRITICAL(critical_var);
    fpos_t offset = get_offset(id_);
    tflow_id flowid;
    fsetpos(fp_content_file,&offset);
    if(fread(&flowid,sizeof(flowid),1,fp_content_file) != 1)
        assert(false&&("Can not read content file for CFlow"));
    flowid.change_endian();
    
    msg_->rebuild(flowid.dwSize);
    
    if(fread(msg_->data(),1,flowid.dwSize, fp_content_file) != (size_t)flowid.dwSize)
        assert(false&&("Can not read content file for CFlow"));
    
    last_read_id = id_;
    FPOS_SET(last_read_offset,FPOS_GET(offset)+ flowid.dwSize + sizeof(flowid));
    LEAVE_CRITICAL(critical_var);
    
    return flowid.dwSize;
}


void file_flow::open_file(const char* flow_name_, const char* path_, bool reuse_) {
    ASSERT(path_);
    close_file();
    char szIdFilename[512];
    char szContentFilename[512];
    sprintf(szIdFilename,"%s%s.id",path_,flow_name_);
    sprintf(szContentFilename,"%s%s.con",path_,flow_name_);
    
    count = 0;
    FPOS_SET(content_size, 0);
    block_offset.clear();
    
    fpos_t firstOffset;
    FPOS_SET(firstOffset,0);
    block_offset.push_back(firstOffset);
    
    fp_id_file=mfopen(szIdFilename,"a+b");
    fp_content_file=mfopen(szContentFilename,"a+b");
    if(!fp_content_file || !fp_content_file) {
        close_file();
        ASSERT(false&&"can not open CFlow file");
    }
    
    if(!reuse_) {
        if(!init_file()) {
            close_file();
            ASSERT(false&&"can not init CFlow file");
        }
        return;
    }
    
    struct stat statbuf;
    int result = stat(szContentFilename,&statbuf);
    if(result != 0)
        ASSERT(false);
    FPOS_SET(content_size,statbuf.st_size);
    
    fpos_t size;
    FPOS_SET(size,0);
    fseek(fp_id_file,0,SEEK_SET);
    if(fread(&comm_phase_no,sizeof(comm_phase_no),1,fp_id_file) != 1) {
        if(!init_file()) {
            close_file();
            assert(false&&"can not init CFlow file");
        }
        return;
    }
    CHANGE_ENDIAN(comm_phase_no);
    for(;;) {
        UF_INT8 dwOffset = 0;
        if(fread(&dwOffset,sizeof(dwOffset),1,fp_id_file) != 1) break;
        CHANGE_ENDIAN(dwOffset);
        fpos_t fpOffset;
        FPOS_SET(fpOffset,dwOffset);
        
        block_offset.push_back(fpOffset);
        size=fpOffset;
        count+=BLOCK_SIZE;
    }
    tflow_id flowid;
    int i ;
    for(i = 0; ; i++) {
        if (fsetpos(fp_content_file,&size) != 0) {
            ASSERT(false);
        }
        if (fread(&flowid,sizeof(flowid),1,fp_content_file) != 1) {
            break;
        }
        
        flowid.change_endian();
        FPOS_SET(size,FPOS_GET(size)+flowid.dwSize + sizeof(flowid));
    }
    if(FPOS_GET(size) != FPOS_GET(content_size) || i >=BLOCK_SIZE)
        ASSERT(false);
    
    count+=i;
}

bool file_flow::init_file() {
    content_size = get_offset(count);
    int nIdCount = count/BLOCK_SIZE;
    block_offset.resize(nIdCount+1);
    
    ftruncate(fileno(fp_id_file),nIdCount);
    ftruncate(fileno(fp_content_file),FPOS_GET(content_size));
    
    if (fseek(fp_id_file,0,SEEK_SET) != 0) {
        return false;
    }
    WORD nCommPhaseNo = comm_phase_no;
    CHANGE_ENDIAN(nCommPhaseNo);
    if (fwrite(&nCommPhaseNo,sizeof(nCommPhaseNo),1,fp_id_file) != 1) {
        return false;
    }
    fflush(fp_id_file);
    return true;
}

bool file_flow::truncated_file() {
    if(count == 0)
        return init_file();
    content_size = get_offset(count);
    int nIdCount = count/BLOCK_SIZE;
    block_offset.resize(nIdCount+1);
    
    ftruncate(fileno(fp_id_file),nIdCount*sizeof(fpos_t)+sizeof(WORD));
    ftruncate(fileno(fp_content_file),FPOS_GET(content_size));
    return true;
}

void file_flow::close_file() {
    if (fp_id_file != NULL) {
        fclose(fp_id_file);
        fp_id_file = NULL;
    }
    if (fp_content_file != NULL) {
        fclose(fp_content_file);
        fp_content_file = NULL;
    }
}

WORD file_flow::get_comm_phase_no() {
    return comm_phase_no;
}

void file_flow::set_comm_phase_no(WORD comm_phase_no_) {
    if (comm_phase_no != comm_phase_no_) {
        comm_phase_no = comm_phase_no_;
        count = 0;
        init_file();
    }

}

bool file_flow::truncate(int count_) {
    if (count <= count_) {
        return false;
    }
    count = count_;
    return truncated_file();
}
















