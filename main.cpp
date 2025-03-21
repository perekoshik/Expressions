#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
#include <complex>
#include "Expression.hpp"

using namespace std;

bool nearlyEqual(double a, double b, double epsilon = 1e-6) {
    return fabs(a - b) < epsilon;
}

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

void testAddition() {
    cout << "=== Тесты для оператора + ===" << endl;
    runTest("1 + 2", Expression<double>(1) + Expression<double>(2), {}, 3.0);
    runTest("x + 3, x=7", Expression<double>("x") + Expression<double>(3), { {"x", 7} }, 10.0);
    runTest("10 + y, y=5", Expression<double>(10) + Expression<double>("y"), { {"y", 5} }, 15.0);
    runTest("a + b, a=2, b=8", Expression<double>("a") + Expression<double>("b"), { {"a", 2}, {"b", 8} }, 10.0);
    runTest("x + y + z, x=1,y=2,z=3", Expression<double>("x") + Expression<double>("y") + Expression<double>("z"), { {"x", 1}, {"y", 2}, {"z", 3} }, 6.0);
    cout << endl;
}

void testSubtraction() {
    cout << "=== Тесты для оператора - ===" << endl;
    runTest("5 - 3", Expression<double>(5) - Expression<double>(3), {}, 2.0);
    runTest("x - 4, x=10", Expression<double>("x") - Expression<double>(4), { {"x", 10} }, 6.0);
    runTest("15 - y, y=5", Expression<double>(15) - Expression<double>("y"), { {"y", 5} }, 10.0);
    runTest("a - b, a=8, b=3", Expression<double>("a") - Expression<double>("b"), { {"a", 8}, {"b", 3} }, 5.0);
    runTest("x - y - z, x=20,y=5,z=3", Expression<double>("x") - Expression<double>("y") - Expression<double>("z"), { {"x", 20}, {"y", 5}, {"z", 3} }, 12.0);
    cout << endl;
}

void testMultiplication() {
    cout << "=== Тесты для оператора * ===" << endl;
    runTest("3 * 4", Expression<double>(3) * Expression<double>(4), {}, 12.0);
    runTest("x * 5, x=2", Expression<double>("x") * Expression<double>(5), { {"x", 2} }, 10.0);
    runTest("7 * y, y=3", Expression<double>(7) * Expression<double>("y"), { {"y", 3} }, 21.0);
    runTest("a * b, a=4, b=6", Expression<double>("a") * Expression<double>("b"), { {"a", 4}, {"b", 6} }, 24.0);
    runTest("x * y * z, x=2,y=3,z=4", Expression<double>("x") * Expression<double>("y") * Expression<double>("z"), { {"x", 2}, {"y", 3}, {"z", 4} }, 24.0);
    cout << endl;
}

void testDivision() {
    cout << "=== Тесты для оператора / ===" << endl;
    runTest("10 / 2", Expression<double>(10) / Expression<double>(2), {}, 5.0);
    runTest("x / 2, x=8", Expression<double>("x") / Expression<double>(2), { {"x", 8} }, 4.0);
    runTest("20 / y, y=4", Expression<double>(20) / Expression<double>("y"), { {"y", 4} }, 5.0);
    runTest("a / b, a=9, b=3", Expression<double>("a") / Expression<double>("b"), { {"a", 9}, {"b", 3} }, 3.0);
    runTest("x / y / z, x=100,y=5,z=2", Expression<double>("x") / Expression<double>("y") / Expression<double>("z"), { {"x", 100}, {"y", 5}, {"z", 2} }, 10.0);
    cout << endl;
}

void testPower() {
    cout << "=== Тесты для оператора ^ (Power) ===" << endl;
    runTest("2 ^ 3", Expression<double>(2) ^ Expression<double>(3), {}, 8.0);
    runTest("x ^ 2, x=5", Expression<double>("x") ^ Expression<double>(2), { {"x", 5} }, 25.0);
    runTest("2 ^ y, y=4", Expression<double>(2) ^ Expression<double>("y"), { {"y", 4} }, 16.0);
    runTest("a ^ b, a=3, b=3", Expression<double>("a") ^ Expression<double>("b"), { {"a", 3}, {"b", 3} }, 27.0);
    runTest("(x ^ 2) ^ 3, x=2", (Expression<double>("x") ^ Expression<double>(2)) ^ Expression<double>(3), { {"x", 2} }, 64.0);
    cout << endl;
}

void testComplexExpressions() {
    cout << "=== Тесты для комплексных выражений (Expression<std::complex<double>>) ===" << endl;
    
    Expression<std::complex<double>> expr1(std::complex<double>(1.0, 2.0));
    std::map<std::string, std::complex<double>> vars1;
    auto res1 = expr1.evaluate(vars1);
    cout << "Complex Test 1: " << expr1.toString() << " = " << res1 
         << " (ожидается: (1,2))" << endl;
    
    Expression<std::complex<double>> expr2("z");
    std::map<std::string, std::complex<double>> vars2 = { {"z", std::complex<double>(3.0, -1.0)} };
    auto res2 = expr2.evaluate(vars2);
    cout << "Complex Test 2: " << expr2.toString() << " при z=(3,-1) = " << res2 
         << " (ожидается: (3,-1))" << endl;
    
    Expression<std::complex<double>> expr3 = expr2 + Expression<std::complex<double>>(std::complex<double>(1.0, 1.0));
    std::map<std::string, std::complex<double>> vars3 = { {"z", std::complex<double>(2.0, 3.0)} };
    auto res3 = expr3.evaluate(vars3);
    cout << "Complex Test 3: (" << expr2.toString() << " + (1+1i)) при z=(2,3) = " << res3 
         << " (ожидается: (3,4))" << endl;
    
    Expression<std::complex<double>> expr4 = expr2 * Expression<std::complex<double>>(std::complex<double>(1.0, 0.0));
    std::map<std::string, std::complex<double>> vars4 = { {"z", std::complex<double>(2.0, -3.0)} };
    auto res4 = expr4.evaluate(vars4);
    cout << "Complex Test 4: (" << expr2.toString() << " * 1) при z=(2,-3) = " << res4 
         << " (ожидается: (2,-3))" << endl;
    
    Expression<std::complex<double>> expr5 = expr2 ^ Expression<std::complex<double>>(std::complex<double>(2.0, 0.0));
    std::map<std::string, std::complex<double>> vars5 = { {"z", std::complex<double>(1.0, 1.0)} };
    auto res5 = expr5.evaluate(vars5);
    cout << "Complex Test 5: (" << expr2.toString() << " ^ 2) при z=(1,1) = " << res5 
         << " (ожидается: (0,2))" << endl;
    
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
