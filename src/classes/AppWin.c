/**
 * AppWin.c - GHexEdit ApplicationWindow.
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

#include "AppWin.h"
#include "App.h"
#include "HexView.h"

#include "appid.h"

#include <gtk/gtk.h>


struct _GHexEditAppWindow
{
    GtkApplicationWindow parent;
    GSettings *settings;
    GtkWidget *notebook;
};

G_DEFINE_TYPE(GHexEditAppWindow, ghexedit_app_window, GTK_TYPE_APPLICATION_WINDOW);


/** TextBuffer::changed callback: Apply the selected font to a buffer. */
void apply_font(GObject *self, gpointer user_data)
{
    GtkTextBuffer *buffer = GTK_TEXT_BUFFER(self);
    GHexEditAppWindow *win = GHEXEDIT_APP_WINDOW(user_data);
    // Create a new tag for the buffer
    GtkTextTag *tag = gtk_text_buffer_create_tag(buffer, NULL, NULL);
    // Tag's font comes from settings
    g_settings_bind(win->settings, "font", tag, "font", G_SETTINGS_BIND_DEFAULT);
    // Tag applies to whole buffer
    GtkTextIter start_iter, end_iter;
    gtk_text_buffer_get_start_iter(buffer, &start_iter);
    gtk_text_buffer_get_end_iter(buffer, &end_iter);
    gtk_text_buffer_apply_tag(buffer, tag, &start_iter, &end_iter);
}


/* ===[ GHexEditAppWindow ]=== */
/** Called by App to send a file to open. */
void ghexedit_app_window_open(GHexEditAppWindow *win, GFile *file)
{
    char *basename = g_file_get_basename(file);

    // Add a ScrolledWindow (to hold file contents)
    GtkWidget *scrolled = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(scrolled, TRUE);
    gtk_widget_set_vexpand(scrolled, TRUE);
    // Create HexView for file contents
    GtkWidget *view = ghexedit_hex_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), TRUE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(view), TRUE);
    // Add view as child of scrolled
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled), view);
    // Add scrolled as a notebook page
    gtk_notebook_append_page(GTK_NOTEBOOK(win->notebook), scrolled, gtk_label_new(basename));
    // Get HexView's buffer
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

    // Need to reapply font tag when text changes
    g_signal_connect(buffer, "changed", G_CALLBACK(apply_font), win);

    // Load file contents
    GBytes *content;
    if (content = g_file_load_bytes(file, NULL, NULL, NULL))
    {
        gsize size;
        ghexedit_hex_view_set_underlying(GHEXEDIT_HEX_VIEW(view), content);
    }
    g_free(basename);
}

/** Close the current NotebookPage. */
void ghexedit_app_window_close_current(GHexEditAppWindow *win)
{
    GtkNotebook *notebook = GTK_NOTEBOOK(win->notebook);
    gtk_notebook_remove_page(notebook, gtk_notebook_get_current_page(notebook));
}


/* ===[ GObject ]=== */
/** Instantiate a new instance of the class. */
GHexEditAppWindow *ghexedit_app_window_new(GHexEditApp *app)
{
    // Instantiate new AppWindow
    GHexEditAppWindow *win = g_object_new(GHEXEDIT_TYPE_APP_WINDOW, "application", app, NULL);
    // Enable window menubar
    gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(win), TRUE);
    return win;
}

/**
 * Drop held references.
 * Can be executed more than once!
 * Should chain up before returning.
 */
void ghexedit_app_window_dispose(GObject *object)
{
    GHexEditAppWindow *win = GHEXEDIT_APP_WINDOW(object);
    // Clear the settings
    g_clear_object(&win->settings);
    // Call parent class's dispose method
    G_OBJECT_CLASS(ghexedit_app_window_parent_class)->dispose(object);
}


/* ===[ Base GLib ]=== */
/** Equivalent to C++ constructor. */
void ghexedit_app_window_init(GHexEditAppWindow *win)
{
    // Create child widgets from class template
    gtk_widget_init_template(GTK_WIDGET(win));
    // Set notebook's properties
    gtk_notebook_set_scrollable(GTK_NOTEBOOK(win->notebook), TRUE);
    // Create settings object from schema
    win->settings = g_settings_new(GHX_APPLICATION_ID);
}

/**
 * Called upon first instantiation of an object of this class.
 * Sets/overrides class methods/signals/properties.
 */
void ghexedit_app_window_class_init(GHexEditAppWindowClass *class)
{
    // Method overrides
    G_OBJECT_CLASS(class)->dispose = ghexedit_app_window_dispose;
    // Set widget template
    gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), GHX_GRESOURCE_PREFIX "AppWindow.ui");
    // Bind class children in template
    gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), GHexEditAppWindow, notebook);
}
