#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <iostream>
#include <memory>

template <class T>
class btNode_t {
public:
    T key;
    btNode_t<T>* left;
    btNode_t<T>* right;
    btNode_t(T k) : key(k), left(nullptr), right(nullptr) {};
    ~btNode_t() {};
};

//yet T is key type
//if T will become a complex class replace "->key" with the "->key->k" for key
//for T must be defined following operations:
// ==
template <class T>
class topology_t {
private:
    void add_rec(btNode_t<T>** ptr2, T & key) {
        if ((*ptr2) == nullptr) {
            (*ptr2) = new btNode_t<T>(key);
        } else if (key < (*ptr2)->key) {
            add_rec(&(*ptr2)->left, key);
        } else if (key > (*ptr2)->key) {
            add_rec(&(*ptr2)->right, key);
        } else {
            std::cout << "Node with key " << key << " already exists\n";
        }
    }

    btNode_t<T>* find_rec(btNode_t<T>* ptr, T & key) {
        btNode_t<T>* ptr1 = nullptr;

        if (ptr->key == key) {
            return ptr;
        } else {
            if (ptr->left != nullptr) {
                ptr1 = find_rec(ptr->left, key);
            }

            if (ptr1 != nullptr) {
                return ptr1;
            }

            if (ptr->right != nullptr) {
                ptr1 = find_rec(ptr->right, key);
            }

            return ptr1;
        }
    }

    void rm_node(btNode_t<T>** ptr2) {
        btNode_t<T>* ptr;
        //there are three cases
        //1. no children
        //2. one child
        //3. both children

        //1.
        if ((*ptr2) != nullptr) {
            if ((*ptr2)->left == nullptr) {
                if ((*ptr2)->right == nullptr) {
                    //1.
                    ptr = *ptr2;
                    *ptr2 = nullptr;
                    delete ptr;
                } else {
                    //2. there is no left child
                    ptr = *ptr2;
                    *ptr2 = (*ptr2)->right;
                    delete ptr;
                }
            } else
            if ((*ptr2)->right == nullptr) {
                //2. there is no right child
                ptr = *ptr2;
                *ptr2 = (*ptr2)->left;
                delete ptr;
            } else {
                //3.
                ptr = (*ptr2)->left;
                while (ptr->right->right != nullptr) {
                    ptr = ptr->right;
                }
                (*ptr2)->key = ptr->right->key;
                auto buff = ptr->right;
                ptr->right = ptr->right->left;
                delete buff;
            }
        }
    }

    void rm_rec(btNode_t<T>** ptr2, T & key) {
        if ((*ptr2) != nullptr) {
            if ((*ptr2)->key == key) {
                rm_node(ptr2);
            } else if (key < (*ptr2)->key) {
                rm_rec(&(*ptr2)->left, key);
            } else if (key > (*ptr2)->key) {
                rm_rec(&(*ptr2)->right, key);
            }
        }
    }

    void print_rec(btNode_t<T>* node, int tab) {
        if (node->right != nullptr) {
            print_rec(node->right, tab + 1);
        }
        for (int i = 0; i < tab; ++i) {
            std::cout << "  ";
        }
        std::cout << node->key << std::endl;
        if (node->left != nullptr) {
            print_rec(node->left, tab + 1);
        }
    }

public:
    btNode_t<T>* root;
    topology_t() : root(nullptr) {}
    ~topology_t() {}

    btNode_t<T>* find(T & key) {
        return find_rec(root, key);
    }

    void add(T & key) {
        add_rec(&root, key);
        std::cout << "root now: " << root << std::endl;
    }

    void rm(T & key) {
        rm_rec(&root, key);
    }

    void print() {
        print_rec(root, 0);
    }

};

#endif
