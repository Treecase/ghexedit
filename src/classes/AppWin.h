/**
 * AppWin.h - GHexEdit ApplicationWindow.
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

#ifndef _GHX_APPWIN_H
#define _GHX_APPWIN_H

#include "App.h"

#include <gtk/gtk.h>


#define GHEXEDIT_TYPE_APP_WINDOW ghexedit_app_window_get_type()
G_DECLARE_FINAL_TYPE(GHexEditAppWindow, ghexedit_app_window, GHEXEDIT, APP_WINDOW, GtkApplicationWindow);

GHexEditAppWindow *ghexedit_app_window_new(GHexEditApp *app);
void ghexedit_app_window_open(GHexEditAppWindow *win, GFile *file);
void ghexedit_app_window_close_current(GHexEditAppWindow *win);

#endif
