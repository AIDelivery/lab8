#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_SIZE       256
#define BUF_SIZE      8192

struct msg_type {
    time_t time;
    string text;
    pid_t sender_pid;
};

int send_message (mqd_t msgq_id, string str_to_send) {
    msg_type msg;
    timespec ts; ts.tv_sec = 1;
    
    msg.sender_pid = getpid();
    msg.text = str_to_send;
    msg.time = 0;
    
    int res = mq_send(msgq_id, (char*) &msg, sizeof(struct msg_type), 1);
    
    if (res < 0) {
        return 0;
    }
    
    cout << "[Server send] [" << msg.text << "] [" << msg.sender_pid << "]" << endl;
    
    return 1;
}

int read_message (mqd_t msgq_id, msg_type &msg_to_receive, unsigned int &sender) {
    timespec ts; ts.tv_sec = 1;
    int res = mq_receive(msgq_id, (char*) &msg_to_receive, sizeof(struct msg_type), &sender);
    
    if (res < 0)
        return 0;
    
    cout << "[Server read] [" << msg_to_receive.text << "] [" << msg_to_receive.sender_pid << "]" << endl;
    
    return 1;
}


int main(int argc, char* argv[])
{

        mqd_t msgq_id;
        struct msg_type msg;
        unsigned int sender;
        struct mq_attr msgq_attr;
        unsigned int prio = 1;

        unsigned int recv_size = BUF_SIZE;
        const char file[] = "/test1";

        /* mq_open() for opening an existing queue */
        msgq_id = mq_open(file, O_RDWR | O_EXCL);
        if (msgq_id < 0) {
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

        time_t time_awaiting = stoi(argv[1]);
        if(!send_message(msgq_id, "cc"))
        {
            perror("mq_send");
            exit(1);
        }

        int time_consumed;

        for (time_consumed = 0; time_consumed < time_awaiting; time_consumed++) {
            if (read_message(msgq_id, msg, sender))
                break;

            sleep(1);
            send_message(msgq_id, "cc");
        }

        if (time_consumed == time_awaiting) {
            cout << "[Server?]" << endl;
            mq_unlink(file);
            mq_close(msgq_id);
            exit(EXIT_FAILURE);
        }

        cout << endl << "[MQ-Connection established]" << endl;
        mq_unlink(file);
        mq_close(msgq_id);
        exit(EXIT_SUCCESS);

//         while(strcmp(msg.buf, "exit()") != 0) {
//             cout << "Message: ";
//             cin >> msg.buf;
//             mq_send(msgq_id, (char*)&msg, sizeof(struct msg_type_s), prio);
//         }

        return 0;
}
