#include <iostream>
#include <iomanip>
#include <cmath>
#include <map>
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
    cout << "=== Тесты для сложных выражений ===" << endl;
    runTest("Complex 1: (x+y)*(a-b)/c, x=10,y=5,a=8,b=3,c=5", 
        ((Expression<double>("x") + Expression<double>("y")) *
         (Expression<double>("a") - Expression<double>("b"))) / Expression<double>("c"),
        { {"x", 10}, {"y", 5}, {"a", 8}, {"b", 3}, {"c", 5} },
        (10+5)*(8-3)/5.0);

    runTest("Complex 2: sin(x)+cos(y)-ln(a)*exp(b), x=1.57,y=0,a=2.71828,b=1", 
        sin(Expression<double>("x")) + cos(Expression<double>("y")) - ln(Expression<double>("a")) * exp(Expression<double>("b")),
        { {"x", 1.57}, {"y", 0.0}, {"a", 2.71828}, {"b", 1} },
        sin(1.57) + cos(0.0) - log(2.71828)*exp(1));

    runTest("Complex 3: (x^2)+(y^2)-(z^2), x=3,y=4,z=5", 
        (Expression<double>("x") ^ Expression<double>(2)) + (Expression<double>("y") ^ Expression<double>(2)) - (Expression<double>("z") ^ Expression<double>(2)),
        { {"x", 3}, {"y", 4}, {"z", 5} },
        9 + 16 - 25);

    runTest("Complex 4: ((a+b)/c)^d, a=5,b=7,c=3,d=2", 
        ((Expression<double>("a") + Expression<double>("b")) / Expression<double>("c")) ^ Expression<double>("d"),
        { {"a", 5}, {"b", 7}, {"c", 3}, {"d", 2} },
        pow(((5+7)/3.0), 2));

    runTest("Complex 5: exp(x)-ln(y)+sin(a)*cos(b), x=1,y=2.71828,a=1,b=0", 
        exp(Expression<double>("x")) - ln(Expression<double>("y")) + sin(Expression<double>("a")) * cos(Expression<double>("b")),
        { {"x", 1}, {"y", 2.71828}, {"a", 1}, {"b", 0} },
        exp(1) - log(2.71828) + sin(1)*cos(0));

    runTest("Complex 6: (x+y+z)/(a-b+c), x=10,y=5,z=15,a=20,b=5,c=3", 
        (Expression<double>("x") + Expression<double>("y") + Expression<double>("z")) / (Expression<double>("a") - Expression<double>("b") + Expression<double>("c")),
        { {"x", 10}, {"y", 5}, {"z", 15}, {"a", 20}, {"b", 5}, {"c", 3} },
        (10+5+15) / (20-5+3));

    runTest("Complex 7: (x*y)+(a/b)-(c^d), x=3,y=4,a=20,b=5,c=2,d=3", 
        (Expression<double>("x") * Expression<double>("y")) + (Expression<double>("a") / Expression<double>("b")) - (Expression<double>("c") ^ Expression<double>("d")),
        { {"x", 3}, {"y", 4}, {"a", 20}, {"b", 5}, {"c", 2}, {"d", 3} },
        (3*4) + (20/5.0) - pow(2, 3));

    runTest("Complex 8: ln(exp(x)+sin(y)*cos(z)), x=1,y=0.5,z=0.5", 
        ln(exp(Expression<double>("x")) + sin(Expression<double>("y")) * cos(Expression<double>("z"))),
        { {"x", 1}, {"y", 0.5}, {"z", 0.5} },
        log(exp(1) + sin(0.5)*cos(0.5)));

    runTest("Complex 9: (x^y)+((a-(b*c))/d), x=2,y=3,a=10,b=2,c=3,d=4", 
        (Expression<double>("x") ^ Expression<double>("y")) + ((Expression<double>("a") - (Expression<double>("b") * Expression<double>("c"))) / Expression<double>("d")),
        { {"x", 2}, {"y", 3}, {"a", 10}, {"b", 2}, {"c", 3}, {"d", 4} },
        pow(2,3) + ((10 - (2*3))/4.0));

    runTest("Complex 10: exp(sin(x))*ln(1+y)-cos(z), x=1,y=2,z=0", 
        exp(sin(Expression<double>("x"))) * ln(Expression<double>(1) + Expression<double>("y")) - cos(Expression<double>("z")),
        { {"x", 1}, {"y", 2}, {"z", 0} },
        exp(sin(1)) * log(1+2) - cos(0));

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
