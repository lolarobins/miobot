// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |     miobot.h - all-purpose header file for the miobot project     |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <concord/discord.h>

extern u64snowflake app_id;

extern time_t start_time;

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

extern void
   starboard_add_reaction_cb (struct discord *handle,
                              const struct discord_message_reaction_add *event);
extern void starboard_remove_reaction_cb (
   struct discord *handle, const struct discord_message_reaction_remove *event);
extern void
   starboard_message_delete_cb (struct discord *handle,
                                const struct discord_message_delete *event);

extern void responses_message_cb (struct discord *handle,
                                  const struct discord_message *event);

#ifdef __cplusplus
}
#endif
