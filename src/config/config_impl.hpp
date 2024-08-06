/**
 * @file
 * @brief Реализация интерфейсов для работы с конфигурационным файлом.
 */
#ifndef TASP_CONFIG_CONFIG_IMPL_HPP_
#define TASP_CONFIG_CONFIG_IMPL_HPP_

#include <yaml-cpp/yaml.h>

#include <experimental/filesystem>
#include <mutex>

#include "tasp/logging.hpp"

namespace fs = std::experimental::filesystem;

namespace tasp
{

class ConfigNodePath;

/**
 * @brief Реализация интерфейса для работы с конфигурационным файлом.
 */
class ConfigImpl
{
public:
    /**
     * @brief Конструктор создания нового конфигурационного файла.
     */
    ConfigImpl() noexcept;

    /**
     * @brief Конструктор с открытием конфигурационного файла.
     *
     * Считывает информацию из конфигурационного файла.
     *
     * @param path Путь к конфигурационному файлу
     */
    explicit ConfigImpl(const fs::path &path) noexcept;

    /**
     * @brief Деструктор.
     *
     * Если в конфигурационный файл вносились изменения необходимо вызвать
     * функцию сохранения.
     */
    virtual ~ConfigImpl() noexcept;

    /**
     * @brief Запрос значения из конфигурационного файла.
     *
     * В шаблоне передается тип значения в конфигурационном файле.
     * Путь состоит из последовательных значений ключей разделенных знаком ".".
     * В случае отсутствия значения в конфигурационном файле или при
     * несовпадении типа, переданного в шаблоне, с типом в конфигурационном
     * файле возвращается значение по умолчанию.
     *
     * @param path Полный путь к значению
     * @param default_value Значение по умолчанию
     *
     * @return Значение из конфигурационного файла или переданное значение по
     * умолчанию
     */
    template<typename Type>
    Type Get(const ConfigNodePath &path,
             const Type &default_value = {}) const noexcept;

    /**
     * @brief Установка нового значения в конфигурационном файле.
     *
     * В шаблоне передается тип значения в конфигурационном файле.
     * Путь состоит из последовательных значений ключей разделенных знаком ".".
     * После изменения необходимо вызвать функцию сохранения файла для фиксации
     * изменений.
     *
     * @param path Полный путь к значению
     * @param value Новое значение
     */
    template<typename Type>
    void Set(const ConfigNodePath &path, const Type &value) noexcept;

    /**
     * @brief Установка нового полного пути к конфигурационному файлу.
     *
     * @param path Путь к конфигурационному файлу
     */
    void SetPath(const fs::path &path) noexcept;

    /**
     * @brief Запрос расположения конфигурационного файла.
     *
     * @return Полный путь к конфигурационному файлу
     */
    const fs::path &GetPath() const noexcept;

    /**
     * @brief Сохранение значений в конфигурационный файл.
     *
     * @return Результат сохранения файла
     */
    bool Save() const noexcept;

    /**
     * @brief Перезагрузка конфигурационного файла.
     *
     * Конфигурационный файл перечитывает. Вся старая информация удаляется.
     * Если в конфигурационный файл вносились изменения необходимо вызвать
     * функцию сохранения.
     */
    void Reload() noexcept;

    /**
     * @brief Проверка корректного открытия и парсинга конфигурационного файла.
     *
     * @return Результат проверки
     */
    bool Valid() const noexcept;

    /**
     * @brief Загрузка сторонних конфигурационных файлов из параметра include.
     *
     * В конфигурационном файле может присутствовать пути к другим
     * конфигурационным файлам в параметре include. При вызове этой функции
     * перечисленные конфигурационные файлы будет последовательно прочитаны и
     * слиты с текущим.
     */
    void Include() noexcept;

    /**
     * @brief Конвертация конфигурационного файла в текстовый формат.
     *
     * @return Конфигурационный файл в виде текста
     */
    virtual std::string String() const noexcept;

    ConfigImpl(const ConfigImpl &) = delete;
    ConfigImpl(ConfigImpl &&) = delete;
    ConfigImpl &operator=(const ConfigImpl &) = delete;
    ConfigImpl &operator=(ConfigImpl &&) = delete;

private:
    /**
     * @brief Слитие двух веток конфигурационного файла.
     *
     * @param destination Ветка для данных
     * @param source Ветка с данными для слития
     */
    void MergeNode(YAML::Node &destination, const YAML::Node &source) noexcept;

    /**
     * @brief Запрос параметра из конфигурационного файла.
     *
     * @param path Разобранный путь к параметру
     *
     * @return Запрошенный параметр или пустой элемент в случае отсутствия
     * параметра
     */
    YAML::Node GetNode(const ConfigNodePath &path) const noexcept;

    /**
     * @brief Создание параметра в конфигурационном файле.
     *
     * Создаются все отсутствующие ключи в пути.
     *
     * @param path Разобранный путь к параметру
     *
     * @return Созданный параметр
     */
    YAML::Node CreateNode(const ConfigNodePath &path) noexcept;

    /**
     * Полный путь к конфигурационному файлу.
     */
    fs::path fullpath_;

    /**
     * Корневой элемент конфигурационного файла.
     */
    YAML::Node document_;
};

/**
 * @brief Реализация интерфейса глобального конфигурационного файла.
 *
 * Глобальный конфигурационный файл загружается один раз. Загружает переданный
 * в конструкторе конфигурационный файл или, в случае пустого значения, ищет
 * конфигурационный файл НАЗВАНИЕ_ПРОГРАММЫ.yml по пути /etc/tasp
 * или в директории с программой.
 */
class ConfigGlobalImpl final : public ConfigImpl
{
public:
    /**
     * @brief Конструктор.
     *
     * В случае передачи пустого пути ищет конфигурационный файл в стандартных
     * местах размещения.
     *
     * @param path Полный путь к конфигурационному файлу
     */
    explicit ConfigGlobalImpl(const fs::path &path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ConfigGlobalImpl() noexcept override;

    /**
     * @brief Запрос значения из конфигурационного файла.
     *
     * В шаблоне передается тип значения в конфигурационном файле.
     * Путь состоит из последовательных значений ключей разделенных знаком ".".
     * В случае отсутствия значения в конфигурационном файле или при
     * несовпадении типа, переданного в шаблоне, с типом в конфигурационном
     * файле возвращается значение по умолчанию.
     *
     * @param path Полный путь к значению
     * @param default_value Значение по умолчанию
     *
     * @return Значение из конфигурационного файла или переданное значение по
     * умолчанию
     */
    template<typename Type>
    Type Get(const ConfigNodePath &path,
             const Type &default_value = {}) const noexcept;

    /**
     * @brief Перезагрузка конфигурационного файла.
     *
     * Конфигурационный файл перечитывает. Вся старая информация удаляется.
     */
    void Reload() noexcept;

    /**
     * @brief Конвертация конфигурационного файла в текстовый формат.
     *
     * @return Конфигурационный файл в виде текста
     */
    [[nodiscard]] std::string String() const noexcept override;

    ConfigGlobalImpl(const ConfigGlobalImpl &) = delete;
    ConfigGlobalImpl(ConfigGlobalImpl &&) = delete;
    ConfigGlobalImpl &operator=(const ConfigGlobalImpl &) = delete;
    ConfigGlobalImpl &operator=(ConfigGlobalImpl &&) = delete;

private:
    /**
     * @brief Добавление стандартных параметров для каждой программы.
     *
     * Параметры:
     * program.name - название программы
     * program.path - пусть запуска программы
     * program.systemd - тип запуска. Пользовательская программа или сервис.
     */
    void InitBaseParams() noexcept;

    /**
     * @brief Поиск конфигурационного файла в стандартных местах размещения.
     *
     * @return Полный путь к найденному файлу
     */
    static fs::path DefaultPath() noexcept;

    /**
     * @brief Мьютекс для синхронизации чтения и перезагрузки файла.
     */
    mutable std::mutex mutex_;
};

/**
 * @brief Класс пути к параметрам в конфигурационном файле.
 */
class ConfigNodePath final
{
public:
    /**
     * @brief Конструктор.
     *
     * Путь состоит из последовательных значений ключей разделенных знаком ".".
     * Путь разделяется на элементы и помещается в вектор для последовательного
     * доступа по ключам конфигурационного файла.
     *
     * @param path Полный путь к значению
     */
    explicit ConfigNodePath(std::string_view path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ConfigNodePath() noexcept;

    /**
     * @brief Запрос количества ключей в пути.
     *
     * @return Количество ключей
     */
    [[nodiscard]] std::size_t Size() const noexcept;

    /**
     * @brief Запрос пути в виде строки.
     *
     * @return Путь в виде строки
     */
    [[nodiscard]] std::string String() const noexcept;

    // Выключается проверка стиля наименований для этого участка, т.к. это
    // методы для использования в стандартной библиотеке c++.
    // NOLINTBEGIN(readability-identifier-naming)

    /**
     * @brief Запрос итератора первого элемента.
     *
     * @return Итератор
     */
    [[nodiscard]] std::vector<std::string>::const_iterator begin()
        const noexcept;

    /**
     * @brief Запрос итератора последнего элемента.
     *
     * @return Итератор
     */
    [[nodiscard]] std::vector<std::string>::const_iterator end() const noexcept;
    // NOLINTEND(readability-identifier-naming)

    ConfigNodePath(const ConfigNodePath &) = delete;
    ConfigNodePath(ConfigNodePath &&) = delete;
    ConfigNodePath &operator=(const ConfigNodePath &) = delete;
    ConfigNodePath &operator=(ConfigNodePath &&) = delete;

private:
    /**
     * @brief Полный путь.
     */
    std::string path_;

    /**
     * Символ разделителя ключей в пути.
     */
    static const char delimiter_{'.'};

    /**
     * Последовательность ключей для доступа к параметру.
     */
    std::vector<std::string> keys_;
};

/*------------------------------------------------------------------------------
    ConfigImpl
------------------------------------------------------------------------------*/
template<typename Type>
Type ConfigImpl::Get(const ConfigNodePath &path,
                     const Type &default_value) const noexcept
{
    Type value{default_value};

    try
    {
        const YAML::Node node{GetNode(path)};
        if (node.Type() != YAML::NodeType::Undefined)
        {
            value = node.as<Type>();
        }
    }
    catch (const std::exception &exception)
    {
        Logging::Error(
            "Ошибка при конвертации значения из конфигурационного файла: {}",
            exception.what());
    }

    return value;
}

//------------------------------------------------------------------------------
template<typename Type>
void ConfigImpl::Set(const ConfigNodePath &path, const Type &value) noexcept
{
    try
    {
        YAML::Node node{GetNode(path)};

        if (!node)
        {
            node.reset();
            node = CreateNode(path);
        }

        node = value;
    }
    catch (const std::exception &exception)
    {
        Logging::Error(
            "Ошибка при добавлении нового элемента в конфигурационный файл: {}",
            exception.what());
    }
}

/*------------------------------------------------------------------------------
    ConfigGlobalImpl
------------------------------------------------------------------------------*/
template<typename Type>
Type ConfigGlobalImpl::Get(const ConfigNodePath &path,
                           const Type &default_value) const noexcept
{
    const std::scoped_lock lock{mutex_};
    return ConfigImpl::Get(path, default_value);
}

}  // namespace tasp

// Выключается проверка стиля наименований для этого участка, т.к. это
// расширение для библиотеки.
// NOLINTBEGIN(readability-identifier-naming)
namespace YAML
{
/**
 * @brief Класс добавляющий библиотеке YAML возможность работы с типом данных
 * filesystem::path.
 */
template<> struct convert<fs::path>
{
    /**
     * @brief Конвертация значения типа filesystem::path к типу YAML::Node.
     *
     * @param rhs Значение для конвертации
     *
     * @return Конвертированное значение
     */
    static Node encode(const fs::path &rhs)
    {
        return Node{rhs.string()};
    }

    /**
     * @brief Конвертация значения типа YAML::Node к типу filesystem::path.
     *
     * @param node Значение для конвертации
     * @param rhs Конвертированное значение
     *
     * @return Результат конвертации
     */
    static bool decode(const Node &node, fs::path &rhs)
    {
        if (!node.IsScalar())
        {
            return false;
        }
        rhs = node.Scalar();
        return true;
    }
};

/**
 * @brief Класс добавляющий библиотеке YAML возможность работы с типом данных
 * std::chrono::seconds.
 */
template<> struct convert<std::chrono::seconds>
{
    /**
     * @brief Конвертация значения типа std::chrono::seconds к типу YAML::Node.
     *
     * @param rhs Значение для конвертации
     *
     * @return Конвертированное значение
     */
    static Node encode(const std::chrono::seconds &rhs)
    {
        return Node(rhs.count());
    }

    /**
     * @brief Конвертация значения типа YAML::Node к типу std::chrono::seconds.
     *
     * @param node Значение для конвертации
     * @param rhs Конвертированное значение
     *
     * @return Результат конвертации
     */
    static bool decode(const Node &node, std::chrono::seconds &rhs)
    {
        if (!node.IsScalar())
        {
            return false;
        }

        const std::string &input = node.Scalar();
        if (input.front() == '-')
        {
            return false;
        }

        uint64_t value{0};

        try
        {
            value = stoul(input);
        }
        catch (const std::exception &exception)
        {
            return false;
        }

        rhs = std::chrono::seconds(value);

        return true;
    }
};

}  // namespace YAML
// NOLINTEND(readability-identifier-naming)

#endif  // TASP_CONFIG_CONFIG_IMPL_HPP_
