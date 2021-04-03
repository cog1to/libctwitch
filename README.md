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
- `include/helix.h` is an umbrella header for Helix API data structs and methods.

Currently just a handful of methods from API V5 and Helix are implemented.

# License

GPLv3
