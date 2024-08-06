#include "data_impl.hpp"

using std::string;
using std::string_view;

namespace tasp::http
{
/*------------------------------------------------------------------------------
    DataImpl
------------------------------------------------------------------------------*/
DataImpl::DataImpl() noexcept = default;

//------------------------------------------------------------------------------
DataImpl::~DataImpl() noexcept = default;

//------------------------------------------------------------------------------
template<> std::string DataImpl::Get<std::string>() noexcept
{
    return data_.str();
}

//------------------------------------------------------------------------------
template<> Json::Value DataImpl::Get<Json::Value>() noexcept
{
    JSONCPP_STRING err{};
    Json::Value root;
    const Json::CharReaderBuilder builder;

    const bool res = Json::parseFromStream(builder, data_, &root, &err);
    if (!res || root.empty())
    {
        Logging::Error("Ошибка парсинга JSON: {}", err);
    }

    return root;
}

//------------------------------------------------------------------------------
size_t DataImpl::Read(char *data, size_t length) noexcept
{
    data_.read(data, static_cast<int64_t>(length));

    return static_cast<size_t>(data_.gcount());
}

//------------------------------------------------------------------------------
void DataImpl::Append(char *data, size_t length) noexcept
{
    data_.write(data, static_cast<int64_t>(length));
}

//------------------------------------------------------------------------------
size_t DataImpl::Length() const noexcept
{
    return data_.str().length();
}

//------------------------------------------------------------------------------
const string &DataImpl::GetType() const noexcept
{
    return type_;
}

//------------------------------------------------------------------------------
void DataImpl::SetType(string_view type) noexcept
{
    type_ = type;
}

}  // namespace tasp::http
