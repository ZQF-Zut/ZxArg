#include <ZxArg/Platform.h>
#include <memory>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdexcept>
#elif __linux__
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#endif


namespace ZQF::ZxArg
{
#ifdef _WIN32
    auto GetCmdLine() -> std::vector<std::string>
    {
        auto [cmd_line_bytes, cmd_line_buffer] = []() -> std::pair<size_t, std::unique_ptr<char[]>>
            {
                auto cmd_line_ptr = ::GetCommandLineW();
                auto cmd_line_chars = ::wcslen(cmd_line_ptr);

                const auto bytes = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, cmd_line_ptr, static_cast<int>(cmd_line_chars), nullptr, 0, nullptr, nullptr));
                auto buffer = std::make_unique_for_overwrite<char[]>(bytes + 1);
                const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, cmd_line_ptr, static_cast<int>(cmd_line_chars), buffer.get(), static_cast<int>(bytes), nullptr, nullptr));
                buffer[bytes_real] = {};

                return { static_cast<size_t>(bytes_real), std::move(buffer) };
            }();

        std::vector<std::string> cmd_line_vec;

        std::string_view cmd_line_str{ cmd_line_buffer.get(), cmd_line_bytes };

        size_t sub_str_beg{};
        for (size_t idx{}; idx < cmd_line_str.size(); idx++)
        {
            const char cur_char = cmd_line_str[idx];
            if (cur_char == '"')
            {
                if (auto pos = cmd_line_str.find('"', idx + 1);pos != std::string_view::npos)
                {
                    idx = pos;
                }
                else
                {
                    throw std::runtime_error("ZxArg::GetCmdLine(): error cmd line!");
                }
            }
            else if (cur_char == ' ')
            {
                const char* beg_ptr = cmd_line_str.data() + sub_str_beg;
                const char* end_ptr = cmd_line_str.data() + idx;
                cmd_line_vec.emplace_back(std::string{ beg_ptr, end_ptr });

                // skip white
                for (; idx < cmd_line_str.size(); idx++) { if (cmd_line_str[idx] != ' ') { break; } }

                sub_str_beg = idx;
                idx -= 1;
            }
        }

        if (sub_str_beg < cmd_line_str.size())
        {
            cmd_line_vec.emplace_back(std::string{ cmd_line_str.data() + sub_str_beg , cmd_line_str.data() + cmd_line_str.size() });
        }

        return cmd_line_vec;
    }
#else
    auto GetCmdLine() -> std::vector<std::string>
    {
        auto fn_get_cmd_line_data = []() -> std::pair<size_t, std::unique_ptr<char[]>>
            {
                const auto arg_max_bytes = ::sysconf(_SC_ARG_MAX);
                auto arg_buffer = std::make_unique_for_overwrite<char[]>(arg_max_bytes);
                const auto fd = ::open("/proc/self/cmdline", O_RDONLY);
                const auto arg_bytes = ::read(fd, arg_buffer.get(), arg_max_bytes);
                ::close(fd);
                return { static_cast<size_t>(arg_bytes), std::move(arg_buffer) };
            };

        auto [arg_size_bytes, arg_buffer] = fn_get_cmd_line_data();

        std::vector<std::string> cmd_line_vec;
        for (size_t idx = 0; idx < arg_size_bytes; idx++)
        {
            std::string_view val = arg_buffer.get() + idx;
            idx += val.size();
            cmd_line_vec.emplace_back(val);
        }

        return cmd_line_vec;
    }
#endif

}