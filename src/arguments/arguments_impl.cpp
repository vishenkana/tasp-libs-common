#include "arguments_impl.hpp"

#include <algorithm>

#include <gsl/span>

using std::find;
using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    ProgramArgumentsImpl
------------------------------------------------------------------------------*/
ProgramArgumentsImpl::ProgramArgumentsImpl(int argc, const char **argv) noexcept
{
    auto args{gsl::span<const char *>(argv, static_cast<size_t>(argc))};

    arguments_.reserve(static_cast<size_t>(argc));
    arguments_.assign(args.begin(), args.end());
}

//------------------------------------------------------------------------------
ProgramArgumentsImpl::~ProgramArgumentsImpl() noexcept = default;

//------------------------------------------------------------------------------
const string &ProgramArgumentsImpl::Get(string_view argument) const noexcept
{
    auto element{find(arguments_.begin(), arguments_.end(), argument)};
    if (element != arguments_.end() && (element + 1) != arguments_.end())
    {
        return *element;
    }

    static const string empty_string{};
    return empty_string;
}

//------------------------------------------------------------------------------
bool ProgramArgumentsImpl::Exists(string_view argument) const noexcept
{
    auto element{find(arguments_.begin(), arguments_.end(), argument)};
    return element != arguments_.end();
}

}  // namespace tasp
