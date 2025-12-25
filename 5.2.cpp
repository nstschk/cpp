#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;

template<typename T>
class ThreadSafeQueue {
private:
    queue<T> data_queue;           // было queue<T> queue
    mutex queue_mutex;             // было mutex mutex
    condition_variable queue_cond; // было condition_variable cond
    bool shutdown_flag = false;

public:
    void push(T value) {
        unique_lock<mutex> lock(queue_mutex);
        if (shutdown_flag) return;
        data_queue.push(move(value));
        queue_cond.notify_one();
    }

    bool pop(T& value) {
        unique_lock<mutex> lock(queue_mutex);
        queue_cond.wait(lock, [this]() { return shutdown_flag || !data_queue.empty(); });

        if (data_queue.empty()) return false;

        value = move(data_queue.front());
        data_queue.pop();
        return true;
    }

    void shutdown() {
        unique_lock<mutex> lock(queue_mutex);
        shutdown_flag = true;
        queue_cond.notify_all();
    }
};

void producer(ThreadSafeQueue<int>& queue, int count) {
    for (int i = 1; i <= count; ++i) {
        queue.push(i);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void consumer(ThreadSafeQueue<int>& queue, int id) {
    int value;
    while (queue.pop(value)) {
        cout << "Consumer " << id << ": processed " << value
            << " (result: " << value * 2 << ")\n";
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    cout << "Consumer " << id << " finished\n";
}

int main() {
    ThreadSafeQueue<int> queue;

    vector<thread> producers;
    vector<thread> consumers;

    producers.emplace_back(producer, ref(queue), 10);
    producers.emplace_back(producer, ref(queue), 15);

    consumers.emplace_back(consumer, ref(queue), 1);
    consumers.emplace_back(consumer, ref(queue), 2);
    consumers.emplace_back(consumer, ref(queue), 3);

    for (auto& t : producers) {
        t.join();
    }

    queue.shutdown();

    for (auto& t : consumers) {
        t.join();
    }

    cout << "All threads finished\n";
    return 0;
}