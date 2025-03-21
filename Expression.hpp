#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <memory>
#include <map>
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <complex>
#include <type_traits>

enum class ExprType {
    Constant,
    Variable,
    Binary,
    Unary
};

enum class BinaryOp {
    Add,
    Subtract,
    Multiply,
    Divide,
    Power
};

enum class UnaryOp {
    Sin,
    Cos,
    Ln,
    Exp
};

template<typename T>
class Expression {
public:
    Expression(T value);
    Expression(const std::string &variableName);
    Expression(const Expression<T> &other);        // копирование
    Expression(Expression<T> &&other) noexcept;      // перемещение
    Expression<T>& operator=(const Expression<T> &other);  // оператор копирования
    Expression<T>& operator=(Expression<T> &&other) noexcept; // оператор перемещения
    ~Expression();

    friend Expression<T> operator+(const Expression<T> &lhs, const Expression<T> &rhs) {
        return Expression<T>::createBinary(BinaryOp::Add, lhs, rhs);
    }
    friend Expression<T> operator-(const Expression<T> &lhs, const Expression<T> &rhs) {
        return Expression<T>::createBinary(BinaryOp::Subtract, lhs, rhs);
    }
    friend Expression<T> operator*(const Expression<T> &lhs, const Expression<T> &rhs) {
        return Expression<T>::createBinary(BinaryOp::Multiply, lhs, rhs);
    }
    friend Expression<T> operator/(const Expression<T> &lhs, const Expression<T> &rhs) {
        return Expression<T>::createBinary(BinaryOp::Divide, lhs, rhs);
    }
    friend Expression<T> operator^(const Expression<T> &lhs, const Expression<T> &rhs) {
        return Expression<T>::createBinary(BinaryOp::Power, lhs, rhs);
    }

    friend Expression<T> sin(const Expression<T> &expr) {
        return Expression<T>::createUnary(UnaryOp::Sin, expr);
    }
    friend Expression<T> cos(const Expression<T> &expr) {
        return Expression<T>::createUnary(UnaryOp::Cos, expr);
    }
    friend Expression<T> ln(const Expression<T> &expr) {
        return Expression<T>::createUnary(UnaryOp::Ln, expr);
    }
    friend Expression<T> exp(const Expression<T> &expr) {
        return Expression<T>::createUnary(UnaryOp::Exp, expr);
    }

    std::string toString() const;

    Expression<T> substitute(const std::string &var, const Expression<T> &value) const;

    T evaluate(const std::map<std::string, T> &variables) const;

private:
    struct Node;
    std::shared_ptr<Node> root;

    struct Node {
        ExprType type;
        T value;                   // константа
        std::string variableName;  // переменная
        BinaryOp binOp;            // бинарная операция
        UnaryOp unOp;              // унарная операция
        std::shared_ptr<Node> left;  // левый операнд или единственный операнд для унарной операции
        std::shared_ptr<Node> right; // правый операнд (только для бинарных операций)

        Node(T val) : type(ExprType::Constant), value(val) {}
        Node(const std::string &var) : type(ExprType::Variable), variableName(var) {}
        Node(BinaryOp op, std::shared_ptr<Node> l, std::shared_ptr<Node> r)
            : type(ExprType::Binary), binOp(op), left(l), right(r) {}
        Node(UnaryOp op, std::shared_ptr<Node> operand)
            : type(ExprType::Unary), unOp(op), left(operand) {}
    };

    static Expression<T> createBinary(BinaryOp op, const Expression<T> &lhs, const Expression<T> &rhs);
    static Expression<T> createUnary(UnaryOp op, const Expression<T> &operand);

    T evaluateNode(const std::shared_ptr<Node> &node, const std::map<std::string, T> &variables) const;
    std::shared_ptr<Node> substituteNode(const std::shared_ptr<Node> &node, const std::string &var, const Expression<T> &value) const;
    std::string nodeToString(const std::shared_ptr<Node> &node) const;
};

#include "Expression.tpp"

#endif
