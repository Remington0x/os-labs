#include <zmq.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

int main() {
    void* context_charlie = zmq_ctx_new();
    void* requester_charlie = zmq_socket(context_charlie, ZMQ_REQ);
    zmq_connect(requester_charlie, "tcp://127.0.0.1:5555");

    void* context_bravo = zmq_ctx_new();
    void* requester_bravo = zmq_socket(context_bravo, ZMQ_REQ);
    zmq_connect(requester_bravo, "tcp://127.0.0.1:5556");

    std::string s;
    int status = 0;
    int syms_num;
    while (true) {
        std::getline(std::cin, s);
        if (s == "exit") break;
        std::cout << "Sending to Charlie \"" << s << "\": ";
        zmq_send(requester_charlie, s.c_str(), 255, 0);
        zmq_recv(requester_charlie, &status, sizeof(int), 0);
        std::cout << (status == 1 ? "OK" : "Something went wrong") << std::endl;
        std::cout << "Sending data to Bravo: ";
        syms_num = s.length();
        zmq_send(requester_bravo, &syms_num, sizeof(int), 0);
        zmq_recv(requester_bravo, &status, sizeof(int), 0);
        std::cout << (status == 1 ? "OK" : "Something went wrong") << std::endl;
    }
    zmq_close(requester_charlie);
    zmq_ctx_destroy(context_charlie);
    return 0;
}
