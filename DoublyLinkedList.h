#pragma once
#include <functional>
#include <utility>
#include <iostream>

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T d) : data(std::move(d)), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), count(0) {}
    ~DoublyLinkedList() { clear(); }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    DoublyLinkedList(DoublyLinkedList&& other) noexcept : head(other.head), tail(other.tail), count(other.count) {
        other.head = other.tail = nullptr;
        other.count = 0;
    }
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            other.head = other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }

    // Inserta al final
    void push_back(T item) {
        Node* node = new Node(std::move(item));
        if (!tail) {
            head = tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++count;
    }

    // Inserta al inicio
    void push_front(T item) {
        Node* node = new Node(std::move(item));
        if (!head) {
            head = tail = node;
        }
        else {
            head->prev = node;
            node->next = head;
            head = node;
        }
        ++count;
    }

    // Elimina el primer elemento que cumpla el parametro
    bool removeIf(std::function<bool(const T&)> predicate) {
        Node* cur = head;
        while (cur) {
            if (predicate(cur->data)) {
                Node* prev = cur->prev;
                Node* next = cur->next;
                if (prev) prev->next = next; else head = next;
                if (next) next->prev = prev; else tail = prev;
                delete cur;
                --count;
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    // Elimina por id 
    bool removeById(int id) {
        return removeIf([id](const T& item) { return item.id == id; });
    }

    // Busca el primer elemento que cumpla el paraemtro y devuelve puntero a dato 
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

    // Vacía la lista
    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = tail = nullptr;
        count = 0;
    }

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }

    // Acceso a head/tail, por si acaso
    const T* first() const { return head ? &head->data : nullptr; }
    const T* last() const { return tail ? &tail->data : nullptr; }
};
