#include "func.h"
using namespace std;

int main(int argc, char* argv[]) {
    int msgq_id = msgget(KEY, (0644 | IPC_CREAT));
    msg_type msg;
    int timer = stoi(argv[1]);

    if (msgq_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    
    int i;
    for (i = 0; i < timer; i++) {
        printf("[Ожидание отклика от клиента: %d секунд из %d]\n", i, timer);
        if (read_message(msgq_id, &msg, 1) != -1) break;

        sleep(1);
    }
    
    if (i == timer)
        printf("[Закрытие...]\n");
    else {
        printf("[Сигнал от клиента получен]\n");
        
        time_t client_timer = msg.timer;
        
        char b[] = "Hi!";
        send_message(msgq_id, &msg, 2, b, timer);
        
        for (i = 0; i < client_timer; i++) {
            printf("[Ожидание сообщения от клиента: %d секунд из %d]\n", i, (int) client_timer);
            if (read_message(msgq_id, &msg, 4) != -1) {
                i = 0;
                char b[] = "[Server: OK]";
                send_message(msgq_id, &msg, 3, b, timer);
                continue;
            }

            sleep(1);
        }
        
        if (i == client_timer)
            printf("[Клиент? Закрытие...]\n");
    }
    
    msgctl(msgq_id, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
