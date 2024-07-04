#include <atomic>
#include <cstddef>
#include <iostream>

template<typename T>
class Node {
public:
    T data;
    std::atomic<Node<T>*> next;

    Node(const T& data) : data(data), next(nullptr) {}
};

template<typename T>
class LockFreeQueue {
private:
    alignas(64) std::atomic<Node<T>*> head;
    alignas(64) std::atomic<Node<T>*> tail;

public:
    LockFreeQueue() : head(nullptr), tail(nullptr) {}

    ~LockFreeQueue() {
        while (auto node = head.load()) {
            head.store(node->next.load(std::memory_order_relaxed));
            delete node;
        }
    }

    void enqueue(const T& value) {
        auto newNode = new Node<T>(value);
        while (true) {
            auto oldTail = tail.load(std::memory_order_acquire);
            newNode->next.store(oldTail, std::memory_order_relaxed);
            if (tail.compare_exchange_weak(oldTail, newNode, std::memory_order_release, std::memory_order_relaxed)) {
                break;
            }
        }
    }

    bool dequeue(T& value) {
        while (true) {
            auto oldHead = head.load(std::memory_order_acquire);
            if (oldHead == nullptr) {
                return false;
            }
            auto nextNode = oldHead->next.load(std::memory_order_relaxed);
            if (head.compare_exchange_weak(oldHead, nextNode, std::memory_order_release, std::memory_order_relaxed)) {
                value = oldHead->data;
                delete oldHead;
                return true;
            }
        }
    }
};

int main() {
    LockFreeQueue<int> queue;

    // Enqueue elements
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    // Dequeue elements
    int value;
    while (queue.dequeue(value)) {
        std::cout << "Dequeued: " << value << std::endl;
    }

    return 0;
}

//g++ -o day1 day1.cpp -pthread -lleveldb -std=c++11

/** ls

mkdir build
cd build
cmake ..
make
*/