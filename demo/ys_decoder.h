//
// Created by chengye.ke on 2018/1/16.
//

#ifndef DEMO_YS_DECODER_H
#define DEMO_YS_DECODER_H
#include "decoder_base.h"

// 与总线通讯的协议包头

struct NtPkgHead {
    unsigned char bStartFlag; // 协议包起始标志 0xFF
    unsigned char bVer; // 版本号
    unsigned char bEncryptFlag; // 加密标志(如果不加密,则为0)
    unsigned char bFrag; // 是否有包分片(1:有包分片 0:无包分片)
    unsigned short wLen; // 总包长 (网络字节序)
    unsigned short wCmd; // 命令号 (网络字节序)
    unsigned short wSeq; // 包的序列号,业务使用
    unsigned short wCrc; // Crc16校验码
    unsigned int dwSID; // 会话ID
    unsigned short wTotal; // 有包分片时，分片总数
    unsigned short wCurSeq; // 有包分片时，分片序号
} __attribute__ ((packed));

#define RMQ_MAX_PACKET_SIZE (8000)

class ys_decoder : public decoder_base<ys_decoder> {
public:
    ys_decoder();
    ys_decoder (size_t bufsize_, int64_t maxmsgsize_);
    virtual ~ys_decoder ();

    //  i_decoder interface.
    virtual zmq::message_t *msg () { return &in_progress; }
    virtual NtPkgHead *head() { return &headbuf;}
    virtual uint16_t  get_cmd() { return cmd;}
private:

    int head_ready (unsigned char const*);
    int bodyready (unsigned char const*);

    int dyna_buffer_ready(unsigned char const*);
    int dyna_buffer_not_ready(unsigned char const*);

    NtPkgHead headbuf;
    unsigned char msg_flags;
    zmq::message_t in_progress;

    const int64_t maxmsgsize;
    uint16_t cmd;

    std::string dyna_buffer;
};


#endif //DEMO_YS_DECODER_H
