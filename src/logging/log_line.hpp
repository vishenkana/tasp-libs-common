/**
 * @file
 * @brief Классы представления данных для вывода в лог.
 */
#ifndef TASP_LOGGING_LOG_LINE_HPP_
#define TASP_LOGGING_LOG_LINE_HPP_

#include <any>
#include <experimental/source_location>
#include <functional>
#include <string_view>
#include <typeindex>
#include <vector>

#include "log_level.hpp"

/**
 * @brief Убирание использования std::experimental.
 */
using SourceLocation = std::experimental::source_location;

namespace tasp
{

/**
 * @brief Тип данных для списка типов данных поддерживаемых для вывода в лог с
 * функциями преобразования их в текстовое представление
 */
using VisitorList =
    std::unordered_map<std::type_index,
                       std::function<std::string(const std::any &)>>;

/**
 * @brief Класс для подготовки и хранения данных для вывода в лог.
 */
class LogLine final
{
public:
    /**
     * @brief Конструктор.
     *
     * Преобразует полученные данные в формат вывода в лог.
     *
     * @param level Уровень сообщения
     * @param location Информация о месте вызова функции логирования
     * @param format Формат сообщения для вывода с местами для вставки
     * параметров
     * @param params Параметры для добавления в формат
     */
    LogLine(const LogLevel &level,
            const SourceLocation &location,
            std::string_view format,
            const std::vector<std::any> &params) noexcept;

    /**
     * @brief Конструктор с передачей сразу полного сообщения.
     *
     * @param level Уровень сообщения
     * @param message Сообщение для вывода
     */
    LogLine(LogLevel::Level level, std::string_view message) noexcept;

    /**
     * @brief Конструктор копирования.
     *
     * @param line Данные для вывода
     */
    LogLine(const LogLine &line) = default;

    /**
     * @brief Деструктор.
     */
    ~LogLine() noexcept;

    /**
     * @brief Формирование строки для вывода в лог со всеми полями.
     *
     * Формат вывода:
     *  - Дата и время в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС
     *  - Название файла
     *  - Номер строки
     *  - Идентификатор потока в формате [0xНОМЕР_ПОТОКА]
     *  - Уровень сообщения
     *  - Сообщение
     *
     * @return Сформированная строка
     */
    [[nodiscard]] std::string ToString() const noexcept;

    /**
     * @brief Запрос даты и времени формирования сообщения в лог.
     *
     * @return Дата и время в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС
     */
    [[nodiscard]] const std::string &Timestamp() const noexcept;

    /**
     * @brief Запрос названия файла в котором произошел вызов функции добавления
     * сообщения в лог.
     *
     * @return Название файла
     */
    [[nodiscard]] const std::string &Source() const noexcept;

    /**
     * @brief Запрос номер строки вызова функции добавления сообщения в лог.
     *
     * @return Номер строки
     */
    [[nodiscard]] unsigned int Line() const noexcept;

    /**
     * @brief Запрос идентификатора потока из которого была вызвана функция
     * добавления сообщения в лог.
     *
     * @return Идентификатор потока в формате [0xНОМЕР_ПОТОКА]
     */
    [[nodiscard]] const std::string &ThreadId() const noexcept;

    /**
     * @brief Запрос уровня сообщения.
     *
     * @return Уровень сообщения
     */
    [[nodiscard]] const LogLevel &Level() const noexcept;

    /**
     * @brief Запрос сформированного сообщения.
     *
     * @return Сообщение
     */
    [[nodiscard]] const std::string &Message() const noexcept;

    LogLine(LogLine &&) = delete;
    LogLine &operator=(const LogLine &) = delete;
    LogLine &operator=(LogLine &&) = delete;

private:
    /**
     * @brief Получение текущего времени.
     *
     * @return Дата и время в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС
     */
    static std::string CurrentTimestamp() noexcept;

    /**
     * @brief Получение идентификатора текущего процесса.
     *
     * @return Идентификатор потока в формате [0xНОМЕР_ПОТОКА]
     */
    static std::string CurrentThreadId() noexcept;

    /**
     * @brief Вырезание пути из переданного значения оставляя только имя файла.
     *
     * @param path Путь к исходному файлу
     *
     * @return Имя файла
     */
    static std::string StripFilename(std::string_view path) noexcept;

    /**
     * @brief Формирования сообщения на основе формата.
     *
     * В формате могут присутствовать последовательности символов {}. На это
     * место вставляются значения из параметров.
     *
     * @param format Формат сообщения для вывода с местами для вставки
     * параметров
     * @param params Параметры для добавления в формат
     *
     * @return Сообщение
     */
    static std::string CreateMessage(
        std::string_view format,
        const std::vector<std::any> &params) noexcept;

    /**
     * @brief Дата и время в формате ГГГГ-ММ-ДД ЧЧ:ММ:СС.
     */
    std::string timestamp_;

    /**
     * @brief Название файла.
     */
    std::string source_;

    /**
     * @brief Номер строки.
     */
    unsigned int line_;

    /**
     * @brief Идентификатор потока в формате [0xНОМЕР_ПОТОКА].
     */
    std::string thread_id_;

    /**
     * @brief Уровень сообщения.
     */
    LogLevel level_;

    /**
     * @brief Сообщение.
     */
    std::string message_;

    /**
     * @brief Список типов данных поддерживаемых для вывода в лог с функциями
     * преобразования их в текстовое представление.
     */
    static const VisitorList any_visitor_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_LOG_LINE_HPP_
