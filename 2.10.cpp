#include <iostream>
#include <string>
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


void sortBooksByYear(Book* books, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (books[j].year > books[j + 1].year) {
                Book temp = books[j];
                books[j] = books[j + 1];
                books[j + 1] = temp;
            }
        }
    }
}


Book* findBookByAuthor(Book* books, int size, const string& author) {
    for (int i = 0; i < size; i++) {
        if (books[i].author == author) {
            return &books[i]; 
        }
    }
    return nullptr;
}

int main() {
    setlocale(LC_ALL, "rus");

    const int librarySize = 5;
    Book* library = new Book[librarySize];

    library[0] = { "The Great Gatsby", "F. Scott Fitzgerald", 1925, 1200 };
    library[1] = { "1984", "George Orwell", 1949, 950 };
    library[2] = { "Pride and Prejudice", "Jane Austen", 1813, 1100 };
    library[3] = { "To Kill a Mockingbird", "Harper Lee", 1960, 1300 };
    library[4] = { "The Catcher in the Rye", "J.D. Salinger", 1951, 890 };
    printBooks(library, librarySize);

    sortBooksByYear(library, librarySize);
    cout << "Сортируем массив по году";
    printBooks(library, librarySize);

    
    cout << "Ищем книгу по автору" << endl;

    string authorToFind = "George Orwell";
    Book* foundBook = findBookByAuthor(library, librarySize, authorToFind);

    if (foundBook != nullptr) {
        cout << "Книга автора " << authorToFind << " найдена" << endl;
        cout << "  Название: " << foundBook->title << endl;
        cout << "  Год издания: " << foundBook->year << endl;
        cout << "  Цена: " << foundBook->price << endl;
    }
    else {
        cout << "Книга автора " << authorToFind << " не найдена" << endl;
    }

    authorToFind = "Leo Tolstoy";
    foundBook = findBookByAuthor(library, librarySize, authorToFind);

    if (foundBook != nullptr) {
        cout << "Книга автора " << authorToFind << " найдена" << endl;
        cout << "  Название: " << foundBook->title << endl;
        cout << "  Год издания: " << foundBook->year << endl;
        cout << "  Цена: " << foundBook->price << endl;
    }
    else {
        cout << "Книга автора " << authorToFind << " не найдена" << endl;
    }

    
    delete[] library;
    return 0;
}