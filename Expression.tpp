#ifndef EXPRESSION_TPP
#define EXPRESSION_TPP
#include "Expression.hpp"
template<typename T>
Expression<T>::Expression(T value) {
    root = std::make_shared<Node>(value);
}

template<typename T>
Expression<T>::Expression(const std::string &variableName) {
    root = std::make_shared<Node>(variableName);
}

template<typename T>
Expression<T>::Expression(const Expression<T> &other) {
    root = other.root;
}

template<typename T>
Expression<T>::Expression(Expression<T> &&other) noexcept {
    root = std::move(other.root);
}

template<typename T>
Expression<T>& Expression<T>::operator=(const Expression<T> &other) {
    if (this != &other) {
        root = other.root;
    }
    return *this;
}

template<typename T>
Expression<T>& Expression<T>::operator=(Expression<T> &&other) noexcept {
    if (this != &other) {
        root = std::move(other.root);
    }
    return *this;
}

// Деструктор
template<typename T>
Expression<T>::~Expression() {
}

// Бинарный узел
template<typename T>
Expression<T> Expression<T>::createBinary(BinaryOp op, const Expression<T> &lhs, const Expression<T> &rhs) {
    Expression<T> expr(0);
    expr.root = std::make_shared<Node>(op, lhs.root, rhs.root);
    return expr;
}

// Унарный
template<typename T>
Expression<T> Expression<T>::createUnary(UnaryOp op, const Expression<T> &operand) {
    Expression<T> expr(0);
    expr.root = std::make_shared<Node>(op, operand.root);
    return expr;
}

template<typename T>
T Expression<T>::evaluateNode(const std::shared_ptr<Node> &node, const std::map<std::string, T> &variables) const {
    if (!node) {
        throw std::runtime_error("Пустой узел");
    }
    switch (node->type) {
        case ExprType::Constant:
            return node->value;
        case ExprType::Variable: {
            auto it = variables.find(node->variableName);
            if (it != variables.end()) {
                return it->second;
            } else {
                throw std::runtime_error("Не задано значение для переменной: " + node->variableName);
            }
        }
        case ExprType::Binary: {
            T leftVal = evaluateNode(node->left, variables);
            T rightVal = evaluateNode(node->right, variables);
            switch (node->binOp) {
                case BinaryOp::Add:      return leftVal + rightVal;
                case BinaryOp::Subtract: return leftVal - rightVal;
                case BinaryOp::Multiply: return leftVal * rightVal;
                case BinaryOp::Divide:
                    if (rightVal == T(0)) {
                        throw std::runtime_error("Деление на ноль");
                    }
                    return leftVal / rightVal;
                case BinaryOp::Power:    return std::pow(leftVal, rightVal);
                default:
                    throw std::runtime_error("Неизвестная операция");
            }
        }
        case ExprType::Unary: {
            T operandVal = evaluateNode(node->left, variables);
            switch (node->unOp) {
                case UnaryOp::Sin: return std::sin(operandVal);
                case UnaryOp::Cos: return std::cos(operandVal);
                case UnaryOp::Ln:
                    if (operandVal <= T(0)) {
                        throw std::runtime_error("Логарифм от нуля или отрицательного числа не исчеслим");
                    }
                    return std::log(operandVal);
                case UnaryOp::Exp: return std::exp(operandVal);
                default:
                    throw std::runtime_error("Неизвестная функция");
            }
        }
        default:
            throw std::runtime_error("Неизвестный тип узла?????");
    }
}

template<typename T>
T Expression<T>::evaluate(const std::map<std::string, T> &variables) const {
    return evaluateNode(root, variables);
}

template<typename T>
std::shared_ptr<typename Expression<T>::Node> Expression<T>::substituteNode(const std::shared_ptr<Node> &node,
                                                                            const std::string &var,
                                                                            const Expression<T> &value) const {
    if (!node) {
        return nullptr;
    }
    switch (node->type) {
        case ExprType::Constant:
            return std::make_shared<Node>(node->value);
        case ExprType::Variable:
            if (node->variableName == var) {
                return value.root;
            } else {
                return std::make_shared<Node>(node->variableName);
            }
        case ExprType::Binary: {
            auto leftSub = substituteNode(node->left, var, value);
            auto rightSub = substituteNode(node->right, var, value);
            return std::make_shared<Node>(node->binOp, leftSub, rightSub);
        }
        case ExprType::Unary: {
            auto operandSub = substituteNode(node->left, var, value);
            return std::make_shared<Node>(node->unOp, operandSub);
        }
        default:
            throw std::runtime_error("Неизвестный тип узла(подстановка)???");
    }
}

template<typename T>
Expression<T> Expression<T>::substitute(const std::string &var, const Expression<T> &value) const {
    Expression<T> result(0);
    result.root = substituteNode(root, var, value);
    return result;
}

template<typename T>
std::string Expression<T>::nodeToString(const std::shared_ptr<Node> &node) const {
    if (!node) return "";
    std::ostringstream oss;
    switch (node->type) {
        case ExprType::Constant:
            oss << node->value;
            break;
        case ExprType::Variable:
            oss << node->variableName;
            break;
        case ExprType::Binary:
            oss << "(" << nodeToString(node->left);
            switch (node->binOp) {
                case BinaryOp::Add:      oss << " + "; break;
                case BinaryOp::Subtract: oss << " - "; break;
                case BinaryOp::Multiply: oss << " * "; break;
                case BinaryOp::Divide:   oss << " / "; break;
                case BinaryOp::Power:    oss << " ^ "; break;
                default: oss << " ? "; break;
            }
            oss << nodeToString(node->right) << ")";
            break;
        case ExprType::Unary:
            switch (node->unOp) {
                case UnaryOp::Sin: oss << "sin("; break;
                case UnaryOp::Cos: oss << "cos("; break;
                case UnaryOp::Ln:  oss << "ln(";  break;
                case UnaryOp::Exp: oss << "exp("; break;
                default: oss << "func("; break;
            }
            oss << nodeToString(node->left) << ")";
            break;
        default:
            oss << "?";
            break;
    }
    return oss.str();
}

template<typename T>
std::string Expression<T>::toString() const {
    return nodeToString(root);
}

#endif
