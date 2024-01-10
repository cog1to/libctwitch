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

## User access token

A couple of newly added endpoints (`channel_follows`) require user access token instead of app access token. The only way that I know to get one is to open an authorizeurl in a browser, something like

```
https://id.twitch.tv/oauth2/authorize?response_type=token&client_id=454q3qk5jh0rzgps78fnxrwc5u1i8t&redirect_uri=http://localhost/twitch_redirect&scope=user%3Aread%3Afollows%20moderator%3Aread%3Afollowers&state=12345
```

It will redirect the browser to `http://localhost/twitch_redirect` with some parameters, from which you can grab the user access token.

# Contents

- `include/ctwitch.h` contains common methods. Currently there's only one `twitch_init()` method defined there.
- `include/helix.h` is an umbrella header for Helix API data structs and methods.

Currently just a handful of methods from Helix are implemented.

# License

GPLv3
