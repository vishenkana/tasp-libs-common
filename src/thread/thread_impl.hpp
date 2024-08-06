/**
 * @file
 * @brief Реализация интерфейсов для запуска функции обработки в потоке.
 */
#ifndef TASP_THREAD_THREAD_HPP_
#define TASP_THREAD_THREAD_HPP_

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

// NOLINTNEXTLINE(google-global-names-in-headers)
using namespace std::chrono_literals;

namespace tasp
{

/**
 * @brief Реализация интерфейса для запуска функции обработки в потоке.
 */
class ThreadImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param timeout Интервал между повторным вызовом функции
     */
    explicit ThreadImpl(std::chrono::seconds timeout = 240s) noexcept;

    /**
     * @brief Деструктор.
     */
    ~ThreadImpl() noexcept;

    /**
     * @brief Установка функции для выполнения в потоке.
     *
     * @param func Функция для выполнения
     */
    void Worker(const std::function<void()> &func) noexcept;

    /**
     * @brief Запуск потока на исполнение.
     */
    void Start() noexcept;

    /**
     * @brief Остановка потока.
     */
    void Stop() noexcept;

    ThreadImpl(const ThreadImpl &) = delete;
    ThreadImpl(ThreadImpl &&) = delete;
    ThreadImpl &operator=(const ThreadImpl &) = delete;
    ThreadImpl &operator=(ThreadImpl &&) = delete;

private:
    /**
     * @brief Главная функция исполняемая в потоке. Вызывает функцию worker_ по
     * таймауту.
     */
    void Main() noexcept;

    /**
     * @brief Статусу для потока обработки.
     */
    enum class Status : std::uint8_t
    {
        Work = 0, /*!< Поток работает */
        NeedStop = 1, /*!< Потоку необходимо остановиться */
        Stop = 2 /*!< Поток остановлен */
    };

    /**
     * @brief Функция для работы в потоке.
     */
    std::function<void()> worker_{nullptr};

    /**
     * @brief Поток обработки.
     */
    std::unique_ptr<std::thread> thread_{nullptr};

    /**
     * @brief Текущий статус для потока обработки.
     */
    std::atomic<Status> status_{Status::Stop};

    /**
     * @brief Интервал между повторным вызовом функции обработки.
     */
    std::chrono::seconds timeout_;

    /**
     * @brief Условная переменная для блокировки потока обработки, пока не
     * пройдет таймаут или не придет сигнал об остановке выполнения.
     */
    std::condition_variable condition_;

    /**
     * @brief Мьютекс для условной переменной.
     */
    std::mutex condition_mutex_;
};

}  // namespace tasp

#endif  // TASP_THREAD_THREAD_HPP_