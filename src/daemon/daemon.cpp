#include "tasp/daemon.hpp"

#include "daemon_impl.hpp"

using std::make_unique;

namespace tasp
{
/*------------------------------------------------------------------------------
    Daemon
------------------------------------------------------------------------------*/
Daemon::Daemon(int argc, const char **argv) noexcept
: impl_(make_unique<DaemonImpl>(argc, argv))
{
    impl_->Reload(
        [this]
        {
            Reload();
        });
}

//------------------------------------------------------------------------------
Daemon::~Daemon() noexcept = default;

//------------------------------------------------------------------------------
int Daemon::Exec() const noexcept
{
    return impl_->Exec();
}

//------------------------------------------------------------------------------
void Daemon::Reload() noexcept
{
}

}  // namespace tasp
