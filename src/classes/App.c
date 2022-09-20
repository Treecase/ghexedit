/**
 * App.c - GHexEdit application.
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

#include "App.h"
#include "AppPrefs.h"
#include "AppWin.h"

#include "appid.h"

#include <gtk/gtk.h>


struct _GHexEditApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE(GHexEditApp, ghexedit_app, GTK_TYPE_APPLICATION);


/** File>Open callback. */
void file_picked(GtkNativeDialog *native, int response, gpointer app)
{
    if (response == GTK_RESPONSE_ACCEPT)
    {
        GtkWindow *win = gtk_application_get_active_window(GTK_APPLICATION(app));
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(native);
        GFile *file = gtk_file_chooser_get_file(chooser);
        ghexedit_app_window_open(GHEXEDIT_APP_WINDOW(win), file);
        g_object_unref(file);
    }
    g_object_unref(native);
}


/* ===[ Actions ]=== */
/** Open a file. */
void open_activated(GSimpleAction *action, GVariant *parameter, gpointer app)
{
    GtkWindow *win = gtk_application_get_active_window(GTK_APPLICATION(app));
    GtkFileChooserNative *chooser = gtk_file_chooser_native_new("Open", win, GTK_FILE_CHOOSER_ACTION_OPEN, "Open", "Cancel");
    g_signal_connect(chooser, "response", G_CALLBACK(file_picked), app);
    gtk_native_dialog_show(GTK_NATIVE_DIALOG(chooser));
}

/** Close a file. */
void close_activated(GSimpleAction *action, GVariant *parameter, gpointer app)
{
    GtkWindow *win = gtk_application_get_active_window(GTK_APPLICATION(app));
    ghexedit_app_window_close_current(GHEXEDIT_APP_WINDOW(win));
}

/** Quit the App. */
void quit_activated(GSimpleAction *action, GVariant *parameter, gpointer app)
{
    g_application_quit(G_APPLICATION(app));
}

/** Display Preferences window. */
void preferences_activated(GSimpleAction *action, GVariant *parameter, gpointer app)
{
    GtkWindow *win = gtk_application_get_active_window(GTK_APPLICATION(app));
    GHexEditAppPrefs *prefs = ghexedit_app_prefs_new(GHEXEDIT_APP_WINDOW(win));
    gtk_window_present(GTK_WINDOW(prefs));
}

/** `app.XXX` action definitions for GActionMap. */
static GActionEntry const app_entries[] = {
    // File menu
    {"open", open_activated, NULL, NULL, NULL},
    {"close", close_activated, NULL, NULL, NULL},
    {"quit", quit_activated, NULL, NULL, NULL},
    // Edit menu
    {"preferences", preferences_activated, NULL, NULL, NULL},
};


/* ===[ GtkApplication ]=== */
/** Called before the App is displayed. */
void ghexedit_app_startup(GApplication *app)
{
    GtkBuilder *builder;
    GMenuModel *app_menu;
    char const *open_accels[2] = {"<Ctrl>O", NULL};
    char const *close_accels[2] = {"<Ctrl>W", NULL};
    char const *quit_accels[2] = {"<Ctrl>Q", NULL};

    G_APPLICATION_CLASS(ghexedit_app_parent_class)->startup(app);

    g_action_map_add_action_entries(G_ACTION_MAP(app), app_entries, G_N_ELEMENTS(app_entries), app);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.open", open_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.close", close_accels);
    gtk_application_set_accels_for_action(GTK_APPLICATION(app), "app.quit", quit_accels);
}

/** Called when App is started without any files passed. */
void ghexedit_app_activate(GApplication *app)
{
    GHexEditAppWindow *win = ghexedit_app_window_new(GHEXEDIT_APP(app));
    gtk_window_present(GTK_WINDOW(win));
}

/** Called when App is started with files passed. */
void ghexedit_app_open(GApplication *app, GFile **files, int n_files, char const *hint)
{
    GHexEditAppWindow *win;
    GList *windows = gtk_application_get_windows(GTK_APPLICATION(app));
    if (windows)
        win = GHEXEDIT_APP_WINDOW(windows->data);
    else
        win = ghexedit_app_window_new(GHEXEDIT_APP(app));

    for (int i = 0; i < n_files; ++i)
        ghexedit_app_window_open(win, files[i]);

    gtk_window_present(GTK_WINDOW(win));
}


/* ===[ GObject ]=== */
/** Instantiate a new instance of the class. */
GHexEditApp *ghexedit_app_new(void)
{
    return g_object_new(GHEXEDIT_TYPE_APP, "application-id", GHX_APPLICATION_ID, "flags", G_APPLICATION_HANDLES_OPEN, NULL);
}


/* ===[ Base GLib ]=== */
/** Equivalent to C++ constructor. */
void ghexedit_app_init(GHexEditApp *app)
{
}

/**
 * Called upon first instantiation of an object of this class.
 * Sets/overrides class methods/signals/properties.
 */
void ghexedit_app_class_init(GHexEditAppClass *class)
{
    G_APPLICATION_CLASS(class)->startup = ghexedit_app_startup;
    G_APPLICATION_CLASS(class)->activate = ghexedit_app_activate;
    G_APPLICATION_CLASS(class)->open = ghexedit_app_open;
}
