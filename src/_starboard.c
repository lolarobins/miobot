// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+
// | miobot - discord bot for the 'Sputnik Supporters' discord server  |
// |           MIT license - copyright (c) 2025 lolasnotunix           |
// |                                                                   |
// |         starboard.c - starboard management and callbacks          |
// +--- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---+

#include <stdlib.h>
#include <string.h>

#include <concord/discord.h>

// cleanup for memory safety will be needed!!

// amount of stars needed to create/manage a starboard post
#define STARBOARD_THRESHOLD 1
// max users to fetch for starboard count
#define STARBOARD_USER_LIMIT 50

// json message fmt start for starboard post
const char _post_fmt[]
   = "[\n"
     "%s" // insert _reply_fmt if applicable, otherwise use ""
     "  {\n"
     "    \"type\": 10,\n"
     "    \"content\": \"**from <@%lld>:**\\n%s%s\"\n"
     "  },\n"
     "%s"
     "  {\n"
     "    \"type\": 14,\n"
     "    \"divider\": true,\n"
     "    \"spacing\": 1\n"
     "  },\n"
     "  {\n" // star count / reference
     "    \"type\": 10,\n"
     "    \"content\": \"https://discord.com/channels/%llu/%llu/%llu "
     "\u2014 \u2B50 %d\"\n"
     "  }\n"
     "]\n",
   _reply_fmt[]
   = "  {\n"
     "    \"type\": 10,\n"
     "    \"content\": \">>> **reply to <@%lld>:**\\n%s%s\"\n"
     "  },\n",
   _attachment_fmt_start[]
   = "  {\n"
     "    \"type\": 12,\n"
     "    \"items\": [\n",
   _attachment_fmt_end[]
   = "    ]\n"
     "  },\n",
   _attachment_item[]
   = "      {\n"
     "        \"media\": {\"url\": \"%s\"},\n"
     "      }";

// filter content formatting to fix newlines, etc. must free ptr
// CLEAN THIS UP!! WRITTEN FAST WITHOUT VERIFYING ACCURACY!!
static char *_message_content_filter (const char *content) {
    // sizeof content
    size_t original_len = strlen (content);
    // starting position of next append
    size_t last_copy_pos = 0;
    // next place to copy into
    size_t cursor = 0;

    // MAKE THIS MEMORY SAFE SOMEHOW!
    char *new_str = malloc (original_len + 1024);

    for (int i = 0; i < strlen (content); i++) {
        if (content[i] == '\n') {
            strncpy (new_str + cursor, &content[last_copy_pos],
                     i - last_copy_pos);
            last_copy_pos = i + 1;
            // cursor
            cursor += last_copy_pos + 1;
            new_str[cursor - 2] = '\\';
            new_str[cursor - 1] = 'n';
        }
    }
    strcpy (new_str + cursor, &content[last_copy_pos]);

    return new_str;
}

// response->keep for func below
struct _fetched_original_message {
    int users;
};

// used to get a formatted string for a starboard messages attachment and embed
// count
static void _write_embed_count (char *msg_buf, size_t buf_len, int attachments,
                                int embeds) {
    char attachment_msg[64], embed_msg[64];
    msg_buf[0]        = 0;
    attachment_msg[0] = 0;
    embed_msg[0]      = 0;

    if (attachments)
        snprintf (attachment_msg, 64, "%d attachment%s", attachments,
                  attachments == 1 ? "" : "s");
    if (embeds)
        snprintf (embed_msg, 64, "%s%d embed%s",
                  // will add ", " if both attachments/embeds exist
                  attachment_msg[0] ? ", " : "", embeds,
                  embeds == 1 ? "" : "s");
    if (attachment_msg[0] || embed_msg[0])
        snprintf (msg_buf, buf_len, "\\n**[%s%s]**", attachment_msg, embed_msg);
}

// RUNS AFTER FETCHING MESSAGE NOT YET ADDED TO STARBOARD
static void _fetched_original_message (struct discord *handle,
                                       struct discord_response *response,
                                       const struct discord_message *message) {
    // data to carry over from _added_fetched_users
    const struct _fetched_original_message *data     = response->data;
    const struct discord_message_reaction_add *event = response->keep;

    char attachment_count_msg[64];

    // reply component for if message is a reply
    char reply_component[4096];
    reply_component[0] = 0;

    if (message->referenced_message) {
        struct discord_message *ref = message->referenced_message;

        // message filter for reply
        char *filtered_reply = (_message_content_filter (ref->content));

        // get attachment/embed count in lieu of attaching it to starboard post
        _write_embed_count (attachment_count_msg, 64, ref->attachments->size,
                            ref->embeds->size);

        snprintf (reply_component, 4096, _reply_fmt, ref->author->id,
                  filtered_reply, attachment_count_msg);

        // clean up filtered reply
        free (filtered_reply);
    }

    // message filter for original message
    char *filtered_message = (_message_content_filter (message->content));

    // buf for image gallery json
    char media_component[2048];
    media_component[0] = 0;
    int gallery_count = 0, str_cur = 0,
        attachment_count = message->attachments->size;

    // set up embeds/attachments for images
    for (int i = 0; i < message->attachments->size; i++) {
        if (gallery_count > 10) break;

        struct discord_attachment *attachment = &message->attachments->array[i];

        // make sure mime type is image
        if (strncmp ("image", attachment->content_type, 5)) return;

        // print media json header if it doesnt exist
        // or add , to signify new list item
        if (media_component[0]) media_component[str_cur++] = ',';
        else
            str_cur = snprintf (media_component, 2048, _attachment_fmt_start);

        str_cur += snprintf (&media_component[str_cur], 2048 - str_cur,
                             _attachment_item, attachment->url);

        gallery_count++;
        attachment_count--;
    }

    if (media_component[0]) {
        media_component[str_cur++] = '\n';

        strncpy (&media_component[str_cur], _attachment_fmt_end,
                 2048 - str_cur);
    }

    // write embed/attachment count, exclude ones added to media
    _write_embed_count (attachment_count_msg, 64, attachment_count,
                        message->embeds->size);

    // get final component json to let concord form a struct
    char component[4096];
    snprintf (component, 4096, _post_fmt, reply_component, message->author->id,
              filtered_message, attachment_count_msg, media_component,
              event->guild_id, event->channel_id, event->message_id,
              data->users);

    // ditch filtered message
    free (filtered_message);

    struct discord_components msg_components;
    discord_components_from_json (component, 4096, &msg_components);

    struct discord_create_message msg
       = { .components = &msg_components,
           .flags      = DISCORD_MESSAGE_HAS_COMPONENTS_V2 };

    // create post and cleanup
    discord_create_message (handle, event->channel_id, &msg, 0);
    discord_components_cleanup (&msg_components);
}

// RUNS AFTER FETCHING EXISTING STARBOARD MESSAGE TO UPDATE COUNT
// todo

// RUNS AFTER FETCHING USERS IN ADD REACT CB
static void _added_fetched_users (struct discord *handle,
                                  struct discord_response *response,
                                  const struct discord_users *users) {
    if (users->size < STARBOARD_THRESHOLD || // not enough stars yet
        users->size == STARBOARD_USER_LIMIT) // too many users
        return;

    const struct discord_message_reaction_add *event = response->keep;

    // TODO !!!
    // will need to fetch message, then use that message to post new starboard
    // post IF starboard post not already existent will need to check for
    // existence of a starboard message before doing this
    struct _fetched_original_message data = { .users = users->size };
    struct discord_ret_message ret        = { .done = _fetched_original_message,
                                              .data = &data,
                                              .keep = response->keep };

    // jumps to _fetched_original_message
    discord_get_channel_message (handle, event->channel_id, event->message_id,
                                 &ret);
}

void starboard_add_reaction_cb (
   struct discord *handle, const struct discord_message_reaction_add *event) {
    // end now if not star
    if (strcmp (event->emoji->name, "\u2B50")) return;

    struct discord_ret_users ret
       = { .done = &_added_fetched_users, .keep = event };
    struct discord_get_reactions params = { .limit = STARBOARD_USER_LIMIT };

    // will move onto _fetched_users once fetched to update starboard
    discord_get_reactions (handle, event->channel_id, event->message_id, 0,
                           "\u2B50", &params, &ret);
}

static void _removed_fetched_users (struct discord *handle,
                                    struct discord_response *response,
                                    const struct discord_users *users) {}

// basically the same as the above
void starboard_remove_reaction_cb (
   struct discord *handle,
   const struct discord_message_reaction_remove *event) {
    // ignore if not star emoji
    if (strcmp (event->emoji->name, "\u2B50")) return;

    struct discord_ret_users ret
       = { .done = &_removed_fetched_users, .keep = event };
    struct discord_get_reactions params = { .limit = 50 };

    // this will jump to _remove_fetched_users
    discord_get_reactions (handle, event->channel_id, event->message_id, 0,
                           "\u2B50", &params, &ret);
}

void starboard_message_delete_cb(struct discord *handle, const struct discord_message_delete *event) {

}
