#include "runner.hpp"
#include "subprocess.hpp"
#include "tests.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <string.h>
#include <cstdlib> // std::getenv

template <typename Source>
struct ReadLineAdaptor {
    static constexpr int BUFLEN = 1024;

    ReadLineAdaptor(Source src) : read(std::move(src)) {}

    bool getline(std::string &str);
    explicit operator bool() const {
        return m_eof;
    }
    std::vector<char> m_buffer;
    Source read;
    bool m_eof = false;
};

template <typename Source>
static inline ReadLineAdaptor<Source> make_readline_adaptor(Source src) {
    return ReadLineAdaptor<Source>(std::move(src));
}


#if defined(__linux__) || defined(__APPLE__)
# include <signal.h>
static struct ignore_sigpipe {
    ignore_sigpipe() {
        signal(SIGPIPE, SIG_IGN);
    }
} ignore_sigpipe_inst;
#endif

std::string to_string(Commands::Command const &e) {
    std::stringstream ss;
    ss << e;
    return ss.str();
}

int
process(std::vector<Commands::Command> const &cmds,
        std::vector<std::string> const &exprs,
        validate_func check)
{
    std::vector<std::string> a;
    std::transform(begin(cmds), end(cmds), std::back_inserter(a), to_string);

    const char *test_cmd = std::getenv("TEST_CMD");
    if (!test_cmd)
        throw std::runtime_error("TEST_CMD env variable not defined");

    std::string cmd = test_cmd;
    for (const auto &s : a)
        (cmd += ' ') += s;

    subprocess::Process p(cmd.c_str());

    auto pstdin = make_readline_adaptor([&p](char *buf, int bl){return p.read(buf, bl);});

    for (const auto &expr : exprs) {
        p.write(expr.data(), expr.size());
        p.write("\n", 1);

        std::vector<std::string> lines;
        std::string line;

        // for now just always fetch one line
        // TODO: it would be nice to have a timeout here
        if (pstdin.getline(line))
            lines.push_back(std::move(line));

        check(expr, lines);
    }
    p.close_stdin();

    p.close_stdout();
    int errcode = p.wait();

    return errcode;
}

template <typename Source>
bool ReadLineAdaptor<Source>::getline(std::string &str)
{
    if (m_eof)
        return false;

    size_t sz = m_buffer.size();
    m_buffer.resize(BUFLEN);
    int len = read(&m_buffer[sz], BUFLEN-sz);
    assert(len >= 0);
    m_buffer.resize(sz+len);
    if (len == 0) {
        m_eof = true;
        if (m_buffer.empty())
            return false;
    }

    auto e = std::find(begin(m_buffer), end(m_buffer), '\n');
    auto ee = e;
    if (e == end(m_buffer)) {
        if (m_buffer.size() == BUFLEN) // line too long
            throw std::runtime_error("line too long");
        else
            return getline(str); // tail recursive, start again
    } else {
        ++ee; // skip the \n
    }

    if (e != begin(m_buffer) && e[-1] == '\r')
        --e;

    str.assign(begin(m_buffer), e);
    m_buffer.erase(begin(m_buffer), ee);

    return true;
}
