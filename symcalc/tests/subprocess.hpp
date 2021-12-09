#pragma once

#include <cstdio> // fclose
#include <memory>
#include <string>

namespace subprocess {

struct file_closer {void operator()(void *f);};

class Process {
public:
    Process(const char *cmd);
    ~Process();

    void close_stdin() {m_stdin.reset(nullptr);}
    void close_stdout() {m_stdout.reset(nullptr);}
    void write(const char *data, size_t size);
    void write(std::string const &str) {write(str.data(), str.size());}
    int read(char *buf, int buflen);

    int wait();
private:
    using file_type = void; // HANDLE / int

    // unique_ptr would be enough, but cannot have dynamic deleter
    std::unique_ptr<file_type, file_closer> m_stdin;
    std::unique_ptr<file_type, file_closer> m_stdout;
    union {
        int m_pid; // pid_t
        void *m_handle; // HANDLE
    };
    bool m_waited = false;
};


} // namespace subprocess
