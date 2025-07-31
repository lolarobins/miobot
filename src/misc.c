// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |        misc.c - miscellaneous functions and consts for use        |
// |                        across the project                         |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+


#include <concord/discord.h>

#include "miobot.h"

const u64snowflake server_ids[2] = {
    // bot test server (0)
    1399840781786812516,
    // sputnik supporters (1)
    789234738350653460
};

int server_array_pos (u64snowflake server_id) {
    for (int i = 0; i < sizeof (server_ids) / sizeof (u64snowflake); i++)
        if (server_ids[i] == server_id) return i;
    return -1;
}
