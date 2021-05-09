#include <iostream>
#include <zmq.h>
#include <string>
#include <unistd.h>
#include <assert.h>

int str_length(char* str) {
    int i;
    for (i = 0; str[i] != '\n' && str[i] != '\0' && i < 256; ++i) {}
    return i;
}

int main() {
    void* context = zmq_ctx_new();
    void* responder = zmq_socket(context, ZMQ_REP);
    int rc = zmq_bind(responder, "tcp://*:5555");
    assert(rc == 0);
    //pub-sub for Bravo
    void *context_bravo = zmq_ctx_new();
    void *publisher_bravo = zmq_socket(context_bravo, ZMQ_PUB);
    rc = zmq_bind(publisher_bravo, "tcp://*:5557");
    assert(rc == 0);

    int status = 1;
    while (true) {
        char s[255];
        zmq_recv(responder, &s, 255, 0);
        std::cout << "Recieved: " << s << std::endl;
        zmq_send(responder, &status, sizeof(int), 0);
        int syms_num = str_length(s);
        //std::cout << "str_length = " << syms_num << std::endl;
        zmq_send(publisher_bravo, &syms_num, sizeof(int), 0);
    }
    zmq_close(publisher_bravo);
    zmq_ctx_destroy(context_bravo);
    return 0;
}
