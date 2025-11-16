#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Shape {
protected:
    string color;
    string name;

public:
    Shape(string n, string c) : name(n), color(c) {}
    virtual ~Shape() {}
    virtual double calculateArea() const = 0;
    virtual double calculatePerimeter() const = 0;
    virtual void draw() const {
        cout << "Рисую фигуру цвета " << color << endl;
    }

    virtual void displayInfo() const {
        cout << "Фигура: " << name << ", Цвет: " << color << endl;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(string c, double r) : Shape("Круг", c), radius(r) {}
    double calculateArea() const override {
        return 3.14159 * radius * radius;
    }
    double calculatePerimeter() const override {
        return 3.14159 * radius * 2;
    }
    void draw() const override {
        cout << "Рисую круг радиуса " << radius
            << " цвета " << color << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(string c, double w, double h) : Shape("Прямоугольник", c), width(w), height(h) {}
    double calculateArea() const override {
        return width * height;
    }
    double calculatePerimeter() const override {
        return (width + height) * 2;
    }
    void draw() const override {
        cout << "Рисую прмоугольник со сторонами " << width<< " и "<< height
            << " цвета " << color << endl;
    }
};

class Triangle : public Shape {
private:
    double sideA, sideB, sideC;

public:
    Triangle(string c, double a, double b, double d) : Shape("Треугольник", c), sideA(a), sideB(b), sideC(d) {}
    double calculateArea() const override {
        return sqrt(calculatePerimeter() / 2 * (calculatePerimeter() / 2 - sideA) * (calculatePerimeter() / 2 - sideB) * (calculatePerimeter() / 2 - sideC));
    }
    double calculatePerimeter() const override {
        return sideA + sideB + sideC;
    }
    void draw() const override {
        cout << "Рисую треугольник со сторонами " << sideA << " ," << sideB << " и " << sideC
            << " цвета " << color << endl;
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    vector<Shape*> shapes;
    shapes.push_back(new Circle("Красный", 5.0));
    shapes.push_back(new Rectangle("Синий", 4.0, 6.0));
    shapes.push_back(new Triangle("Зеленый", 3.0, 4.0, 5.0));
    for (int i = 0; i < 3; ++i) {
        shapes[i]->draw();
        cout << "Площадь: " << shapes[i]->calculateArea() << endl;
        cout << "Периметр: " << shapes[i]->calculatePerimeter() << endl;
        cout << "---" << endl;
    }

    // Очистка памяти
    for (auto shape : shapes) {
        delete shape;
    }

    return 0;
}