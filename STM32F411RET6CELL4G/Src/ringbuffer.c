/**
************************************************************
* @file         ringbuffer.c
* @brief        Loop buffer processing
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include "ringBuffer.h"



int8_t ICACHE_FLASH_ATTR rbInit(rb_t* rb,uint8_t* src_buf,size_t size)
{
       if(NULL == src_buf)
    {
        return -1;
    }
      rb->rbBuff = src_buf;
      rb->rbCapacity = size;
   return rbCreate(rb); 
}
int8_t ICACHE_FLASH_ATTR rbCreate(rb_t* rb)
{
    if(NULL == rb)
    {
        return -1;
    }

    rb->rbHead = rb->rbBuff;
    rb->rbTail = rb->rbBuff;
    return 0;
}

int8_t ICACHE_FLASH_ATTR rbDelete(rb_t* rb)
{
    if(NULL == rb)
    {
        return -1;
    }

    rb->rbBuff = NULL;
    rb->rbHead = NULL;
    rb->rbTail = NULL;
    rb->rbCapacity = 0;
		return 0;
}

int32_t ICACHE_FLASH_ATTR rbCapacity(rb_t *rb)
{
    if(NULL == rb)
    {
        return -1;
    }

    return rb->rbCapacity;
}
//设置指针位置
int32_t ICACHE_FLASH_ATTR rbSetPtr(rb_t *rb,uint16_t offset)
{
     if(NULL == rb)
    {
        return -1;
    }
     rb->rbTail = rb->rbBuff+(offset%rb->rbCapacity);
    return (int32_t)(rb->rbTail);
}
//能读出来的数据长度
int32_t ICACHE_FLASH_ATTR rbCanRead(rb_t *rb)
{
    if(NULL == rb)
    {
        return -1;
    }

    if (rb->rbHead == rb->rbTail)
    {
        return 0;
    }

    if (rb->rbHead < rb->rbTail)
    {
        return rb->rbTail - rb->rbHead;
    }

    return rbCapacity(rb) - (rb->rbHead - rb->rbTail);
}
//能写的数据长度
int32_t ICACHE_FLASH_ATTR rbCanWrite(rb_t *rb)
{
    if(NULL == rb)
    {
        return -1;
    }

    return rbCapacity(rb) - rbCanRead(rb);
}
//读出数据
int32_t ICACHE_FLASH_ATTR rbRead(rb_t *rb, void *data, size_t count)
{
    int copySz = 0;

    if(NULL == rb)
    {
        return -1;
    }

    if(NULL == data)
    {
        return -1;
    }

    if (rb->rbHead < rb->rbTail)          //尾指针在头指针之前
    {
        copySz = min(count, rbCanRead(rb));
        memcpy(data, rb->rbHead, copySz);
        rb->rbHead += copySz;
        return copySz;
    }
    else                                 //尾指针在头指针之后
    {
        if (count < rbCapacity(rb)-(rb->rbHead - rb->rbBuff))//要读的数据长度小于缓存的数据长度
        {
            copySz = count;
            memcpy(data, rb->rbHead, copySz);
            rb->rbHead += copySz;
            return copySz;
        }
        else                                        //要读的数据长度大于缓存的数据长度
        {
            copySz = rbCapacity(rb) - (rb->rbHead - rb->rbBuff);
            memcpy(data, rb->rbHead, copySz);
            rb->rbHead = rb->rbBuff;
            copySz += rbRead(rb, (char*)data+copySz, count-copySz);//补上之前不够读取的数据
            return copySz;
        }
    }
}
//写入数据
int32_t ICACHE_FLASH_ATTR rbWrite(rb_t *rb, const void *data, size_t count)
{
    int tailAvailSz = 0;

    if((NULL == rb)||(NULL == data))
    {
        return -1;
    }

    if (count >= rbCanWrite(rb)) //写入数据长度大于能写的数据长度
    {
        return -2;
    }

    if (rb->rbHead <= rb->rbTail)
    {
        tailAvailSz = rbCapacity(rb) - (rb->rbTail - rb->rbBuff);
        if (count <= tailAvailSz)         //写进去之后尾指针不会过起点
        {
            memcpy(rb->rbTail, data, count);
            rb->rbTail += count;
            if (rb->rbTail == rb->rbBuff+rbCapacity(rb))
            {
                rb->rbTail = rb->rbBuff;
            }
            return count;
        }
        else                            //写进去之后尾指针会超过起点
        {
            memcpy(rb->rbTail, data, tailAvailSz);
            rb->rbTail = rb->rbBuff;   //重置尾指针

            return tailAvailSz + rbWrite(rb, (char*)data+tailAvailSz, count-tailAvailSz);//写入超过起点剩余的数据
        }
    }
    else
    {
        memcpy(rb->rbTail, data, count);
        rb->rbTail += count;
        return count;
    }
}
