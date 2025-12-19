#include <iostream>
#include <string>
using namespace std;

class Complex {
private:
    double real;
    double imaginary;

public:
    Complex(double r = 0, double i = 0) : real(r), imaginary(i) {}

    double getReal() const {
        return real;
    }

    double getImaginary() const {
        return imaginary;
    }

    void setReal(double real) {
        Complex::real = real;
    }

    void setImaginary(double imaginary) {
        Complex::imaginary = imaginary;
    }
  

    Complex operator+(const Complex& other) const {
        return {real + other.getReal(), imaginary + other.getImaginary()};
    }

    Complex operator-(const Complex& other) const {
        return { real - other.getReal(), imaginary - other.getImaginary() };
    }

    Complex operator*(const Complex& other) const {
        return { real * other.getReal() - imaginary * other.getImaginary(), real * other.getImaginary() + imaginary * other.getReal()};
    }

    bool operator==(const Complex& other) const {
        return (real == other.real) && (imaginary == other.imaginary);
    }

    friend ostream& operator<<(ostream& os, const Complex& c) {
        if (c.imaginary >= 0)
            os << c.real << " + " << c.imaginary << "i";
        else
            os << c.real << " - " << -c.imaginary << "i";
        return os;
    }

    friend istream& operator>>(istream& is, Complex& c) {
    is>> c.real >> c.imaginary;
    return is;
    }

    void display() const {
        cout << real << " + " << imaginary << "i" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    Complex a(3, 4);      
    Complex b(1, -2);          
    Complex sum = a + b;
    cout << "a + b = " << a << " + " << b << " = " << sum << endl;
    Complex diff = a - b;
    cout << "a - b = " << a << " - " << b << " = " << diff << endl;
    Complex prod = a * b;
    cout << "a * b = " << a << " * " << b << " = " << prod << endl;
    Complex d(3, 4);  // Такой же как a
    cout << "a == d: " << (a == d ? "true" : "false") << endl;
    cout << "a == b: " << (a == b ? "true" : "false") << endl;
    cout << endl;

    cout << "Введите комплексное число: ";
    Complex input;
    cin >> input;
    cout << "Вы ввели: " << input << endl;
    cout << "a + input = " << a + input << endl;
    cout << "a - input = " << a - input << endl;
    cout << "a * input = " << a * input << endl;
    cout << "a == input: " << (a == input ? "true" : "false") << endl;

    return 0;
}