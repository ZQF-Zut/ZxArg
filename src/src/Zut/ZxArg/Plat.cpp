#include "Plat.h"
#include <memory>


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdexcept>


namespace ZQF::Zut::ZxArg::Plat
{
    auto GetCmdLine() -> std::vector<std::string>
    {
        auto [cmd_line_bytes, cmd_line_buffer] = []() -> std::pair<size_t, std::unique_ptr<char[]>>
            {
                auto cmd_line_ptr = ::GetCommandLineW();
                auto cmd_line_chars = ::wcslen(cmd_line_ptr);

                const size_t buffer_max_bytes = (cmd_line_chars * sizeof(wchar_t) + 1) * 2;
                auto buffer = std::make_unique_for_overwrite<char[]>(buffer_max_bytes);
                const auto bytes_real = static_cast<size_t>(::WideCharToMultiByte(CP_UTF8, 0, cmd_line_ptr, static_cast<int>(cmd_line_chars), buffer.get(), static_cast<int>(buffer_max_bytes), nullptr, nullptr));
                buffer[bytes_real] = {};

                return { static_cast<size_t>(bytes_real), std::move(buffer) };
    }();

        std::vector<std::string> cmd_line_vec;

        std::string_view cmd_line_str{ cmd_line_buffer.get(), cmd_line_bytes };

        // parse
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

        // read last value
        if (sub_str_beg < cmd_line_str.size())
        {
            cmd_line_vec.emplace_back(std::string{ cmd_line_str.data() + sub_str_beg , cmd_line_str.data() + cmd_line_str.size() });
        }

        // format exe path
        if (cmd_line_vec.size())
        {
            for (auto& char_v : cmd_line_vec.front())
            {
                if (char_v == '\\') { char_v = '/'; }
            }
        }

        return cmd_line_vec;
}
} // namespace ZQF::Zut::ZxArg::Plat
#elif __linux__
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>


namespace ZQF::Zut::ZxArg::Plat
{
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
} // namespace ZQF::Zut::ZxArg::Plat
#endif



