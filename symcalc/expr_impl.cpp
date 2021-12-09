#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>
using namespace std;
namespace exprs {
    number::number(double number){
        num = number;
    }

    double number::evaluate(const expr_base::variable_map_t &variables) const {
        return num;
    }

    expr number::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr number::simplify() const {
        throw logic_error("not implemented yet");
    }

    void number::write(ostream &out, expr_base::WriteFormat fmt) const {
        out << num;
    }

    bool number::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    variable::variable(string val): var(val) {}

    double variable::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr variable::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr variable::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool variable::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void variable::write(ostream &out, expr_base::WriteFormat fmt) const {
        out << var;
    }

    expr_plus::expr_plus(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_plus::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_plus::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_plus::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool expr_plus::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void expr_plus::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " +)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " + " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(+ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_minus::expr_minus(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_minus::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_minus::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_minus::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool expr_minus::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void expr_minus::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " -)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " - " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(- " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }

    }

    expr_multiply::expr_multiply(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_multiply::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_multiply::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_multiply::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool expr_multiply::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void expr_multiply::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " *)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " * " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(* " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_divide::expr_divide(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_divide::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_divide::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_divide::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool expr_divide::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void expr_divide::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " /)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " / " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(/ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_pow::expr_pow(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_pow::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_pow::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_pow::simplify() const {
        throw logic_error("not implemented yet");
    }

    bool expr_pow::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    void expr_pow::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " ^)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " ^ " << fmt_expr{second, fmt} << ") ";
        } else {
            out << "(^ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_sin::expr_sin(const expr &one) : first(one) {}

    double expr_sin::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_sin::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_sin::simplify() const {
        throw logic_error("not implemented yet");
    }

    void expr_sin::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " sin)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "sin (" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(sin " << fmt_expr{first, fmt} << ")";
        }

    }

    bool expr_sin::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    expr_cos::expr_cos(const expr &one) : first(one) {}

    double expr_cos::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_cos::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_cos::simplify() const {
        throw logic_error("not implemented yet");
    }

    void expr_cos::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " cos)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "cos (" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(cos " << fmt_expr{first, fmt} << ")";
        }
    }

    bool expr_cos::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }

    expr_log::expr_log(const expr &one) : first(one) {}

    double expr_log::evaluate(const expr_base::variable_map_t &variables) const {
        throw logic_error("not implemented yet");
    }

    expr expr_log::derive(string const &temp) const {
        throw logic_error("not implemented yet");
    }

    expr expr_log::simplify() const {
        throw logic_error("not implemented yet");
    }

    void expr_log::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " log)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "log (" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(log " << fmt_expr{first, fmt} << ")";
        }
    }

    bool expr_log::equals(const expr_base &variable) const {
        throw logic_error("not implemented yet");
    }
}