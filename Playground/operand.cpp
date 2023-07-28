#include <iostream>
#include <stack>
#include <sstream>
#include <cstdlib>

// 计算两个操作数的结果
int calculate(int operand1, int operand2, char op) {
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            return operand1 / operand2;
        default:
            return 0; // 如果运算符无效，返回默认值
    }
}

// 判断字符是否为运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// 执行括号化表达式的计算
int evaluateExpression(const std::string& expression) {
    std::stack<char> operators;
    std::stack<int> operands;

    for (char c : expression) {
        if (c == '(') {
            operators.push(c);
        } else if (isdigit(c)) {
            // 将连续的数字字符转换为整数并入栈
            std::stringstream ss;
            ss << c;
            char nextChar = expression[expression.find(c) + 1];
            while (isdigit(nextChar)) {
                ss << nextChar;
                nextChar = expression[expression.find(nextChar) + 1];
            }
            int operand = std::atoi(ss.str().c_str());
            operands.push(operand);
        } else if (isOperator(c)) {
            operators.push(c);
        } else if (c == ')') {
            // 弹出操作符和操作数，直到遇到左括号
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();

                int operand2 = operands.top();
                operands.pop();

                int operand1 = operands.top();
                operands.pop();

                int result = calculate(operand1, operand2, op);
                operands.push(result);
            }

            // 弹出左括号
            if (!operators.empty() && operators.top() == '(') {
                operators.pop();
            }
        }
    }

    // 执行剩余的操作符和操作数的计算
    while (!operators.empty() && !operands.empty()) {
        char op = operators.top();
        operators.pop();

        int operand2 = operands.top();
        operands.pop();

        int operand1 = operands.top();
        operands.pop();

        int result = calculate(operand1, operand2, op);
        operands.push(result);
    }

    // 返回最终结果
    if (!operands.empty()) {
        return operands.top();
    } else {
        return 0; // 如果没有操作数，返回默认值
    }
}

int main() {
    std::string expression;
    std::cout << "Input: ";
    std::cin >> expression;

    int result = evaluateExpression(expression);
    std::cout << "Ans: " << result << std::endl;

    return 0;
}
