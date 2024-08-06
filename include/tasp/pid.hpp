/**
 * @file
 * @brief Интерфейсы для работы с pid (Process IDentifier).
 */
#ifndef TASP_PID_HPP_
#define TASP_PID_HPP_

#include <memory>

namespace tasp
{

class PIDImpl;

/**
 * @brief Интерфейс для работы с pid (Process IDentifier).
 *
 * Класс скрывает от пользователя реализацию с помощью идиомы PIMPL
 * (Pointer to Implementation – указатель на реализацию).
 */
class [[gnu::visibility("default")]] PID final
{
public:
    /**
     * @brief Конструктор.
     *
     * Считывает идентификатор текущего процесса и идентификатор из сохраненного
     * файла.
     *
     * Если файл с идентификатором отсутствует или процесса с идентификатором не
     * существует, сохраняет идентификатор текущего процесса в файл.
     */
    PID() noexcept;

    /**
     * @brief Деструктор.
     *
     * Удаляет файл с идентификатор, если нет другого процесса.
     */
    ~PID() noexcept;

    /**
     * @brief Идентификатор текущего процесса.
     *
     * @return Идентификатор
     */
    [[nodiscard]] int Current() const noexcept;

    /**
     * @brief Идентификатор запущенного процесса.
     *
     * Если другого процесса программы не существует, вернет идентификатор
     * текущего процесса.
     *
     * @return Идентификатор
     */
    [[nodiscard]] int Running() const noexcept;

    /**
     * @brief Проверка на наличие уже запущенного процесса.
     *
     * @return Результат проверки
     */
    [[nodiscard]] bool SecondLaunch() const noexcept;

    PID(const PID &) = delete;
    PID(PID &&) = delete;
    PID &operator=(const PID &) = delete;
    PID &operator=(PID &&) = delete;

private:
    /**
     * @brief Указатель на реализацию.
     */
    std::unique_ptr<PIDImpl> impl_;
};

}  // namespace tasp

#endif  // TASP_PID_HPP_
