#include "log_line.hpp"

#include <experimental/filesystem>
#include <iomanip>
#include <iterator>
#include <sstream>
#include <thread>

#include <tasp/date.hpp>

using std::any;
using std::any_cast;
using std::ostream_iterator;
using std::setw;
using std::string;
using std::string_view;
using std::stringstream;
using std::time_t;
using std::to_string;
using std::type_index;
using std::vector;

namespace fs = std::experimental::filesystem;

namespace tasp
{
/*------------------------------------------------------------------------------
    LogLine
------------------------------------------------------------------------------*/
LogLine::LogLine(const LogLevel &level,
                 const SourceLocation &location,
                 string_view format,
                 const vector<any> &params) noexcept
: timestamp_(CurrentTimestamp())
, source_(StripFilename(location.file_name()))
, line_(location.line())
, thread_id_(CurrentThreadId())
, level_(level)
, message_(CreateMessage(format, params))
{
}

/*------------------------------------------------------------------------------
    LogLine
------------------------------------------------------------------------------*/
LogLine::LogLine(LogLevel::Level level, string_view message) noexcept
: timestamp_(CurrentTimestamp())
, source_(StripFilename(SourceLocation::current().file_name()))
, line_(SourceLocation::current().line())
, thread_id_(CurrentThreadId())
, level_(level)
, message_(message)
{
}

//------------------------------------------------------------------------------
LogLine::~LogLine() noexcept = default;

//------------------------------------------------------------------------------
string LogLine::ToString() const noexcept
{
    stringstream buf{};

    const int source_width{24};
    const int line_width{4};
    const int level_width{7};

    buf << Timestamp() << " ";
    buf << setw(source_width) << Source();
    buf << setw(line_width) << Line() << " ";
    buf << ThreadId() << " ";
    buf << setw(level_width) << Level().ToString() << " ";
    buf << Message();

    return buf.str();
}

//------------------------------------------------------------------------------
const string &LogLine::Timestamp() const noexcept
{
    return timestamp_;
}

//------------------------------------------------------------------------------
const string &LogLine::Source() const noexcept
{
    return source_;
}

//------------------------------------------------------------------------------
unsigned int LogLine::Line() const noexcept
{
    return line_;
}

//------------------------------------------------------------------------------
const string &LogLine::ThreadId() const noexcept
{
    return thread_id_;
}

//------------------------------------------------------------------------------
const LogLevel &LogLine::Level() const noexcept
{
    return level_;
}

//------------------------------------------------------------------------------
const string &LogLine::Message() const noexcept
{
    return message_;
}

//------------------------------------------------------------------------------
string LogLine::CurrentTimestamp() noexcept
{
    return Date().ToString();
}

//------------------------------------------------------------------------------
string LogLine::CurrentThreadId() noexcept
{
    stringstream buf{};
    buf << "[0x" << std::this_thread::get_id() << "]";

    return buf.str();
}

//------------------------------------------------------------------------------
string LogLine::StripFilename(string_view path) noexcept
{
    auto pos{path.find_last_of('/')};

    return pos != string_view::npos ? path.substr(++pos).data() : path.data();
}

//------------------------------------------------------------------------------
string LogLine::CreateMessage(string_view format,
                              const vector<any> &params) noexcept
{
    const string format_pattern{"{}"};
    string message{format};

    for (const auto &value : params)
    {
        const auto visitor{any_visitor_.find(type_index(value.type()))};

        string text{"UNKNOWN TYPE: "};
        if (visitor != any_visitor_.cend())
        {
            text = visitor->second(value);
        }
        else
        {
            text += value.type().name();
        };

        auto pos = message.find(format_pattern);
        if (pos == string::npos)
        {
            break;
        }

        message.replace(pos, format_pattern.length(), text);
    }

    return message;
}

namespace
{
/*------------------------------------------------------------------------------
    VisitorList
------------------------------------------------------------------------------*/
/**
 * @brief Установки функции для конвертации определенного типа.
 *
 * @param func Функция для исполнения
 *
 * @return Функция для конвертации
 */
template<class Type, class Func>
inline VisitorList::value_type ToAnyVisitor(const Func &func) noexcept
{
    return {type_index{typeid(Type)},
            [func](const any &value) -> string
            {
                return func(any_cast<const Type &>(value));
            }};
}

//------------------------------------------------------------------------------
/**
 * @brief Конвертация стандартным конструктором std::string.
 *
 * @param value Значение для конвертации
 *
 * @return Строковое представление
 */
template<class Type> inline string ConvertToString(const Type &value) noexcept
{
    return string{value};
}

//------------------------------------------------------------------------------
/**
 * @brief Конвертация c использованием stringstream.
 *
 * @param value Значение для конвертации
 *
 * @return Строковое представление
 */
template<class Type> inline string ConvertByStream(const Type &value) noexcept
{
    stringstream stream{};
    stream.flush();
    stream << value;
    return stream.str();
}

//------------------------------------------------------------------------------
/**
 * @brief Конвертация вектора.
 *
 * @param value Значение для конвертации
 *
 * @return Строковое представление
 */
template<class Type>
inline string ConvertVectorToString(const vector<Type> &value) noexcept
{
    stringstream stream{};
    copy(value.begin(), value.end() - 1, ostream_iterator<Type>(stream, " "));
    stream << value.back();
    return stream.str();
}

//------------------------------------------------------------------------------
/**
 * @brief Инициализация функциями конвертации.
 *
 * @return Список функций конвертации
 */
inline VisitorList VisitorInitialization() noexcept
{
    VisitorList list = {
        ToAnyVisitor<int>(static_cast<string (&)(int)>(to_string)),
        ToAnyVisitor<unsigned>(static_cast<string (&)(unsigned)>(to_string)),
        ToAnyVisitor<float>(static_cast<string (&)(float)>(to_string)),
        ToAnyVisitor<double>(static_cast<string (&)(double)>(to_string)),
        ToAnyVisitor<size_t>(static_cast<string (&)(size_t)>(to_string)),
        ToAnyVisitor<uint16_t>(ConvertByStream<uint16_t>),
        ToAnyVisitor<int64_t>(ConvertByStream<int64_t>),
        ToAnyVisitor<char *>(ConvertToString<char *>),
        ToAnyVisitor<char const *>(ConvertToString<char const *>),
        ToAnyVisitor<string>(ConvertToString<string>),
        ToAnyVisitor<string_view>(ConvertToString<string_view>),
        ToAnyVisitor<fs::path>(ConvertToString<fs::path>),
        ToAnyVisitor<vector<string>>(ConvertVectorToString<string>),
        ToAnyVisitor<vector<int>>(ConvertVectorToString<int>),
        ToAnyVisitor<Timepoint>(
            [](const Timepoint &time_point)
            {
                return Date{time_point}.ToString();
            }),
        ToAnyVisitor<Date>(
            [](const Date &date)
            {
                return date.ToString();
            })};
    return list;
}
}  // namespace

//------------------------------------------------------------------------------
const VisitorList LogLine::any_visitor_{VisitorInitialization()};

}  // namespace tasp
