#include "daemon_impl.hpp"

#include <execinfo.h>

#include <string>

#include "tasp/arguments.hpp"
#include "tasp/config.hpp"
#include "tasp/logging.hpp"

using std::array;
using std::function;
using std::make_unique;
using std::string;
using std::unique_ptr;

namespace tasp
{
/*------------------------------------------------------------------------------
    DaemonImpl
------------------------------------------------------------------------------*/
DaemonImpl::DaemonImpl(int argc, const char **argv) noexcept
{
    string config_name{};
    if (argc > 1)
    {
        const ProgramArguments arguments{argc, argv};
        config_name = arguments.Get("--config");
    }

    ConfigGlobal::Instance(config_name);
    Logging::Instance();

    pid_ = make_unique<PID>();

    sigemptyset(&sigset_);

    sigaddset(&sigset_, SIGINT);
    sigaddset(&sigset_, SIGTERM);
    sigaddset(&sigset_, SIGQUIT);
    sigaddset(&sigset_, SIGKILL);
    sigaddset(&sigset_, SIGUSR1);
    sigaddset(&sigset_, SIGUSR2);

    if (signal(SIGSEGV, DaemonImpl::SigsegvHandler) == SIG_ERR)
    {
        Logging::Error("Не удалось установить обработчик ошибок SIGSEGV");
    }

    pthread_sigmask(SIG_BLOCK, &sigset_, nullptr);
}

//------------------------------------------------------------------------------
DaemonImpl::~DaemonImpl() noexcept = default;

//------------------------------------------------------------------------------
int DaemonImpl::Exec() const noexcept
{
    Logging::Instance();
    ConfigGlobal::Instance();

    if (pid_->SecondLaunch())
    {
        Logging::Info("Повторный запуск запрещен");
        return 1;
    }

    int signo{0};
    while (true)
    {
        sigwait(&sigset_, &signo);

        Logging::Info("Получен сигнал: {}", signo);

        if (signo == SIGUSR1 || signo == SIGUSR2)
        {
            ConfigGlobal::Instance().Reload();
            Logging::Instance().Reload();

            if (reload_)
            {
                reload_();
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

//------------------------------------------------------------------------------
void DaemonImpl::Reload(const function<void()> &func) noexcept
{
    reload_ = func;
}

//------------------------------------------------------------------------------
void DaemonImpl::SigsegvHandler(int signum) noexcept
{
    Logging::Error("Программа упала! Код: {}", signum);

    array<void *, 50> trace_elems{};
    const int size = backtrace(trace_elems.data(), 50);

    const unique_ptr<char *, decltype(&std::free)> messages(
        backtrace_symbols(trace_elems.data(), size), &std::free);
    for (int i = 0; i < size; ++i)
    {
        Logging::Error("[bt]: {}", messages.get()[i]);
    }

    std::quick_exit(EXIT_FAILURE);
}

}  // namespace tasp
