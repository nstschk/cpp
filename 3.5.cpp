#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Book {
private:
    string title;
    string author;
    string isbn;
    int year;
    bool isAvailable;

public:

    Book(string t, string a, string i, int y) : title(t), author(a), isbn(i), year(y), isAvailable(true) {}

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    string getISBN() const {
        return isbn;
    }

    int getYear() const {
        return year;
    }

    bool getAvailability() const {
        return isAvailable;
    }
    
    void setAuthor(const string& a) {
        author = a;
    }
    
    void setISBN(const string& i) {
        isbn = i;
    }

    void setYear(int y) {
        year = y;
    }

    void setTitle(const string& t) {
        title = t;
    }
    
    bool borrow() {
        if (isAvailable) {
            isAvailable = false;
            cout << "Книга " << title << " выдана" << endl;
            return true;
        }
        else {
            cout << "Книга " << title << " уже была выдана" << endl;
            return false;
        }
    }

    void returnBook() {
        isAvailable = true;
        cout << "Книга " << title << " возвращена" << endl;
    }

    void displayInfo() const {
    cout<<"Название книги: "<<title<< "Автор: " << author<< "ISBN: " << isbn<< "Год издания: " << year<< "Статус книги: " << (isAvailable ? "Доступна" : "Выдана") << endl;
    }
};

class Library {
private:
    string name;
    string address;
    vector<Book> books;

public:
    Library(string n, string addr) : name(n), address(addr) {}
    void addBook(const Book& book) {
        books.push_back(book);
        cout << "Книга добавлена: " << book.getTitle() << endl;
    }
    void removeBook(const string& isbn) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].getISBN() == isbn) {
                cout << " Книга удалена: " << books[i].getTitle() << endl;
                books.erase(books.begin() + i);
                return;
            }
        }
        cout << "Книга с ISBN " << isbn << " не найдена в библиотеке" << endl;
    }
    void findBook(const string& title) {
        for (Book& book : books) {
            if (book.getTitle() == title) {
                book.displayInfo();
                return;
            }
        }
        cout << "Книга \"" << title << "\" не найдена" << endl;
    }
    void borrowBook(const string& isbn) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].getISBN() == isbn) {
                books[i].borrow();  // Вызываем метод книги
                return;
            }
        }
        cout << "Книга с ISBN " << isbn << " не найдена" << endl;
    }
    void returnBook(const string& isbn) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].getISBN() == isbn) {
                books[i].returnBook();
                return;
            }
        }
        cout << "Книга с ISBN " << isbn << " не найдена" << endl;
    }
    void displayAllBooks() const {
        for (int i = 0; i < books.size(); i++) {
            books[i].displayInfo(); 
        }
    }
    void displayAvailableBooks() const {
        int count = 0;

        for (int i = 0; i < books.size(); i++) {
            if (books[i].getAvailability()) {  
                count++;
                books[i].displayInfo();
            }
        }

        if (count == 0) {
            cout << "Нет доступных книг" << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    Library lib("Центральная городская библиотека", "ул. Пушкина, д. 10");

  
    Book book1("Преступление и наказание", "Ф.М. Достоевский", "978-5-389-12345-6", 1866);
    Book book2("Война и мир", "Л.Н. Толстой", "978-5-389-23456-7", 1869);
    Book book3("Мастер и Маргарита", "М.А. Булгаков", "978-5-389-34567-8", 1967);
    Book book4("Отцы и дети", "И.С. Тургенев", "978-5-389-45678-9", 1862);
    Book book5("Герой нашего времени", "М.Ю. Лермонтов", "978-5-389-56789-0", 1840);

  
    lib.addBook(book1);
    lib.addBook(book2);
    lib.addBook(book3);
    lib.addBook(book4);
    lib.addBook(book5);
    

  
    lib.displayAllBooks();
    cout << endl;

    


 
    cout << "Пытаемся взять 'Война и мир' (ISBN 978-5-389-23456-7):" << endl;
    lib.borrowBook("978-5-389-23456-7");
    cout << endl;


 
    lib.displayAvailableBooks();
    cout << endl;

    cout << "Возвращаем 'Война и мир' (ISBN 978-5-389-23456-7):" << endl;
    lib.returnBook("978-5-389-23456-7");
    cout << endl;


    cout << "Ищем 'Мастер и Маргарита':" << endl;
    lib.findBook("Мастер и Маргарита");
    cout << endl;

    cout << "Ищем несуществующую книгу 'Евгений Онегин':" << endl;
    lib.findBook("Евгений Онегин");
    cout << endl;

  
    cout << "Удаляем 'Отцы и дети' (ISBN 978-5-389-45678-9):" << endl;
    lib.removeBook("978-5-389-45678-9");
    cout << endl;

    cout << "Пытаемся удалить несуществующую книгу:" << endl;
    lib.removeBook("999-999-99999-9");
    cout << endl;

    return 0;

}
