#include "pid_impl.hpp"

#include <unistd.h>

#include <csignal>
#include <experimental/filesystem>

using std::error_code;
using std::string;

namespace fs = std::experimental::filesystem;

namespace tasp
{
/*------------------------------------------------------------------------------
    PIDImpl
------------------------------------------------------------------------------*/
PIDImpl::PIDImpl() noexcept
: current_pid_(getpid())
{
    auto &conf{ConfigGlobal::Instance()};

    auto fullpath = conf.Get<fs::path>("pid.path");
    if (fullpath.empty())
    {
        fullpath = conf.Get<fs::path>("program.path");
        fullpath /= "pid";
    }

    string name{conf.Get<fs::path>("pid.name")};
    if (name.empty())
    {
        name = conf.Get<fs::path>("program.name");
        name += ".pid";
    }

    fullpath /= name;

    file_.SetPath(fullpath);
    if (fs::exists(fullpath))
    {
        file_.Reload();
    }

    if (!RunningExists())
    {
        Save();
    }
}

//------------------------------------------------------------------------------
PIDImpl::~PIDImpl() noexcept
{
    if (current_pid_ == running_pid_)
    {
        Remove();
    }
}

//------------------------------------------------------------------------------
int PIDImpl::Current() const noexcept
{
    return current_pid_;
}

//------------------------------------------------------------------------------
int PIDImpl::Running() const noexcept
{
    return running_pid_;
}

//------------------------------------------------------------------------------
bool PIDImpl::SecondLaunch() const noexcept
{
    return current_pid_ != running_pid_;
}

//------------------------------------------------------------------------------
bool PIDImpl::RunningExists() noexcept
{
    running_pid_ = file_.Get<int>("pid");
    if (running_pid_ == 0)
    {
        return false;
    }

    return kill(running_pid_, 0) == 0;
}

//------------------------------------------------------------------------------
bool PIDImpl::Save() noexcept
{
    running_pid_ = current_pid_;
    file_.Set("pid", current_pid_);
    return file_.Save();
}

//------------------------------------------------------------------------------
void PIDImpl::Remove() const noexcept
{
    error_code error{};
    fs::remove_all(file_.GetPath(), error);
}

}  // namespace tasp
