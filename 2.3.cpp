#include <iostream>
using namespace std;

struct Complex {
    double real;
    double imaginary;
};

Complex add(Complex a, Complex b) {
    Complex result; 
    result.real = a.real + b.real;
    result.imaginary = a.imaginary + b.imaginary;
    return result;
}
Complex subtract(Complex a, Complex b) {
    Complex result;
    result.real = a.real - b.real;
    result.imaginary = a.imaginary - b.imaginary;
    return result;
}

Complex multiply(Complex a, Complex b) {
    Complex result;
    result.real = a.real * b.real - a.imaginary * b.imaginary;
    result.imaginary = a.real * b.imaginary + a.imaginary * b.real;
    return result;
}


void print(Complex c) {
    if (c.imaginary >= 0) {
        cout << c.real << " + " << c.imaginary << "i";
    }
    else {
        cout << c.real << " - " << -c.imaginary << "i";
    }
    

}

int main() {



    Complex a = { 3.0, 4.1 };
    Complex b = { 4.3, 8.0 };

    cout << "a = "; print(a); cout << endl;
    cout << "b = "; print(b); cout << endl << endl;

    Complex sum = add(a, b);
    cout << "a + b = "; print(sum); cout << endl;

    Complex diff = subtract(a, b);
    cout << "a - b = "; print(diff); cout << endl;

    Complex prod = multiply(a, b);
    cout << "a * b = "; print(prod); cout << endl;


    return 0;
}