/**
 * @file
 * @brief Классы для вывода сообщений в файловый лог.
 */
#ifndef TASP_LOGGING_SINKS_FILE_SINK_HPP_
#define TASP_LOGGING_SINKS_FILE_SINK_HPP_

#include <experimental/filesystem>
#include <fstream>

#include "../log_line.hpp"
#include "sink.hpp"

namespace fs = std::experimental::filesystem;

namespace tasp
{

/**
 * @brief Класс для ротации текстового лога.
 */
class FileSinkRotate final
{
public:
    /**
     * @brief Конструктор.
     *
     * @param config_path Путь к параметрам лога в конфигурационном файле
     */
    explicit FileSinkRotate(std::string_view config_path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~FileSinkRotate() noexcept;

    /**
     * @brief Установка полного пути к основному файлу лога.
     *
     * @param fullpath Полный путь к файлу
     */
    void SetFullPath(const fs::path &fullpath) noexcept;

    /**
     * @brief Ротация лог файла.
     *
     * При вызове функции проверятся включена ротация и достигнут ли
     * максимально допустимый размер файла.
     *
     * @param message_length Размер нового сообщения
     *
     * @return Была ли произведена ротация
     */
    bool Rotate(size_t message_length = 0) noexcept;

    FileSinkRotate(const FileSinkRotate &) = delete;
    FileSinkRotate(FileSinkRotate &&) = delete;
    FileSinkRotate &operator=(const FileSinkRotate &) = delete;
    FileSinkRotate &operator=(FileSinkRotate &&) = delete;

private:
    /**
     * @brief Текущий размер файла.
     */
    std::uintmax_t current_size_{0};

    /**
     * @brief Флаг включения ротации.
     */
    bool enable_{false};

    /**
     * @brief Максимальное количество файлов в ротации.
     */
    uint16_t max_files_{10};

    /**
     * @brief Максимальный размер файла.
     */
    std::uintmax_t max_size_{10};

    /**
     * @brief Полный путь к файлу лога.
     */
    fs::path fullpath_;
};

/**
 * @brief Реализация вывода сообщений в файловый лог.
 */
class FileSink final : public Sink
{
public:
    /**
     * @brief Конструктор.
     *
     * @param config_path Путь к параметрам лога в конфигурационном файле
     */
    explicit FileSink(std::string_view config_path) noexcept;

    /**
     * @brief Деструктор.
     */
    ~FileSink() noexcept override;

    FileSink(const FileSink &) = delete;
    FileSink(FileSink &&) = delete;
    FileSink &operator=(const FileSink &) = delete;
    FileSink &operator=(FileSink &&) = delete;

private:
    /**
     * @brief Реализация вывода сообщения в лог.
     *
     * @param line Данные для вывода
     */
    void PrintImpl(const LogLine &line) noexcept override;

    /**
     * @brief Полный путь к логу.
     */
    fs::path fullpath_;

    /**
     * @brief Открытый файл для вывода сообщений.
     */
    std::ofstream file_;

    /**
     * @brief Ротация лог-файлов.
     */
    FileSinkRotate rotate_;
};

}  // namespace tasp

#endif  // TASP_LOGGING_SINKS_FILE_SINK_HPP_
