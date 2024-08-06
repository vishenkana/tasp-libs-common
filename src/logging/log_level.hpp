/**
 * @file
 * @brief Классы для работы с уровнями сообщения.
 */
#ifndef TASP_LOGGING_LOG_LEVEL_HPP_
#define TASP_LOGGING_LOG_LEVEL_HPP_

#include <string>
#include <unordered_map>

namespace tasp
{

/**
 * @brief Класс представления уровней сообщений.
 */
class LogLevel final
{
public:
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

    /**
     * @brief Конструктор с параметром в формате перечисления.
     *
     * @param level Уровень сообщения
     */
    explicit LogLevel(Level level = Level::Info) noexcept;

    /**
     * @brief Конструктор с текстовым представлением уровня.
     *
     * @param level Уровень сообщения
     */
    explicit LogLevel(std::string_view level) noexcept;

    /**
     * @brief Конструктор копирования.
     *
     * @param level Уровень сообщения
     */
    LogLevel(const LogLevel &level);

    /**
     * @brief Деструктор.
     */
    ~LogLevel() noexcept = default;

    /**
     * @brief Установка нового значения в формате перечисления.
     *
     * @param level Уровень сообщения
     */
    void Set(Level level) noexcept;

    /**
     * @brief Установка нового значения текстовым представлением.
     *
     * @param level Уровень сообщения
     */
    void Set(std::string_view level) noexcept;

    /**
     * @brief Запрос уровня сообщения в в формате перечисления.
     *
     * @return Уровень сообщения
     */
    [[nodiscard]] Level Get() const noexcept;

    /**
     * @brief Запрос уровня сообщения в текстовом представлении.
     *
     * @return Уровень сообщения
     */
    [[nodiscard]] std::string ToString() const noexcept;

    /**
     * @brief Оператор равно.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator==(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор не равно.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator!=(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор меньше.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator<(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор меньше или равно.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator<=(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор больше.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator>(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор больше или равно.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator>=(const LogLevel &rhs) const noexcept;

    /**
     * @brief Оператор равно для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator==(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Оператор не равно для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator!=(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Оператор меньше для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator<(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Оператор меньше или равно для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator<=(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Оператор больше для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator>(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Оператор больше или равно для перечисления.
     *
     * @param rhs Уровень сообщения для сравнения
     *
     * @return Результат сравнение
     */
    bool operator>=(LogLevel::Level rhs) const noexcept;

    /**
     * @brief Тип данных для списка уровней сообщений с их текстовым
     * представлением.
     */
    using LevelList = std::unordered_map<Level, std::string>;

    LogLevel(LogLevel &&) = delete;
    LogLevel &operator=(const LogLevel &) = delete;
    LogLevel &operator=(LogLevel &&) = delete;

private:
    /**
     * @brief Уровень сообщения.
     */
    Level value_{Level::None};

    /**
     * @brief Список уровней сообщений с их текстовым представлением.
     */
    static const LevelList level_string_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_LOG_LEVEL_HPP_
