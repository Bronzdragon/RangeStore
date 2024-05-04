# Range Store data structure
The Range Store data structure allows you to store a single value, and assign it to a sequence of keys, in an computationally and space-efficient manner. An example:

```c++
#include "RangeStore.hpp"

// We assign districts to different newspaper delivery people
range_store<int, std::string> route_assignments {"default"};
route_assignments.assign(0,10, "Alice");
route_assignments.assign(10,25, "Bob");
route_assignments.assign(25,30, "Alice");

std::string delivery_person = route_assignments[14]; // Bob's responsible for Zone 14
```

This is a simple one-header library. Just drop the header in your include folder, and you're ready to go. No special setup required.

# Building this repository:
If you'd like to build the examples yourself, this project is set up to be built with cmake.
1. Clone this repository to your computer.
2. Make a 'build' directory.
3. Switch to the build directory, and run `cmake ..` to configure the project.
4. Run `cmake --build .` to build it.

## Running tests:
You can find the testing executable after building at `build/src/test/tests`. You can also run `ctest` in the `build` directory.