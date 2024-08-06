/**
 * @file
 * @brief Классы для вывода сообщений в консольный лог.
 */
#ifndef TASP_LOGGING_SINKS_CONSOLE_SINK_HPP_
#define TASP_LOGGING_SINKS_CONSOLE_SINK_HPP_

#include "../log_line.hpp"
#include "sink.hpp"

namespace tasp
{

/**
 * @brief Реализация вывода сообщений в консольный лог.
 */
class ConsoleSink final : public Sink
{
public:
    /**
     * @brief Конструктор.
     *
     * @param config_path Путь к параметрам лога в конфигурационном файле
     */
    explicit ConsoleSink(std::string_view config_path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ConsoleSink() noexcept override;

    ConsoleSink(const ConsoleSink &) = delete;
    ConsoleSink(ConsoleSink &&) = delete;
    ConsoleSink &operator=(const ConsoleSink &) = delete;
    ConsoleSink &operator=(ConsoleSink &&) = delete;

private:
    /**
     * @brief Реализация вывода сообщения в лог.
     *
     * @param line Данные для вывода
     */
    void PrintImpl(const LogLine &line) noexcept override;

    /**
     * @brief Преобразования уровня лога в вывод в консоль с цветом.
     *
     * @param level Уровень лога
     *
     * @return Уровень лога с цветом
     */
    static std::string ToColorLogLevel(const LogLevel &level) noexcept;
};

}  // namespace tasp

#endif  // TASP_LOGGING_SINKS_CONSOLE_SINK_HPP_
