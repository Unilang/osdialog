#include "osdialog.h"
#include <assert.h>
#include <gtk/gtk.h>



int osdialog_message(osdialog_message_level level, osdialog_message_buttons buttons, const char *message) {
	assert(gtk_init_check(NULL, NULL));

	GtkMessageType messageType;
	switch (level) {
		default:
		case OSDIALOG_INFO: messageType = GTK_MESSAGE_INFO; break;
		case OSDIALOG_WARNING: messageType = GTK_MESSAGE_WARNING; break;
		case OSDIALOG_ERROR: messageType = GTK_MESSAGE_ERROR; break;
	}

	GtkButtonsType buttonsType;
	switch (buttons) {
		default:
		case OSDIALOG_OK: buttonsType = GTK_BUTTONS_OK; break;
		case OSDIALOG_OK_CANCEL: buttonsType = GTK_BUTTONS_OK_CANCEL; break;
		case OSDIALOG_YES_NO: buttonsType = GTK_BUTTONS_YES_NO; break;
	}

	GtkWidget *dialog = gtk_message_dialog_new (NULL,
		0,
		messageType,
		buttonsType,
		"%s", message);

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);

	switch (result) {
		case GTK_RESPONSE_OK:
		case GTK_RESPONSE_YES:
			return 1;
		default:
			return 0;
	}
}


char *osdialog_file(osdialog_file_action action, const char *path, const char *filename, const char *filters) {
	assert(gtk_init_check(NULL, NULL));

	GtkFileChooserAction gtkAction;
	const char *title;
	const char *acceptText;
	if (action == OSDIALOG_OPEN) {
		title = "Open File";
		acceptText = "Open";
		gtkAction = GTK_FILE_CHOOSER_ACTION_OPEN;
	}
	else if (action == OSDIALOG_OPEN_DIR) {
		title = "Open Folder";
		acceptText = "Open Folder";
		gtkAction = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	}
	else {
		title = "Save File";
		acceptText = "Save";
		gtkAction = GTK_FILE_CHOOSER_ACTION_SAVE;
	}

	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		title,
		NULL,
		gtkAction,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		acceptText, GTK_RESPONSE_ACCEPT,
		NULL);

	if (action == OSDIALOG_SAVE)
		gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

	if (path)
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);

	if (action == OSDIALOG_SAVE && filename)
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), filename);

	char *result = NULL;
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		result = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}
	gtk_widget_destroy(dialog);

	while (gtk_events_pending())
		gtk_main_iteration();
	return result;
}


void osdialog_color_picker() {
	assert(gtk_init_check(NULL, NULL));

	GtkWidget *dialog = gtk_color_selection_dialog_new("Color");
	GtkColorSelection *colorsel = GTK_COLOR_SELECTION(GTK_COLOR_SELECTION_DIALOG(dialog)->colorsel);

	// gtk_color_selection_set_has_opacity_control(GTK_COLOR_SELECTION_DIALOG(dialog), true);

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		GdkColor color;
		gtk_color_selection_get_current_color(colorsel, &color);
		// gtk_color_selection_dialog_get_color_selection(dialog);
	}
	gtk_widget_destroy(dialog);
}
