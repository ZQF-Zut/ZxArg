# ZxArg
Command Line Parser

## Example
```cpp
#include <print>
#include <iostream>
#include <Zut/ZxArg.h>


auto main(void) -> int
{
    ZxArg::Parser arg;
    arg.SetAbout("this is a demo");
    arg.SetAuthor("github.com/Dir-A");
    arg.AddOption("-name", "your name [default=xiao]", "xiao");
    arg.AddOption("-sex", "your sex");
    arg.AddOption("-age", "your age");
    arg.AddOption("-weight", "your weight");
    arg.AddOption("-furry", "furry or not");
    arg.AddExample("-name xiao -sex male -age 16 -furry true -weight 55.5");
    arg.AddExample("-name lee -sex female -age 15 -furry false -weight 50.65");

    if (!arg.Parse()) { return 0; }

    std::println("Your name is {}, {}, {} years old, {}kg, {}furry.",
        arg["-name"].Get<std::string_view>(),
        arg["-sex"].Get<std::string_view>(),
        arg["-age"].Get<size_t>(),
        arg["-weight"].Get<double>(),
        arg["-furry"].Get<bool>() ? "" : "not ");
}
```

```shell
> ./ZxArg_Test
About  : this is a demo
Author : github.com/Dir-A
Options:
        -furry   -> furry or not
        -weight  -> your weight
        -sex     -> your sex
        -age     -> your age
        -name    -> your name [default=xiao]
Example:
        ./ZxArg_Test -name xiao -sex male -age 16 -furry true -weight 55.5
        ./ZxArg_Test -name lee -sex female -age 15 -furry false -weight 50.65
```

```shell
> ./ZxArg_Test -name xiao -sex male -age 16 -furry true -weight 55.5
Your name is xiao, male, 16 years old, 55.5kg, furry.
> ./ZxArg_Test -sex male -age 16 -furry true -weight 55.5
Your name is xiao, male, 16 years old, 55.5kg, furry.
```
