#pragma once

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <string>

#define KEY 0x044
#define BUF_SIZE 256

struct msg_type {
    long mtype;
    int timer;
    char mtext[BUF_SIZE];
    pid_t pid;
};

int read_message(int, msg_type*, long);
int send_message(int, msg_type*, long int, char*, int);

int read_message(int id_msg, msg_type* buf, long type) {
    //printf("Чтение сообщения...\n");
	buf->mtype = type;

    int res = msgrcv(id_msg, (msg_type*) buf, BUF_SIZE, type, IPC_NOWAIT);

    if (res == -1) return -1;
	else {
        printf("Тип: %ld Текст: %s Время ожидания: %d\n", buf->mtype, buf->mtext, buf->timer);
        return res;
    }
}

int send_message(int id_msg, msg_type* buf, long type, char* text, int timer) {
    buf->pid = getpid();
	buf->mtype = type;
	strcpy(buf->mtext, text);
    buf->timer = timer;

	if ((msgsnd(id_msg, (msg_type*)buf, BUF_SIZE, 0)) == -1) {
		perror("msgsnd");
		return 0;
	}

	return 1;
}
