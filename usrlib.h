#include <unistd.h>
#include <mqueue.h>

class client_msg {
public:
    pid_t cl_pid;
    mqd_t cl_mqid;
};

class server_msg {
public:
    pid_t srv_pid;
    time_t send_time[1];
};
