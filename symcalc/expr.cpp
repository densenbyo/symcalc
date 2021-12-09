#include "expr.hpp"
#include "expr_impl.hpp"
#include "tokenizer.hpp"
#include <stdexcept>
#include <stack>
#include <sstream>
#include <iostream>

using namespace std;

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree

void check_left(int r_paren_left, bool awaiting_left_parent, bool awaiting_operator){
    if (r_paren_left != 0) {
        throw parse_error("! unclosed parenthesis");
    } else if (awaiting_left_parent) {
        throw parse_error("! awaited left parent after function");
    } else if (awaiting_operator) {
        throw parse_error("! last token is operator");
    }
}

void parse(const std::string& expression, std::deque<Token>& postfix) {
    std::stack<Token> operators;
    basic_stringstream<char> input(expression);
    auto tokenizer = Tokenizer(input);
    Token prev_token;
    Token last;

    int leftP = 0;
    bool needed_operand = false;
    bool needed_operator = false;
    bool leftP_exist = false;

    while (true) {
        try {
            auto token = tokenizer.next();
            switch (token.id) {
                case TokenId::End:
                    check_left(leftP, leftP_exist, needed_operand);
                    while (!operators.empty()) {
                        postfix.push_back(operators.top());
                        operators.pop();
                    }
                    break;
                case TokenId::Number:
                    postfix.push_back(token);
                    if (prev_token.id == TokenId::RParen) throw parse_error("no operator present after closing bracket");
                    if (needed_operand == true) {
                        needed_operand = false;
                        needed_operator = false;
                    } else {
                        if (needed_operator) throw parse_error("no operator present");
                        else needed_operator = true;
                    }

                    if (leftP_exist) leftP_exist = false;
                    break;
                case TokenId::Identifier:
                    postfix.push_back(token);
                    if (prev_token.id == TokenId::RParen) throw parse_error("no operator present after closing bracket");
                    if (needed_operand == true) {
                        needed_operand = false;
                        needed_operator = false;
                    } else {
                        if (needed_operator) throw parse_error("no operator present");
                        else needed_operator = true;
                    }

                    if (leftP_exist) leftP_exist = false;
                    break;
                case TokenId::Plus: case TokenId::Minus: case TokenId::Multiply: case TokenId::Divide: case TokenId::Power:
                    while ((!operators.empty()) && (operators.top().id != TokenId::LParen) &&
                           ((operators.top().is_sin() || operators.top().is_cos() ||
                             operators.top().is_log()) ||
                            (operators.top().is_binary_op() && (token.op_precedence() == operators.top().op_precedence() &&
                                                                operators.top().associativity() == Associativity::Left)) ||
                            (operators.top().is_binary_op() && (token.op_precedence() < operators.top().op_precedence())))) {
                        postfix.push_back(operators.top());
                        operators.pop();
                    }

                    operators.push(token);
                    needed_operand = true;
                    break;
                case TokenId::LParen:

                    if (!postfix.empty() && postfix.back().id == TokenId::Identifier) {
                        last = postfix.back();
                        if (last.number == 0.0 && (last.is_sin() || last.is_log()|| last.is_cos())) {
                            last.number = 1.0; // indicates that this token is the function
                            operators.push(last);
                            postfix.pop_back();
                            operators.push(token);
                            if (needed_operator) needed_operator = false;
                        } else if (!operators.empty() && (operators.top().id  == TokenId::Plus || operators.top().id == TokenId::Minus ||
                                                        operators.top().id == TokenId::Multiply ||operators.top().id == TokenId::Divide ||
                                                        operators.top().id == TokenId::Power || operators.top().id == TokenId::LParen)) {
                            operators.push(token);
                        } else {
                            throw unknown_function_exception("unknown function");
                        }
                    } else {
                        operators.push(token);
                    }
                    ++leftP;
                    leftP_exist = true;
                    break;
                case TokenId::RParen:

                    while (!operators.empty() && operators.top().id != TokenId::LParen) {
                        postfix.push_back(operators.top());
                        operators.pop();
                    }

                    if (operators.empty()) throw parse_error("no left parenthesis found");
                    else { operators.pop(); --leftP; } //pop the left parenthesis
                    break;
            }
            prev_token = token;
            if (token.id == TokenId::End) break;
        } catch (const tokenize_error &e) {
            throw e;
        }
    }
}

expr create_expression_tree(const std::string& expression) {
    deque<Token> postfix;
    parse(expression, postfix);
    stack<expr> output;
    while (!postfix.empty()) {
        auto token = postfix.front(); postfix.pop_front();
        expr rhs, lhs;
        switch (token.id) {
            case TokenId::Number:
                output.push(expr::number(token.number));
                break;
            case TokenId::Identifier:
                if (token.is_sin() && token.number == 1.0) {
                    auto temp = output.top();
                    output.pop();
                    output.push(sin(temp));
                } else if (token.is_cos() && token.number == 1.0) {
                    auto temp = output.top();
                    output.pop();
                    output.push(cos(temp));
                } else if (token.is_log() && token.number == 1.0) {
                    auto temp = output.top();
                    output.pop();
                    output.push(log(temp));
                } else {
                    output.push(expr::variable(token.identifier));
                }
                break;
            case TokenId::Plus:
                lhs = output.top(); output.pop();
                rhs = output.top(); output.pop();
                output.push(rhs + lhs);
                break;
            case TokenId::Minus:
                lhs = output.top(); output.pop();
                rhs = output.top(); output.pop();
                output.push(rhs - lhs);
                break;
            case TokenId::Multiply:
                lhs = output.top(); output.pop();
                rhs = output.top(); output.pop();
                output.push(rhs * lhs);
                break;
            case TokenId::Divide:
                lhs = output.top(); output.pop();
                rhs = output.top(); output.pop();
                output.push(rhs / lhs);
                break;
            case TokenId::Power:
                lhs = output.top(); output.pop();
                rhs = output.top(); output.pop();
                output.push(pow(rhs, lhs));
                break;
        }
    }
    return output.top();
}

bool operator==(const expr &a, const expr &b) {
    if (a->equals(b->shared_from_this().operator*())) {
        return true;
    } else {
        return false;
    }
}

std::ostream& operator<<(std::ostream &os, const expr &e) {
    e->write(os, expr::WriteFormat::Prefix);
    return os;
}

std::ostream &operator<<(std::ostream &os, const fmt_expr &fmt) {
    fmt.e->write(os, fmt.fmt);
    return os;
}

expr expr::number(double n) {
    return make_shared<exprs::number>(exprs::number(n));
}

expr expr::variable(std::string name) {
    return make_shared<exprs::variable>(exprs::variable(name));
}

expr operator+(expr a, expr b) {
    return make_shared<exprs::expr_plus>(exprs::expr_plus(a, b));
}

expr operator-(expr a, expr b) {
    return make_shared<exprs::expr_minus>(exprs::expr_minus(a, b));
}

expr operator*(expr a, expr b) {
    return make_shared<exprs::expr_multiply>(exprs::expr_multiply(a, b));
}

expr operator/(expr a, expr b) {
    return make_shared<exprs::expr_divide>(exprs::expr_divide(a, b));
}

expr pow(expr m, expr e) {
    return make_shared<exprs::expr_pow>(exprs::expr_pow(m, e));
}

expr sin(expr e) {
    return make_shared<exprs::expr_sin>(exprs::expr_sin(e));
}

expr cos(expr e) {
    return make_shared<exprs::expr_cos>(exprs::expr_cos(e));
}

expr log(expr e) {
    return make_shared<exprs::expr_log>(exprs::expr_log(e));
}





