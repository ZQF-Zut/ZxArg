# ZxArg
command line parser but without argc and argv

## Example
```cpp
#include <print>
#include <iostream>
#include <ZxArg/Parser.h>


auto main(void) -> int
{
    ZQF::ZxArg::Parser arg;
    arg.AddCmd("-name", "your name [default=xiao]", "xiao");
    arg.AddCmd("-sex", "your sex");
    arg.AddCmd("-age", "your age");
    arg.AddCmd("-weight", "your weight");
    arg.AddCmd("-furry", "furry or not");
    arg.AddExample("-name xiao -sex male -age 16 -furry true -weight 55.5");
    arg.AddExample("-name lee -sex female -age 15 -furry false -weight 50.65");

    if (!arg.Parse()) { return; }

    std::println("Your name is {}, {}, {} years old, {}kg, {}furry.",
        arg["-name"].Get<std::string_view>(),
        arg["-sex"].Get<std::string_view>(),
        arg["-age"].Get<size_t>(),
        arg["-weight"].Get<double>(),
        arg["-furry"].Get<bool>() ? "" : "not ");
}
```
