#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

// response storage struct
struct _response {
    const char *phrase;
    const char **list;
    size_t len;
    bool (*callback) (char *message, char *response);
};

// string key / string array value pair
struct _kv_list_pair {
    // string key
    const char *key;
    // NULL terminated string list
    const char **value;
    // string list len
    const size_t len;
};

extern const struct _response *_basic_responses;
extern const size_t _basic_responses_len;

extern const struct _kv_list_pair *_synonyms;
extern const size_t _synonyms_len;

// callbacks for internal use
extern bool _pick_response (char *message, char *response);

#ifdef __cplusplus
}
#endif
