#include "Plat.h"


namespace ZQF::Zut::ZxArg::Plat
{
    auto CMDLineData::ParseCMD(const int argc, const char** const argv) -> void
    {
        for (int idx{}; idx < argc; idx++)
        {
            m_vcCMD.emplace_back(argv[idx]);
        }
    }

    auto CMDLineData::GetRawStrView() const -> std::string_view
    {
        return { m_RawStr.second.get(), m_RawStr.first };
    }

    auto CMDLineData::GetVec() const -> const std::vector<std::string_view>&
    {
        return m_vcCMD;
    }
}


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdexcept>


namespace ZQF::Zut::ZxArg::Plat
{
    auto CMDLineData::LoadRawStr() -> void
    {
        const auto cmd_line_ptr{ ::GetCommandLineW() };
        const auto cmd_line_chars{ ::wcslen(cmd_line_ptr) };

        const auto buffer_max_bytes{ (cmd_line_chars * sizeof(wchar_t) + 1) * 2 };
        auto buffer{ std::make_unique_for_overwrite<char[]>(buffer_max_bytes) };
        const auto bytes_real{ static_cast<std::size_t>(::WideCharToMultiByte(CP_UTF8, 0, cmd_line_ptr, static_cast<int>(cmd_line_chars), buffer.get(), static_cast<int>(buffer_max_bytes), nullptr, nullptr)) };
        buffer[bytes_real] = {};

        // format exe path
        for (std::size_t idx = 0; idx < bytes_real; idx++)
        {
            auto& char_v = buffer[idx];
            if (char_v == '\\')
            {
                char_v = '/';
            }
        }

        m_RawStr = { static_cast<std::size_t>(bytes_real), std::move(buffer) };
    }

    auto CMDLineData::ParseCMD() -> void
    {
        const auto cmd_line_str{ this->GetRawStrView() };

        // parse command line
        std::size_t sub_str_beg{};
        for (std::size_t idx{}; idx < cmd_line_str.size(); idx++)
        {
            const char cur_char{ cmd_line_str[idx] };
            if (cur_char == '"')
            {
                if (const auto pos{ cmd_line_str.find('"', idx + 1) }; pos != std::string_view::npos)
                {
                    idx = pos;
                }
                else
                {
                    throw std::runtime_error("ZxArg::CMDLineData::ParseCMD(): error command line!");
                }
            }
            else if (cur_char == ' ')
            {
                m_vcCMD.emplace_back(cmd_line_str.data() + sub_str_beg, cmd_line_str.data() + idx);

                // skip white
                for (; idx < cmd_line_str.size(); idx++) 
                {
                    if (cmd_line_str[idx] != ' ')
                    {
                        break;
                    }
                }

                sub_str_beg = idx;
                idx -= 1;
            }
        }

        // read last value
        if (sub_str_beg < cmd_line_str.size())
        {
            m_vcCMD.emplace_back(cmd_line_str.data() + sub_str_beg, cmd_line_str.data() + cmd_line_str.size());
        }

        // format exe path
        //if (info.m_vcCMD.size())
        //{
        //    for (auto& char_v : info.m_vcCMD.front())
        //    {
        //        if (char_v == '\\')
        //        {
        //            char_v = '/';
        //        }
        //    }
        //}
    }
} // namespace ZQF::Zut::ZxArg::Plat
#elif __linux__
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>


namespace ZQF::Zut::ZxArg::Plat
{
    auto CMDLineData::LoadRawStr() -> void
    {
        const auto arg_max_bytes = ::sysconf(_SC_ARG_MAX);
        auto arg_buffer = std::make_unique_for_overwrite<char[]>(arg_max_bytes);
        const auto fd = ::open("/proc/self/cmdline", O_RDONLY);
        const auto arg_bytes = ::read(fd, arg_buffer.get(), arg_max_bytes);
        ::close(fd);
        m_RawStr = { static_cast<std::size_t>(arg_bytes), std::move(arg_buffer) };
    }

    auto CMDLineData::ParseCMD() -> void
    {
        const auto arg_buffer_ptr = m_RawStr.second.get();
        const auto arg_size_bytes = m_RawStr.first;
        for (std::size_t idx = 0; idx < arg_size_bytes; idx++)
        {
            std::string_view val{ arg_buffer_ptr + idx };
            idx += val.size();
            m_vcCMD.emplace_back(val);
        }
    }
} // namespace ZQF::Zut::ZxArg::Plat
#endif



