/**
 * @file
 * @brief Реализация интерфейсов для создания демона.
 */
#ifndef TASP_DAEMON_DAEMON_IMPL_HPP_
#define TASP_DAEMON_DAEMON_IMPL_HPP_

#include <csignal>
#include <functional>

#include "tasp/pid.hpp"

namespace tasp
{

/**
 * @brief Реализация интерфейса для создания демона.
 *
 * Сигналы завершающие выполнение: SIGINT, SIGTERM, SIGQUIT, SIGKILL
 * Сигналы для обновление конфигурации: SIGUSR1, SIGUSR2
 */
class DaemonImpl final
{
public:
    /**
     * @brief Конструктор.
     *
     * В параметрах можно передать путь к глобальному конфигурационному файлу
     * через "--config".
     *
     * @param argc Количество параметров запуска
     * @param argv Массив параметров
     */
    DaemonImpl(int argc, const char **argv) noexcept;

    /**
     * @brief Деструктор.
     */
    ~DaemonImpl() noexcept;

    /**
     * @brief Запуск демона.
     *
     * @return Код завершения демона. 0 - при нормальном завершении.
     */
    [[nodiscard]] int Exec() const noexcept;

    /**
     * @brief Установка дополнительной функции перезагрузки.
     *
     * @param func Функция дополнительных действия при перезагрузки демона
     */
    void Reload(const std::function<void()> &func) noexcept;

    DaemonImpl(const DaemonImpl &) = delete;
    DaemonImpl(DaemonImpl &&) = delete;
    DaemonImpl &operator=(const DaemonImpl &) = delete;
    DaemonImpl &operator=(DaemonImpl &&) = delete;

private:
    /**
     * @brief Обработка падения программы для записи в лог всей накопленной
     * информации.
     *
     * @param signum Номер сигнала
     */
    static void SigsegvHandler(int signum) noexcept;

    /**
     * @brief Список сигналов для перехвата.
     */
    sigset_t sigset_{};

    /**
     * @brief Функция дополнительных действия при перезагрузки демона.
     */
    std::function<void()> reload_{nullptr};

    /**
     * @brief Идентификатор процесса.
     */
    std::unique_ptr<PID> pid_{nullptr};
};

}  // namespace tasp

#endif  // TASP_DAEMON_DAEMON_IMPL_HPP_
