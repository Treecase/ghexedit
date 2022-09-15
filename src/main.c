/**
 * main.c - Program entry point.
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

#include "version.h"
#include "classes/App.h"

#include <gtk/gtk.h>


int main(int argc, char *argv[])
{
    /* Since this example is running uninstalled,
     * we have to help it find its schema. This
     * is *not* necessary in properly installed
     * application. */
    g_setenv("GSETTINGS_SCHEMA_DIR", "/home/trevor/Documents/source/ghexedit/build", FALSE);
    return g_application_run(G_APPLICATION(ghexedit_app_new()), argc, argv);
}
