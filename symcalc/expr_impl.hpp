#pragma once

#include "expr.hpp"
#include <iosfwd>

using namespace std;
namespace exprs {
    class number : public expr_base {
        friend class expr;

    public:
        explicit number(double);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        double num;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class variable : public expr_base {
        friend class expr;

    public:
        explicit variable(std::string);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        std::string var;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class expr_plus : public expr_base {
        friend class expr;

    public:
        explicit expr_plus(const expr&, const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first, second;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;

    };

    class expr_minus : public expr_base {
        friend class expr;

    public:
        explicit expr_minus(const expr&, const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first, second;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class expr_multiply : public expr_base {
        friend class expr;

    public:
        explicit expr_multiply(const expr&, const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:

        expr first, second;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class expr_divide : public expr_base {
        friend class expr;

    public:
        explicit expr_divide(const expr&, const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first, second;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class expr_pow : public expr_base {
        friend class expr;

    public:
        explicit expr_pow(const expr&, const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first, second;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };

    class expr_sin : public expr_base {
        friend class expr;

    public:
        explicit expr_sin(const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };


    class expr_cos : public expr_base {
        friend class expr;

    public:
        explicit expr_cos(const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };


    class expr_log : public expr_base {
        friend class expr;

    public:
        explicit expr_log(const expr&);

        double evaluate(const variable_map_t &variables) const override;

        expr derive(const std::string &variable) const override;

        expr simplify() const override;

    private:
        expr first;

        bool equals(const expr_base &b) const override;

        void write(std::ostream &out, WriteFormat fmt) const override;
    };
}