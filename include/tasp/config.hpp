/**
 * @file
 * @brief Интерфейсы для работы с конфигурационным файлом.
 */
#ifndef TASP_CONFIG_HPP_
#define TASP_CONFIG_HPP_

#include <experimental/filesystem>
#include <string_view>

namespace fs = std::experimental::filesystem;

namespace tasp
{

class ConfigImpl;
class ConfigGlobalImpl;

/**
 * @brief Интерфейс для работы с конфигурационным файлом.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Config final
{
public:
    /**
     * @brief Конструктор для создания нового конфигурационного файла.
     */
    Config() noexcept;

    /**
     * @brief Конструктор для открытия существующего файла.
     *
     * Считывает информацию из конфигурационного файла.
     *
     * @param path Путь к конфигурационному файлу
     */
    explicit Config(const fs::path &path) noexcept;

    /**
     * @brief Деструктор.
     *
     * Если в конфигурационный файл вносились изменения необходимо вызвать
     * функцию сохранения.
     */
    ~Config() noexcept;

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
    [[nodiscard]] Type Get(std::string_view path,
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
    void Set(std::string_view path, const Type &value) noexcept;

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
    [[nodiscard]] const fs::path &GetPath() const noexcept;

    /**
     * @brief Сохранение значений в конфигурационный файл.
     *
     * @return Результат сохранения файла
     */
    [[nodiscard]] bool Save() const noexcept;

    /**
     * @brief Перезагрузка конфигурационного файла.
     *
     * Конфигурационный файл перечитывает. Вся старая информация удаляется.
     * Если в конфигурационный файл вносились изменения необходимо вызвать
     * функцию сохранения.
     */
    void Reload() noexcept;

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
    [[nodiscard]] std::string String() const noexcept;

    Config(const Config &) = delete;
    Config(Config &&) = delete;
    Config &operator=(const Config &) = delete;
    Config &operator=(Config &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ConfigImpl> impl_;
};

/**
 * @brief Интерфейс для работы с глобальным конфигурационным файлом.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 *
 * Глобальный конфигурационный файл загружается один раз. Загружает переданный
 * в конструкторе конфигурационный файл или, в случае пустого значения, ищет
 * конфигурационный файл НАЗВАНИЕ_ПРОГРАММЫ.yml по пути /etc/tasp
 * или в директории с программой.
 */
class [[gnu::visibility("default")]] ConfigGlobal final
{
public:
    /**
     * @brief Запрос ссылки на глобальный конфигурационный файл.
     *
     * Файл считывается при первом вызове функции.
     *
     * В случае передачи пустого пути ищет конфигурационный файл в стандартных
     * местах размещения.
     *
     * @param path Полный путь к конфигурационному файлу
     *
     * @return Ссылка на глобальный конфигурационный файл
     */
    static ConfigGlobal &Instance(const fs::path &path = {}) noexcept;

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
    [[nodiscard]] Type Get(std::string_view path,
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
    [[nodiscard]] std::string String() const noexcept;

    ConfigGlobal(const ConfigGlobal &) = delete;
    ConfigGlobal(ConfigGlobal &&) = delete;
    ConfigGlobal &operator=(const ConfigGlobal &) = delete;
    ConfigGlobal &operator=(ConfigGlobal &&) = delete;

private:
    /**
     * @brief Конструктор.
     *
     * В случае передачи пустого пути ищет конфигурационный файл в стандартных
     * местах размещения.
     *
     * @param path Полный путь к конфигурационному файлу
     */
    explicit ConfigGlobal(const fs::path &path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ConfigGlobal() noexcept;

    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ConfigGlobalImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_CONFIG_HPP_
