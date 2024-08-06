/**
 * @file
 * @brief Интерфейсы для запуска функции обработки в потоке.
 */
#ifndef TASP_THREAD_HPP_
#define TASP_THREAD_HPP_

#include <chrono>
#include <memory>

// NOLINTNEXTLINE(google-global-names-in-headers)
using namespace std::chrono_literals;

namespace tasp
{

class ThreadImpl;

/**
 * @brief Интерфейс для запуска функции обработки в потоке.
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Thread
{
public:
    /**
     * @brief Конструктор.
     *
     * @param timeout Интервал между повторным вызовом функции
     */
    explicit Thread(std::chrono::seconds timeout = 240s) noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~Thread() noexcept;

    Thread(const Thread &) = delete;
    Thread(Thread &&) = delete;
    Thread &operator=(const Thread &) = delete;
    Thread &operator=(Thread &&) = delete;

private:
    /**
     * @brief Функция для работы в потоке, необходимо переопределить.
     */
    virtual void Worker() noexcept;

    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<ThreadImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_THREAD_HPP_
