/**
 * @file
 * @brief Базовые классы для работы с типами логов.
 */
#ifndef TASP_LOGGING_SINKS_SINK_HPP_
#define TASP_LOGGING_SINKS_SINK_HPP_

#include <memory>

#include "../log_line.hpp"

namespace tasp
{

/**
 * @brief Абстрактный класс лога.
 */
class Sink
{
public:
    /**
     * @brief Конструктор.
     *
     * @param config_path Путь к параметрам лога в конфигурационном файле
     */
    explicit Sink(std::string_view config_path) noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~Sink() noexcept;

    /**
     * @brief Вывод сообщения в лог.
     *
     * @param line Данные для вывода
     */
    void Print(const LogLine &line) noexcept;

    /**
     * @brief Запрос максимального уровня сообщений выводимых в лог.
     *
     * @return Уровень сообщений
     */
    [[nodiscard]] const LogLevel &Level() const noexcept;

    /**
     * @brief Запрос пути к параметрам лога в конфигурационном файле.
     *
     * @return Путь к параметрам
     */
    [[nodiscard]] const std::string &ConfigPath() const noexcept;

    Sink(const Sink &) = delete;
    Sink(Sink &&) = delete;
    Sink &operator=(const Sink &) = delete;
    Sink &operator=(Sink &&) = delete;

private:
    /**
     * @brief Реализация вывода сообщения в лог.
     *
     * @param line Данные для вывода
     */
    virtual void PrintImpl(const LogLine &line) noexcept = 0;

    /**
     * @brief Путь к параметрам лога в конфигурационном файле.
     */
    std::string config_path_;

    /**
     * @brief Максимальный уровень сообщений выводимый в лог.
     */
    LogLevel level_;
};

/**
 * @brief Фабрика для создания конкретных логов.
 */
class SinkFactory final
{
public:
    /**
     * @brief Конструктор.
     */
    SinkFactory() noexcept;

    /**
     * @brief Деструктор.
     */
    ~SinkFactory() noexcept;

    /**
     * @brief Функция создания лога.
     *
     * Поддерживаемые типы:
     *   file - вывод в файл
     *   console - вывод в консоль
     *   syslog - вывод в syslog
     *
     * Если тип лога не поддерживается, возвращается nullptr.
     *
     * @param type Тип лога
     * @param config_path Путь к параметрам лога в конфигурационном файле
     *
     * @return Созданный лог или nullptr
     */
    std::unique_ptr<Sink> Create(std::string_view type,
                                 std::string_view config_path) const noexcept;

    SinkFactory(const SinkFactory &) = delete;
    SinkFactory(SinkFactory &&) = delete;
    SinkFactory &operator=(const SinkFactory &) = delete;
    SinkFactory &operator=(SinkFactory &&) = delete;

private:
    /**
     * @brief Типы поддерживаемых логов с функциями для их создания.
     */
    std::unordered_map<std::string,
                       std::function<std::unique_ptr<Sink>(std::string_view)>>
        types_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_SINKS_SINK_HPP_
