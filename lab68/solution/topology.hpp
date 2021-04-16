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
}

//yet T is key type
//if T will become a complex class replace "->key" with the "->key->k" for key
//for T must be defined following operations:
// ==
template <class T>
class topology_t {
private:
    void add1(btNode_t<T>* ptr, T & key) {
        if (ptr == nullptr) {
            ptr = new btNode_t<T>(key);
        } else if (key < ptr->key) {
            add1(ptr->left, key);
        } else if (key > ptr->key) {
            add1(ptr->right, key);
        } else {
            std::cout << "Node with key " << key << " already exists\n";
        }
    }

    btNode_t<T>* find1(btNode_t<T>* ptr, T & key) {
        btNode_t<T>* ptr1 = nullptr;

        if (ptr->key == key) {
            return ptr;
        } else {
            if (ptr->left != nullptr) {
                ptr1 = find1(ptr->left, key);
            }

            if (ptr1 != nullptr) {
                return ptr1;
            }

            if (ptr->right != nullptr) {
                ptr1 = find1(ptr->right, key);
            }

            return ptr1;
        }
    }

public:
    btNode_t<T>* root;
    topology_t() {}
    ~topology_t() {}

    btNode_t<T>* find(T & key) {
        return find1(root, key);
    }

    void add(T & key) {
        add1(root, key);
    }

}
