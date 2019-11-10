# WIP: Twitch API C library

C library to work with Twitch API. It's single-threaded, based on curl_easy API.

Heavily WIP, use on your own discretion.

# Build requirements

- `libcurl`
- `libm` on some systems

# Usage

I don't have any build system in place right now, so either DIY or just copy the sources and include and build them directly.

For usage examples, see `twitch-test.c`.

To compile `twitch-test.c`:

```gcc twitch-test.c src/*.c src/v5/*.c src/json/*.c src/utils/*.c -I/usr/include/curl -I./include -I./src -L/usr/lib -lcurl -lm -o twitch-test```

# Contents

- `data.h` contains data structures definitions and helper methods to work with them.
- `v5.h` contains some methods to work with V5 API.

Currently just a handful of methods from API V5 are implemented. I plan to expand this to include all V5 API first, and then move on to the Twitch New API.

