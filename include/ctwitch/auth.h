/**
 * Twitch API: Authorization
 *
 * @author Alexander Rogachev
 * @version 0.1
 *
 * Twitch uses OAuth for granting access tokens to apps. There are 2 types of
 * tokens and 4 different ways to obtain them.
 *
 * "App access token" is a token that can be used to query public data from
 * Twitch API. It cannot be used for querying user specific data, like their
 * follow list. App access token can be obtained by "client credentials" grant
 * flow, which requires a valid client app registered in a Twitch Dev portal,
 * from where you can obtain Client ID and Client Secret strings for your app.
 *
 * Given those two strings, you can call twitch_get_app_access_token() function
 * to obtain a valid app access token. These tokens don't have a TTL (as far as
 * I know) and can be used indefinitely.
 *
 * "User access token" is a token that can be used, given the right "scope", to
 * access public AND private data on Twitch. There are 3 ways to get them:
 *   - Implicit grant flow
 *   - Authorization code flow
 *   - Device grant flow
 *
 * Twitch recommends using user access token instead of app acces token,
 * because it, in most cases, gives you access to both public and private data.
 *
 * Implicit grant flow requires you to construct a link for a browser that, when
 * opened, will prompt you to grant access to your app, listing requested
 * scopes. Granting the access will redirect the browser to a redirect URL,
 * where fragment portion (the stuff after '#' and before any params) will
 * contain the access token.
 *
 * Example:
 *
 * - Auth URL: https://id.twitch.tv/oauth2/authorize?response_type=token&client_id=hof5gwx0su6owfnys0yan9c87zr6t&redirect_uri=http://localhost:3000&scope=channel%3Amanage%3Apolls+channel%3Aread%3Apolls&state=c3ab8aa609ea11e793ae92361f002671
 * - Redirect URL: http://localhost:3000/#access_token=73d0f8mkabpbmjp921asv2jaidwxn&scope=channel%3Amanage%3Apolls+channel%3Aread%3Apolls&state=c3ab8aa609ea11e793ae92361f002671&token_type=bearer
 *
 * Implicit grant flow does not return expiration date or a refresh token, so it
 * is unclear whether the access token has infinite TTL or not. The Twitch
 * developer docs are messy in that part. There's a section in implicit grant
 * flow description that should belong to authorization code grant flow instead.
 *
 * Authorization code grant flow is more complex, but it is more flexible. It
 * also requires opening an URL in a browser, but with "code" response_type
 * parameter instead of "token", and it will redirect it back to an URL that
 * will contain an Authorization Code.
 *
 * Example:
 * - Auth URL: https://id.twitch.tv/oauth2/authorize?response_type=code&client_id=hof5gwx0su6owfnys0nyan9c87zr6t&redirect_uri=http://localhost:3000&scope=channel%3Amanage%3Apolls+channel%3Aread%3Apolls&state=c3ab8aa609ea11e793ae92361f002671
 * - Redirect URL: http://localhost:3000/?code=gulfwdmys5lsm6qyz4xiz9q32l10&scope=channel%3Amanage%3Apolls+channel%3Aread%3Apolls&state=c3ab8aa609ea11e793ae92361f002671
 *
 * We can exchange the code from redirect URL for a user access token using
 * twitch_get_user_access_token() function, providing both client ID and secret.
 *
 * When user access token expires, we can refresh it using
 * twitch_refresh_user_access_token() function, again using client ID and
 * secret, and a refresh token string from the user access token response.
 *
 * There's also Device code grant flow, but I am not yet familiar with how it
 * works, so I'll skip it for now.
 */

#ifndef _H_TWITCH_AUTH
#define _H_TWITCH_AUTH

#include <ctwitch/common.h>

/** App access token **/
typedef struct {
	char *token;
	int expires_in;
	char *token_type;
} twitch_app_access_token;

/** User access token **/
typedef struct {
	char *access_token;
	char *refresh_token;
	int expires_in;
	char *token_type;
	twitch_string_list scopes;
} twitch_user_access_token;

/**
 * Allocates and clears memory for new twitch_app_access_token struct.
 *
 * @return Pointer to allocated twitch_app_access_token struct.
 */
twitch_app_access_token *twitch_app_access_token_alloc();

/**
 * Frees the memory for given twitch_app_access_token struct and all its
 * properties.
 *
 * @param token Struct to deallocate.
 */
void twitch_app_access_token_free(twitch_app_access_token *token);

/**
 * Allocates and clears memory for new twitch_user_access_token struct.
 *
 * @return Pointer to allocated twitch_user_access_token struct.
 */
twitch_user_access_token *twitch_user_access_token_alloc();

/**
 * Frees the memory for given twitch_user_access_token struct and all its
 * properties.
 *
 * @param token Struct to deallocate.
 */
void twitch_user_access_token_free(twitch_user_access_token *token);

/**
 * Tries to obtain app access token given client ID and secret using client
 * credentials grant flow. App access tokens have limited API access, and cannot
 * be used to access account data, like specific user's follows.
 *
 * @param client_id Client ID string.
 * @param clint_secret Client secret string.
 *
 * @return Either a valid twitch_app_access_token struct filled with token data,
 * or NULL in case something went wrong. It's caller's responsibility to
 * deallocate the token with twitch_app_access_token_free().
 */
twitch_app_access_token *twitch_get_app_access_token(
	const char *client_id,
	const char *client_secret
);

/**
 * Tries to obtain a user access token from given authorization code from
 * Authorization Code grant flow.
 *
 * @param client_id Client ID string.
 * @param client_secret Client secret sctring.
 * @param code Authorization code string obtained by OAuth flow with
 * "code" grant type.
 * @param redirect_uri Redirect URI string registered for given client.
 *
 * @return Either a valid user acess token or NULL if there were some errors.
 */
twitch_user_access_token *twitch_get_user_access_token(
	const char *client_id,
	const char *client_secret,
	const char *code,
	const char *redirect_uri
);

/**
 * Tries to refresh a user access token using given refresh token obtained from
 * Authorization Code grant flow.
 *
 * @param client_id Client ID string.
 * @param client_secret Client secret sctring.
 * @param token Refresh token string.
 *
 * @return Either a valid user acess token or NULL if there were some errors.
 */
twitch_user_access_token *twitch_refresh_access_token(
	const char *client_id,
	const char *client_secret,
	const char *token
);

#endif
