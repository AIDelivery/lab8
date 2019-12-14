/*
 * ========================================================
 *
 *       Filename:  posix消息队列  消费者
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年6月12日 凌晨3时11分
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haiyang Tang (http://geektcp.com), admin@geektcp.com
 *
 * =======================================================
 */
#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_SIZE       256      //定义buf大小
#define BUF_SIZE      8192      //定义Msg大小

struct msg_type_s {
        int len;
        char buf[MAX_SIZE];
        char sender;
};


int main(int argc, char* argv[])
{
        
        mqd_t msgq_id;
        struct msg_type_s msg;
        unsigned int sender;
        struct mq_attr msgq_attr;
        unsigned int prio = 1;

        unsigned int recv_size = BUF_SIZE;
        const char file[] = "/test";
        
        /* mq_open() for opening an existing queue */
        msgq_id = mq_open(file, O_RDWR);
        if (msgq_id == (mqd_t) - 1) {
                perror("mq_open");
                exit(1);
        }

        /* getting the attributes from the queue        --  mq_getattr() */
        if (mq_getattr(msgq_id, &msgq_attr) == -1) {
                perror("mq_getattr");
                exit(1);
        }
        
        msgq_attr.mq_curmsgs = 0;
        mq_setattr(msgq_id, &msgq_attr, NULL);

        printf("        Queue: %d\n\
        stores at most %ld messages\n\
        large at most %ld bytes each\n\
        currently holds %ld messages\n",
        msgq_id, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);
        

        if (recv_size < msgq_attr.mq_msgsize) { recv_size = msgq_attr.mq_msgsize; }
        
        msg.sender = 'c';
        msg.len = 2;
        if(mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio) == -1)
        {
            perror("mq_send");
            exit(1);
        }
        
        int time_consumed;
        
        for (time_consumed = 0; time_consumed < msg.len; time_consumed++) {
            mq_receive(msgq_id, (char *)&msg, recv_size, &sender);
            
            if (msg.sender == 's')
                break;
            
            msg.sender = 'c';
            sleep(1);
            mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio);
        }
        
        if (time_consumed == msg.len) {
            cout << "Server?" << endl;
            mq_unlink(file);
            exit(EXIT_FAILURE);
        }
        
        cout << endl << "MQ-Connection established" << endl;
        
        while(strcmp(msg.buf, "exit()") != 0) {
            cout << "Message: ";
            cin >> msg.buf;
            mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio);
        }
        
        return 0;
}
