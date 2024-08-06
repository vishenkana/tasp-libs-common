#include "command_impl.hpp"

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

#include <array>
#include <sstream>

#include "tasp/logging.hpp"

using std::array;
using std::error_code;
using std::getline;
using std::make_unique;
using std::string;
using std::string_view;
using std::stringstream;
using std::system_category;
using std::unique_ptr;
using std::vector;

namespace tasp
{
/*------------------------------------------------------------------------------
    Command
------------------------------------------------------------------------------*/
CommandImpl::CommandImpl(string_view command, string_view args) noexcept
: command_(command)
{
    ChangeArgs(args);
}

//------------------------------------------------------------------------------
CommandImpl::~CommandImpl() noexcept = default;

//------------------------------------------------------------------------------
void CommandImpl::ChangeArgs(string_view args) noexcept
{
    args_.clear();

    args_.emplace_back(command_);
    ParseArgs(args);

    args_count_ = static_cast<int>(args_.size());
}

//------------------------------------------------------------------------------
unique_ptr<CommandResult> CommandImpl::Exec(string_view extra_args) noexcept
{
    args_.erase(args_.begin() + args_count_, args_.end());
    ParseArgs(extra_args);

    Logging::Debug("Выполнение команды: {}", args_);

    vector<char *> argv{};
    argv.reserve(args_.size() + 1);
    for (auto &param : args_)
    {
        argv.emplace_back(param.data());
    }
    argv.emplace_back(nullptr);

    Pipe stdout{STDOUT_FILENO};
    Pipe stderr{STDERR_FILENO};

    const pid_t pid{fork()};
    if (pid == 0)
    {
        stderr.Child();
        stdout.Child();

        const int ret{execvp(command_.data(), argv.data())};

        stderr << "Отсутствует программа " << command_;

        _exit(ret);
    }

    int code{0};
    waitpid(pid, &code, 0);

    return make_unique<CommandResultImpl>(code, stdout.Data(), stderr.Data());
}

//------------------------------------------------------------------------------
void CommandImpl::ParseArgs(string_view args) noexcept
{
    if (!args.empty())
    {
        stringstream stream{args.data()};

        string param;
        while (getline(stream, param, ' '))
        {
            args_.emplace_back(param.data());
        }
    }
}

/*------------------------------------------------------------------------------
    CommandResultImpl
------------------------------------------------------------------------------*/
CommandResultImpl::CommandResultImpl(int code,
                                     string_view stdout,
                                     string_view stderr) noexcept
: code_(code)
, stdout_(stdout)
, stderr_(stderr)
{
    if (code_ != 0)
    {
        Logging::Error("Ошибка при выполнении команды: {}", stderr_);
    }
}

//------------------------------------------------------------------------------
CommandResultImpl::~CommandResultImpl() noexcept = default;

//------------------------------------------------------------------------------
int CommandResultImpl::Code() const noexcept
{
    return code_;
}

//------------------------------------------------------------------------------
const string &CommandResultImpl::StdOut() const noexcept
{
    return stdout_;
}

//------------------------------------------------------------------------------
const string &CommandResultImpl::StdErr() const noexcept
{
    return stderr_;
}

/*------------------------------------------------------------------------------
    Pipe
------------------------------------------------------------------------------*/
Pipe::Pipe(int type) noexcept
: type_(type)
{
    const int res{pipe2(pipes_.data(), O_CLOEXEC)};
    if (res != 0)
    {
        const error_code error{errno, system_category()};
        Logging::Error("Ошибка при создании конвееров: {}", error.message());
    }
}

//------------------------------------------------------------------------------
void Pipe::Child() noexcept
{
    close(pipes_[0]);
    dup2(pipes_[1], type_);
    close(pipes_[1]);
}

//------------------------------------------------------------------------------
string Pipe::Data() noexcept
{
    close(pipes_[1]);

    array<char, 512> buffer{};
    string data;
    ssize_t size{0};

    while ((size = read(pipes_[0], buffer.data(), buffer.size())) > 0)
    {
        data.append(buffer.data(), static_cast<uint64_t>(size));
    }

    if (data.back() == '\n')
    {
        data.pop_back();
    }

    return data;
}

//------------------------------------------------------------------------------
Pipe &Pipe::operator<<(string_view text)
{
    const ssize_t size{write(type_, text.data(), text.length() + 1)};
    if (size == -1)
    {
        const error_code error{errno, system_category()};
        Logging::Error("Ошибка записи в поток: {}", error.message());
    }

    return *this;
}

}  // namespace tasp
