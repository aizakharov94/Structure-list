#pragma once
#include <utility>


template <typename T>
class List {
 private:
    struct FirstNode {
        FirstNode() {
            previous = this;
            next = this;
        };

        FirstNode(FirstNode* PreviousNode, FirstNode* NextNode) {
            previous = PreviousNode;
            next = NextNode;
        }
        FirstNode* previous;
        FirstNode* next;
    };
    struct Node : FirstNode {
        Node(const T & elem, FirstNode * PreviousNode, FirstNode * NextNode)
                : value{elem}, FirstNode({PreviousNode, NextNode}) {}
        Node(T && elem, FirstNode * PreviousNode, FirstNode * NextNode)
                :value{std::move(elem)}, FirstNode({PreviousNode, NextNode}) {}
        T value;
    };
    FirstNode* bridge;
    size_t length;



 public:
    class Iterator {
     public:

        explicit Iterator(FirstNode* node) {
            ptr = node;
        }
        Iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator it(*this);
            ++*this;
            return it;
        }
        T& operator*() const {
            return static_cast<Node*>(ptr)->value;
        }
        T* operator->() const {
            return &(static_cast<Node*>(ptr)->value);
        };
        bool operator==(const Iterator& rhs) const {
            return (rhs.ptr == ptr);
        }
        bool operator!=(const Iterator& rhs) const {
            return (rhs.ptr != ptr);
        }
     private:
        FirstNode* ptr;
    };

    List() {
        bridge = new FirstNode();
        length = 0;
    }
    List(const List& list) {
        bridge = new FirstNode();
        length = 0;
        for (Iterator iter = list.Begin(); iter != list.End(); ++iter) {
            PushBack(*iter);
        }
    }
    List(List& list1) {
        bridge = new FirstNode();
        length = 0;
        for (Iterator iter = list1.Begin(); iter != list1.End(); ++iter) {
            PushBack(*iter);
        }
    }
    List(List&& list) {
        bridge = new FirstNode();
        length = 0;
        for (Iterator iter = list.Begin(); iter != list.End(); ++iter) {
            PushBack(std::move(*iter));
        }
    }

    ~List() {
        while (length > 0) {
            PopBack();
        }
        delete bridge;
    }

    List& operator=(const List& list) {
        List<T> copy = List<T>(list);
        return copy;
    }

    List& operator=(List&& list) {
        List<T> move = List<T>(list);
        return move;
    }

    bool IsEmpty() const {
        return length == 0;
    }
    size_t Size() const {
        return length;
    }

    void PushBack(const T& elem) {
        Node* To_tail = new Node(elem, bridge->previous, bridge);
        bridge->previous->next = To_tail;
        bridge->previous = To_tail;
        ++length;
        return;
    }
    void PushBack(T&& elem) {
        Node* To_tail = new Node(std::move(elem), bridge->previous, bridge);
        bridge->previous->next = To_tail;
        bridge->previous = To_tail;
        ++length;
        return;
    }
    void PushFront(const T& elem) {
        Node* To_head = new Node(elem, bridge, bridge->next);
        bridge->next->previous = To_head;
        bridge->next = To_head;
        ++length;
        return;
    }
    void PushFront(T&& elem) {
        Node* To_head = new Node(std::move(elem), bridge, bridge->next);
        bridge->next->previous = To_head;
        bridge->next = To_head;
        ++length;
        return;
    }

    T& Front() {
        return static_cast<Node*>(bridge->next)->value;
    }
    const T& Front() const {
        return static_cast<Node*>(bridge->next)->value;
    }
    T& Back() {
        return static_cast<Node*>(bridge->previous)->value;
    }
    const T& Back() const {
        return static_cast<Node*>(bridge->previous)->value;
    }

    void PopBack() {
        bridge->previous = bridge->previous->previous;
        delete static_cast<Node *>(bridge->previous->next);
        bridge->previous->next = bridge;
        --length;
    };
    void PopFront() {
        bridge->next = bridge->next->next;
        delete static_cast<Node *>(bridge->next->previous);
        bridge->next->previous = bridge;
        --length;
    }

    Iterator Begin() {
        return Iterator(bridge->next);
    }
    Iterator End() {
        return Iterator(bridge);
    }

    const Iterator Begin() const {
        return Iterator(bridge->next);
    }
    const Iterator End() const {
        return Iterator(bridge);
    }
};

template <typename T>
typename List<T>::Iterator begin(List<T>& list) {
    return list.Begin();
}

template <typename T>
typename List<T>::Iterator end(List<T>& list) {
    return list.End();
}
