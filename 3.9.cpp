#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template<typename T>
class Stack {
private:
    vector<T> elements;
    int capacity;

public:
    Stack(int size = 10) : capacity(size) {}

    void push(const T& element) {
        if (isFull()) {
            throw overflow_error("Стэк полон");
        }
        elements.push_back(element);
    }

    T pop() {
        if (isEmpty()) {
            throw underflow_error("Стэк пуст");
        }
        T topElement = elements.back();
        elements.pop_back();
        return topElement;
    }

    T top() const {
        if (isEmpty()) {
            throw underflow_error("Стэк пуст");
        }
        return elements.back();
    }

    bool isEmpty() const {
        return elements.empty();
    }

    bool isFull() const {
        return elements.size() >= capacity;
    }

    int size() const {
        return elements.size();
    }

    void display() const {
        cout << "Стек: ";
        for (const auto& elem : elements) {
            cout << elem << " ";
        }
        cout << endl;
    }
};

template<typename T>
class Queue {
private:
    vector<T> elements;
    int capacity;

public:
    Queue(int size = 10) : capacity(size) {}

    void enqueue(const T& element) {
        if (isFull()) {
            throw overflow_error("Очередь полна");
        }
        elements.push_back(element);
    }

    T dequeue() {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста");
        }
        T frontElement = elements.front();
        elements.erase(elements.begin());
        return frontElement;
    }

    T front() const {
        if (isEmpty()) {
            throw underflow_error("Очередь пуста");
        }
        return elements.front();
    }

    bool isEmpty() const {
        return elements.empty();
    }

    bool isFull() const {
        return elements.size() >= capacity;
    }

    int size() const {
        return elements.size();
    }
};

int main() {
    setlocale(LC_ALL, "rus");

    cout << "Тест int:" << endl;

    Stack<int> st1 = Stack<int>();
    st1.push(1);
    st1.push(2);
    st1.push(3);
    st1.display();
    cout << "Размер: " << st1.size() << endl;
    cout << "Пустой? - " << (st1.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (st1.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << st1.top() << endl;
    cout << "Удалили: " << st1.pop() << " элемент" << endl;
    st1.display();

    cout << endl;

    Queue<int> q1 = Queue<int>();
    q1.enqueue(1);
    q1.enqueue(2);
    q1.enqueue(3);
    cout << "Размер: " << q1.size() << endl;
    cout << "Пустой? - " << (q1.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (q1.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << q1.front() << endl;
    cout << "Удалили: " << q1.dequeue() << " элемент" << endl;



    cout << "Тест double:" << endl;

    Stack<double> st2 = Stack<double>();
    st2.push(1.2);
    st2.push(3.4);
    st2.push(5.6);
    st2.display();
    cout << "Размер: " << st2.size() << endl;
    cout << "Пустой? - " << (st2.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (st2.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << st2.top() << endl;
    cout << "Удалили: " << st2.pop() << " элемент" << endl;
    st2.display();

    cout << endl;

    Queue<double> q2 = Queue<double>();
    q2.enqueue(1.2);
    q2.enqueue(3.4);
    q2.enqueue(5.6);
    cout << "Размер: " << q2.size() << endl;
    cout << "Пустой? - " << (q2.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (q2.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << q2.front() << endl;
    cout << "Удалили: " << q2.dequeue() << " элемент" << endl;

    cout << "Тест string:" << endl;

    Stack<string> st3 = Stack<string>();
    st3.push("Hello");
    st3.push("World");
    st3.push("!");
    st3.display();
    cout << "Размер: " << st3.size() << endl;
    cout << "Пустой? - " << (st3.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (st3.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << st3.top() << endl;
    cout << "Удалили: " << st3.pop() << " элемент" << endl;
    st3.display();

    cout << endl;

    Queue<string> q3 = Queue<string>();
    q3.enqueue("Hello");
    q3.enqueue("World");
    q3.enqueue("!");
    cout << "Размер: " << q3.size() << endl;
    cout << "Пустой? - " << (q3.isEmpty() ? "Да" : "Нет") << endl;
    cout << "Полный? - " << (q3.isFull() ? "Да" : "Нет") << endl;
    cout << "Последний элемент: " << q3.front() << endl;
    cout << "Удалили: " << q3.dequeue() << " элемент" << endl;


    return 0;
}