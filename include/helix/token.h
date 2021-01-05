#ifndef _H_TWITCH_HELIX_TOKEN
#define _H_TWITCH_HELIX_TOKEN

/**
 * Tries to obtain app access token given client ID and secret, and a list of scopes.
 *
 * @param client_id Client ID string.
 * @param clint_secret Client secret string.
 * @param scopesv Number of strings in scopes array.
 * @param scopesc Array of scopes to request access to.
 *
 * @return Either a valid twitch_helix_auth_token struct filled with token data, or NULL in case something went wrong. It's caller's responsibility to deallocate the token with twitch_helix_auth_token_free().
 * */
twitch_helix_auth_token *twitch_helix_get_app_access_token(const char *client_id, const char *client_secret, int scopesv, char **scopesc);

#endif
