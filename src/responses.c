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

#define RESPONSE_MAX 4096

static const char *_drain_responses[] = {
    "https://tenor.com/view/"
    "bladee-drain-gang-sexy-face-bladee-cute-gif-1146993514578815466",
    "https://tenor.com/view/bladee-draingang-drain-drained-dg-gif-27078473",
    "https://tenor.com/view/"
    "bladee-icarus-bladee-died-spiderr-dies-of-cringe-gif-26856765",
    "https://tenor.com/view/bladee-drain-gang-gif-22638833",
    "https://tenor.com/view/"
    "bladee-bladee-hi-chat-gluee-unreal-bladee-gif-25437197",
    "https://tenor.com/view/bladee-drain-gang-silly-kitty-review-gif-27702138"
};

static const char *_piss_response[] = {
    "I'm sorry, due to legal restrictions, I am not able to respond to this.",
    "I am not legally allowed to respond to this message.",
    "ok so let me get this straight, i am NOT into piss, i do not have a piss "
    "kink, neither does lola, do not believe the lies that the media puts out "
    "and the rumours that are spread within this community",
    "ok fr im not into it pls leave me alone"
};

static const char *_insult_responses[]
   = { "\xF0\x9F\x92\x94", "wtf did i do????", "fuck you.", "im sorry :((",
       "literally you rn: <:weathersoy:879057369043197982>" };

static const char *_is_this_true_responses[]
   = { "no",
       "fuck no",
       "https://tenor.com/view/"
       "walter-white-false-nuke-walter-white-false-breaking-bad-false-breaking-"
       "bad-false-nuke-gif-11851145952566757188",
       "https://tenor.com/view/"
       "false-that%27s-false-true-fake-not-true-gif-7463876790346893009",
       "yes.",
       "yeah",
       "<:feesh:1401969460621086762>",
       "https://tenor.com/view/"
       "trvthnvke-truth-nuke-truth-nuke-soyjakparty-gif-4864347364955876086",
       "https://tenor.com/view/"
       "true-truth-nuke-super-truth-nova-truth-meme-gif-16889273424352737553" };

static const char *_yes_no_responses[] = { "yea", "yes", "nah", "no", "idk" };

static const char *_pick_response_prefix[] = { "im choosing ",
                                               "gonna go with ",
                                               "definitely ",
                                               "going w/ ",
                                               "probably ",
                                               "picking ",
                                               "" };

// response storage struct
struct _response {
    const char *phrase;
    const char **list;
    size_t len;
    bool (*callback) (char *message, char *response);
};

static int _rand_range (int min, int max) {
    return (rand () % (max - min + 1)) + min;
}
static int _rand (int max) { return _rand_range (0, max); }

// callback funcs to be added to struct below

// pick from range or list
bool _pick_response (char *message, char *response) {
    int skip_len = 0;

    const char *prefix = _pick_response_prefix[_rand (
       (sizeof (_pick_response_prefix) / sizeof (char *)) - 1)];

    // a number
    if (!strncasecmp (message, "a number between ", skip_len = 17)
        || !strncasecmp (message, "a number from ", skip_len = 14)) {
        message += skip_len;

        // pointers to start of number strings
        char *str_1 = message, *str_2 = NULL;
        // 1 is set to 1 so increment is not needed in if statement below
        int str_1_len = 1, str_2_len = 1;

        // read first num
        if (!(message[0] && (message[0] == '-' || isdigit (message[0]))))
            return false;
        while ((++message)[0] && isdigit (message[0])) str_1_len++;

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

        snprintf (response, RESPONSE_MAX, "%s%d", prefix,
                  _rand_range (min, max));

        return true;
    }

    // between a list of items
    if (!strncasecmp (message, "between ", 8)
        || !strncasecmp (message, "from ", 5)
        || !strncasecmp (message, "one: ", 5))
        ;

    return false;
}

bool _can_you (char *message, char *response) { return false; }

#define _br_list(x)  x, sizeof (x) / sizeof (char *), NULL
#define _callback(x) NULL, 0, x

static const struct _response _responses[] = { // callbacks first
    { "pick ", _callback (_pick_response) },
    { "choose ", _callback (_pick_response) },
    //    { "can you ", _callback (_can_you) },

    // simple responses
    { "drain", _br_list (_drain_responses) },
    { "bladee", _br_list (_drain_responses) },
    { "piss", _br_list (_piss_response) },
    { "pee", _br_list (_piss_response) },
    { "urine", _br_list (_piss_response) },
    { "fuck you", _br_list (_insult_responses) },
    { "i hate you", _br_list (_insult_responses) },
    { "kys", _br_list (_insult_responses) },
    { "bitch", _br_list (_insult_responses) },
    { "kill yourself", _br_list (_insult_responses) },
    { "is this true", _br_list (_is_this_true_responses) },
    { "yes or no", _br_list (_yes_no_responses) },

    { "can ", _br_list (_yes_no_responses) },
    { "am ", _br_list (_yes_no_responses) },
    { "are ", _br_list (_yes_no_responses) },
    { "does ", _br_list (_yes_no_responses) },
    { "do ", _br_list (_yes_no_responses) }
};

void responses_message_cb (struct discord *handle,
                           const struct discord_message *event) {
    // fish react override
    if (event->referenced_message
        && !strncasecmp (event->content, "fish react this", 15)) {
        discord_create_reaction (handle, event->channel_id,
                                 event->referenced_message->id,
                                 1401969460621086762, 0, 0);
        return;
    }

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

    for (int i = 0; i < sizeof (_responses) / sizeof (struct _response); i++) {
        const struct _response *resp = &_responses[i];

        if (!strncasecmp (message, resp->phrase, strlen (resp->phrase))) {
            if (resp->callback) {
                // give message pointer after message
                if (resp->callback (message + strlen (resp->phrase), response))
                    break;
            } else {
                snprintf (response, 2048, "%s",
                          resp->list[_rand (resp->len - 1)]);
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
