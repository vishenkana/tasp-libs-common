#include "tasp/http/data.hpp"

#include "data_impl.hpp"

using std::make_unique;
using std::string;
using std::string_view;

namespace tasp::http
{
/*------------------------------------------------------------------------------
    Data
------------------------------------------------------------------------------*/
Data::Data() noexcept
: impl_(make_unique<DataImpl>())
{
}

//------------------------------------------------------------------------------
Data::~Data() noexcept = default;

//------------------------------------------------------------------------------
template<typename Type> void Data::Set(const Type &data) noexcept
{
    impl_->Set(data);
}

/// \cond NOPE
template __attribute__((visibility("default"))) void Data::Set(
    const Json::Value &data) noexcept;
template __attribute__((visibility("default"))) void Data::Set(
    const string &data) noexcept;
/// \endcond

//------------------------------------------------------------------------------
template<typename Type> Type Data::Get() const noexcept
{
    return impl_->Get<Type>();
}

/// \cond NOPE
template __attribute__((visibility("default"))) Json::Value Data::Get()
    const noexcept;
template __attribute__((visibility("default"))) string
Data::Get() const noexcept;
/// \endcond

//------------------------------------------------------------------------------
size_t Data::Read(char *data, size_t length) noexcept
{
    return impl_->Read(data, length);
}

//------------------------------------------------------------------------------
void Data::Append(char *data, size_t length) noexcept
{
    impl_->Append(data, length);
}

//------------------------------------------------------------------------------
size_t Data::Length() const noexcept
{
    return impl_->Length();
}

//------------------------------------------------------------------------------
const string &Data::GetType() const noexcept
{
    return impl_->GetType();
}

//------------------------------------------------------------------------------
void Data::SetType(string_view type) noexcept
{
    impl_->SetType(type);
}

}  // namespace tasp::http
