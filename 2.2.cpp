#include <iostream>
#include <string>
using namespace std;

int stringLength(char* name) {
    int length = 0;
    while (*name != '\0') {
        length++;
        name++;
    }
    return length;
}

void reverseString(char* name) {
    int length = stringLength(name);
    char* first = name;           
    char* last = name + length - 1;  

    while (first < last) {
        char temp = *first;
        *first = *last;
        *last = temp;
        first++;  
        last--;    
    }
}

int countValue(char* name, char value) {
    int count = 0;
    while (*name != '\0') {
        if (*name == value) {
            count++;
        }
        name++;
    }
    return count;
}
int main()
{
    setlocale(LC_ALL, "rus");
    char name[255];
    char value;
    cout << "Введите строку (не более 254 символовЮ. в противном случае она будет обрезана до 254 символов) и на английском языке! Версия моего компилятора ни в какую не хочет подключать русский язык:("<<endl;
    cin.getline(name, size(name));
    int length = stringLength(name);
    cout << "Длина строки = " << length << endl;
    reverseString(name);
    cout << "Перевернутая строка: " << name << endl;
    cout << "Введите символ для поиска его количества в строке " << endl;
    cin >> value;
    int count = countValue(name, value);
    cout << "Количество символов " << value << " в строке = "<< count  <<endl;
    return 0;
}

