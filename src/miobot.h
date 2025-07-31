#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <concord/discord.h>

extern const u64snowflake server_ids[2];

extern int server_array_pos (u64snowflake server_id);

extern struct discord_create_guild_application_command pronouns_add_cmd;
extern struct discord_create_guild_application_command pronouns_remove_cmd;

extern void
   pronoun_command_interaction (struct discord *handle,
                                const struct discord_interaction *event);

extern struct discord_create_guild_application_command color_cmd;

extern void color_command_interaction (struct discord *handle,
                                       const struct discord_interaction *event);

#ifdef __cplusplus
}
#endif
