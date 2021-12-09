#include "expr.hpp"
#include "cmdline.hpp"

#include <iostream>

using namespace std;

void process_expr(ostream &os, expr initial_expr, vector<Commands::Command> const &commands) {
    expr e = move(initial_expr);
    for (const auto &cmd : commands) {
        using namespace Commands;
        cmd.match(
            [&](Commands::Derive const &derive) {
                throw logic_error("x");
            },
            [&](Commands::Simplify const &) {
                throw logic_error("y");
            },
            [&](Commands::Evaluate const &evaluate) {
                throw logic_error("u");
            },
            [&](Commands::Print const &p) {
                os << e;
                os << endl;
            }
        );
    }
}

void handle_expr_line(ostream &os, string const &line, vector<Commands::Command> const &commands) {
    expr temp = create_expression_tree(line);
    process_expr(os, temp, commands);
}
