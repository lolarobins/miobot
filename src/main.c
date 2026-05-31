// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |        MIT license - copyright (c) 2025-2026 lolasnotunix         |
// |                                                                   |
// |         main.c = bot setup and other initialization stuff         |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

// stored app id
u64snowflake app_id;

// start time
time_t start_time;

// server id
// test: 1399840781786812516
// main: 789234738350653460
u64snowflake server_id = 1399840781786812516;

// bot initialization
static void bot_ready (struct discord *handle,
                       const struct discord_ready *event) {
    printf ("connected to discord as %s#%s\n", event->user->username,
            event->user->discriminator);

    // store provided app id
    app_id = event->application->id;

    discord_create_guild_application_command (
       handle, event->application->id, server_id, &pronouns_add_cmd, NULL);
    discord_create_guild_application_command (
       handle, event->application->id, server_id, &pronouns_remove_cmd, NULL);
    discord_create_guild_application_command (handle, event->application->id,
                                              server_id, &ping_add_cmd, NULL);
    discord_create_guild_application_command (
       handle, event->application->id, server_id, &ping_remove_cmd, NULL);
    discord_create_guild_application_command (handle, event->application->id,
                                              server_id, &color_cmd, NULL);
}

// bot interraction routines
static void bot_interaction (struct discord *handle,
                             const struct discord_interaction *event) {
    // deferring event handling to respective files
    pronoun_command_interaction (handle, event);
    ping_command_interaction (handle, event);
    color_command_interaction (handle, event);
}

// assign default role (too small to warrant its own file)
static const u64snowflake default_role         = 789248904565882881,
                          verification_channel = 1388901393045262567,
                          notification_channel = 952656685221179502;

static void verify_cb (struct discord *handle,
                       const struct discord_message *event) {
    if (event->channel_id != verification_channel) return;

    // sumn wrong
    if (event->content[0] == '\0') {
        struct discord_create_message msg
           = { .content = "miobot fucked up again (ignore this!)" };
        discord_create_message (handle, 789235818367549522, &msg, NULL);
    } else {
        if (!strncasecmp ("verify", event->content, 6)) {
            struct discord_add_guild_member_role role
               = { .reason = "adding default role" };

            discord_add_guild_member_role (handle, event->guild_id,
                                           event->author->id, default_role,
                                           &role, NULL);
        } else {
            char str_buf[1024];
            snprintf (str_buf, 1024,
                      "user **%s** (<@%llu>) sent an invalid or "
                      "suspicious verification response in #waiting-room\n`%s`",
                      event->author->username, event->author->id,
                      event->content);
            strcpy (&str_buf[1024] - 5, "...`");

            struct discord_create_message msg = { .content = str_buf };

            discord_create_message (handle, notification_channel, &msg, 0);
        }
    }

    struct discord_delete_message delete
       = { "standard auto-deletion of messages in verification channel" };

    discord_delete_message (handle, event->channel_id, event->id, &delete,
                            NULL);
}

static void create_message (struct discord *handle,
                            const struct discord_message *event) {
    verify_cb (handle, event);
    responses_message_cb (handle, event);
}

// main function to set up bot and run
int main (int argc, char *argv[]) {
    // start time, used for uptime calculations
    start_time = time (NULL);

    // seed random num
    srand (time (NULL));

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
    // discord_set_on_guild_member_add (handle, &user_join);
    discord_set_on_message_create (handle, &create_message);

    // run
    discord_run (handle);

    // cleanup routine
    discord_cleanup (handle);
    ccord_global_cleanup ();
    return 0;
}
