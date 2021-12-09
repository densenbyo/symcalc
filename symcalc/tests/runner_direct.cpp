#include "runner.hpp"
#include "../app.hpp" // declares handle_expr_line

#include <sstream>

int
process(std::vector<Commands::Command> const &cmds,
        std::vector<std::string> const &exprs,
        validate_func check)
{

    for (const auto &e : exprs) {
        std::stringstream ss;
        handle_expr_line(ss, e, cmds); // defined in app.cpp
        auto lines = split(ss.str(), '\n');  // split is declared in cmdline.h
        if (!lines.empty() && lines.back().empty())
            lines.pop_back();
        check(e, lines);
    }
    return 0;
}
