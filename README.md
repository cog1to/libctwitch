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

There's a sample usage app in `example/` directory. You can also build it with
`make twitch-remote` command.

## Access tokens

Helix API requires OAuth access tokens to use. There are two types of tokens:
app access and user access tokens. App access token can be used to query public
information on channels, videos, etc. and for search queries. User access tokens
should be used when querying private user data (like user's followed channels),
and they have to be created with an appropriate scope that allows access to the
desired information.

`ctwitch/auth.h` header has a couple of methods for managing tokens. If you're
only interested in public data, you can get an app access token with your
client ID and client secret, which you obtain by registering an app at Twitch
Dev portal. To get an app access token, call `twitch_get_app_access_token()`.

User access tokens have to be obtained outside of this library, because they
require a user's authorization through a web browser.

For the implicit grant flow, you have to open an URL like this one:

```
https://id.twitch.tv/oauth2/authorize?response_type=token&client_id=454q3qk5jh0rzgps78fnxrwc5u1i8t&redirect_uri=http://localhost/twitch_redirect&scope=user%3Aread%3Afollows%20moderator%3Aread%3Afollowers&state=12345
```

There `twitch_redirect` param has to match the redirect URL that you set up for
your app in Twitch Dev portal, and `client_id` must be your app's client ID.
After granting the permission, the browser will get redirected to the
`twitch_redirect` URL from which you can grab the user access token provided in
`access_token` fragment.

For the authorization code flow, the link will look like this:

```
https://id.twitch.tv/oauth2/authorize?response_type=code&client_id=454q3qk5jh0rzgps78fnxrwc5u1i8t&redirect_uri=http://localhost/twitch_redirect&scope=channel%3Amanage%3Apolls+channel%3Aread%3Apolls&state=c3ab8aa609ea11e793ae92361f002671
```

It will redirect you to the URL containing an authorization code which you can
exchange into user access token and a refresh token using
`twitch_get_user_access_token()`.

You can also refresh expired tokens obtained from the authorization code
flow using `twitch_refresh_access_token()`.

# Contents

- `include/ctwitch/ctwitch.h` contains common methods. Currently there's only
  one `twitch_init()` method defined there, which calls an init method for CURL.
- `include/ctwitch/common.h` contains some common data structures.
- `include/ctwitch/auth.h` contains various methods for getting access tokens.
- `include/ctwich/helix.h` is an umbrella header for Helix API data structs and
  methods.

Currently just a handful of methods from Helix are implemented.

# License

GPLv2
