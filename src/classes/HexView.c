/**
 * HexView.c - View a text buffer as hexadecimal bytes.
 * Copyright (C) 2022 Trevor Last
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "HexView.h"

#include "appid.h"

#include <gtk/gtk.h>


struct _GHexEditHexView
{
    GtkTextView parent;
    GSettings *settings;
    guint bytes_per_line;
    guint grouping;
    GBytes *underlying;
};

G_DEFINE_TYPE(GHexEditHexView, ghexedit_hex_view, GTK_TYPE_TEXT_VIEW)


typedef enum
{
    PROP_BYTES_PER_LINE = 1,
    PROP_GROUPING,
    PROP_UNDERLYING,
    N_PROPERTIES,
} GHexEditHexViewProperty;

GParamSpec *properties[N_PROPERTIES] = {NULL,};


char nybble_char(guint8 nybble)
{
    nybble &= 0x0F;
    if (nybble < 0xA)
        return '0' + nybble;
    else
        return 'A' + (nybble - 0xA);
}

char make_printable(char ch)
{
    if (g_ascii_isprint(ch))
        return ch;
    else
        return '.';
}

/** Convert a buffer to hex format. */
void hex(guint8 const *data, int data_length, guint8 **out, int *out_length, guint bytes_per_line, guint grouping)
{
    if (grouping == 0)
        grouping = 1;
    if (bytes_per_line == 0)
        bytes_per_line = 1;

    gsize groups_size = (bytes_per_line / grouping) + 1;
    gsize line_size = 10 + bytes_per_line * 3 + groups_size + 3 + bytes_per_line + 1;

    gsize line_count = (data_length + bytes_per_line - 1) / bytes_per_line;
    gsize leftover = bytes_per_line - (data_length % bytes_per_line);
    if (leftover == 16)
        leftover = 0;

    *out_length = line_count * line_size;
    *out = g_malloc0((*out_length) + 1);

    guint8 *ptr = *out;
    gsize i = 0;
    for (i = 0; i < data_length + leftover; ++i)
    {
        if (i % bytes_per_line == 0)
        {
            for (int n = 0; n < 8; ++n)
                *ptr++ = nybble_char((i >> (4 * (7 - n))) & 0xF);
            *ptr++ = ' ';
            *ptr++ = ' ';
        }
        else if (i % grouping == 0)
            *ptr++ = ' ';

        if (i < data_length)
        {
            *ptr++ = nybble_char((data[i] >> 4) & 0x0F);
            *ptr++ = nybble_char(data[i] & 0x0F);
            *ptr++ = ' ';
        }
        else
        {
            *ptr++ = ' ';
            *ptr++ = ' ';
            *ptr++ = ' ';
        }

        if ((i + 1) % bytes_per_line == 0)
        {
            *ptr++ = ' ';
            *ptr++ = '|';
            for (gsize b = 0; b < bytes_per_line; ++b)
                if ((i - 15) + b < data_length)
                    *ptr++ = make_printable(data[(i - 15) + b]);
            *ptr++ = '|';
            *ptr++ = '\n';
        }
    }
    *out_length = ptr - *out;
}

/** Refresh view. */
void refresh_view(GHexEditHexView *view)
{
    if (view->underlying == NULL)
        return;

    gsize buf_length;
    guint8 const *buf = g_bytes_get_data(view->underlying, &buf_length);

    guint8 *out = NULL;
    int out_length = 0;
    hex(buf, buf_length, &out, &out_length, view->bytes_per_line, view->grouping);

    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(view)), out, out_length);
}


/* ===[ GHexEditHexView ]=== */
/** Set underlying buffer. */
void ghexedit_hex_view_set_underlying(GHexEditHexView *view, GBytes *bytes)
{
    view->underlying = bytes;
    refresh_view(view);
}

/** Get underlying buffer. */
GBytes *ghexedit_hex_view_get_underlying(GHexEditHexView *view)
{
    return view->underlying;
}


/* ===[ GObject ]=== */
/** Instantiate a new instance of the class. */
GtkWidget *ghexedit_hex_view_new()
{
    // Instantiate new AppPrefs
    return g_object_new(GHEXEDIT_TYPE_HEX_VIEW, NULL);
}

/** Set a property on an instance. */
void ghexedit_hex_view_set_property(GObject *object, guint property_id, GValue const *value, GParamSpec *pspec)
{
    GHexEditHexView *self = GHEXEDIT_HEX_VIEW(object);
    switch ((GHexEditHexViewProperty)property_id)
    {
    case PROP_BYTES_PER_LINE:
        self->bytes_per_line = g_value_get_uint(value);
        refresh_view(self);
        break;
    case PROP_GROUPING:
        self->grouping = g_value_get_uint(value);
        refresh_view(self);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

/** Get a property from an instance. */
void ghexedit_hex_view_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    GHexEditHexView *self = GHEXEDIT_HEX_VIEW(object);
    switch ((GHexEditHexViewProperty)property_id)
    {
    case PROP_BYTES_PER_LINE:
        g_value_set_uint(value, self->bytes_per_line);
        break;
    case PROP_GROUPING:
        g_value_set_uint(value, self->grouping);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}


/* ===[ Base GLib ]=== */
/** Equivalent to C++ constructor. */
void ghexedit_hex_view_init(GHexEditHexView *view)
{
    view->underlying = NULL;
    // Create new Settings object
    view->settings = g_settings_new(GHX_APPLICATION_ID);
    // Bind prefs properties from settings
    view->bytes_per_line = 16;
    g_settings_bind(view->settings, "bytes-per-line", view, "bytes-per-line", G_SETTINGS_BIND_DEFAULT);
    view->grouping = 8;
    g_settings_bind(view->settings, "grouping", view, "grouping", G_SETTINGS_BIND_DEFAULT);
}

/**
 * Called upon first instantiation of an object of this class.
 * Sets/overrides class methods/signals/properties.
 */
void ghexedit_hex_view_class_init(GHexEditHexViewClass *class)
{
    GObjectClass *klass = G_OBJECT_CLASS(class);
    // Overrides
    klass->set_property = ghexedit_hex_view_set_property;
    klass->get_property = ghexedit_hex_view_get_property;
    // Install properties
    properties[PROP_BYTES_PER_LINE] = g_param_spec_uint("bytes-per-line", "Bytes per line", "Number of bytes per line.", 1, G_MAXUINT, 16, G_PARAM_READWRITE);
    properties[PROP_GROUPING] = g_param_spec_uint("grouping", "Grouping", "Bytes grouping.", 1, G_MAXUINT, 8, G_PARAM_READWRITE);
    properties[PROP_UNDERLYING] = g_param_spec_pointer("underlying", "Underlying", "The raw data buffer.", G_PARAM_READWRITE);
    g_object_class_install_properties(klass, N_PROPERTIES, properties);
}
