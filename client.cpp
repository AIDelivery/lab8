#include "func.h"
using namespace std;


int main(int argc, char* argv[]) {
    int msgq_id;
    msg_type msg;

    time_t timer = stoi(argv[1]);
    if ((msgq_id = msgget(KEY, IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    char b[] = "Hello";
    int i;

    for (i = 0; i < timer; i++) {
        printf("Посылка эхо-сигнала серверу: %d секунд из %d\n", i, (int) timer);
        // Сообщение серверу о запуске клиента
        if(!send_message(msgq_id, &msg, 1, b, timer)) {
            msgctl(msgq_id, IPC_RMID, 0);
            exit(EXIT_FAILURE);
        }

        sleep(1);

        // Чтение сообщения от сервера
        if (read_message(msgq_id, &msg, 2) != -1)
            break;
    }

    if (i == timer)
        printf("[Закрытие...]\n");
    else {
        printf("[Сигнал от сервера получен]\n");
        msg_type msg_to_send;

        while (strcmp(msg_to_send.mtext, "exit()") != 0) {
            printf("Введите сообщение для сервера: ");
            scanf("%s", msg_to_send.mtext);
            timer = atoi(msg_to_send.mtext);
            if (!send_message(msgq_id, &msg_to_send, 4, msg_to_send.mtext, timer))
                break;
            read_message(msgq_id, &msg, 3);
        }
    }

    msgctl(msgq_id, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
