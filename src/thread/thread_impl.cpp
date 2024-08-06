#include "thread_impl.hpp"

using std::function;
using std::make_unique;
using std::thread;
using std::unique_lock;

namespace tasp
{
/*------------------------------------------------------------------------------
    ThreadImpl
------------------------------------------------------------------------------*/
ThreadImpl::ThreadImpl(std::chrono::seconds timeout) noexcept
: timeout_(timeout)
{
}

//------------------------------------------------------------------------------
ThreadImpl::~ThreadImpl() noexcept
{
    if (status_ == Status::Work)
    {
        Stop();
    }
}

//------------------------------------------------------------------------------
void ThreadImpl::Worker(const function<void()> &func) noexcept
{
    worker_ = func;
}

//------------------------------------------------------------------------------
void ThreadImpl::Start() noexcept
{
    if (status_ == Status::Stop)
    {
        status_ = Status::Work;
        thread_ = make_unique<thread>(&ThreadImpl::Main, this);
    }
}

//------------------------------------------------------------------------------
void ThreadImpl::Stop() noexcept
{
    if (status_ < Status::NeedStop)
    {
        status_ = Status::NeedStop;
        condition_.notify_one();
    }

    thread_->join();
    thread_.reset(nullptr);
}

//------------------------------------------------------------------------------
void ThreadImpl::Main() noexcept
{
    while (status_ != Status::NeedStop)
    {
        worker_();

        unique_lock condition_lock{condition_mutex_};
        condition_.wait_for(condition_lock,
                            timeout_,
                            [&]()
                            {
                                return status_ != Status::Work;
                            });
    }

    status_ = Status::Stop;
}

}  // namespace tasp