#include <gtk/gtk.h>

void gui_quit(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gtk_main_quit();
}

int gui_init() {
    GtkWidget *window;

    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    g_signal_connect(window, "destroy", G_CALLBACK(gui_quit), NULL);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}