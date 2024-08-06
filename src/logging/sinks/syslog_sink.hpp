/**
 * @file
 * @brief Классы для вывода сообщений в syslog.
 */
#ifndef TASP_LOGGING_SINKS_SYSLOG_SINK_HPP_
#define TASP_LOGGING_SINKS_SYSLOG_SINK_HPP_

#include "../log_line.hpp"
#include "sink.hpp"

namespace tasp
{

/**
 * @brief Реализация вывода сообщений в syslog.
 */
class SyslogSink final : public Sink
{
public:
    /**
     * @brief Конструктор.
     *
     * @param config_path Путь к параметрам лога в конфигурационном файле
     */
    explicit SyslogSink(std::string_view config_path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~SyslogSink() noexcept override;

    /**
     * @brief Тип данных для списка уровней сообщений с их аналогами в syslog.
     */
    using LevelList = std::unordered_map<LogLevel::Level, int>;

    SyslogSink(const SyslogSink &) = delete;
    SyslogSink(SyslogSink &&) = delete;
    SyslogSink &operator=(const SyslogSink &) = delete;
    SyslogSink &operator=(SyslogSink &&) = delete;

private:
    /**
     * @brief Реализация вывода сообщения в лог.
     *
     * @param line Данные для вывода
     */
    void PrintImpl(const LogLine &line) noexcept override;

    /**
     * @brief Конвертация внутреннего уровня сообщения в формат syslog.
     *
     * @param level Уровень сообщения
     *
     * @return Уровень сообщения в формате syslog
     */
    static inline int ConvertLevelToSyslog(const LogLevel &level) noexcept;

    /**
     * @brief Список уровней сообщений с их текстовым представлением.
     */
    static const LevelList level_syslog_;

    /**
     * @brief Название программы.
     */
    std::string name_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_SINKS_SYSLOG_SINK_HPP_
