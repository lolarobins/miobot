// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |      pronouns.c - allows users to change their pronoun roles      |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

const struct {
    u64snowflake she, he, they, it, any;
} pronoun_ids[] = {
    // test server
    { .she  = 1399970513240985740,
      .he   = 1399970417338220666,
      .it   = 1399970562259816562,
      .they = 1399970539430215700,
      .any  = 1400025156914778193 },
    // main server
    { .she  = 789243449123012608,
      .he   = 789243385289769060,
      .they = 789243421734600765,
      .it   = 789243472577560646,
      .any  = 1009951522735534120 }
};

static struct discord_application_command_option_choice _role_choices[]
   = { { .name = "she/her", .value = "\"she/her\"" },
       { .name = "he/him", .value = "\"he/him\"" },
       { .name = "they/them", .value = "\"they/them\"" },
       { .name = "it/its", .value = "\"it/its\"" },
       { .name = "any/all", .value = "\"any/all\"" } };

static struct discord_application_command_option _role_opts[] =
    { { .type = DISCORD_APPLICATION_OPTION_STRING,
                .name = "role",
                .description = "pronoun role",
                .choices =
                    &(struct discord_application_command_option_choices) {
                        .array = _role_choices,
                        .size = 5,
                    },
                .required = true
    }
};

struct discord_create_guild_application_command pronouns_add_cmd
   = { .name        = "add_pronouns",
       .description = "add a pronoun role",
       .options
       = &(struct discord_application_command_options) { .array = _role_opts,
                                                         .size  = 1 } };

struct discord_create_guild_application_command pronouns_remove_cmd
   = { .name        = "remove_pronouns",
       .description = "remove a pronoun role",
       .options
       = &(struct discord_application_command_options) { .array = _role_opts,
                                                         .size  = 1 } };

#define match_role(x, y) \
    if (!strcmp (x, response)) return pronoun_ids[pos].y;

u64snowflake pronoun_role_id (u64snowflake server_id, const char *response) {
    int pos = server_array_pos (server_id);
    if (pos == -1) return 0;

    match_role ("she/her", she);
    match_role ("he/him", he);
    match_role ("they/them", they);
    match_role ("it/its", it);
    match_role ("any/all", any);

    return 0;
}

void pronoun_command_interaction (struct discord *handle,
                                  const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

    if (!strcmp ("add_pronouns", event->data->name)) {
        char response[128];

        if (event->data->options->size != 1)
            strcpy (response, "invalid command syntax");
        else {
            u64snowflake role = pronoun_role_id (
               event->guild_id, event->data->options->array->value);

            if (!role) strcpy (response, "invalid role specified");
            else {
                struct discord_add_guild_member_role params
                   = { .reason = "adding pronouns" };

                discord_add_guild_member_role (handle, event->guild_id,
                                               event->member->user->id, role,
                                               &params, NULL);

                sprintf (response, "<@&%llu> role added", role);
            }
        }

        struct discord_interaction_response message
           = { .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
               .data = &(struct discord_interaction_callback_data) {
                  .content = response,
                  .flags   = DISCORD_MESSAGE_EPHEMERAL } };

        discord_create_interaction_response (handle, event->id, event->token,
                                             &message, NULL);
    } else if (!strcmp ("remove_pronouns", event->data->name)) {
        char response[128];

        if (event->data->options->size != 1)
            strcpy (response, "invalid command syntax");
        else {
            u64snowflake role = pronoun_role_id (
               event->guild_id, event->data->options->array->value);

            if (!role) strcpy (response, "invalid role specified");
            else {
                struct discord_remove_guild_member_role params
                   = { .reason = "removing pronouns" };

                discord_remove_guild_member_role (handle, event->guild_id,
                                                  event->member->user->id, role,
                                                  &params, NULL);

                sprintf (response, "<@&%llu> role removed", role);
            }
        }

        struct discord_interaction_response message
           = { .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
               .data = &(struct discord_interaction_callback_data) {
                  .content = response,
                  .flags   = DISCORD_MESSAGE_EPHEMERAL } };

        discord_create_interaction_response (handle, event->id, event->token,
                                             &message, NULL);
    }
}
