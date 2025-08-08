// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |   responses.c - bot message responses to messages mentioning it   |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <stdlib.h>
#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

struct _response {
    char *phrase;
    char **list;
    size_t len;
};

static char *_drain_responses[] = {
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

static char *_piss_response[] = {
    "I'm sorry, due to legal restrictions, I am not able to respond to this.",
    "I am not legally allowed to respond to this message."
};

static char *_insult_responses[]
   = { "stfu faggot idc", "\xF0\x9F\x92\x94", "wtf did i do????", "fuck you." };

static char *_meow_responses[]
   = { "meow!", ":3", "shut up gayass", "mrow :3", "ok homo" };

static char *_is_this_true_responses[]
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

#define _r_list(x) x, sizeof (x) / sizeof (char *)

static const struct _response _responses[]
   = { { "drain", _r_list (_drain_responses) },
       { "bladee", _r_list (_drain_responses) },
       { "piss", _r_list (_piss_response) },
       { "does", _r_list (_is_this_true_responses) },
       { "fuck you", _r_list (_insult_responses) },
       { "i hate you", _r_list (_insult_responses) },
       { "you are gay", _r_list (_insult_responses) },
       { "you're gay", _r_list (_insult_responses) },
       { "your gay", _r_list (_insult_responses) },
       { "kys", _r_list (_insult_responses) },
       { "kill yourself", _r_list (_insult_responses) },
       { "meow", _r_list (_meow_responses) },
       { "mrow", _r_list (_meow_responses) },
       { "nya", _r_list (_meow_responses) },
       { "is this true", _r_list (_is_this_true_responses) } };

static int _rand (int max) { return rand () % (max + 1); }

void responses_message_cb (struct discord *handle,
                           const struct discord_message *event) {
    // start of message must be mentioning bot by id
    char bot_mention_str[64];

    snprintf (bot_mention_str, 64, "<@%lld>", app_id);
    size_t mention_len = strlen (bot_mention_str);

    if (strncmp (bot_mention_str, event->content, mention_len)) return;

    // get str ptr after mention
    char *message = event->content + mention_len + 1;
    // response buf
    char response[2048];
    response[0] = 0;

    for (int i = 0; i < sizeof (_responses) / sizeof (struct _response); i++)
        if (!strncasecmp (message, _responses[i].phrase,
                          strlen (_responses[i].phrase))) {
            snprintf (response, 2048, "%s",
                      _responses[i].list[_rand (_responses[i].len - 1)]);
            break;
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
