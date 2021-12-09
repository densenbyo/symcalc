#include "subprocess.hpp"

#include <string>
#include <algorithm>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <stdexcept>
#include <system_error>

namespace subprocess {

void throw_stdc_error(std::string msg)
{
    std::error_code ec(errno, std::generic_category());
    throw std::system_error(ec, std::move(msg));
}

void file_closer::operator()(void *f) {
    // ignore error
    close(reinterpret_cast<intptr_t>(f));
}

Process::Process(const char *cmd)
{
    int infd[2], outfd[2];
    pipe(infd);
    pipe(outfd);

    m_pid = fork();
    if (m_pid < 0) {
        throw_stdc_error("fork");
    } else if (m_pid == 0) { // child
        close(infd[1]);
        close(outfd[0]);
        close(0);
        close(1);
        dup2(infd[0], 0);
        dup2(outfd[1], 1);
        execlp("/bin/sh", "/bin/sh", "-c", cmd, nullptr);
        _exit(1);
    }
    close(infd[0]);
    close(outfd[1]);

    m_stdin.reset(reinterpret_cast<void*>(infd[1]));
    m_stdout.reset(reinterpret_cast<void*>(outfd[0]));
}

int Process::wait()
{
    int errcode = 1;
    int res = waitpid(m_pid, &errcode, 0);
    if (res == -1)
        throw_stdc_error("waitpid");
    m_waited = true;
    return errcode;
}

Process::~Process()
{
    close_stdin();
    close_stdout();
    if (!m_waited)
        wait();
}

void Process::write(const char *data, size_t size)
{
    int res = ::write(reinterpret_cast<intptr_t>(m_stdin.get()), data, size);
    if (res == -1)
        throw_stdc_error("write");
    else if (res != size)
        throw std::runtime_error("short write");
}

int Process::read(char *buf, int buflen)
{
    int res = ::read(reinterpret_cast<intptr_t>(m_stdout.get()), buf, buflen);
    if (res == -1)
        throw_stdc_error("read");
    return res;
}


} // namespace subprocess
