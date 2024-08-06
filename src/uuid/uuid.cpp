#include "tasp/uuid.hpp"

#include "uuid_impl.hpp"

using std::make_unique;
using std::string;
using std::string_view;

namespace tasp
{
/*------------------------------------------------------------------------------
    UUID
------------------------------------------------------------------------------*/
UUID::UUID() noexcept
: impl_(make_unique<UUIDImpl>())
{
}

//------------------------------------------------------------------------------
UUID::UUID(string_view uuid) noexcept
: impl_(make_unique<UUIDImpl>(uuid))
{
}

//------------------------------------------------------------------------------
UUID::~UUID() noexcept = default;

//------------------------------------------------------------------------------
const string &UUID::Get() const noexcept
{
    return impl_->Get();
}

}  // namespace tasp
