/**
 * @file
 * @brief Интерфейсы для логирования информации.
 */
#ifndef TASP_LOGGING_HPP_
#define TASP_LOGGING_HPP_

#include <any>
#include <experimental/source_location>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

/**
 * @brief Убирание использования std::experimental.
 */
using SourceLocation = std::experimental::source_location;

namespace tasp
{

class LoggingImpl;
class FormatWithLocationImpl;

/**
 * @brief Формат строки с локацией.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] FormatWithLocation final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param format Формат
     * @param location Локация
     */
    // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
    FormatWithLocation(
        const char *format,
        const SourceLocation &location = SourceLocation::current()) noexcept;

    /**
     * @brief Деструктор.
     */
    ~FormatWithLocation() noexcept;

    /**
     * @brief Запрос формата строки.
     *
     * @return Формат
     */
    [[nodiscard]] std::string Format() const noexcept;

    /**
     * @brief Запрос локации.
     *
     * @return Локация
     */
    [[nodiscard]] SourceLocation Location() const noexcept;

    FormatWithLocation(const FormatWithLocation &) = delete;
    FormatWithLocation(FormatWithLocation &&) = delete;
    FormatWithLocation &operator=(const FormatWithLocation &) = delete;
    FormatWithLocation &operator=(FormatWithLocation &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<FormatWithLocationImpl> impl_;
};

/**
 * @brief Интерфейс единого вывода сообщений во все типы логов.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 *
 * Из глобального конфигурационного файла загружается список типов активных
 * логов. И настраиваются по параметрам из конфигурационного файла.
 *
 * Каждое переданное сообщение в лог передается на запись в каждый активный лог.
 */
class [[gnu::visibility("default")]] Logging final
{
public:
    /**
     * @brief Запрос ссылки на единый вывод сообщений.
     *
     * @return Ссылка на единый вывод сообщений
     */
    static Logging &Instance() noexcept;

    /**
     * @brief Список уровней сообщений.
     */
    enum class Level : std::uint8_t
    {
        Debug = 0,   /*!< Отладка */
        Info = 1,    /*!< Информация */
        Warning = 2, /*!< Предупреждение */
        Error = 3,   /*!< Ошибка */
        None = 4     /*!< Без уровня/неизвестный */
    };

    /**                                                                    \
     * @brief Структура с функцией для вывода сообщений в логи типа level. \
     */
    // clang-format off
#define generate_nested_class(level)                                    \
    /**                                                                 \
     * @brief Вывод сообщения в логи.                                   \
     *                                                                  \
     * @param format Формат сообщения для вывода с местами для вставки  \
     * параметров                                                       \
     * @param params Параметры для добавления в формат                  \
     */                                                                 \
    template<typename... Args>                                          \
    static inline void level(FormatWithLocation format, Args &&...params) \
    {                                                                   \
        Instance().Print(Level::level,                                  \
                         format.Location(),                             \
                         format.Format(),                                \
                         {std::any(std::forward<Args>(params))...});    \
    }

    generate_nested_class(Error)
    generate_nested_class(Warning)
    generate_nested_class(Info)
    generate_nested_class(Debug)
    generate_nested_class(None)

#undef generate_nested_class

    /**
     * @brief Вывод сообщения в логи любого типа.
     *
     * @param level Уровень сообщения
     * @param location Информация о месте вызова функции логирования
     * @param format Формат сообщения для вывода с местами для вставки
     * параметров
     * @param params Параметры для добавления в формат
     */
    void Print(Level level = Level::Info,
            const SourceLocation &location = SourceLocation::current(),
            std::string_view format = "{}",
            const std::vector<std::any> &params = {}) noexcept;
    // clang-format on

    /**
     * @brief Перезагрузка логирования.
     *
     * Информация из конфигурационного файла перечитывается. И открываются логи
     * с новыми параметрами.
     */
    void Reload() noexcept;

    Logging(const Logging &) = delete;
    Logging(Logging &&) = delete;
    Logging &operator=(const Logging &) = delete;
    Logging &operator=(Logging &&) = delete;

private:
    /**
     * @brief Конструктор.
     */
    Logging() noexcept;

    /**
     * @brief Деструктор.
     */
    ~Logging() noexcept;

    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<LoggingImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_HPP_
