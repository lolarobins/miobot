#include <concord/discord.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

const u64snowflake registered_guilds[] = {
    // bot test server
    1399840781786812516,
    // sputnik supporters
    789234738350653460
};
u64snowflake app_id;

// bot initialization
void bot_ready (struct discord *handle, const struct discord_ready *event) {
    printf ("connected to discord as %s#%s\n", event->user->username,
            event->user->discriminator);

    // store provided app id
    app_id = event->application->id;

    // define available commands
    struct discord_create_guild_application_command pronouns
       = { .name = "pronouns", .description = "manage your pronoun roles" };
    struct discord_create_guild_application_command color
       = { .name = "color", .description = "manage your color role" };
    struct discord_create_guild_application_command colour
       = { .name        = "colour", // non-US English supremacy
           .description = "manage your colour role" };

    // register commands in all guilds
    for (size_t i = 0; i < sizeof (registered_guilds) / sizeof (char *); i++) {
        discord_create_guild_application_command (
           handle, event->application->id, registered_guilds[i], &pronouns,
           NULL);
        discord_create_guild_application_command (
           handle, event->application->id, registered_guilds[i], &color, NULL);
        discord_create_guild_application_command (
           handle, event->application->id, registered_guilds[i], &colour, NULL);
    }
}

// slash command interraction
void bot_command_interaction (struct discord *client,
                              const struct discord_interaction *event) {
    // return if not a slash command
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

    // pronouns command
    if (strcmp (event->data->name, "pronouns") == 0) {
        struct discord_interaction_response params
           = { .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
               .data = &(struct discord_interaction_callback_data) {
                  .content = "meow :3",
                  .flags   = DISCORD_MESSAGE_EPHEMERAL } };

        discord_create_interaction_response (client, event->id, event->token,
                                             &params, NULL);
    }
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

    // run
    discord_run (handle);

    // cleanup routine
    discord_cleanup (handle);
    ccord_global_cleanup ();
    return 0;
}
