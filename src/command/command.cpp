#include "tasp/command.hpp"

#include "command_impl.hpp"

using std::make_unique;
using std::string_view;
using std::unique_ptr;

namespace tasp
{
/*------------------------------------------------------------------------------
    Command
------------------------------------------------------------------------------*/
Command::Command(string_view command, string_view args) noexcept
: impl_(make_unique<CommandImpl>(command, args))
{
}

//------------------------------------------------------------------------------
Command::~Command() noexcept = default;

//------------------------------------------------------------------------------
void Command::ChangeArgs(string_view args) noexcept
{
    impl_->ChangeArgs(args);
}

//------------------------------------------------------------------------------
unique_ptr<CommandResult> Command::Exec(string_view extra_args) noexcept
{
    return impl_->Exec(extra_args);
}

}  // namespace tasp
