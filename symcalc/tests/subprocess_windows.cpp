#include "subprocess.hpp"

#include <string>
#include <stdexcept>
#include <system_error>

#include <windows.h>
#include <io.h>
#include <fcntl.h>

namespace subprocess {

void throw_windows_error(std::string msg)
{
    DWORD dwErrVal = GetLastError();
    std::error_code ec(dwErrVal, std::system_category());
    throw std::system_error(ec, std::move(msg));
}

void throw_stdc_error(std::string msg)
{
    std::error_code ec(errno, std::generic_category());
    throw std::system_error(ec, std::move(msg));
}

void file_closer::operator()(void *f) {
    // ignore error
    CloseHandle(f);
}


/*
 * NOTE: Cannot use fgets() with the pipe end wrapped in FILE*, because
 *       when the pipe breaks, the EOF/error is swallowed and fgets blocks
 *       indefinitely.
 */
Process::Process(const char *cmd)
{
    SECURITY_ATTRIBUTES saAttr;

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE infd[2], outfd[2];

    // Create a pipe for the child process's STDOUT.
    if (!CreatePipe(&outfd[0], &outfd[1], &saAttr, 0))
        throw_windows_error("StdoutRd CreatePipe");
    m_stdout.reset(outfd[0]);

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(outfd[0], HANDLE_FLAG_INHERIT, 0))
        throw_windows_error("Stdout SetHandleInformation");

    // Create a pipe for the child process's STDIN.
    if (!CreatePipe(&infd[0], &infd[1], &saAttr, 0))
        throw_windows_error("Stdin CreatePipe");
    m_stdin.reset(infd[1]);

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if (!SetHandleInformation(infd[1], HANDLE_FLAG_INHERIT, 0))
        throw_windows_error("Stdin SetHandleInformation");

    // Create the child process.
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

// Set up members of the PROCESS_INFORMATION structure.

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

// Set up members of the STARTUPINFO structure.
// This structure specifies the STDIN and STDOUT handles for redirection.

    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    siStartInfo.hStdOutput = outfd[1];
    siStartInfo.hStdInput = infd[0];
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

// Create the child process.

    bSuccess = CreateProcess(NULL,
                             const_cast<LPSTR>(cmd),           // command line
                             NULL,          // process security attributes
                             NULL,          // primary thread security attributes
                             TRUE,          // handles are inherited
                             0,             // creation flags
                             NULL,          // use parent's environment
                             NULL,          // use parent's current directory
                             &siStartInfo,  // STARTUPINFO pointer
                             &piProcInfo);  // receives PROCESS_INFORMATION
    if (!bSuccess)
        throw_windows_error(std::string("CreateProcess: ") + cmd);
    CloseHandle(piProcInfo.hThread);

    m_handle = piProcInfo.hProcess;
}

int Process::wait()
{
    DWORD res;

    res = WaitForSingleObject(m_handle, INFINITE);
    if (res == -1)
        throw_windows_error("WaitForSingleObject");
    else if (res != 0)
        throw std::runtime_error("WaitForSingleObject failed: " + std::to_string(res));

    DWORD errcode;
    BOOL ok = GetExitCodeProcess(m_handle, &errcode);
    if (ok == FALSE)
        throw_windows_error("GetExitCodeProcess");

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
    DWORD num_written;
    if (!WriteFile(m_stdin.get(), data, size, &num_written, nullptr))
        throw_windows_error("WriteFile");
    else if (num_written != size)
        throw std::runtime_error("short write");
}

int Process::read(char *buf, int buflen)
{
    DWORD num_read;
    if (!ReadFile(m_stdout.get(), buf, buflen, &num_read, nullptr))
        throw_windows_error("ReadFile");

    return num_read;
}

} // namespace subprocess
