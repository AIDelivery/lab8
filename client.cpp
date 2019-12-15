#include "func.h"
using namespace std;

int init_sleep(void);
void signal_handler(int);

int main(int argc, char* argv[]) {
    int msgq_id;

    /* Параметры сообщения */
    msg_type msg;

    /* Установка реакции на сигнал от сервера */
    signal(SIGINT, signal_handler);
    
    time_t timer = stoi(argv[1]);

    //key = ftok("./", 'a');

    /* Создание очереди с флагами
        0644 - чтение/запись для владельца и чтение для членов группы и прочих пользователей
        IPC_CREAT, IPC_EXCL - проверка на наличие очереди с тем же ключом
    */
    if ((msgq_id = msgget(KEY, IPC_CREAT)) == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    
    char b[] = "Hello";
    int i;
    
    for (i = 0; i < timer; i++) {
        printf("Посылка эхо-сигнала серверу: %d секунд из %d\n", i, (int) timer);
        /* Сообщение серверу о запуске клиента */
        send_message(msgq_id, &msg, 1, b, timer);
        
        sleep(1);
        
        /* Чтение сообщения от сервера */
        if (read_message(msgq_id, &msg, 2) != -1)
            break;
    }
    
    if (i == timer)
        printf("[Закрытие...]\n");
    else {
        printf("[Сигнал от сервера получен]\n");
        
        while (strcmp(msg.mtext, "exit()") != 0) {
            printf("Введите сообщение для сервера: ");
            scanf("%s", msg.mtext);
            send_message(msgq_id, &msg, 4, msg.mtext, timer);
            read_message(msgq_id, &msg, 3);
        }
    }
    
    /* Уничтожение очереди */
    msgctl(msgq_id, IPC_RMID, 0);
    exit(EXIT_SUCCESS);
}
