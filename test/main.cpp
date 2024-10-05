#include <print>
#include <cassert>
#include <iostream>
#include <Zut/ZxArg.h>



[[maybe_unused]] static auto TestValue() -> void
{
    ZxArg::Value value;
    value.SetHelp("set the out put path");
    value.SetValue("out_put_dir");
    assert(value.GetStrView() == "out_put_dir");
    assert(value.GetHelp() == "set the out put path");

    ZxArg::Value value1;
    value1.SetValue("120");
    assert(value1.GetStrView() == "120");
    assert(value1.GetNum() == 120);

    ZxArg::Value value2("0x123", "set num");
    assert(value2.GetStrView() == "0x123");
    assert(value2.GetNum<uint32_t>() == 0x123);
    assert(value2.GetHelp() == "set num");

    ZxArg::Value value3("false", "is out");
    assert(value3.GetBool() == false);

    ZxArg::Value value4("true", "is out");
    assert(value4.GetBool() == true);

    ZxArg::Value value3_1("yes", "is out");
    assert(value3_1.GetBool() == true);

    ZxArg::Value value4_1("no", "is out");
    assert(value4_1.GetBool() == false);

    try
    {
        ZxArg::Value value5("trxe", "is out");
        [[maybe_unused]] const auto status = value5.GetBool();
        assert(false);
    }
    catch ([[maybe_unused]] const std::exception& err)
    {

    }

    ZxArg::Value value6("12.3", "pos x");
    assert(value6.GetFloat() != 0.0);

    [[maybe_unused]] int x = 0;
}

[[maybe_unused]] static auto TestParser() -> void
{
    const char* argv[]{ "game.exe", "-mode", "batch", "-name", "\"[061215][EX12] 雛鳥の堕ちる音\"", "-ver", "1.2", "-size", "10", "-make", "false", "-export", "true" };

    ZxArg::Parser arg;
    arg
        .AddOption("-name", "game name in filter json file")
        .AddOption("-mode", "mode: [batch]")
        .AddOption("-ver", "version")
        .AddOption("-size", "game size")
        .AddOption("-make", "make data")
        .AddOption("-export", "export data")

        .AddExample("-ver 1.3 -size 11 -export fale")
        .AddExample("-mode batch -size 11 -make true")

        .Parse(std::size(argv), argv);

    assert(arg["-name"].GetStrView() == "[061215][EX12] 雛鳥の堕ちる音");
    assert(arg["-mode"].GetStrView() == "batch");
    assert(arg["-ver"].GetFloat() == 1.2);
    assert(arg["-size"].GetNum() == 10);
    assert(arg["-make"].GetBool() == false);
    assert(arg["-export"].GetBool() == true);

    [[maybe_unused]] int x = 0;
}


[[maybe_unused]] static auto TestParserViaSys() -> void
{
    ZxArg::Parser arg;
    arg
        .SetAbout("this is a demo")
        .SetAuthor("github.com/Dir-A")
        .AddOption("-name", "your name [default=xiao]", "xiao")
        .AddOption("-sex", "your sex")
        .AddOption("-age", "your age")
        .AddOption("-weight", "your weight")
        .AddOption("-furry", "furry or not")
        .AddExample("-name xiao -sex male -age 16 -furry true -weight 55.5");
    
    if (!arg.Parse()) { return; }

    std::println
    (
        "Your name is {}, {}, {} years old, {}kg, {}furry.",
        arg["-name"].GetStrView(),
        arg["-sex"].GetStrView(),
        arg["-age"].GetNum(),
        arg["-weight"].GetFloat(),
        arg["-furry"].GetBool() ? "" : "not "
    );
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
