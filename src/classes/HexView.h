/**
 * HexView.h - View a text buffer as hexadecimal bytes.
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

#ifndef _GHX_HEXVIEW_H
#define _GHX_HEXVIEW_H

#include <gtk/gtk.h>


#define GHEXEDIT_TYPE_HEX_VIEW ghexedit_hex_view_get_type()
G_DECLARE_FINAL_TYPE (GHexEditHexView, ghexedit_hex_view, GHEXEDIT, HEX_VIEW, GtkTextView);

GtkWidget *ghexedit_hex_view_new();
void ghexedit_hex_view_set_underlying(GHexEditHexView *view, GBytes *bytes);
GBytes *ghexedit_hex_view_get_underlying(GHexEditHexView *view);

#endif
