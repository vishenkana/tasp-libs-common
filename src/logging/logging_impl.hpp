/**
 * @file
 * @brief Реализация интерфейсов для логирования информации.
 */
#ifndef TASP_LOGGING_LOGGING_IMPL_HPP_
#define TASP_LOGGING_LOGGING_IMPL_HPP_

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

#include "log_line.hpp"
#include "sinks/sink.hpp"

namespace tasp
{
/**
 * @brief Реализация интерфейса единого вывода сообщений во все типы логов.
 *
 * Из глобального конфигурационного файла загружается список типов активных
 * логов. И настраиваются по параметрам из конфигурационного файла.
 *
 * Каждое переданное сообщение в лог передается на запись в каждый активный лог.
 */
class LoggingImpl final
{
public:
    /**
     * @brief Конструктор.
     */
    LoggingImpl() noexcept;

    /**
     * @brief Деструктор.
     */
    ~LoggingImpl() noexcept;

    /**
     * @brief Вывод сообщения в логи.
     *
     * Функция только добавляет сообщение в очередь. Сам вывод сообщения
     * происходит в потоке обработки и реализована в функции @ref PrintImpl
     *
     * @param line Данные для вывода
     */
    void Print(const LogLine &line) noexcept;

    /**
     * @brief Перезагрузка логирования.
     *
     * Информация из конфигурационного файла перечитывается. И открываются логи
     * с новыми параметрами.
     *
     * Функция меняет только статус о необходимости перезагрузки. Сама
     * перезагрузка происходит в потоке обработки и реализована в функции
     * @ref ReloadImpl
     */
    void Reload() noexcept;

    LoggingImpl(const LoggingImpl &) = delete;
    LoggingImpl(LoggingImpl &&) = delete;
    LoggingImpl &operator=(const LoggingImpl &) = delete;
    LoggingImpl &operator=(LoggingImpl &&) = delete;

private:
    /**
     * @brief Статусу для потока обработки.
     */
    enum class Status : std::uint8_t
    {
        Work = 0, /*!< Поток работает и выводит сообщения */
        NeedReload = 1, /*!< Поток нуждается в перезагрузке */
        NeedStop = 2, /*!< Потоку необходимо остановиться */
        Stop = 3 /*!< Поток остановлен */
    };

    /**
     * @brief Потоковая функция читающая очередь сообщений и выводящая их в
     * открытые логи.
     */
    void Worker() noexcept;

    /**
     * @brief Смена текущего статуса для обработки. После смены отправляет
     * сигнал потоку о необходимо обработать новый статус.
     *
     * После установки статуса Status::NeedStop все другие смены статуса
     * игнорируются.
     *
     * @param status Новый статус
     */
    void ChangeStatus(Status status) noexcept;

    /**
     * @brief Потоковая функция вывода сообщений в логи.
     *
     * Реализация всей логики вывода в лог.
     */
    void PrintImpl() noexcept;

    /**
     * @brief Потоковая функция перезагрузки логирования.
     *
     * Реализация всей логики перезагрузки.
     */
    void ReloadImpl() noexcept;

    /**
     * @brief Мьютекс для синхронизации чтения/записи и перезагрузки логов.
     */
    std::mutex mutex_;

    /**
     * @brief Фабрика для создания объектов логирования.
     */
    SinkFactory factory_;

    /**
     * @brief Интервал между записью данных в лог.
     */
    std::chrono::seconds timeout_{};

    /**
     * @brief Список открытых логов.
     */
    std::vector<std::unique_ptr<Sink>> sinks_;

    /**
     * @brief Список сообщений для вывода в лог.
     */
    std::queue<LogLine> messages_;

    /**
     * @brief Поток обработки сообщений.
     */
    std::unique_ptr<std::thread> thread_{nullptr};

    /**
     * @brief Текущий статус для потока обработки.
     */
    std::atomic<Status> status_{Status::NeedReload};

    /**
     * @brief Условная переменная для блокировки потока обработки сообщений,
     * пока не пройдет таймаут для отправки сообщений в лог или не придет сигнал
     * об остановке логирования.
     */
    std::condition_variable condition_;

    /**
     * @brief Мьютекс для условной переменной.
     */
    std::mutex condition_mutex_;
};

/**
 * @brief Реализация интерфейса формата строки с локацией.
 */
class FormatWithLocationImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param format Формат
     * @param location Локация
     */
    FormatWithLocationImpl(const char *format,
                           const SourceLocation &location) noexcept;

    /**
     * @brief Деструктор.
     */
    ~FormatWithLocationImpl() noexcept;

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

    FormatWithLocationImpl(const FormatWithLocationImpl &) = delete;
    FormatWithLocationImpl(FormatWithLocationImpl &&) = delete;
    FormatWithLocationImpl &operator=(const FormatWithLocationImpl &) = delete;
    FormatWithLocationImpl &operator=(FormatWithLocationImpl &&) = delete;

private:
    /**
     * @brief Формат.
     */
    std::string format_;

    /**
     * @brief Локация.
     */
    SourceLocation location_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_LOGGING_IMPL_HPP_
