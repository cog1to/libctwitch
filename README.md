# WIP: Twitch API C library

C library to work with Twitch API.

Heavily WIP, use on your own discretion.

# Build requirements

- `libcurl`

# Usage

I don't have any build system in place right now, so either DIY or just copy the sources and include and build them directly.

For examples, see `twitch-test.c`.

To compile `twitch-test.c`:

```gcc twitch-test.c src/*.c -o twitch-test -L/<path-to-curl-lib> -I<path-to-curl-include> -I./src/ -I/./include -lcurl -lm```

# Contents

- `data.h` contains data structures definitions and helper methods to work with them.
- `v5.h` contains some methods to work with V5 API.

Currently just a handful of methods from API V5 are implemented. I plan to expand this to include all V5 API first, and then move on to the Twitch New API.

