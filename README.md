# WIP: Twitch API C library

C library to work with Twitch API. It's single-threaded and based on curl_easy API.

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

- `include/data.h` contains data structures definitions and helper methods to work with them.
- `include/ctwitch.h` contains common method. Currently there's only one `twitch_init()` method defined there.
- `include/v5.h` is an umbrella header for V5 API methods inside `include/v5` directory.

Currently just a handful of methods from API V5 are implemented. I plan to expand this to include all V5 API first, and then move on to the Twitch New API.

# License

See `LICENSE.md`

