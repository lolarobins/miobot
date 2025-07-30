#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

const struct {
    u64snowflake she, he, they, it;
} pronoun_ids[] = {
    // test server
    { .she  = 1399970513240985740,
      .he   = 1399970417338220666,
      .it   = 1399970562259816562,
      .they = 1399970539430215700 },
    // main server
    { .she  = 789243449123012608,
      .he   = 789243385289769060,
      .they = 789243421734600765,
      .it   = 789243472577560646 }
};

static struct discord_application_command_option_choice _role_choices[]
   = { { .name = "she/her", .value = "\"she/her\"" },
       { .name = "he/him", .value = "\"he/him\"" },
       { .name = "they/them", .value = "\"they/them\"" },
       { .name = "it/its", .value = "\"it/its\"" } };

static struct discord_application_command_option _role_opts[] =
    { { .type = DISCORD_APPLICATION_OPTION_STRING,
                .name = "role",
                .description = "pronoun role",
                .choices =
                    &(struct discord_application_command_option_choices) {
                        .array = _role_choices,
                        .size = 4,
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

u64snowflake pronoun_role_id (u64snowflake server_id, const char *response) {
    int pos = server_array_pos (server_id);
    if (pos == -1) return 0;

    if (!strcmp ("she/her", response)) return pronoun_ids[pos].she;
    else if (!strcmp ("he/him", response))
        return pronoun_ids[pos].he;
    else if (!strcmp ("they/them", response))
        return pronoun_ids[pos].they;
    else if (!strcmp ("it/its", response))
        return pronoun_ids[pos].it;

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
