// #include <fcntl.h>
#include <mqueue.h>
#include <unistd.h>
#include <iostream>

#include <errno.h>
#include <string.h>


// #include "usrlib.h"
// #include "MQ.h"
using namespace std;

char mqname[] = "/myqueue";

int main(int argc, char* argv[]) {
    time_t time_awaiting = stoi(argv[1]);
    mqd_t queue = mq_open(mqname, O_RDWR | O_CREAT, 0664, 0);
    char ech[] = "[Server echo]"; char buffer[100];
    size_t sz = sizeof(ech);
    unsigned int prio = 1;
    timespec t; t.tv_nsec = 1; t.tv_sec = 1;
    int temp;
    
    cout << mq_send(queue, ech, sz, 1);
    
    /*
    MQ* queue = new MQ("cool_af", MQ::EndpointType::Server);
    
    string buff;
    
    queue->listen([queue](std::string message){
        std::cout << "Received: " << message << std::endl;
        queue->sendMessage("[Hello from server]");
        
        if(strcmp(message.c_str(), "exit()") == 0) exit(0);
    });
    
    */
    
    cout << "[Server init. Waiting " << time_awaiting << " seconds for client...]" << endl;
    for (int i = 0; i < time_awaiting; i++)
        temp = mq_timedreceive(queue, buffer, 100, &prio, &t);
    
//     buff = queue->readMessage();
//     queue->sendMessage(buff);
    
//     while(strcmp(buff.c_str(), "exit()") != 0) {
//         buff = queue->readMessage();
//         cout << "[server] " << buff << endl;
//         queue->sendMessage("1");
//     }
    cout << temp << " " << strerror(errno) << endl;
    // delete queue;
    mq_close(queue);
    return 0;
    /*
    time_t time_to_wait_sec = 3;
    mqd_t queue_to_read;
    char message[6];
    struct mq_attr attr;
    
    
    queue_to_read = mq_open(mqname, O_RDONLY | O_CREAT, 0777, &attr);
    cout << "[server]Opening queue... " << strerror(errno) << endl << "Waiting for " << time_to_wait_sec << " seconds" << endl;
    
    
    for(int i = 0; i < time_to_wait_sec; i++) {
        cout << '.';
        if(mq_receive(queue_to_read, message, sizeof(message), NULL) != -1)
            break;
    }
    
        cout << message;
    
    mq_unlink(mqname);
    */
}
