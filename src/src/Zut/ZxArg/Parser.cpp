#include "Parser.h"


namespace ZQF::Zut::ZxArg
{
    auto Parser::Parse(const bool isShowHelp) -> bool
    {
        m_CMDLineData.LoadRawStr();
        m_CMDLineData.ParseCMD();
        return this->ParseOptions(isShowHelp);
    }

    auto Parser::Parse(const int argc, const char** const argv, const bool isShowHelp) -> bool
    {
        m_CMDLineData.ParseCMD(argc, argv);
        return this->ParseOptions(isShowHelp);
    }

    auto Parser::ParseOptions(const bool isShowHelp) -> bool
    {
        const auto cmd_vec{ m_CMDLineData.GetVec() };

        if (cmd_vec.size() < 1)
        {
            throw std::runtime_error("ZxArg::Parser::ParseOptions(): arg empty!");
        }

        if (cmd_vec.size() == 1)
        {
            if (isShowHelp)
            {
                this->ShowHelp();
            }

            return false;
        }

        if ((cmd_vec.size() % 2) == 0)
        {
            throw std::runtime_error("ZxArg::Parser::ParseOptions(): arg count error!");
        }

        for (std::size_t ite_arg{ 1 }; ite_arg < cmd_vec.size(); ite_arg += 2)
        {
            const std::string_view option{ cmd_vec[ite_arg + 0] };
            const std::string_view value{ cmd_vec[ite_arg + 1] };

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

    auto Parser::ShowHelp() -> void
    {
        // get exe pure name
        std::string_view exe_name{ m_CMDLineData.GetVec().front() };
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


    auto Parser::AddOption(const std::string_view msOption, const std::string_view msHelp, const std::string_view msDefaultVal) -> Parser&
    {
        auto& val_ref{ m_mpOption[msOption] };
        val_ref.SetHelp(msHelp);
        if (!msDefaultVal.empty()) { val_ref.SetValue(msDefaultVal); }
        return *this;
    }

    auto Parser::AddExample(const std::string_view msExample) -> Parser&
    {
        m_vcExample.emplace_back(msExample);
        return *this;
    }

    auto Parser::SetAbout(const std::string_view msAbout) -> Parser&
    {
        m_msAbout.assign(msAbout);
        return *this;
    }

    auto Parser::SetAuthor(const std::string_view msAuthor) -> Parser&
    {
        m_msAuthor.assign(msAuthor);
        return *this;
    }

    auto Parser::operator[](const std::string_view msOption) const -> const Value&
    {
        if (const auto ite_map{ m_mpOption.find(msOption) }; ite_map != m_mpOption.end())
        {
            if (ite_map->second.GetValue().empty())
            {
                throw std::runtime_error(std::string{ "ZxArg::Parser::operator[](): arg is empty! -> " }.append(msOption));
            }

            return ite_map->second;
        }

        throw std::runtime_error(std::string("ZxArg::Parser::operator[](): arg is not find! -> ").append(msOption));
    }
} // namespace ZQF::Zut::ZxArg
