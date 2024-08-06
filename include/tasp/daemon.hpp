/**
 * @file
 * @brief Интерфейсы для создания демонов.
 */
#ifndef TASP_DAEMON_HPP_
#define TASP_DAEMON_HPP_

#include <memory>

namespace tasp
{

class DaemonImpl;

/**
 * @brief Интерфейс для создания демона.
 *
 * Сигналы завершающие выполнение: SIGINT, SIGTERM, SIGQUIT, SIGKILL
 * Сигналы для обновление конфигурации: SIGUSR1, SIGUSR2
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] Daemon
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
    Daemon(int argc, const char **argv) noexcept;

    /**
     * @brief Деструктор.
     */
    virtual ~Daemon() noexcept;

    /**
     * @brief Запуск демона.
     *
     * @return Код завершения демона. 0 - при нормальном завершении.
     */
    [[nodiscard]] int Exec() const noexcept;

    /**
     * @brief Функция для дополнительный действий при перезагрузки демона.
     *
     * По умолчанию перечитывается конфигурационный файл и перезагружается
     * логирование.
     */
    virtual void Reload() noexcept;

    Daemon(const Daemon &) = delete;
    Daemon(Daemon &&) = delete;
    Daemon &operator=(const Daemon &) = delete;
    Daemon &operator=(Daemon &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<DaemonImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_DAEMON_HPP_
