// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |        MIT license - copyright (c) 2025-2026 lolasnotunix         |
// |                                                                   |
// |         pings.c - allows users to change their ping roles         |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

const struct {
    u64snowflake vc, nmajor, rasslin;
} ping_ids = { .vc      = 790051500172247040,
               .nmajor  = 790052255818186782,
               .rasslin = 1002764192681566209 };

static struct discord_application_command_option_choice _role_choices[]
   = { { .name = "vc ping", .value = "\"vc ping\"" },
       { .name = "non-major pings", .value = "\"non-major pings\"" },
       { .name = "rasslin", .value = "\"rasslin\"" } };

static struct discord_application_command_option _role_opts[] =
    { { .type = DISCORD_APPLICATION_OPTION_STRING,
                .name = "role",
                .description = "ping roles",
                .choices =
                    &(struct discord_application_command_option_choices) {
                        .array = _role_choices,
                        .size = sizeof(_role_choices) / sizeof (struct discord_application_command_option_choice),
                    },
                .required = true
    }
};

struct discord_create_guild_application_command ping_add_cmd
   = { .name        = "add_ping",
       .description = "add a ping role",
       .options
       = &(struct discord_application_command_options) { .array = _role_opts,
                                                         .size  = 1 } };

struct discord_create_guild_application_command ping_remove_cmd
   = { .name        = "remove_ping",
       .description = "remove a ping role",
       .options
       = &(struct discord_application_command_options) { .array = _role_opts,
                                                         .size  = 1 } };

#define match_role(x, y) \
    if (!strcmp (x, response)) return ping_ids.y;

u64snowflake ping_role_id (const char *response) {
    match_role ("vc ping", vc);
    match_role ("non-major pings", nmajor);
    match_role ("rasslin", rasslin);

    return 0;
}

void ping_command_interaction (struct discord *handle,
                               const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

    if (!strcmp ("add_ping", event->data->name)) {
        char response[128];

        if (event->data->options->size != 1)
            strcpy (response, "invalid command syntax");
        else {
            u64snowflake role
               = ping_role_id (event->data->options->array->value);

            if (!role) strcpy (response, "invalid role specified");
            else {
                struct discord_add_guild_member_role params
                   = { .reason = "adding ping" };

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
    } else if (!strcmp ("remove_ping", event->data->name)) {
        char response[128];

        if (event->data->options->size != 1)
            strcpy (response, "invalid command syntax");
        else {
            u64snowflake role
               = ping_role_id (event->data->options->array->value);

            if (!role) strcpy (response, "invalid role specified");
            else {
                struct discord_remove_guild_member_role params
                   = { .reason = "removing ping" };

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
