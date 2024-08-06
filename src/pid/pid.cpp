#include "tasp/pid.hpp"

#include "pid_impl.hpp"

using std::make_unique;

namespace tasp
{
/*------------------------------------------------------------------------------
    PID
------------------------------------------------------------------------------*/
PID::PID() noexcept
: impl_(make_unique<PIDImpl>())
{
}

//------------------------------------------------------------------------------
PID::~PID() noexcept = default;

//------------------------------------------------------------------------------
int PID::Current() const noexcept
{
    return impl_->Current();
}

//------------------------------------------------------------------------------
int PID::Running() const noexcept
{
    return impl_->Running();
}

//------------------------------------------------------------------------------
bool PID::SecondLaunch() const noexcept
{
    return impl_->SecondLaunch();
}

}  // namespace tasp
