//
// Created by chengye.ke on 2018/1/16.
//

#include "ys_decoder.h"

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
    cmd = ntohs(head->wCmd);
    int len = ntohs(head->wLen);
    size_t bodysize = len - sizeof(NtPkgHead);
    in_progress.rebuild(len);
    next_step(in_progress.data(),bodysize,&ys_decoder::bodyready);
    return 0;
}

int ys_decoder::bodyready(unsigned char const *) {
    next_step (&headbuf, sizeof(headbuf), &ys_decoder::head_ready);
    return 1;
}