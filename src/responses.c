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
    "ok fr im not into it pls leave me alone",
    "this is a piss-free community"
};

static const char *_insult_responses[]
   = { "\xF0\x9F\x92\x94",
       "wtf did i do????",
       "fuck you.",
       "im sorry :((",
       "literally you rn: <:weathersoy:879057369043197982>",
       "https://tenor.com/view/"
       "sad-cry-sniffle-emoji-tears-gif-13575003772459623402" };

static const char *_threat_responses[]
   = { ":((", "please dont", "\xF0\x9F\x92\x94" };

static const char *_greeting_responses[]
   = { "hai!!!", "hi", "hello",
       "https://tenor.com/view/"
       "bladee-bladee-hi-chat-gluee-unreal-bladee-gif-25437197" };

static const char *_farewell_responses[]
   = { "bye :3", "bye", "cya",
       "https://tenor.com/view/bladee-bye-drain-gang-gif-34981710625335844" };

static const char *_goodmorning_responses[]
   = { "https://media.discordapp.net/attachments/789244345915277411/"
       "1400290840974196757/"
       "12898552_56eb2.gif?ex=689a9a78&is=689948f8&hm="
       "9dbc096359907c0d1b9b00179a338a3abf2a2a5e84ca7be14001c41db0461a04&=&"
       "width=1000&height=1000",
       "https://images-ext-1.discordapp.net/external/"
       "KmpLp7DAqfCCH5k-tvvoykbm1xGvo0uSTD3lU0-GmQ0/https/media.tenor.com/"
       "JeLaN-8HthUAAAPo/toma-bad-morning-toma.mp4",
       "good morning.", "goodmorning!!!" };

static const char *_goodnight_responses[] = {
    "https://media.discordapp.net/attachments/789244345915277411/"
    "1400290841272123413/"
    "12898593_51253.gif?ex=689a9a78&is=689948f8&hm="
    "5ded6b26de331745b864f45210dff7a2cefeef0406bba22fa44af4414142d3a3&=&width="
    "1000&height=1000",
    "https://tenor.com/view/"
    "rei-evangelion-dabbingflavorx-good-night-gif-5757985808685799828",
    "gn<3", "goodnight!", "good night."
};

static const char *_brainrot_meme_responses[]
   = { "please, sybau.", "dawg literally shut the fuck up", "-1/10 brainrot" };

static const char *_dap_up_responses[] = {
    "https://tenor.com/view/"
    "bornskywalker-dap-me-up-woody-woody-handshake-woody-toy-story-gif-26021440",
    "dap me up twin"
};

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
       "https://media.discordapp.net/attachments/1399840782416085024/"
       "1404313152086212640/"
       "image.png?ex=689abc0a&is=68996a8a&hm="
       "60058a32720986fdf2d9a635f8f6f970beae56e9a8dd588717f89d74953700bb&=&"
       "format=webp&quality=lossless&width=1876&height=1876",
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
static bool _pick_response (char *message, char *response) {
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

    // between a list of items, comma seperated list, max 32
    if (!strncasecmp (message, "between ", skip_len = 8)
        || !strncasecmp (message, "from ", skip_len = 5)
        || !strncasecmp (message, "one: ", skip_len = 5)) {
        message += skip_len;

        char *choices[8]  = { 0 };
        char *current_ptr = message;
        int current_len   = 0;

        // seperate by commas, remove "or", check for end
    }

    return false;
}

// static bool _can_you (char *message, char *response) { return false; }

#define _br_list(x)  x, sizeof (x) / sizeof (char *), NULL
#define _callback(x) NULL, 0, x

// list to iterate through for callbacks and responses at the start of a reply
static const struct _response _basic_responses[] = { // callbacks first
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

    // this has to exist for a reason, will be changed up on approach
    { "im gonna kms", _br_list (_threat_responses) },
    { "im going to kms", _br_list (_threat_responses) },
    { "im gonna kill myself", _br_list (_threat_responses) },
    { "im going to kill myself", _br_list (_threat_responses) },
    { "killing myself", _br_list (_threat_responses) },
    { "should i kms", _br_list (_threat_responses) },
    { "should i kill myself", _br_list (_threat_responses) },

    { "hi", _br_list (_greeting_responses) },
    { "hello", _br_list (_greeting_responses) },
    { "hai", _br_list (_greeting_responses) },
    { "hi", _br_list (_greeting_responses) },

    { "bye", _br_list (_farewell_responses) },
    { "goodbye", _br_list (_farewell_responses) },
    { "good bye", _br_list (_farewell_responses) },
    { "cya", _br_list (_farewell_responses) },

    { "goodmorning", _br_list (_goodmorning_responses) },
    { "good morning", _br_list (_goodmorning_responses) },
    { "gm", _br_list (_goodmorning_responses) },

    { "goodnight", _br_list (_goodnight_responses) },
    { "good night", _br_list (_goodnight_responses) },
    { "gn", _br_list (_goodnight_responses) },

    { "dubai", _br_list (_brainrot_meme_responses) },
    { "matcha", _br_list (_brainrot_meme_responses) },
    { "sybau", _br_list (_brainrot_meme_responses) },
    { "sdyibt", _br_list (_brainrot_meme_responses) },

    { "im gay", _br_list (_dap_up_responses) },
    { "im trans", _br_list (_dap_up_responses) },

    { "is this true", _br_list (_is_this_true_responses) },
    { "true or false", _br_list (_is_this_true_responses) },

    { "yes or no", _br_list (_yes_no_responses) },
    { "can", _br_list (_yes_no_responses) },
    { "am", _br_list (_yes_no_responses) },
    { "are", _br_list (_yes_no_responses) },
    { "does", _br_list (_yes_no_responses) },
    { "do", _br_list (_yes_no_responses) },
    { "is", _br_list (_yes_no_responses) },
    { "will", _br_list (_yes_no_responses) }
};

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

    for (int i = 0; i < sizeof (_basic_responses) / sizeof (struct _response);
         i++) {
        const struct _response *resp = &_basic_responses[i];

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
