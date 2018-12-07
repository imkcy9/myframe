/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   dumptool.cpp
 * Author: kecy
 * 
 * Created on 2018年12月3日, 下午8:23
 */

#include "dumptool.h"
#include <stdio.h>

void kt::hexdump(char *buf, int len, int addr)
{
    int i, j, k;
    char binstr[80];

    for (i = 0;i<len;i++) 
    {
        if (0 == (i % 16)) 
        {
            sprintf(binstr, "%08x -", i + addr);
            sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
        }
        else if (15 == (i % 16)) 
        {
            sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
            sprintf(binstr, "%s  ", binstr);
            for (j = i - 15;j <= i;j++) 
            {
                sprintf(binstr, "%s%c", binstr, ('!'<buf[j] && buf[j] <= '~') ? buf[j] : '.');
            }
            printf("%s\n", binstr);
        }
        else 
        {
            sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
        }
    }
    if (0 != (i % 16)) 
    {
        k = 16 - (i % 16);
        for (j = 0;j<k;j++) 
        {
            sprintf(binstr, "%s   ", binstr);
        }
        sprintf(binstr, "%s  ", binstr);
        k = 16 - k;
        for (j = i - k;j<i;j++) 
        {
            sprintf(binstr, "%s%c", binstr, ('!'<buf[j] && buf[j] <= '~') ? buf[j] : '.');
        }
        printf("%s\n", binstr);
    }
}

