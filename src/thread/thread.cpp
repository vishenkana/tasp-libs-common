#include "tasp/thread.hpp"

#include "thread_impl.hpp"

using std::make_unique;

namespace tasp
{
/*------------------------------------------------------------------------------
    Thread
------------------------------------------------------------------------------*/
Thread::Thread(std::chrono::seconds timeout) noexcept
: impl_(make_unique<ThreadImpl>(timeout))
{
    impl_->Worker(
        [this]
        {
            Worker();
        });

    impl_->Start();
}

//------------------------------------------------------------------------------
Thread::~Thread() noexcept
{
    impl_->Stop();
}

//------------------------------------------------------------------------------
void Thread::Worker() noexcept
{
}

}  // namespace tasp