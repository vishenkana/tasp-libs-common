#include "file_sink.hpp"

#include "tasp/config.hpp"
#include "tasp/logging.hpp"

using std::error_code;
using std::exception;
using std::ofstream;
using std::string;
using std::string_view;
using std::to_string;

namespace tasp
{
/*------------------------------------------------------------------------------
    FileSinkRotate
------------------------------------------------------------------------------*/
FileSinkRotate::FileSinkRotate(string_view config_path) noexcept
{
    auto &conf{ConfigGlobal::Instance()};

    const string path{config_path};

    enable_ = conf.Get(path + ".rotate.enable", enable_);
    max_size_ = conf.Get(path + ".rotate.max_size", max_size_);
    max_files_ = conf.Get(path + ".rotate.max_files", max_files_);

    const int base{1024};
    max_size_ = max_size_ * base * base;
}

//------------------------------------------------------------------------------
FileSinkRotate::~FileSinkRotate() noexcept = default;

//------------------------------------------------------------------------------
void FileSinkRotate::SetFullPath(const fs::path &fullpath) noexcept
{
    fullpath_ = fullpath;

    try
    {
        current_size_ = fs::file_size(fullpath_);
    }
    catch (const exception &exception)
    {
        current_size_ = 0;
    }
}

//------------------------------------------------------------------------------
bool FileSinkRotate::Rotate(size_t message_length) noexcept
{
    if (!enable_)
    {
        return false;
    }

    current_size_ += message_length;
    if (current_size_ < max_size_)
    {
        return false;
    }

    fs::path src_log{fullpath_};
    src_log += "." + to_string(max_files_);

    for (auto number = max_files_; number > 0; number--)
    {
        const fs::path dst_log{src_log};

        const string extension{number != 1 ? to_string(number - 1) : ""};
        src_log.replace_extension(extension);

        error_code error{};
        if (fs::exists(fullpath_, error))
        {
            fs::rename(src_log, dst_log, error);
        }
    }

    current_size_ = 0;

    return true;
}

/*------------------------------------------------------------------------------
    FileSink
------------------------------------------------------------------------------*/
FileSink::FileSink(string_view config_path) noexcept
: Sink(config_path)
, rotate_(config_path)
{
    auto &conf{ConfigGlobal::Instance()};

    const string path{config_path};

    fullpath_ = conf.Get<fs::path>(path + ".path");
    if (fullpath_.empty())
    {
        fullpath_ = conf.Get<fs::path>("program.path");
        fullpath_ /= "log";
    }

    error_code error{};
    if (!fs::exists(fullpath_, error))
    {
        fs::create_directories(fullpath_, error);
        fs::permissions(
            fullpath_, fs::perms::owner_all | fs::perms::group_all, error);
    }

    string name{conf.Get<fs::path>(path + ".name")};
    if (name.empty())
    {
        name = conf.Get<fs::path>("program.name");
        name += ".log";
    }

    fullpath_ /= name;

    rotate_.SetFullPath(fullpath_);
    rotate_.Rotate();

    file_.open(fullpath_, ofstream::out | ofstream::app);
}

//------------------------------------------------------------------------------
FileSink::~FileSink() noexcept
{
    file_.close();
}

//------------------------------------------------------------------------------
void FileSink::PrintImpl(const LogLine &line) noexcept
{
    const string message{line.ToString()};

    if (rotate_.Rotate(message.length()))
    {
        file_.close();
        file_.open(fullpath_, ofstream::out | ofstream::app);
    }

    file_ << message << '\n';
}

}  // namespace tasp
