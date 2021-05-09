#include <iostream>
#include <zmq.h>
#include <string>
#include <unistd.h>
#include <assert.h>

int main() {
    void* context_alpha = zmq_ctx_new();
    void* responder_alpha = zmq_socket(context_alpha, ZMQ_REP);
    int rc = zmq_bind(responder_alpha, "tcp://*:5556");
    assert(rc == 0);

    void *context_charlie = zmq_ctx_new();
    void *subscriber_charlie = zmq_socket(context_charlie, ZMQ_SUB);
    rc = zmq_connect(subscriber_charlie, "tcp://localhost:5557");
    assert (rc == 0);
    rc = zmq_setsockopt(subscriber_charlie, ZMQ_SUBSCRIBE, "", 0);
    assert (rc == 0);


    int status = 1;
    while (true) {
        int syms_num_alpha;
        zmq_recv(responder_alpha, &syms_num_alpha, sizeof(int), 0);
        std::cout << "Alpha sent " << syms_num_alpha << " symbols" << std::endl;
        zmq_send(responder_alpha, &status, sizeof(int), 0);
        int syms_num_charlie;
        zmq_recv(subscriber_charlie, &syms_num_charlie, sizeof(int), 0);
        std::cout << "Charlie recieved " << syms_num_charlie << " symbols" << std::endl;
    }
    return 0;
}
