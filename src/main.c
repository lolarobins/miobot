// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |         main.c = bot setup and other initialization stuff         |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <concord/discord.h>

#include "miobot.h"

// stored app id
u64snowflake app_id;

// bot initialization
static void bot_ready (struct discord *handle,
                       const struct discord_ready *event) {
    printf ("connected to discord as %s#%s\n", event->user->username,
            event->user->discriminator);

    // store provided app id
    app_id = event->application->id;

    // register commands in all guilds
    for (size_t i = 0; i < sizeof (server_ids) / sizeof (u64snowflake); i++) {
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &pronouns_add_cmd,
           NULL);
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &pronouns_remove_cmd,
           NULL);
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &color_cmd, NULL);
    }
}

// bot interraction routines
static void bot_interaction (struct discord *handle,
                             const struct discord_interaction *event) {
    // deferring event handling to respective files
    pronoun_command_interaction (handle, event);
    color_command_interaction (handle, event);
}

// assign default role (too small to warrant its own file)
static const u64snowflake default_roles[] = {
    // test server
    1400739455907659827,
    // main server
    789248904565882881
};

static void user_join (struct discord *handle,
                       const struct discord_guild_member *event) {
    struct discord_add_guild_member_role params
       = { .reason = "adding default role" };

    discord_add_guild_member_role (
       handle, event->guild_id, event->user->id,
       default_roles[server_array_pos (event->guild_id)], &params, NULL);
}

// main function to set up bot and run
int main (int argc, char *argv[]) {
    // seed random num
    srand(time(NULL));

    // fetch bot token from file
    FILE *token_file;
    char token[128];

    if (!(token_file = fopen ("bot_token", "r"))) {
        fprintf (stderr, "could not open 'bot_token': %s\n", strerror (errno));
        return 1;
    }

    if (!fgets (token, 128, token_file)) {
        fprintf (stderr, "could not read from 'bot_token': %s\n",
                 strerror (errno));
        fclose (token_file);
        return 1;
    }

    fclose (token_file);

    // initialize bot
    ccord_global_init ();

    struct discord *handle;
    if (!(handle = discord_init (token))) {
        fprintf (stderr, "could not initialize discord client handle\n");
        ccord_global_cleanup ();
        return 1;
    }

    // add callbacks for bot
    discord_set_on_ready (handle, &bot_ready);
    discord_set_on_interaction_create (handle, &bot_interaction);
    discord_set_on_guild_member_add (handle, &user_join);
    //discord_set_on_message_reaction_add (handle, &starboard_add_reaction_cb);
    //discord_set_on_message_reaction_remove (handle,
    //                                        &starboard_remove_reaction_cb);
    //discord_set_on_message_delete (handle, &starboard_message_delete_cb);
    discord_set_on_message_create (handle, &responses_message_cb);

    // run
    discord_run (handle);

    // cleanup routine
    discord_cleanup (handle);
    ccord_global_cleanup ();
    return 0;
}
