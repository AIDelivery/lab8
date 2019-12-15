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

struct msg_type {
    time_t time;
    string text;
    pid_t sender_pid;
};

int send_message (mqd_t msgq_id, string str_to_send) {
    msg_type msg;
    timespec ts; ts.tv_sec = 20000000;
    
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
//     timespec ts; ts.tv_nsec = 20000000;
    int res = mq_receive(msgq_id, (char*) &msg_to_receive, sizeof(struct msg_type), &sender);
    
    if (res < 0)
        return 0;
    
    cout << "[Server read] [" << msg_to_receive.text << "] [" << msg_to_receive.sender_pid << "]" << endl;
    
    return 1;
}

int main(int argc, char* argv[]) {
    time_t time_awaiting = stoi(argv[1]);
    mqd_t msgq_id;
    struct mq_attr msgq_attr;
    unsigned int prio = 1;
    char queue_path[] = "/test1";
    unsigned int recv_size = 8192;

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

    if (!send_message(msgq_id, "dd")) {
        cout << "[Unexpected error]" << endl;
        mq_unlink(queue_path);
        mq_close(msgq_id);
        exit(EXIT_FAILURE);
    }

    time_t time_consumed;
    msg_type msg;
    unsigned int sender;

    for (time_consumed = 0; time_consumed < time_awaiting; time_consumed++) {
        if (read_message(msgq_id, msg, sender))
            break;
        mq_getattr(msgq_id, &msgq_attr);
        cout << "curmes: " << msgq_attr.mq_curmsgs << endl;
        sleep(1);
        send_message(msgq_id, "dd");
    }

    if(time_consumed >= time_awaiting) {
        cout << "[Client?]" << endl;
        mq_unlink(queue_path);
        mq_close(msgq_id);
        exit(EXIT_FAILURE);
    }

    cout << endl << "[MQ-Connection established]" << endl;
    time_awaiting = msg.time;
    
    mq_unlink(queue_path);
    mq_close(msgq_id);
    exit(EXIT_SUCCESS);

    for (time_consumed = 0; time_consumed < time_awaiting; time_consumed++) {
        if (mq_receive(msgq_id, (char *)&msg, recv_size, &sender) == 0) {
            sleep(time_awaiting);
        }

//         cout << msg.buf << " | " << time_awaiting << " | " << time_consumed << endl;
//         time_consumed = 0;

//         if (strcmp(msg.buf, "unlink()") == 0)
//             break;
    }

    if (time_consumed == time_awaiting)
        cout << "Client?" << endl;
    else
        cout << "Unlink command detected. Terminating..." << endl;


    mq_unlink(queue_path);
    mq_close(msgq_id);
    exit(EXIT_SUCCESS);
}
