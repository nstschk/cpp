#include <iostream>
#include <string>
#include <locale>
using namespace std;

struct Book {
    string title;
    string author;
    int year;
    double price;
};

void printBooks(Book* books, int size) {
    for (int i = 0; i < size; i++) {
        cout << "Книга №" << i + 1 << endl;
        cout << "  Название: " << books[i].title << endl;
        cout << "  Автор: " << books[i].author << endl;
        cout << "  Год издания: " << books[i].year << endl;
        cout << "  Цена: " << books[i].price << endl;
    }
}
void MaxPrice(Book* books, int size){
    int mostExpensiveIndex = 0;
    double maxPrice = books[0].price;

    for (int i = 1; i < size; i++) {
        if (books[i].price > maxPrice) {
            maxPrice = books[i].price;
            mostExpensiveIndex = i;
        }
    }
    cout << "Самая дорогая книга:" << endl;
    cout << "Название: " << books[mostExpensiveIndex].title << endl;
    cout << "Автор: " << books[mostExpensiveIndex].author << endl;
    cout << "Год издания: " << books[mostExpensiveIndex].year << endl;
    cout << "Цена: " << books[mostExpensiveIndex].price << endl;
}


int main() {
    setlocale(LC_ALL, "rus");
    cout << "Введите количество книг в библиотеке" << endl;
    cout << "В случае, если будет введено дробное число, то оно будет округлено до целого числа" << endl;
    int N;
    cin >> N;

    if (N <= 0) {
        cout << "Число должно быть положительным" << endl;
        return 0;
    }
    if (N >= 2147483647) {
        cout << "Слишком большое число! Введите число поменьше" << endl;
        return 0;
    }
    Book* library = new Book[N];
    cin.ignore();
    cout << "Пожалуйста, вводите название книги и автора на английском языке, версия моего компилятора не хочет выводить русский язык:(" << endl;
    for (int i = 0; i < N; i++) {
        cout << "Книга №" << i + 1 << endl;

        cout << "Введите название: ";
        getline(cin, library[i].title);

        cout << "Введите автора: ";
        getline(cin, library[i].author);

        cout << "Введите год издания: ";
        cin >> library[i].year;

        cout << "Введите цену: ";
        cin >> library[i].price;
        cin.ignore();
    }

    printBooks(library, N);
    MaxPrice(library, N);

    delete[] library;
    return 0;
}
