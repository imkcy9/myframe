//
// Created by chengye.ke on 2018/1/16.
//

#include "ys_decoder.h"
#include <netinet/in.h>

ys_decoder::ys_decoder()
:ys_decoder(8196,8196)
{

}

ys_decoder::ys_decoder(size_t bufsize_, int64_t maxmsgsize_)
: maxmsgsize(maxmsgsize_)
, cmd(0)
{
    next_step (&headbuf, sizeof(headbuf), &ys_decoder::head_ready);
}

ys_decoder::~ys_decoder() {

}

int ys_decoder::head_ready(unsigned char const *) {
    NtPkgHead* head = &headbuf;
    if(head->bStartFlag != 255) {
        return -1;
    }
    cmd = ntohs(head->wCmd);
    int len = ntohs(head->wLen);
    size_t bodysize = len - sizeof(NtPkgHead);
    if(1 == head->bFrag) {
        uint16_t wcurseq = ntohs(head->wCurSeq);
        uint16_t  wtotal = ntohs(head->wTotal);
        in_progress.rebuild(bodysize);
        if(wcurseq == wtotal - 1) {
            next_step(in_progress.data(),bodysize,&ys_decoder::dyna_buffer_ready);
        } else {
            next_step(in_progress.data(),bodysize,&ys_decoder::dyna_buffer_not_ready);
        }
    } else {
        in_progress.rebuild(bodysize);
        next_step(in_progress.data(),bodysize,&ys_decoder::bodyready);
    }



    return 0;
}

int ys_decoder::bodyready(unsigned char const *) {
    next_step (&headbuf, sizeof(headbuf), &ys_decoder::head_ready);
    return 1;
}

int ys_decoder::dyna_buffer_ready(unsigned char const*) {
    dyna_buffer.append((char*)in_progress.data(), in_progress.size());
    in_progress.rebuild((char*)dyna_buffer.c_str(), dyna_buffer.size());
    next_step (&headbuf, sizeof(headbuf), &ys_decoder::head_ready);
    dyna_buffer.clear();
    return 1;
}

int ys_decoder::dyna_buffer_not_ready(unsigned char const *) {
    dyna_buffer.append((char*)in_progress.data(), in_progress.size());
    next_step (&headbuf, sizeof(headbuf), &ys_decoder::head_ready);
    return 0;
}