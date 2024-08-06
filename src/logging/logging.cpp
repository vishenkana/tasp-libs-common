#include "tasp/logging.hpp"

#include "logging_impl.hpp"

using std::any;
using std::make_unique;
using std::string;
using std::string_view;
using std::vector;

namespace tasp
{
/*------------------------------------------------------------------------------
    Logging
------------------------------------------------------------------------------*/
Logging &Logging::Instance() noexcept
{
    static Logging instance{};
    return instance;
}

//------------------------------------------------------------------------------
Logging::Logging() noexcept
: impl_(make_unique<LoggingImpl>())
{
}

//------------------------------------------------------------------------------
Logging::~Logging() noexcept = default;

//------------------------------------------------------------------------------
void Logging::Print(Level level,
                    const SourceLocation &location,
                    string_view format,
                    const vector<any> &params) noexcept
{
    impl_->Print(LogLine(LogLevel(static_cast<LogLevel::Level>(level)),
                         location,
                         format,
                         params));
}

//------------------------------------------------------------------------------
void Logging::Reload() noexcept
{
    impl_->Reload();
}

/*------------------------------------------------------------------------------
    FormatWithLocation
------------------------------------------------------------------------------*/
FormatWithLocation::FormatWithLocation(const char *format,
                                       const SourceLocation &location) noexcept
: impl_(make_unique<FormatWithLocationImpl>(format, location))
{
}

//------------------------------------------------------------------------------
FormatWithLocation::~FormatWithLocation() noexcept = default;

//------------------------------------------------------------------------------
string FormatWithLocation::Format() const noexcept
{
    return impl_->Format();
}

//------------------------------------------------------------------------------
SourceLocation FormatWithLocation::Location() const noexcept
{
    return impl_->Location();
}

}  // namespace tasp
