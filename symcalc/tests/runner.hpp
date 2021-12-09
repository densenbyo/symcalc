#pragma once

#include "cmdline.hpp"

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>

using validate_func = std::function<void(std::string const &expr, std::vector<std::string> output_lines)>;

// Return: program error code
// Will call check for each (input, [output])
int process(std::vector<Commands::Command> const &cmds,
            std::vector<std::string> const &exprs,
            validate_func check);

std::pair<int, std::vector<std::string>>
inline process(std::vector<Commands::Command> const &cmds,
               std::vector<std::string> const &exprs) {
    std::vector<std::string> lines;
    int code = process(cmds, exprs, [&lines](std::string const &, std::vector<std::string> ls) {
        std::move(begin(ls), end(ls), std::back_inserter(lines));
    });
    return {code, std::move(lines)};
}
