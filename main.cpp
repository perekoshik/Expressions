#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <complex>
#include "Expression.hpp"

using namespace std;

// Функция для сравнения double с заданной погрешностью
bool nearlyEqual(double a, double b, double epsilon = 1e-6) {
    return fabs(a - b) < epsilon;
}

// Функция для тестирования Expression<double>
void runTest(const string &desc, const Expression<double>& expr, const map<string, double>& vars, double expected) {
    try {
        double result = expr.evaluate(vars);
        bool ok = nearlyEqual(result, expected);
        cout << left << setw(50) << desc << " | " 
             << setw(10) << result << " | " 
             << setw(5) << (ok ? "OK" : "FAIL")
             << " (expected: " << expected << ")" << endl;
    } catch(const exception &ex) {
        cout << left << setw(50) << desc << " | " 
             << "ERROR: " << setw(10) << ex.what()
             << " | FAIL" << endl;
    }
}

// Функция для сравнения комплексных чисел с заданной погрешностью
bool nearlyEqual(complex<double> a, complex<double> b, double epsilon = 1e-6) {
    return fabs(a.real() - b.real()) < epsilon && fabs(a.imag() - b.imag()) < epsilon;
}

// Функция для тестирования Expression<std::complex<double>>
void runTest(const string &desc, const Expression<complex<double>>& expr, 
             const map<string, complex<double>>& vars, complex<double> expected) {
    try {
        complex<double> result = expr.evaluate(vars);
        bool ok = nearlyEqual(result, expected);
        cout << left << setw(50) << desc << " | " 
             << setw(20) << result << " | " 
             << setw(5) << (ok ? "OK" : "FAIL")
             << " (expected: " << expected << ")" << endl;
    } catch(const exception &ex) {
        cout << left << setw(50) << desc << " | " 
             << "ERROR: " << setw(20) << ex.what() 
             << " | FAIL" << endl;
    }
}

// Тесты для Expression<double>
void testAddition() {
    cout << "=== Тесты для оператора + ===" << endl;
    runTest("1 + 2", Expression<double>(1) + Expression<double>(2), {}, 3.0);
    runTest("x + 3, x=7", Expression<double>("x") + Expression<double>(3), { {"x", 7} }, 10.0);
    runTest("10 + y, y=5", Expression<double>(10) + Expression<double>("y"), { {"y", 5} }, 15.0);
    runTest("a + b, a=2, b=8", Expression<double>("a") + Expression<double>("b"), { {"a", 2}, {"b", 8} }, 10.0);
    runTest("x + y + z, x=1,y=2,z=3", Expression<double>("x") + Expression<double>("y") + Expression<double>("z"),
            { {"x", 1}, {"y", 2}, {"z", 3} }, 6.0);
    cout << endl;
}

void testSubtraction() {
    cout << "=== Тесты для оператора - ===" << endl;
    runTest("5 - 3", Expression<double>(5) - Expression<double>(3), {}, 2.0);
    runTest("x - 4, x=10", Expression<double>("x") - Expression<double>(4), { {"x", 10} }, 6.0);
    runTest("15 - y, y=5", Expression<double>(15) - Expression<double>("y"), { {"y", 5} }, 10.0);
    runTest("a - b, a=8, b=3", Expression<double>("a") - Expression<double>("b"), { {"a", 8}, {"b", 3} }, 5.0);
    runTest("x - y - z, x=20,y=5,z=3", Expression<double>("x") - Expression<double>("y") - Expression<double>("z"),
            { {"x", 20}, {"y", 5}, {"z", 3} }, 12.0);
    cout << endl;
}

void testMultiplication() {
    cout << "=== Тесты для оператора * ===" << endl;
    runTest("3 * 4", Expression<double>(3) * Expression<double>(4), {}, 12.0);
    runTest("x * 5, x=2", Expression<double>("x") * Expression<double>(5), { {"x", 2} }, 10.0);
    runTest("7 * y, y=3", Expression<double>(7) * Expression<double>("y"), { {"y", 3} }, 21.0);
    runTest("a * b, a=4, b=6", Expression<double>("a") * Expression<double>("b"), { {"a", 4}, {"b", 6} }, 24.0);
    runTest("x * y * z, x=2,y=3,z=4", Expression<double>("x") * Expression<double>("y") * Expression<double>("z"),
            { {"x", 2}, {"y", 3}, {"z", 4} }, 24.0);
    cout << endl;
}

void testDivision() {
    cout << "=== Тесты для оператора / ===" << endl;
    runTest("10 / 2", Expression<double>(10) / Expression<double>(2), {}, 5.0);
    runTest("x / 2, x=8", Expression<double>("x") / Expression<double>(2), { {"x", 8} }, 4.0);
    runTest("20 / y, y=4", Expression<double>(20) / Expression<double>("y"), { {"y", 4} }, 5.0);
    runTest("a / b, a=9, b=3", Expression<double>("a") / Expression<double>("b"), { {"a", 9}, {"b", 3} }, 3.0);
    runTest("x / y / z, x=100,y=5,z=2", Expression<double>("x") / Expression<double>("y") / Expression<double>("z"),
            { {"x", 100}, {"y", 5}, {"z", 2} }, 10.0);
    cout << endl;
}

void testPower() {
    cout << "=== Тесты для оператора ^ ===" << endl;
    runTest("2 ^ 3", Expression<double>(2) ^ Expression<double>(3), {}, 8.0);
    runTest("x ^ 2, x=5", Expression<double>("x") ^ Expression<double>(2), { {"x", 5} }, 25.0);
    runTest("2 ^ y, y=4", Expression<double>(2) ^ Expression<double>("y"), { {"y", 4} }, 16.0);
    runTest("a ^ b, a=3, b=3", Expression<double>("a") ^ Expression<double>("b"), { {"a", 3}, {"b", 3} }, 27.0);
    runTest("(x ^ 2) ^ 3, x=2", (Expression<double>("x") ^ Expression<double>(2)) ^ Expression<double>(3),
            { {"x", 2} }, 64.0);
    cout << endl;
}

void testComplexExpressions() {
    cout << "=== Тесты для комплексных выражений (Expression<std::complex<double>>) ===" << endl;
    
    runTest("Complex Test 1: (1+2i)",
            Expression<complex<double>>(complex<double>(1.0, 2.0)),
            {},
            {1.0, 2.0});
    
    runTest("Complex Test 2: z, z=(3-1i)",
            Expression<complex<double>>("z"),
            { {"z", {3.0, -1.0}} },
            {3.0, -1.0});
    
    runTest("Complex Test 3: z+(1+1i), z=(2+3i)",
            Expression<complex<double>>("z") + Expression<complex<double>>(complex<double>(1.0, 1.0)),
            { {"z", {2.0, 3.0}} },
            {3.0, 4.0});
    
    runTest("Complex Test 4: z*1, z=(2-3i)",
            Expression<complex<double>>("z") * Expression<complex<double>>(complex<double>(1.0, 0.0)),
            { {"z", {2.0, -3.0}} },
            {2.0, -3.0});
    
    cout << endl;
}

int main() {
    cout << "********** Результаты тестирования выражений **********" << endl << endl;
    
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testPower();
    testComplexExpressions();
    
    cout << "********** Тестирование завершено **********" << endl;
    return 0;
}
