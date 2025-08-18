// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |   responses.c - bot message responses to messages mentioning it   |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <concord/discord.h>

#include "miobot.h"
#include "responses.h"

#define RESPONSE_MAX 4096

// returns amt of characters read if match is found
static size_t _match_kv_list (const struct _kv_list_pair *list, size_t list_len,
                              const char *key, const char *value) {
    for (int i = 0; i < list_len; i++)
        if (!strcmp (key, list[i].key)) {
            for (int j = 0; j < list[i].len; j++) {
                // str len of comparing value
                size_t val_len = strlen (list[i].value[j]);

                // require matching to value in list, and requiring that
                // actual value is not followed by more text
                if (!strncasecmp (value, list[i].value[j], val_len)
                    && !isalpha (value[val_len]))
                    return val_len;
            }

            // returns 0 if key matched but input was not in array
            return 0;
        }

    // return 0 if key is not in synonym list
    return 0;
}

// specify n bytes for input, wraps _match_kv_list
static size_t _match_kv_list_nkey (const struct _kv_list_pair *list,
                                   size_t list_len, const char *key,
                                   size_t key_len, const char *value) {
    // key
    char key_str[key_len + 1];
    memcpy (key_str, key, key_len);
    key_str[key_len] = 0;

    return _match_kv_list (list, list_len, key_str, value);
}

// returns amt of characters read if matching
static size_t _match_phrase (const char *phrase, const char *input) {
    size_t phrase_len = strlen (phrase), synonym_start = -1;
    // i is defined outside to allow returning read count after
    size_t phrase_pos = 0, input_pos = 0;

    for (; phrase_pos < phrase_len; phrase_pos++) {
        if (synonym_start != -1) {
            if (phrase[phrase_pos] == '$') {
                // must offset preceeding and following $ for matching
                size_t len = _match_kv_list_nkey (
                   _synonyms, _synonyms_len, &phrase[synonym_start + 1],
                   phrase_pos - synonym_start - 1, &input[input_pos]);

                // if synonym in phrase doesnt match, return now
                if (!len) return 0;
                else
                    // accounts for leading and following $
                    input_pos += len;

                synonym_start = -1;
            }
        } else if (phrase[phrase_pos] == '$')
            synonym_start = phrase_pos;
        else if (phrase[phrase_pos] != input[input_pos])
            return 0;
        else
            input_pos++;
    }

    return input_pos;
}

// specify n bytes for input, wraps _match_phrase
// static size_t _match_phrase_n (const char *phrase, const char *input,
//                               size_t len) {
//    char cmp_str[len + 1];
//    memcpy (cmp_str, input, len);
//    cmp_str[len - 1] = 0;
//
//    return _match_phrase (phrase, cmp_str);
//}

static int rand_range (int min, int max) {
    return (rand () % (max - min + 1)) + min;
}
static int rand_int (int max) { return rand_range (0, max); }

// callback funcs to be added to struct below

// pick from range or list
static const char *_pick_response_prefix[] = { "im choosing ",
                                               "gonna go with ",
                                               "definitely ",
                                               "going w/ ",
                                               "probably ",
                                               "picking ",
                                               "" };

bool _pick_number_response (char *message, char *response) {
    const char *prefix = _pick_response_prefix[rand_int (
       (sizeof (_pick_response_prefix) / sizeof (char *)) - 1)];

    // pointers to start of number strings
    char *str_1 = message, *str_2 = NULL;
    // 1 is set to 1 so increment is not needed in if statement below
    int str_1_len = 1, str_2_len = 1;

    // read first num
    if (!(message[0] && (message[0] == '-' || isdigit (message[0]))))
        return false;
    while ((++message)[0] && isdigit (message[0])) str_1_len++;

    // bytes to skip reading message
    int skip_len = 0;

    // verify grammar syntax
    if (!(!strncasecmp (message, " and ", skip_len = 5)
          || !strncasecmp (message, " to ", skip_len = 4)))
        return false;

    message += skip_len;
    str_2 = message;

    // read second num
    if (!(message[0] && (message[0] == '-' || isdigit (message[0]))))
        return false;
    while ((++message)[0] && isdigit (message[0])) str_2_len++;

    // set null pointers for using atoi
    str_1[str_1_len] = 0, str_2[str_2_len] = 0;
    int min = atoi (str_1), max = atoi (str_2), swap = 0;

    // swap numbers if min > max
    if (min > max) {
        swap = min;
        max  = min;
        min  = swap;
    }

    snprintf (response, RESPONSE_MAX, "%s%d", prefix, rand_range (min, max));

    return true;
}

void responses_message_cb (struct discord *handle,
                           const struct discord_message *event) {
    // start of message must be mentioning bot by id
    char bot_mention_str[64];

    snprintf (bot_mention_str, 64, "<@%lld>", app_id);
    size_t mention_len = strlen (bot_mention_str);

    // pointer to start of message
    char *message = NULL;

    if (!strncmp (bot_mention_str, event->content, mention_len))
        message = event->content + mention_len + 1;
    else if (event->referenced_message
             && event->referenced_message->author->id == app_id)
        message = event->content;
    else
        return;

    // response buf
    char response[RESPONSE_MAX];
    response[0] = 0;

    for (int i = 0; i < _basic_responses_len; i++) {
        const struct _response *resp = &_basic_responses[i];
        size_t offset = 0;

        if ((offset = _match_phrase (resp->phrase, message))) {
            if (resp->callback) {
                // give message pointer after message
                if (resp->callback (message + offset, response))
                    break;
            } else {
                snprintf (response, 2048, "%s",
                          resp->list[rand_int (resp->len - 1)]);
                break;
            }
        }
    }

    // check if anything was written for a response
    if (!response[0]) return;

    struct discord_message_reference msg_ref
       = { .channel_id = event->channel_id,
           .guild_id   = event->guild_id,
           .message_id = event->id };

    struct discord_create_message msg
       = { .message_reference = &msg_ref, .content = response };

    // create post and cleanup
    discord_create_message (handle, event->channel_id, &msg, 0);
}
