#include "tests.hpp"
#include "runner.hpp"
#include "cmdline.hpp"

#include "catch.hpp"

#if !defined(RUNNER)
# error Compile this only in runner configurations.
#endif

namespace {
    using run_cmd_check_function = std::function<void(std::string const &input, std::vector<std::string> const &lines, std::string const &expected)>;
    int run_cmd(TestCmdList const &cmd, run_cmd_check_function check) {
        std::vector<std::string> inputs;
        std::vector<std::string> expected;
        for (const auto &ie : cmd.tests) {
            inputs.push_back(ie.input);
            expected.push_back(ie.expected);
        }
        int i = 0;
        int code = process(cmd.commands, inputs, [&i,&check,&expected](const auto &expr, std::vector<std::string> lines) {
            check(expr, lines, expected[i]);
            ++i;
        });
        return code;
    }
    using run_cmd_err_check_function = std::function<void(std::string const &input, std::vector<std::string> const &lines)>;
    int run_cmd_err(TestCmdErrList const &cmd, run_cmd_err_check_function check) {
        std::vector<std::string> inputs;
        for (const auto &ie : cmd.tests) {
            inputs.push_back(ie);
        }
        auto code = process(cmd.commands, inputs, check);
        return code;
    }
}

/* We assign assertions to sections based on line number, so unfortunately the checking
 * code must be duplicated to have distinct line numbers on the CHECKs.
 */

TEST_CASE("Complex: print", "[.long][parse_write][valid]") {
    const auto &cmd = test_cmd_print_valid;
    auto errcode = run_cmd(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines, std::string const &expected) {
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        INFO("Expression: " << input);
        INFO("Expected: \"" << expected << "\", got \"" << fmt_range(lines, "\n") << "\"");
        CHECK(lines.size() == 1);
        if (!lines.empty())
            CHECK(lines[0] == expected);
    });
    CHECK(errcode == 0);
}

TEST_CASE("Complex: bonus print", "[.long][bonus_print]") {
    for (const auto &cmd : test_cmd_print_bonus) {
        auto errcode = run_cmd(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines, std::string const &expected) {
            INFO("Commands: " << fmt_range(cmd.commands, " "));
            INFO("Expression: " << input);
            INFO("Expected: \"" << expected << "\", got \"" << fmt_range(lines, "\n") << "\"");
            CHECK(lines.size() == 1);
            if (!lines.empty())
                CHECK(lines[0] == expected);
        });
        CHECK(errcode == 0);
    }
}

TEST_CASE("Complex: error handling", "[.long][errors]") {
    auto &cmd = test_cmd_print_err;
    auto errcode = run_cmd_err(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines) {
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        INFO("Expression: " << input);
        CHECK(lines.size() == 1);
        INFO("Expected error, got \"" << fmt_range(lines, "\n") << "\"");
        bool error_detected = !lines.empty() && !lines[0].empty() && lines[0][0] == '!';
        CHECK(error_detected);
    });
    CHECK(errcode == 0);
}

TEST_CASE("Complex: evaluate (valid expressions)", "[.long][evaluate][valid]") {
    for (const auto &cmd : test_cmd_evaluate_good) {
        auto errcode = run_cmd(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines, std::string const &expected) {
            INFO("Commands: " << fmt_range(cmd.commands, " "));
            INFO("Expression: " << input);
            INFO("Expected: \"" << expected << "\", got \"" << fmt_range(lines, "\n") << "\"");
            CHECK(lines.size() == 1);
            if (!lines.empty())
                CHECK(lines[0] == expected);
        });
        CHECK(errcode == 0);
    }
}

TEST_CASE("Complex: derive", "[.long][derive][valid]") {
    for (const auto &cmd : test_cmd_derive) {
        auto errcode = run_cmd(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines, std::string const &expected) {
            INFO("Commands: " << fmt_range(cmd.commands, " "));
            INFO("Expression: " << input);
            INFO("Expected: \"" << expected << "\", got \"" << fmt_range(lines, "\n") << "\"");
            CHECK(lines.size() == 1);
            if (!lines.empty())
                CHECK(lines[0] == expected);
        });
        CHECK(errcode == 0);
    }
}

TEST_CASE("Complex: simplify", "[simplify][valid]") {
    const auto &cmd = test_cmd_simplify;
    auto errcode = run_cmd(cmd, [&cmd](std::string const &input, std::vector<std::string> const &lines, std::string const &expected) {
        INFO("Commands: " << fmt_range(cmd.commands, " "));
        INFO("Expression: " << input);
        INFO("Expected: \"" << expected << "\", got \"" << fmt_range(lines, "\n") << "\"");
        CHECK(lines.size() == 1);
        if (!lines.empty())
            CHECK(lines[0] == expected);
    });
    CHECK(errcode == 0);
}
