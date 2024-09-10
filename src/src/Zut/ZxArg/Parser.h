#pragma once
#include <format>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>
#include <Zut/ZxArg/Value.h>
#include <Zut/ZxArg/Plat.h>


namespace ZQF::Zut::ZxArg
{
    // https://www.cppstories.com/2021/heterogeneous-access-cpp20/
    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char* cpKey) const
        {
            return std::hash<std::string_view>{}(cpKey);
        }

        [[nodiscard]] size_t operator()(const std::string_view msKey) const
        {
            return std::hash<std::string_view>{}(msKey);
        }

        [[nodiscard]] size_t operator()(const std::string& msKey) const
        {
            return std::hash<std::string>{}(msKey);
        }
    };

    class Parser
    {
    private:
        std::string m_msAbout;
        std::string m_msAuthor;
        std::vector<std::string> m_vcArgData;
        std::vector<std::string_view> m_vcExample;
        std::unordered_map<std::string_view, ZxArg::Value, string_hash, std::equal_to<>> m_mpOption;

    public:
        Parser()
        {

        }

    private:
        auto ParseData(const bool isShowHelp = true) -> bool
        {
            if (m_vcArgData.size() < 1) { throw std::runtime_error("ZxArg::Parser::Parse(): arg empty!"); }

            if (m_vcArgData.size() == 1)
            {
                if (isShowHelp) { this->ShowHelp(); }
                return false;
            }

            if ((m_vcArgData.size() % 2) == 0) { throw std::runtime_error("ZxArg::Parser::Parse(): arg count error!"); }

            for (std::size_t ite_arg{ 1 }; ite_arg < m_vcArgData.size(); ite_arg += 2)
            {
                const std::string_view option{ m_vcArgData[ite_arg + 0] };
                const std::string_view value{ m_vcArgData[ite_arg + 1] };

                if ((value.size()) && (value.front() == '\"') && (value.back() == '\"'))
                {
                    m_mpOption[option].SetValue(value.substr(1, value.size() - 2));
                }
                else
                {
                    m_mpOption[option].SetValue(value);
                }
            }

            return true;
        }

    public:
        auto Parse(const bool isShowHelp = true) -> bool
        {
            m_vcArgData = Plat::GetCmdLine();
            return this->ParseData(isShowHelp);
        }

        auto Parse(int argc, char** argv, bool isShowHelp = true) -> bool
        {
            for (size_t idx = 0; idx < static_cast<size_t>(argc); idx++) { m_vcArgData.emplace_back(argv[idx]); }
            return this->ParseData(isShowHelp);
        }

        auto ShowHelp() -> void
        {
            // get exe pure name
            std::string_view exe_name{ m_vcArgData.front() };
            if (const auto pos{ exe_name.rfind('/') }; pos != std::string_view::npos)
            {
                exe_name = exe_name.substr(pos + 1);
                if (exe_name.ends_with('"'))
                {
                    exe_name = exe_name.substr(0, exe_name.size() - 1);
                }
            }

            std::string help_log;

            if (m_msAbout.size())
            {
                help_log.append("About  : ").append(m_msAbout).append(1, '\n');
            }

            if (m_msAuthor.size())
            {
                help_log.append("Author : ").append(m_msAuthor).append(1, '\n');
            }

            help_log.append("Options:\n");
            for (const auto& cmd : m_mpOption)
            {
                help_log
                    .append(1, '\t')
                    .append(cmd.first)
                    .append("\t -> ")
                    .append(cmd.second.GetHelp())
                    .append(1, '\n');
            }

            help_log.append("Example:\n");
            for (const auto& exp : m_vcExample)
            {
                help_log
                    .append(1, '\t')
#ifdef _WIN32
                    .append(".\\")
#elif __linux__
                    .append("./")
#endif // _WIN32
                    .append(exe_name)
                    .append(1, ' ')
                    .append(exp)
                    .append(1, '\n');
            }

            std::printf("%s", help_log.c_str());
        }

        auto AddOption(const std::string_view msOption, const std::string_view msHelp, const std::string_view msDefaultVal = "") -> void
        {
            auto& val_ref = m_mpOption[msOption];
            val_ref.SetHelp(msHelp);
            if (!msDefaultVal.empty()) { val_ref.SetValue(msDefaultVal); }
        }

        auto AddExample(const std::string_view msExample) -> void
        {
            m_vcExample.emplace_back(msExample);
        }

        auto SetAbout(const std::string_view msAbout)
        {
            m_msAbout.assign(msAbout);
        }

        auto SetAuthor(const std::string_view msAuthor)
        {
            m_msAuthor.assign(msAuthor);
        }

        auto operator[](int) -> const Value & = delete;
        auto operator[](std::size_t) -> const Value & = delete;

        auto operator[](const std::string_view msOption) const -> const Value&
        {
            if (const auto ite_map = m_mpOption.find(msOption); ite_map != m_mpOption.end())
            {
                if (ite_map->second.GetValue().empty())
                {
                    throw std::runtime_error(std::string{ "ZxArg::Parser::operator[](): arg is empty! -> " }.append(msOption));
                }

                return ite_map->second;
            }

            throw std::runtime_error(std::string("ZxArg::Parser::operator[](): arg is not find! -> ").append(msOption));
        }
    };
} // namespace ZQF::Zut::ZxArg
