#include "uuid_impl.hpp"

#include <algorithm>
#include <array>
#include <random>
#include <sstream>

using std::array;
using std::generate;
using std::hex;
using std::mt19937;
using std::ostringstream;
using std::random_device;
using std::string;
using std::string_view;
using std::uniform_int_distribution;

namespace tasp
{
/*------------------------------------------------------------------------------
    UUIDImpl
------------------------------------------------------------------------------*/
UUIDImpl::UUIDImpl() noexcept
: value_(Generate())
{
}

//------------------------------------------------------------------------------
UUIDImpl::UUIDImpl(string_view uuid) noexcept
: value_(uuid)
{
}

//------------------------------------------------------------------------------
UUIDImpl::~UUIDImpl() noexcept = default;

//------------------------------------------------------------------------------
const string &UUIDImpl::Get() const noexcept
{
    return value_;
}

//------------------------------------------------------------------------------
string UUIDImpl::Generate() noexcept
{
    static random_device device{};
    static mt19937 generator{device()};

    static uniform_int_distribution<unsigned> values{0x0, 0xF};
    static uniform_int_distribution<unsigned> variant{0x8, 0xB};

    array<unsigned, 32> uuid{};
    generate(uuid.begin(),
             uuid.end(),
             [&]()
             {
                 return values(generator);
             });

    uuid[12] = 4;
    uuid[16] = variant(generator);

    ostringstream oss{};

    for (size_t pos = 0; pos < uuid.size(); ++pos)
    {
        if (pos == 8 || pos == 12 || pos == 16 || pos == 20)
        {
            oss << '-';
        }

        oss << hex << uuid.at(pos);
    }

    return oss.str();
}

}  // namespace tasp
