# Twitch API C library

Small single-threaded C library to work with Twitch API.

Heavily WIP, use on your own discretion.

# Build requirements

- CMake
- cURL
- `libm` on some systems

# Building

```
mkdir build
cd build
cmake ..
make
```

# Usage

There's a sample usage app in `example/` directory. You can also build it with `make twitch-remote` command.

# Contents

- `include/ctwitch.h` contains common methods. Currently there's only one `twitch_init()` method defined there.
- `include/v5.h` is an umbrella header for V5 API data structs and methods.

Currently just a handful of methods from API V5 are implemented. I plan to expand this to include all V5 API first, and then move on to the Twitch New API.

# License

GPLv3
