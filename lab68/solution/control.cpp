#include <unistd.h>
#include <vector>

#include "topology.hpp"
#include "zmq_std.hpp"

using node_id_t = long long;
const char[] EXEC_NODE_NAME = "calc.out";

int main() {
    int rc;
    topology_t<node_id_t> control_node;
    control_node.add(-1);

    std::pair< std::pair<void*, void*>, std::pair<void*, void*> > childs;

    std::string s;
    node_id_t id;

    while (std::cin >> s >> id) {
        if (s == "create") {
            btNode_t<node_id_t>* node = control_node.look_up(id);
            if (node != nullptr) {
                std::cout << "Error: node with id <" << id << "> already exists\n";
                continue;
            } else {
                node_id_t parent_id = (control_node.find_parent(id))->key;

                if (parent_id == -1) {
                    void* new_context = NULL;
                    void* new_socket = NULL;
                    zmq::init_pair_socket(new_context, new_socket);
                    rc = zmq_bind(new_socket, ("tcp://*:" + std::to_string(BASE_PORT + id)).c_str());
                    assert(rc == 0);

                    int fork_id = fork();
                    if (fork_id == 0) {
                        rc = execl(EXEC_NODE_NAME, EXEC_NODE_NAME, std::to_string(id).c_str(), NULL);
                        assert(rc != -1);
                        return 0;
                    } else {
                        bool ok = true;
                        node_token_t reply_info({fail, id, id});
                        ok = zmq::recieve_msg_wait(reply_info, new_socket);

                        node_token_t* token = new node_token_t({ping, id, id});
                        node_token_t reply({fail, id, id});
                        ok = zmq::send_recieve_wait(token, reply, new_socket);
                        if (ok && (reply.action == success)) {
                            control_node.add(id);
                            if (key < control_node.root->key) {
                                childs.first = std::make_pair(new_context, new_socket);
                            } else {
                                childs.second = std::make_pair(new_context, new_socket);
                            }
                            std::cout << "OK: " << reply_info.id << std::endl;
                        } else {
                            rc = zmq_close(new_socket);
                            assert(rc == 0);
                            rc = zmq_ctx_term(new_context);
                            assert(rc == 0);
                        }
                    }
                } else {
                    node_token_t* token = new node_token_t({create, parent_id, id});
                    node_token_t reply({fail, id, id});
                    if (id < parent_id) { //to the childs.first
                        if (zmq::send_recieve_wait(token, reply, childs.first.second) && reply.action == success) {
                            std::cout << "OK: " << reply.id << std::endl;
                            control_node.add(id);
                        } else {
                            std::cout << "Error: parent is unavailable\n";
                        }
                    } else {    //to the childs.second
                        if (zmq::send_recieve_wait(token, reply, childs.second.second) && reply.action == success) {
                            std::cout << "OK: " << reply.id << std::endl;
                            control_node.add(id);
                        } else {
                            std::cout << "Error: parent is unavailable\n";
                        }
                    }

                }
            }
        } else if (s == "remove") {
            if (control_node.look_up(id) != nullptr) {
                node_id_t parent_id = (control_node.find_parent(id))->key;
                node_token_t* token = new node_token_t({destroy, id, id});
                node_token_t reply({fail, id, id});
                bool ok;
                if (id < parent_id) {
                    ok = zmq::send_recieve_wait(token, reply, childs.first.second);
                } else {
                    ok = zmq::send_recieve_wait(token, reply, childs.second.second);
                }
            }
        }
    }























    return 0;
}
