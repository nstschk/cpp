#include <thread>
#include <vector>
#include <queue>
#include <future>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>

class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop || !this->tasks.empty();
                            });
                        if (this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }
                    task();
                }
                });
        }
    }

    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
                throw std::runtime_error("submit on stopped ThreadPool");
            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return result;
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }
};

unsigned long long factorial(int n) {
    // Имитация нагрузки - чем больше n, тем дольше вычисление
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
        // Замедляем выполнение для наглядности
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "rus");
    std::cout << "ПОСЛЕДОВАТЕЛЬНОЕ ВЫЧИСЛЕНИЕ" << std::endl;
    auto start_seq = std::chrono::high_resolution_clock::now();

    // Последовательное выполнение
    for (int i = 1; i <= 10; ++i) {
        unsigned long long result = factorial(i);
        std::cout << "Factorial of " << i << " = " << result << std::endl;
    }

    auto end_seq = std::chrono::high_resolution_clock::now();
    auto duration_seq = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq);
    std::cout << "Sequential time: " << duration_seq.count() << " ms\n" << std::endl;

    std::cout << "ПАРАЛЛЕЛЬНОЕ ВЫЧИСЛЕНИЕ" << std::endl;
    auto start_par = std::chrono::high_resolution_clock::now();

    // Создаем пул из 4 потоков
    ThreadPool pool(4);
    std::vector<std::future<unsigned long long>> futures;

    // Отправляем все задачи в пул
    for (int i = 1; i <= 10; ++i) {
        futures.emplace_back(pool.submit(factorial, i));
    }

    // Получаем результаты (будут готовы по мере выполнения)
    for (size_t i = 0; i < futures.size(); ++i) {
        unsigned long long result = futures[i].get();
        std::cout << "Factorial of " << (i + 1) << " = " << result << std::endl;
    }

    auto end_par = std::chrono::high_resolution_clock::now();
    auto duration_par = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par);

    std::cout << "\nParallel time: " << duration_par.count() << " ms" << std::endl;
    std::cout << "Speedup: " << (double)duration_seq.count() / duration_par.count() << "x faster" << std::endl;

    return 0;
}