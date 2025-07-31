#include <string.h>

#include <concord/discord.h>

#include "miobot.h"

const struct color_id {
    u64snowflake red, orange, yellow, lime, green, cyan, blue, indigo, purple,
       lavender, pink, light_pink, brown, white, gray;
} color_ids[] = {
    // test server
    { .red        = 1400267447440769025,
      .orange     = 1400265820445212742,
      .yellow     = 0,
      .lime       = 0,
      .green      = 0,
      .cyan       = 0,
      .blue       = 0,
      .indigo     = 0,
      .purple     = 0,
      .lavender   = 0,
      .pink       = 0,
      .light_pink = 0,
      .brown      = 0,
      .white      = 0,
      .gray       = 0 },
    // main server
    { .red        = 789253280374849636,
      .orange     = 789254164777271348,
      .yellow     = 789254310919798825,
      .lime       = 789254403739615274,
      .green      = 789254531007250433,
      .cyan       = 789254639815622677,
      .blue       = 789254713346228245,
      .indigo     = 789254763677089852,
      .purple     = 789254897563992076,
      .lavender   = 789255092104593428,
      .pink       = 789255179660034069,
      .light_pink = 789255244370280481,
      .brown      = 789255310962982932,
      .white      = 789255440059334657,
      .gray       = 789255543679746049 }
};

static struct discord_application_command_option_choice _role_choices[]
   = { { .name = "red", .value = "\"red\"" },
       { .name = "orange", .value = "\"orange\"" },
       { .name = "yellow", .value = "\"yellow\"" },
       { .name = "lime", .value = "\"lime\"" },
       { .name = "green", .value = "\"green\"" },
       { .name = "cyan", .value = "\"cyan\"" },
       { .name = "blue", .value = "\"blue\"" },
       { .name = "indigo", .value = "\"indigo\"" },
       { .name = "purple", .value = "\"purple\"" },
       { .name = "lavender", .value = "\"lavender\"" },
       { .name = "pink", .value = "\"pink\"" },
       { .name = "light pink", .value = "\"light pink\"" },
       { .name = "brown", .value = "\"brown\"" },
       { .name = "white", .value = "\"white\"" },
       { .name = "gray", .value = "\"gray\"" } };

static struct discord_application_command_option _role_opts[] =
    { { .type = DISCORD_APPLICATION_OPTION_STRING,
                .name = "role",
                .description = "color role",
                .choices =
                    &(struct discord_application_command_option_choices) {
                        .array = _role_choices,
                        .size = 15,
                    },
                .required = true
    }
};

struct discord_create_guild_application_command color_cmd
   = { .name        = "color",
       .description = "set a color role",
       .options
       = &(struct discord_application_command_options) { .array = _role_opts,
                                                         .size  = 1 } };

#define match_role(x, y) \
    if (!strcmp (x, response)) return color_ids[pos].y;

u64snowflake color_role_id (u64snowflake server_id, const char *response) {
    int pos = server_array_pos (server_id);
    if (pos == -1) return 0;

    match_role ("red", red);
    match_role ("orange", orange);
    match_role ("yellow", yellow);
    match_role ("lime", lime);
    match_role ("green", green);
    match_role ("cyan", cyan);
    match_role ("blue", blue);
    match_role ("indigo", indigo);
    match_role ("purple", purple);
    match_role ("lavender", lavender);
    match_role ("pink", pink);
    match_role ("light pink", light_pink);
    match_role ("brown", brown);
    match_role ("white", white);
    match_role ("gray", gray);

    return 0;
}

void color_command_interaction (struct discord *handle,
                                const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND) return;

    // if it isnt either, return now so rest of func isnt nested
    if (strcmp ("color", event->data->name)) return;

    char response[128];

    if (event->data->options->size != 1)
        strcpy (response, "invalid command syntax");
    else {
        u64snowflake role = color_role_id (event->guild_id,
                                           event->data->options->array->value);

        if (!role) strcpy (response, "invalid role specified");
        else {
            // define role add/remove reasons
            struct discord_add_guild_member_role add_params
               = { .reason = "setting new color role" };
            struct discord_remove_guild_member_role remove_params
               = { .reason = "replacing color role" };

            // remove existing color roles
            // note: cannot remove the target role and add it back, discord api
            // seems to not do things in order of them being called
            for (int i = 0, pos = server_array_pos (event->guild_id);
                 i < sizeof (struct color_id) / sizeof (u64snowflake); i++) {
                u64snowflake target = ((u64snowflake *) &color_ids[pos])[i];

                if (target != role)
                    discord_remove_guild_member_role (
                       handle, event->guild_id, event->member->user->id,
                       ((u64snowflake *) &color_ids[pos])[i], &remove_params,
                       NULL);
            }

            discord_add_guild_member_role (handle, event->guild_id,
                                           event->member->user->id, role,
                                           &add_params, NULL);

            sprintf (response, "<@&%llu> role set", role);
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
