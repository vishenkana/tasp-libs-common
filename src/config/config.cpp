#include "tasp/config.hpp"

#include "config_impl.hpp"

using std::make_unique;
using std::string;
using std::string_view;

namespace tasp
{

/*------------------------------------------------------------------------------
    Config
------------------------------------------------------------------------------*/
Config::Config() noexcept
: impl_(make_unique<ConfigImpl>())
{
}

//------------------------------------------------------------------------------
Config::Config(const fs::path &path) noexcept
: impl_(make_unique<ConfigImpl>(path))
{
}

//------------------------------------------------------------------------------
Config::~Config() noexcept = default;

//------------------------------------------------------------------------------
template<typename Type>
Type Config::Get(string_view path, const Type &default_value) const noexcept
{
    return impl_->Get(ConfigNodePath(path), default_value);
}

//------------------------------------------------------------------------------
template<typename Type>
void Config::Set(string_view path, const Type &value) noexcept
{
    impl_->Set(ConfigNodePath(path), value);
}

//------------------------------------------------------------------------------
void Config::SetPath(const fs::path &path) noexcept
{
    impl_->SetPath(path);
}

//------------------------------------------------------------------------------
const fs::path &Config::GetPath() const noexcept
{
    return impl_->GetPath();
}

//------------------------------------------------------------------------------
bool Config::Save() const noexcept
{
    return impl_->Save();
}

//------------------------------------------------------------------------------
void Config::Reload() noexcept
{
    impl_->Reload();
}

//------------------------------------------------------------------------------
void Config::Include() noexcept
{
    impl_->Include();
}

//------------------------------------------------------------------------------
string Config::String() const noexcept
{
    return impl_->String();
}

/*------------------------------------------------------------------------------
    ConfigGlobal
------------------------------------------------------------------------------*/
ConfigGlobal &ConfigGlobal::Instance(const fs::path &path) noexcept
{
    static ConfigGlobal instance{path};
    return instance;
}

//------------------------------------------------------------------------------
template<typename Type>
Type ConfigGlobal::Get(string_view path,
                       const Type &default_value) const noexcept
{
    return impl_->Get(ConfigNodePath(path), default_value);
}

//------------------------------------------------------------------------------
void ConfigGlobal::Reload() noexcept
{
    impl_->Reload();
}

//------------------------------------------------------------------------------
string ConfigGlobal::String() const noexcept
{
    return impl_->String();
}

//------------------------------------------------------------------------------
ConfigGlobal::ConfigGlobal(const fs::path &path) noexcept
: impl_(make_unique<ConfigGlobalImpl>(path))
{
}

//------------------------------------------------------------------------------
ConfigGlobal::~ConfigGlobal() noexcept = default;

//------------------------------------------------------------------------------
/// \cond NOPE
#define init_template_function(type)                                        \
    template __attribute__((visibility("default"))) void Config::Set(       \
        string_view path, const type &default_value) noexcept;              \
    template __attribute__((visibility("default"))) type Config::Get(       \
        string_view path, const type &default_value) const noexcept;        \
    template __attribute__((visibility("default"))) type ConfigGlobal::Get( \
        string_view path, const type &default_value) const noexcept;
// clang-format off
init_template_function(bool)
init_template_function(int)
init_template_function(unsigned int)
init_template_function(unsigned long)
init_template_function(unsigned char)
init_template_function(unsigned short)
init_template_function(string)
init_template_function(fs::path)
init_template_function(std::vector<string>)
init_template_function(std::vector<int>)
init_template_function(std::chrono::seconds)
// clang-format on
/// \endcond

#undef init_template_function

}  // namespace tasp
