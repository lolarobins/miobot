#include "responses.h"

// groups of general / catch-all responses
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
    "this is a piss-free community",
    "*from Brooxie:*\\n\\nSo Im going to to be honest: This server SUCKS now "
    "that Hornet Disaster has released. Everyone had group INSANITY during the "
    "creation of album of 2025, now everyone is just STUPID and privileged. "
    "Nobody even CARES about Weatherday OR Sputnik anymore. Its just PISS this "
    "and PISS that. I DON'T care about PISS. We should talk about leading "
    "fifth wave emo artist WEATHERDAY, but there is nothing to say anymore. I "
    "blame this on the IDIOTS that take part in this server and the "
    "GATEKEEPERS who keep Weatherday for the GAYS ONLY.  This community will "
    "SUCK until the album of 2030 eventually gets revealed, but even then it "
    "might still only be ABOUT PISS. NOBODY LIKES PISS. EVERYBODY SHOULD LIKE "
    "WEATHERDAY. Thank you for your attention to this matter."
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

static const char *_dap_up_responses[]
   = { "https://tenor.com/view/"
       "bornskywalker-dap-me-up-woody-woody-handshake-woody-toy-story-gif-"
       "26021440",
       "dap me up twin" };

static const char *_user_gay_trans_q_responses[]
   = { "you definitely are",   "maybe idk", "if ur asking me this, most likely",
       "how tf would i know?", "extremely", "erm, clocked as queer" };
static const char *_bot_gay_trans_q_responses[]
   = { "i do not know gendor or sex i am a bot", "im literally just a bot",
       "im just puter idk this whole queer thing", "im a mystery",
       "i cant be anything" };

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

#define _br_list(x)  x, sizeof (x) / sizeof (char *), NULL
#define _callback(x) NULL, 0, x

// basic phrases / sentence starters to look for
static const struct _response __basic_responses[] = { // callbacks first
    { "pick ", _callback (_pick_response) },
    { "choose ", _callback (_pick_response) },
    //    { "can you ", _callback (_can_you) },

    // simple responses
    { "drain", _br_list (_drain_responses) },
    { "bladee", _br_list (_drain_responses) },

    { "$piss$", _br_list (_piss_response) },

    { "fuck $you$", _br_list (_insult_responses) },
    { "i hate $you$", _br_list (_insult_responses) },
    { "$kys$", _br_list (_insult_responses) },
    { "bitch", _br_list (_insult_responses) },
    { "clanker", _br_list (_insult_responses) },
    { "$youre$ a clanker", _br_list (_insult_responses) },

    // this has to exist for a reason, will be changed up on approach
    { "$im$ $gonna$ $kms$", _br_list (_threat_responses) },
    { "$gonna$ $kms$", _br_list (_threat_responses) },
    { "i $wanna$ $kms$", _br_list (_threat_responses) },
    { "$wanna$ $kms$", _br_list (_threat_responses) },
    { "$kms$", _br_list (_threat_responses) },
    { "should i $kms$", _br_list (_threat_responses) },

    { "$hi$", _br_list (_greeting_responses) },
    { "$bye$", _br_list (_farewell_responses) },
    { "$gm$", _br_list (_goodmorning_responses) },
    { "$gn$", _br_list (_goodnight_responses) },

    { "dubai", _br_list (_brainrot_meme_responses) },
    { "matcha", _br_list (_brainrot_meme_responses) },
    { "sybau", _br_list (_brainrot_meme_responses) },
    { "sdyibt", _br_list (_brainrot_meme_responses) },

    { "$im$ $gay$", _br_list (_dap_up_responses) },
    { "$im$ $trans$", _br_list (_dap_up_responses) },
    { "am i $gay$", _br_list (_user_gay_trans_q_responses) },
    { "am i $trans$", _br_list (_user_gay_trans_q_responses) },
    { "$are$ $you$ $gay$", _br_list (_bot_gay_trans_q_responses) },
    { "$are$ $you$ $trans$", _br_list (_bot_gay_trans_q_responses) },
    { "$youre$ $gay$", _br_list (_bot_gay_trans_q_responses) },
    { "$youre$ $trans$", _br_list (_bot_gay_trans_q_responses) },

    { "is $this$ true", _br_list (_is_this_true_responses) },
    { "true or false", _br_list (_is_this_true_responses) },

    { "yes or no", _br_list (_yes_no_responses) },
    { "can", _br_list (_yes_no_responses) },
    { "am", _br_list (_yes_no_responses) },
    { "are", _br_list (_yes_no_responses) },
    { "does", _br_list (_yes_no_responses) },
    { "do", _br_list (_yes_no_responses) },
    { "is", _br_list (_yes_no_responses) },
    { "will ", _br_list (_yes_no_responses) },
    { "should ", _br_list (_yes_no_responses) }
};

const struct _response *_basic_responses = __basic_responses;

const size_t _basic_responses_len
   = sizeof (__basic_responses) / sizeof (struct _response);

// synonym defs, all have to be declared in global scope to be initialized
// for synonym array below
static const char *__you[]      = { "you", "u" };
static const char *__your[]     = { "your", "ur" };
static const char *__youre[]    = { "youre", "ur", "u r", "you're", "you are" };
static const char *__im[]       = { "im", "i am", "i'm" };
static const char *__are[]      = { "are", "r" };
static const char *__why[]      = { "why", "y" };
static const char *__its[]      = { "its", "it's", "it is" };
static const char *__ive[]      = { "ive", "i've", "i have" };
static const char *__this[]     = { "this", "ts" };
static const char *__dont[]     = { "dont", "don't", "do not" };
static const char *__gonna[]    = { "gonna", "going to" };
static const char *__wanna[]    = { "wanna", "want to" };
static const char *__shouldve[] = { "shouldve", "should've", "should have" };
static const char *__yes[]      = { "yes", "yeah", "yea", "ye" };
static const char *__no[]       = { "nah", "nope", "no" };
static const char *__good[]
   = { "good", "goated", "amazing", "excellent", "swag", "good asf" };
static const char *__bad[] = { "bad", "shit", "awful" };

// CONTENT WARNING: this is being used in a server with edgy gay people, who
// more than qualify to use these words
static const char *__gay[]
   = { "gay",        "queer", "lesbian",  "a faggot", "a fag",    "a homo",
       "homosexual", "bi",    "bisexual", "pan",      "pansexual" };
static const char *__trans[]
   = { "transgender", "nb",           "an nb",       "enby",      "non-binary",
       "non binary",  "a trans girl", "a trans guy", "transmasc", "transfem",
       "trans",       "tranny",       "a tranny" };

static const char *__kms[] = { "kms", "kill myself", "kill my self",
                               "killing myself", "killing my self" };
static const char *__kys[] = { "kys", "kill yourself", "kill your self" };

// smh
static const char *__piss[] = { "piss", "pee", "urine" };

static const char *__hi[]  = { "hi", "hai", "hello" };
static const char *__bye[] = { "bye", "goodbye", "good bye" };
static const char *__gn[]  = { "goodnight", "gn", "good night" };
static const char *__gm[]  = { "goodmorning", "gm", "good morning" };

#define _list_item(x) x, sizeof (x) / sizeof (char *)

// null terminated list of synonyms for common words used interchangably
static const struct _kv_list_pair __synonyms[]
   = { { "you", _list_item (__you) },
       { "your", _list_item (__your) },
       { "youre", _list_item (__youre) },
       { "im", _list_item (__im) },
       { "are", _list_item (__are) },
       { "why", _list_item (__why) },
       { "its", _list_item (__its) },
       { "ive", _list_item (__ive) },
       { "this", _list_item (__this) },
       { "dont", _list_item (__dont) },
       { "gonna", _list_item (__gonna) },
       { "wanna", _list_item (__wanna) },
       { "shouldve", _list_item (__shouldve) },
       { "yes", _list_item (__yes) },
       { "no", _list_item (__no) },
       { "good", _list_item (__good) },
       { "bad", _list_item (__bad) },
       { "gay", _list_item (__gay) },
       { "trans", _list_item (__trans) },
       { "kms", _list_item (__kms) },
       { "kys", _list_item (__kys) },
       { "piss", _list_item (__piss) },
       { "hi", _list_item (__hi) },
       { "bye", _list_item (__bye) },
       { "gm", _list_item (__gm) },
       { "gn", _list_item (__gn) } };

const struct _kv_list_pair *_synonyms = __synonyms;

const size_t _synonyms_len
   = sizeof (__synonyms) / sizeof (struct _kv_list_pair);
