#include "tasp/arguments.hpp"

#include "arguments_impl.hpp"

using std::make_unique;
using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    ProgramArguments
------------------------------------------------------------------------------*/
ProgramArguments::ProgramArguments(int argc, const char **argv) noexcept
: impl_(make_unique<ProgramArgumentsImpl>(argc, argv))
{
}

//------------------------------------------------------------------------------
ProgramArguments::~ProgramArguments() noexcept = default;

//------------------------------------------------------------------------------
const string &ProgramArguments::Get(string_view argument) const noexcept
{
    return impl_->Get(argument);
}

//------------------------------------------------------------------------------
bool ProgramArguments::Exists(string_view argument) const noexcept
{
    return impl_->Exists(argument);
}

}  // namespace tasp
