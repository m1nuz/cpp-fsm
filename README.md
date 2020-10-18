# cpp-fsm
[![Actions Status](https://github.com/m1nuz/cpp-fsm/workflows/CMake/badge.svg)](https://github.com/m1nuz/cpp-fsm/actions)
Simple implementation of FSM in C++17

## How to build
```sh
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=<your_path>
cmake --build . --config Release --target install -- -j $(nproc)
```

### Build with examples
```sh
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=<your_path> -DBUILD_EXAMPLES=ON
cmake --build . --config Release --target install -- -j $(nproc)
```

## Example
```cpp
#include <cpp_fsm.hpp>
#include <iostream>

struct OnEvent : fsm::typed_event<OnEvent> {};
struct OffEvent : fsm::typed_event<OffEvent> {};

struct On : fsm::typed_state<On> {};
struct Off : fsm::typed_state<Off> {};

using Light = fsm::state_machine<Off, On>;

int main()
{
    Light light;
    light.transit_to<On, OffEvent, Off>();
    light.transit_to<Off, OnEvent, On>();

    light.on_event<OffEvent>([]() { std::cout << "light off" << std::endl; });
    light.on_event<OnEvent>([]() { std::cout << "light on" << std::endl; });

    light.dispatch(OnEvent{});
    light.dispatch(OffEvent{});

    return 0;
}
```
