#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

// stored app id
u64snowflake app_id;

// bot initialization
void bot_ready (struct discord *handle, const struct discord_ready *event) {
    printf ("connected to discord as %s#%s\n", event->user->username,
            event->user->discriminator);

    // store provided app id
    app_id = event->application->id;

    // register commands in all guilds
    for (size_t i = 0; i < sizeof (server_ids) / sizeof (char *); i++) {
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &pronouns_add_cmd,
           NULL);

        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &pronouns_remove_cmd,
           NULL);

        // colour commands
        struct discord_create_guild_application_command color_cmd
           = { .name = "color", .description = "set your color role" };
        struct discord_create_guild_application_command colour_cmd
           = { .name        = "colour", // non-US English supremacy
               .description = "set your colour role" };

        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &pronouns_remove_cmd,
           NULL);
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &color_cmd, NULL);
        discord_create_guild_application_command (
           handle, event->application->id, server_ids[i], &colour_cmd, NULL);
    }
}

// slash command interraction
void bot_command_interaction (struct discord *handle,
                              const struct discord_interaction *event) {
    // return if not a slash command
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

}

// main function to set up bot and run
int main (int argc, char *argv[]) {
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
    discord_set_on_interaction_create (handle, &bot_command_interaction);
    discord_set_on_interaction_create (handle, &pronoun_command_interaction);

    // run
    discord_run (handle);

    // cleanup routine
    discord_cleanup (handle);
    ccord_global_cleanup ();
    return 0;
}
