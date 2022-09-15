/**
 * AppPrefs.c - AppPrefs window.
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

#include "AppPrefs.h"
#include "App.h"
#include "AppWin.h"

#include "appid.h"

#include <gtk/gtk.h>


struct _GHexEditAppPrefs
{
    GtkDialog parent;
    GSettings *settings;
    GtkWidget *font;
    GtkWidget *grouping;
    GtkWidget *bytes_per_line;
};

G_DEFINE_TYPE(GHexEditAppPrefs, ghexedit_app_prefs, GTK_TYPE_DIALOG)


/* ===[ GObject ]=== */
/** Instantiate a new instance of the class. */
GHexEditAppPrefs *ghexedit_app_prefs_new(GHexEditAppWindow *win)
{
    // Instantiate new AppPrefs
    return g_object_new(GHEXEDIT_TYPE_APP_PREFS, "transient-for", win, "use-header-bar", TRUE, NULL);
}

/**
 * Drop held references.
 * Can be executed more than once!
 * Should chain up before returning.
 */
void ghexedit_app_prefs_dispose(GObject *object)
{
    GHexEditAppPrefs *prefs = GHEXEDIT_APP_PREFS(object);
    // Clear the settings
    g_clear_object(&prefs->settings);
    // Call parent class's dispose method
    G_OBJECT_CLASS(ghexedit_app_prefs_parent_class)->dispose(object);
}


/* ===[ Base GLib ]=== */
/** Equivalent to C++ constructor. */
void ghexedit_app_prefs_init(GHexEditAppPrefs *prefs)
{
    // Create child widgets from class template
    gtk_widget_init_template(GTK_WIDGET(prefs));
    // Create new Settings object
    prefs->settings = g_settings_new(GHX_APPLICATION_ID);
    // Bind prefs properties from settings
    g_settings_bind(prefs->settings, "font", prefs->font, "font", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(prefs->settings, "grouping", prefs->grouping, "value", G_SETTINGS_BIND_DEFAULT);
    g_settings_bind(prefs->settings, "bytes-per-line", prefs->bytes_per_line, "value", G_SETTINGS_BIND_DEFAULT);
}

/**
 * Called upon first instantiation of an object of this class.
 * Sets/overrides class methods/signals/properties.
 */
void ghexedit_app_prefs_class_init(GHexEditAppPrefsClass *class)
{
    // Override dispose
    G_OBJECT_CLASS(class)->dispose = ghexedit_app_prefs_dispose;
    // Set widget template
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), GHX_GRESOURCE_PREFIX "AppPrefs.ui");
    // Bind class children in template
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), GHexEditAppPrefs, font);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), GHexEditAppPrefs, grouping);
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), GHexEditAppPrefs, bytes_per_line);
}
