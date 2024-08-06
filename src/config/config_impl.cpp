#include "config_impl.hpp"

#include <unistd.h>

#include <fstream>
#include <utility>

using std::error_code;
using std::exception;
using std::getline;
using std::ofstream;
using std::scoped_lock;
using std::size_t;
using std::string;
using std::string_view;
using std::stringstream;
using std::system_category;
using std::vector;

namespace tasp
{

/*------------------------------------------------------------------------------
    ConfigImpl
------------------------------------------------------------------------------*/
ConfigImpl::ConfigImpl() noexcept
{
    try
    {
        document_ = YAML::Load("");
    }
    catch (const exception &exception)
    {
        Logging::Error(
            "Ошибка при создании пустого конфигурационного файла: {}",
            exception.what());
    }
}

//------------------------------------------------------------------------------
ConfigImpl::ConfigImpl(const fs::path &path) noexcept
{
    SetPath(path);
    Reload();
}

//------------------------------------------------------------------------------
ConfigImpl::~ConfigImpl() noexcept = default;

//------------------------------------------------------------------------------
/**
 * @brief Запрос массива строк из конфигурационного файла.
 *
 * @param path Полный путь к значению
 * @param default_value Значение по умолчанию
 *
 * @return Значение из конфигурационного файла или переданное значение по
 * умолчанию
 */
template<>
vector<string> ConfigImpl::Get<vector<string>>(
    const ConfigNodePath &path,
    const vector<string> &default_value) const noexcept
{
    vector<string> value{default_value};

    try
    {
        const YAML::Node node{GetNode(path)};
        if (node.Type() != YAML::NodeType::Undefined)
        {
            if (node.Type() == YAML::NodeType::Map)
            {
                value.clear();
                for (const auto &element : node)
                {
                    value.emplace_back(element.first.as<string>());
                }
            }
            else
            {
                value = node.as<vector<string>>();
            }
        }
    }
    catch (const exception &exception)
    {
        Logging::Error("Ошибка при при конвертации данных из конфигурационного "
                       "файла в массив: {}",
                       exception.what());
    }

    return value;
}

//------------------------------------------------------------------------------
void ConfigImpl::SetPath(const fs::path &path) noexcept
{
    fullpath_ = fs::exists(path) ? fs::canonical(path) : fs::absolute(path);
}

//------------------------------------------------------------------------------
const fs::path &ConfigImpl::GetPath() const noexcept
{
    return fullpath_;
}

//------------------------------------------------------------------------------
bool ConfigImpl::Save() const noexcept
{
    const fs::path dir{fullpath_.parent_path()};

    try
    {
        if (!fs::exists(dir))
        {
            fs::create_directories(dir);
            fs::permissions(dir, fs::perms::owner_all | fs::perms::group_all);
        }
    }
    catch (const fs::filesystem_error &error)
    {
        Logging::Error(
            "Ошибка при создании директории {}: {}", dir, error.what());
        return false;
    }

    ofstream output{fullpath_};
    if (!output)
    {
        const error_code error{errno, system_category()};
        Logging::Error("Ошибка при сохранении файла: {}", error.message());
        return false;
    }

    output << document_;
    return true;
}

//------------------------------------------------------------------------------
void ConfigImpl::Reload() noexcept
{
    Logging::Debug("Загрузка конфигурационного файла {}", fullpath_);

    try
    {
        document_ = YAML::LoadFile(fullpath_);
    }
    catch (const YAML::BadFile &exception)
    {
        document_ = YAML::Load("");
        Logging::Error("Конфигурационный {} файл не найден. {}",
                       fullpath_,
                       exception.what());
    }
    catch (const YAML::ParserException &exception)
    {
        Logging::Error("Ошибка чтения конфигурационного файла {}. {}",
                       fullpath_,
                       exception.what());
    }
    catch (const exception &exception)
    {
        Logging::Error("Ошибка при загрузке конфигурационного файла {}: {}",
                       fullpath_,
                       exception.what());
    }
}

//------------------------------------------------------------------------------
bool ConfigImpl::Valid() const noexcept
{
    bool result{false};
    try
    {
        result = !document_.IsNull();
    }
    catch (const exception &exception)
    {
        Logging::Error("Ошибка валидации конфигурационного файла {}: {}",
                       fullpath_,
                       exception.what());
        result = false;
    }

    return result;
}

//------------------------------------------------------------------------------
void ConfigImpl::Include() noexcept
{
    const fs::path config_path{GetPath().parent_path()};

    auto include{Get<vector<fs::path>>(ConfigNodePath("include"))};
    for (auto path : include)
    {
        if (!path.is_absolute())
        {
            path = config_path / path;
        }

        ConfigImpl included_config{path};
        if (included_config.Valid())
        {
            included_config.Include();
            MergeNode(document_, included_config.document_);
        }
    }
}

//------------------------------------------------------------------------------
string ConfigImpl::String() const noexcept
{
    return YAML::Dump(document_);
}

//------------------------------------------------------------------------------
void ConfigImpl::MergeNode(YAML::Node &destination,
                           const YAML::Node &source) noexcept
{
    for (const auto &node : source)
    {
        YAML::Node find_node{};
        YAML::NodeType::value type{YAML::NodeType::Null};
        try
        {
            find_node = destination[node.first.as<string>()];
            type = node.second.Type();
        }
        catch (const exception &exception)
        {
            Logging::Error("Ошибка при поиске элемента в конфигурационном "
                           "файле при слиянии: {}",
                           exception.what());
        }

        if (!find_node || type != YAML::NodeType::Map)
        {
            try
            {
                find_node = node.second;
            }
            catch (const exception &exception)
            {
                Logging::Error(
                    "Ошибка при слиянии значения в конфигурационном файле: {}",
                    exception.what());
            }
        }
        else
        {
            MergeNode(find_node, node.second);
        }
    }
}

//------------------------------------------------------------------------------
YAML::Node ConfigImpl::GetNode(const ConfigNodePath &path) const noexcept
{
    vector<YAML::Node> nodes{document_};
    nodes.reserve(path.Size());

    for (const auto &key : path)
    {
        // защита от преобразования YAML::NodeType::Sequence в
        // YAML::NodeType::Map (yaml-cpp версии 0.6.2-4): если
        // nodes.back().IsSequence, и в ней нет ключа key, то nodes.back()[key]
        // преобразует nodes.back() в Map...
        // (https://github.com/jbeder/yaml-cpp/blob/master/docs/Tutorial.md#how-sequences-turn-into-maps)
        // (https://github.com/jbeder/yaml-cpp/issues/825)

        // при отсутствии элемента функция должна возвращать пустой node, для
        // которого !node -> true и node == nullptr -> true;
        // ниже, если элемент с ключом key не найден, child превращается в
        // "zombie", и для него !child -> true; но, если получено исключение,то
        // child{}: !child -> false;
        // поэтому для предсказуемости результата в качестве пустого элемента
        // возвращаем всегда node{YAML::NodeType::Undefined}: !node -> true
        YAML::Node child{};
        try
        {
            // nodes.back().IsX() будет бросать исключения, только если
            // nodes.back() - невалидный YAML::Node (yaml-cpp версии 0.6.2-4)
            if (!nodes.back().IsMap() && !nodes.back().IsNull())
            {
                throw exception();
            }
            // nodes.back()[key] будет бросать исключения, только если
            // nodes.back() - невалидный YAML::Node (yaml-cpp версии 0.6.2-4)
            child = nodes.back()[key];
        }
        catch (const exception &exception)
        {
            Logging::Error(
                "Ошибка при поиске элемента {} в конфигурационном файле: "
                "недопустимая структура родителя узла {}",
                path.String(),
                key);
            return YAML::Node{YAML::NodeType::Undefined};
        }

        // проверяем child на "zombie"
        if (!child)
        {
            return YAML::Node{YAML::NodeType::Undefined};
        }

        nodes.push_back(child);
    }

    return nodes.back();
}

//------------------------------------------------------------------------------
YAML::Node ConfigImpl::CreateNode(const ConfigNodePath &path) noexcept
{
    vector<YAML::Node> nodes{document_};
    nodes.reserve(path.Size());

    for (const auto &key : path)
    {
        YAML::Node child{};
        try
        {
            if (!nodes.back().IsMap() &&
                !nodes.back().IsNull())  // последовательности (sequences) не
                                         // допускаются
            {
                throw exception();
            }
            child = nodes.back()[key];
        }
        catch (const exception &exception)
        {
            Logging::Error("Ошибка при создании нового значения {} в "
                           "конфигурационном файле: недопустимая структура "
                           "родителя узла {}",
                           path.String(),
                           key);
            return YAML::Node{YAML::NodeType::Undefined};
        }

        if (!child)
        {
            try
            {
                child = YAML::Node(YAML::NodeType::Null);
            }
            catch (const exception &exception)
            {
                Logging::Error("Ошибка при создании нового значения {} в "
                               "конфигурационном файле: недопустимая структура "
                               "узла {}",
                               path.String(),
                               key);
                return YAML::Node{YAML::NodeType::Undefined};
            }
        }

        nodes.push_back(child);
    }

    return nodes.back();
}

/*------------------------------------------------------------------------------
    ConfigGlobalImpl
------------------------------------------------------------------------------*/
ConfigGlobalImpl::ConfigGlobalImpl(const fs::path &path) noexcept
{
    const fs::path config_path{path.empty() ? DefaultPath() : path};

    SetPath(config_path);
    ConfigImpl::Reload();

    InitBaseParams();
    Include();
}

//------------------------------------------------------------------------------
ConfigGlobalImpl::~ConfigGlobalImpl() noexcept = default;

//------------------------------------------------------------------------------
void ConfigGlobalImpl::Reload() noexcept
{
    const scoped_lock lock{mutex_};

    ConfigImpl::Reload();

    InitBaseParams();
    Include();
}

//------------------------------------------------------------------------------
string ConfigGlobalImpl::String() const noexcept
{
    const scoped_lock lock{mutex_};
    return ConfigImpl::String();
}

//------------------------------------------------------------------------------
void ConfigGlobalImpl::InitBaseParams() noexcept
{
    const fs::path program{fs::canonical(program_invocation_name)};

    Set(ConfigNodePath("program.name"), program.filename());
    Set(ConfigNodePath("program.path"), program.parent_path());
    Set(ConfigNodePath("program.systemd"), getppid() == 1);
}

//------------------------------------------------------------------------------
fs::path ConfigGlobalImpl::DefaultPath() noexcept
{
    const fs::path program{fs::canonical(program_invocation_name)};
    const fs::path program_name{program.filename()};
    const fs::path program_path{program.parent_path()};

    auto create_config_path = [&program_name](const fs::path &path) -> fs::path
    {
        fs::path config_path{path / program_name};
        config_path.replace_extension(".yml");
        return config_path;
    };

    fs::path config_path{create_config_path("/etc/tasp")};
    if (!fs::exists(config_path))
    {
        config_path = create_config_path(program_path);
    }

    return config_path;
}

/*------------------------------------------------------------------------------
    ConfigNodePath
------------------------------------------------------------------------------*/
ConfigNodePath::ConfigNodePath(string_view path) noexcept
: path_(path)
{
    string element;
    stringstream stream{path.data()};

    while (getline(stream, element, delimiter_))
    {
        keys_.push_back(element);
    }
}

//------------------------------------------------------------------------------
ConfigNodePath::~ConfigNodePath() noexcept = default;

//------------------------------------------------------------------------------
size_t ConfigNodePath::Size() const noexcept
{
    return keys_.size();
}

//------------------------------------------------------------------------------
std::string ConfigNodePath::String() const noexcept
{
    return path_;
}

// Выключается проверка стиля наименований для этого участка, т.к. это
// методы для использования в стандартной библиотеке c++.
// NOLINTBEGIN(readability-identifier-naming)
//------------------------------------------------------------------------------
vector<string>::const_iterator ConfigNodePath::begin() const noexcept
{
    return keys_.cbegin();
}

//------------------------------------------------------------------------------
vector<string>::const_iterator ConfigNodePath::end() const noexcept
{
    return keys_.cend();
}
// NOLINTEND(readability-identifier-naming)

}  // namespace tasp
