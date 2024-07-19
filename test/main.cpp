#include <print>
#include <cassert>
#include <iostream>
#include <ZxArg/Parser.h>



[[maybe_unused]] static auto TestValue() -> void
{
    ZQF::ZxArg::Value value;
    value.SetHelp("set the out put path");
    value.SetValue("out_put_dir");
    assert(value.Get<std::string_view>() == "out_put_dir");
    assert(value.GetHelp() == "set the out put path");

    ZQF::ZxArg::Value value1;
    value1.SetValue("120");
    assert(value1.Get<std::string>() == "120");
    assert(value1.Get<size_t>() == 120);

    ZQF::ZxArg::Value value2("0x123", "set num");
    assert(value2.Get<std::string_view>() == "0x123");
    assert(value2.Get<uint32_t>() == 0x123);
    assert(value2.GetHelp() == "set num");

    ZQF::ZxArg::Value value3("false", "is out");
    assert(value3.Get<bool>() == false);

    ZQF::ZxArg::Value value4("true", "is out");
    assert(value4.Get<bool>() == true);

    ZQF::ZxArg::Value value3_1("yes", "is out");
    assert(value3_1.Get<bool>() == true);

    ZQF::ZxArg::Value value4_1("no", "is out");
    assert(value4_1.Get<bool>() == false);

    try
    {
        ZQF::ZxArg::Value value5("trxe", "is out");
        [[maybe_unused]] const auto status = value5.Get<bool>();
        assert(false);
    }
    catch ([[maybe_unused]] const std::exception& err)
    {

    }

    ZQF::ZxArg::Value value6("12.3", "pos x");
    assert(value6.Get<double>() != 0.0);

    [[maybe_unused]] int x = 0;
}

[[maybe_unused]] static auto TestParser() -> void
{
    const char* argv[] = { "game.exe", "-mode", "batch", "-name", "\"[061215][EX12] 雛鳥の堕ちる音\"", "-ver", "1.2", "-size", "10", "-make", "false", "-export", "true" };

    ZQF::ZxArg::Parser arg;
    arg.AddCmd("-name", "game name in filter json file");
    arg.AddCmd("-mode", "mode: [batch]");
    arg.AddCmd("-ver", "version");
    arg.AddCmd("-size", "game size");
    arg.AddCmd("-make", "make data");
    arg.AddCmd("-export", "export data");

    arg.AddExample("-ver 1.3 -size 11 -export fale");
    arg.AddExample("-mode batch -size 11 -make true");

    arg.Parse(std::size(argv), (char**)(argv));

    assert(arg["-name"].Get<std::string_view>() == "[061215][EX12] 雛鳥の堕ちる音");
    assert(arg["-mode"].Get<std::string_view>() == "batch");
    assert(arg["-ver"].Get<double>() == 1.2);
    assert(arg["-size"].Get<size_t>() == 10);
    assert(arg["-make"].Get<bool>() == false);
    assert(arg["-export"].Get<bool>() == true);

    [[maybe_unused]] int x = 0;
}


[[maybe_unused]] static auto TestParserViaSys() -> void
{
    ZQF::ZxArg::Parser arg;
    arg.AddCmd("-name", "your name [default=xiao]", "xiao");
    arg.AddCmd("-sex", "your sex");
    arg.AddCmd("-age", "your age");
    arg.AddCmd("-weight", "your weight");
    arg.AddCmd("-furry", "furry or not");
    arg.AddExample("-name xiao -sex male -age 16 -furry true -weight 55.5");

    if (!arg.Parse()) { return; }

    std::println("Your name is {}, {}, {} years old, {}kg, {}furry.",
        arg["-name"].Get<std::string_view>(),
        arg["-sex"].Get<std::string_view>(),
        arg["-age"].Get<size_t>(),
        arg["-weight"].Get<double>(),
        arg["-furry"].Get<bool>() ? "" : "not ");
}

auto main(void) -> int
{
    try
    {
        ::TestValue();
        ::TestParser();
        ::TestParserViaSys();
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
