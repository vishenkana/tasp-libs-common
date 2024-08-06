/**
 * @file
 * @brief Реализация интерфейсов для работы с pid (Process IDentifier).
 */
#ifndef TASP_PID_PID_IMPL_HPP_
#define TASP_PID_PID_IMPL_HPP_

#include "tasp/config.hpp"

namespace tasp
{

/**
 * @brief Реализация интерфейса для работы с pid (Process IDentifier).
 */
class PIDImpl final
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
    PIDImpl() noexcept;

    /**
     * @brief Деструктор.
     *
     * Удаляет файл с идентификатор, если нет другого процесса.
     */
    ~PIDImpl() noexcept;

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

    PIDImpl(const PIDImpl &) = delete;
    PIDImpl(PIDImpl &&) = delete;
    PIDImpl &operator=(const PIDImpl &) = delete;
    PIDImpl &operator=(PIDImpl &&) = delete;

private:
    /**
     * @brief Проверка существования процесса с идентификатором из файла.
     *
     * @return Результат проверки
     */
    bool RunningExists() noexcept;

    /**
     * @brief Сохранение файла с идентификатором.
     *
     * @return Результат сохранения
     */
    bool Save() noexcept;

    /**
     * @brief Удаление файла с идентификатором.
     */
    void Remove() const noexcept;

    /**
     * @brief Идентификатор текущего процесса.
     */
    int current_pid_;

    /**
     * @brief Идентификатор запущенного процесса.
     */
    int running_pid_{0};

    /**
     * @brief Файл с идентификатором процесса.
     */
    Config file_;
};

}  // namespace tasp

#endif  // TASP_PID_PID_IMPL_HPP_
