#include <iostream>
#include <mqueue.h>
#include <errno.h>      // errno
#include <string.h>      // streeror
#include <unistd.h>     // usleep
#include <string>

// #include "MQ.h"
using namespace std;

char mqname[] = "/myqueue";

int main(int argc, char* argv[]) {
    mqd_t queue = mq_open(mqname, O_RDWR | O_CREAT, 0664, 0);
    char buff[] = "[Client echo]"; char buffer[100];
    size_t sz = sizeof(buff);
    unsigned int prio = 1;
    
    cout << mq_receive(queue, buffer, 100, &prio);
    /*
    MQ* queue = new MQ("cool_af", MQ::EndpointType::Client);
    string buff;
    char *buffer = new char[100];
    unsigned int prio = 1; int hj;
    
    cout << "[Client init. Server echo waiting...";
    
    for (int i = 0; i < 5; i++) {
        queue->sendMessage("[Echo]");
        cout << '.';timespec f; f.tv_sec =1; f.tv_nsec = 100000;
        hj = mq_timedreceive(queue->get_mqd1(), buffer, 100, &prio, &f);
    }
    
    cout << "END" << hj << strerror(errno);
    
    while(strcmp(buff.c_str(), "exit()") != 0) {
        cin >> buff;
        queue->sendMessage("2");
        cout << "[send] " << buff << endl;
        cout << "[ans] " << queue->readMessage() << endl;
    }
    
    delete queue;
    cout << "Client terminating..." << endl;
    return 0;
    
    */
    /*
    time_t time_to_wait_sec = 1;
    mqd_t queue_to_read;
    char message[] = "HERE!";
    bool client_is_enabled = false;
    
    
    queue_to_read = mq_open(mqname, O_WRONLY | O_EXCL, 0);
    cout << "[client]Opening queue... " << strerror(errno) << endl << "Waiting for " << time_to_wait_sec << " seconds" << endl;
    
    usleep(1200000);
    
    mq_send(queue_to_read, message, sizeof(message), 1);
    
    mq_close(queue_to_read);
    return 0;
    
    */
}
