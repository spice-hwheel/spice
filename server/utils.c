/*
   Copyright (C) 2009-2015 Red Hat, Inc.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see <http://www.gnu.org/licenses/>.
*/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <spice/enums.h>
#include "utils.h"

int rgb32_data_has_alpha(int width, int height, size_t stride,
                         uint8_t *data, int *all_set_out)
{
    uint32_t *line, *end, alpha;
    int has_alpha;

    has_alpha = FALSE;
    while (height-- > 0) {
        line = (uint32_t *)data;
        end = line + width;
        data += stride;
        while (line != end) {
            alpha = *line & 0xff000000U;
            if (alpha != 0) {
                has_alpha = TRUE;
                if (alpha != 0xff000000U) {
                    *all_set_out = FALSE;
                    return TRUE;
                }
            }
            line++;
        }
    }

    *all_set_out = has_alpha;
    return has_alpha;
}

/* These names are used to parse command line options, don't change them */
static const char *const channel_names[] = {
    [ SPICE_CHANNEL_MAIN     ] = "main",
    [ SPICE_CHANNEL_DISPLAY  ] = "display",
    [ SPICE_CHANNEL_INPUTS   ] = "inputs",
    [ SPICE_CHANNEL_CURSOR   ] = "cursor",
    [ SPICE_CHANNEL_PLAYBACK ] = "playback",
    [ SPICE_CHANNEL_RECORD   ] = "record",
    [ SPICE_CHANNEL_SMARTCARD] = "smartcard",
    [ SPICE_CHANNEL_USBREDIR ] = "usbredir",
    [ SPICE_CHANNEL_WEBDAV   ] = "webdav",
};

/**
 * red_channel_type_to_str:
 *
 * This function returns a human-readable name from a SPICE_CHANNEL_* type.
 * It must be called with a valid channel type.
 *
 * Returns: NULL if the channel type is invalid, the channel name otherwise.
 */
const char *red_channel_type_to_str(int type)
{
    g_return_val_if_fail(type >= 0, NULL);
    g_return_val_if_fail(type < G_N_ELEMENTS(channel_names), NULL);
    g_return_val_if_fail(channel_names[type] != NULL, NULL);

    return channel_names[type];
}

/**
 * red_channel_name_to_type:
 *
 * Converts a channel name to a SPICE_CHANNEL_* type. These names are currently
 * used in our public API (see spice_server_set_channel_security()).
 *
 * Returns: -1 if @name was not a known channel name, the channel
 * type otherwise.
 *
 */
int red_channel_name_to_type(const char *name)
{
    int i;

    for (i = 0; i < G_N_ELEMENTS(channel_names); i++) {
        if (g_strcmp0(channel_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}
