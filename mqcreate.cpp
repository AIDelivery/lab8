/*
 * ========================================================
 *
 *       Filename:  posix��Ϣ����  ������
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2016��6��12�� �賿3ʱ11��
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haiyang Tang (http://geektcp.com), admin@geektcp.com
 *          usage:  gcc -lrt -o mqcreate test_mqcreate.c && ./mqcreate
 * =======================================================
 */

#include <stdio.h>
#include <mqueue.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
using namespace std;

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define MAXSIZE     256
#define BUFFER      8192

struct msg_type_s {
        int len;
        char buf[MAXSIZE];
        char sender;
};

int main(int argc, char* argv[]) {
    time_t time_awaiting = stoi(argv[1]);
    mqd_t msgq_id;
    struct mq_attr msgq_attr;
    struct msg_type_s msg;
    unsigned int prio = 1;
    char queue_path[] = "/test";
    unsigned int recv_size = 8192, sender;

    msgq_id = mq_open(queue_path, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
    if(msgq_id < 0) {
            perror("mq_open");
            exit(1);
    }

    mq_getattr(msgq_id, &msgq_attr);
    msgq_attr.mq_curmsgs = 0;

    printf("        Queue: %d\n\
    stores at most %ld messages\n\
    large at most %ld bytes each\n\
    currently holds %ld messages\n",
    msgq_id, msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

    mq_setattr(msgq_id, &msgq_attr, NULL);
    
    msg.sender = 's';
    if(mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio) == -1)
    {
        perror("mq_send");
        exit(1);
    }
    
    time_t time_consumed;
    
    for (time_consumed = 0; time_consumed < time_awaiting; time_consumed++) {
        mq_receive(msgq_id, (char *)&msg, recv_size, &sender);
        
        if (msg.sender == 'c')
            break;
        
        msg.sender = 's';
        mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio);
        sleep(1);
    }
    
    if(time_consumed >= time_awaiting) {
        cout << "Client?" << endl;
        mq_unlink(queue_path);
        mq_close(msgq_id);
        exit(EXIT_FAILURE);
    }
    
    cout << endl << "MQ-Connection established" << endl;
    time_awaiting = msg.len;
    
    for (time_consumed = 0; time_consumed < time_awaiting; time_consumed++) {
        if (mq_receive(msgq_id, (char *)&msg, recv_size, &sender) == 0) {
            sleep(time_awaiting);
        }
        
        cout << msg.buf << " | " << time_awaiting << " | " << time_consumed << endl;
//         time_consumed = 0;
        
        if (strcmp(msg.buf, "unlink()") == 0)
            break;
    }
    
    if (time_consumed == time_awaiting)
        cout << "Client?" << endl;
    else
        cout << "Unlink command detected. Terminating..." << endl;
    
    
    mq_unlink(queue_path);
    mq_close(msgq_id);
    exit(EXIT_SUCCESS);
}
