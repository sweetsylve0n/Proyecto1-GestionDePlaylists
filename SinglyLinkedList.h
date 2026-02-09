#pragma once
#include <functional>
#include <iostream>
#include <utility>   // std::move
#include <stdexcept>

template<typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T d) : data(std::move(d)), next(nullptr) {}
    };

    Node* head;
    int count;

public:
    SinglyLinkedList() : head(nullptr), count(0) {}
    ~SinglyLinkedList() { clear(); }

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    SinglyLinkedList(SinglyLinkedList&& other) noexcept : head(other.head), count(other.count) {
        other.head = nullptr;
        other.count = 0;
    }
    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            count = other.count;
            other.head = nullptr;
            other.count = 0;
        }
        return *this;
    }

    // Inserta al final 
    void push_back(T item) {
        Node* node = new Node(std::move(item));
        if (!head) {
            head = node;
        }
        else {
            Node* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        ++count;
    }

    // Inserta al inicio 
    void push_front(T item) {
        Node* node = new Node(std::move(item));
        node->next = head;
        head = node;
        ++count;
    }

    // Elimina el primer elemento que cumpla el parametro. Devuelve true si eliminó.
    bool removeIf(std::function<bool(const T&)> predicate) {
        Node* cur = head;
        Node* prev = nullptr;
        while (cur) {
            if (predicate(cur->data)) {
                Node* toDelete = cur;
                if (prev) prev->next = cur->next;
                else head = cur->next;
                cur = cur->next;
                delete toDelete;
                --count;
                return true;
            }
            else {
                prev = cur;
                cur = cur->next;
            }
        }
        return false;
    }

    // Elimina por id (asume T.id existe). Devuelve true si eliminó.
    bool removeById(int id) {
        return removeIf([id](const T& item) { return item.id == id; });
    }

    // Busca el primer elemento que cumpla el parametro y devuelve puntero a dato
    T* findIf(std::function<bool(const T&)> predicate) {
        Node* cur = head;
        while (cur) {
            if (predicate(cur->data)) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    // Busca por id
    T* findById(int id) {
        return findIf([id](const T& item) { return item.id == id; });
    }

    // Recorre la lista 
    void traverse(std::function<void(const T&)> fn) const {
        Node* cur = head;
        while (cur) {
            fn(cur->data);
            cur = cur->next;
        }
    }

    // Recorre la lista permitiendo modificar cada elemento 
    void forEach(std::function<void(T&)> fn) {
        Node* cur = head;
        while (cur) {
            fn(cur->data);
            cur = cur->next;
        }
    }

    // Debug 
    void debugPrint() const {
        Node* cur = head;
        while (cur) {
            std::cout << "[node] ";
            cur = cur->next;
        }
    }

    // Vacía la lista 
    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
        count = 0;
    }

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }
};