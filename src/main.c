#include <adwaita.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <ctype.h>
#include <string.h>

// Función para obtener el texto actual del GtkTextView
char* get_text_from_textview(GtkTextView *textview) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    GtkTextIter start, end;
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

// Función para establecer texto en el GtkTextView
void set_text_in_textview(GtkTextView *textview, const char *text) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    gtk_text_buffer_set_text(buffer, text, -1);
}

// Función para convertir el texto en mayúsculas
void on_mayus_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir a mayúsculas utilizando g_utf8_strup
    char *uppercase_text = g_utf8_strup(text, -1);

    // Establecer el texto modificado
    set_text_in_textview(textview, uppercase_text);
    
    g_free(text);
    g_free(uppercase_text);
}

// Función para convertir el texto en minúsculas
void on_minus_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir a minúsculas utilizando g_utf8_strdown
    char *lowercase_text = g_utf8_strdown(text, -1);

    // Establecer el texto modificado
    set_text_in_textview(textview, lowercase_text);
    
    g_free(text);
    g_free(lowercase_text);
}

// Función para convertir el texto en mayúsculas después de cada punto
void on_mayus_despues_punto_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir todo el texto a minúsculas
    char *lowercase_text = g_utf8_strdown(text, -1);

    // Crear un buffer para el texto final
    GString *result = g_string_new(NULL);
    gboolean capitalize_next = TRUE;

    // Recorrer el texto en minúsculas y capitalizar después de cada punto
    for (char *p = lowercase_text; *p != '\0'; p = g_utf8_next_char(p)) {
        gunichar ch = g_utf8_get_char(p);

        // Convertir a mayúscula si es el primer caracter después de un punto
        if (capitalize_next && g_unichar_isalpha(ch)) {
            ch = g_unichar_toupper(ch);
            capitalize_next = FALSE;
        }

        // Agregar el carácter al resultado
        g_string_append_unichar(result, ch);

        // Si el carácter es un punto, capitalizar el siguiente
        if (ch == '.') {
            capitalize_next = TRUE;
        }
    }

    // Establecer el texto modificado en el GtkTextView
    set_text_in_textview(textview, result->str);

    // Liberar la memoria utilizada
    g_free(text);
    g_free(lowercase_text);
    g_string_free(result, TRUE);
}


// Función para capitalizar la primera letra de cada palabra
void on_capi_text_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);
    char *text = get_text_from_textview(textview);

    // Convertir el texto a minúsculas
    char *lowercase_text = g_utf8_strdown(text, -1);
    GString *result = g_string_new(NULL);

    gboolean capitalize_next = TRUE;

    // Recorrer el texto y capitalizar la primera letra de cada palabra
    for (char *p = lowercase_text; *p != '\0'; p = g_utf8_next_char(p)) {
        gunichar ch = g_utf8_get_char(p);

        // Capitalizar si es la primera letra de una palabra
        if (capitalize_next && g_unichar_isalpha(ch)) {
            ch = g_unichar_toupper(ch);
            capitalize_next = FALSE;
        }

        // Agregar el carácter al resultado
        g_string_append_unichar(result, ch);

        // Detectar espacios o caracteres de separación para capitalizar la próxima letra
        if (g_unichar_isspace(ch)) {
            capitalize_next = TRUE;
        }
    }

    // Establecer el texto modificado en el GtkTextView
    set_text_in_textview(textview, result->str);

    // Liberar la memoria utilizada
    g_free(text);
    g_free(lowercase_text);
    g_string_free(result, TRUE);
}


// Función de callback para el botón "Copiar Texto"
void on_copiar_texto_button_clicked(GtkButton *button, gpointer user_data) {
    GtkTextView *textview = GTK_TEXT_VIEW(user_data);

    // Obtener el texto del GtkTextView
    char *text = get_text_from_textview(textview);

    // Acceder al portapapeles predeterminado en GTK4
    GdkDisplay *display = gdk_display_get_default();
    GdkClipboard *clipboard = gdk_display_get_clipboard(display);

    // Copiar el texto al portapapeles
    gdk_clipboard_set_text(clipboard, text);

    // Liberar la memoria utilizada
    g_free(text);
}


static void
show_toast (AdwToastOverlay *overlay)
{
  AdwToast *toast = adw_toast_new ("Texto Copiado");
  adw_toast_set_timeout (toast, 3); // Duración de 3 segundos
  adw_toast_overlay_add_toast (overlay, toast);
}


// Función para la acción "About"
static void
about_action (GSimpleAction *action, GVariant *parameter, gpointer app)
{
  const char *developers[] = {
    "Codigo Cristo",
    NULL
  };

  // Mostrar el diálogo "Acerca de"
  adw_show_about_dialog (GTK_WIDGET (gtk_application_get_active_window (app)),
                         "application-name", "MayusApp",
                         "application-icon", "preferences-desktop-keyboard",
                         "version", "1.0.0",
                         "copyright", "© 2024 MayusApp",
                         "issue-url", "https://github.com/CodigoCristo/mayusculasminusculas",
                         "license-type", GTK_LICENSE_MIT_X11,
                         "developers", developers,
                         "translator-credits", ("translator-credits"),
                         NULL);

}


static void activate(GtkApplication *app, gpointer user_data) {
    // Usa GtkBuilder para cargar la interfaz desde el recurso empaquetado
    GtkBuilder *builder = gtk_builder_new_from_resource("/org/gtk/MayusApp/window.ui");
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    //AdwToastOverlay
    GtkWidget *toolbar_view = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar_view"));
    GtkWidget *overlay = GTK_WIDGET(gtk_builder_get_object(builder, "overlay"));
    GtkWidget *adwstatus = GTK_WIDGET(gtk_builder_get_object(builder, "adwstatus"));

    
    GtkBuilder *boxview = gtk_builder_new_from_resource("/org/gtk/MayusApp/texview.ui");
    GtkWidget *box = GTK_WIDGET(gtk_builder_get_object(boxview, "box"));

    //adw_toolbar_view_set_content (ADW_TOOLBAR_VIEW (toolbar_view), overlay);
    //adw_toast_overlay_set_child (ADW_TOAST_OVERLAY (overlay), adwstatus);
    adw_status_page_set_child(ADW_STATUS_PAGE(adwstatus), box);


    GtkTextView *textview = GTK_TEXT_VIEW(gtk_builder_get_object(boxview, "textview"));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    GtkButton *button_mayus = GTK_BUTTON(gtk_builder_get_object(boxview, "mayus"));
    GtkButton *button_minus = GTK_BUTTON(gtk_builder_get_object(boxview, "minus"));
    GtkButton *button_mayus_despues_punto = GTK_BUTTON(gtk_builder_get_object(boxview, "mayus_despues_punto"));
    GtkButton *capi_text = GTK_BUTTON(gtk_builder_get_object(boxview, "capi_text"));
    GtkButton *copiar_texto = GTK_BUTTON(gtk_builder_get_object(boxview, "copiar_texto"));
   

    GtkMenuButton *button_menu = GTK_MENU_BUTTON(gtk_builder_get_object(builder, "button_menu"));
    // Crear un modelo para el menú
    GMenu *menu_app = g_menu_new ();

    // Agregar elementos al menú
    GMenu *section = g_menu_new ();
    g_menu_append (section, "About My App", "app.about");
    g_menu_append_section (menu_app, NULL, G_MENU_MODEL (section));

    // Configurar el menú en el botón de menú
    gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (button_menu), G_MENU_MODEL (menu_app));

    // Liberar el modelo después de configurarlo
    g_object_unref (menu_app);



    // Conectar las señales
    g_signal_connect(button_mayus, "clicked", G_CALLBACK(on_mayus_button_clicked), textview);
    g_signal_connect(button_minus, "clicked", G_CALLBACK(on_minus_button_clicked), textview);
    g_signal_connect(button_mayus_despues_punto, "clicked", G_CALLBACK(on_mayus_despues_punto_button_clicked), textview);
    g_signal_connect(capi_text, "clicked", G_CALLBACK(on_capi_text_clicked), textview);
    g_signal_connect(copiar_texto, "clicked", G_CALLBACK(on_copiar_texto_button_clicked), textview);
    g_signal_connect_swapped (copiar_texto, "clicked", G_CALLBACK (show_toast), overlay);
    
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(builder); // Liberar el objeto builder después de su uso
}

int main(int argc, char *argv[]) {
    g_autoptr (AdwApplication) app = adw_application_new("org.gtk.MayusApp", 0);

        // Crear acciones para el menú
    const GActionEntry app_entries[] = {
        { "about", about_action, NULL, NULL, NULL },
    };

    // Añadir las acciones a la aplicación
    g_action_map_add_action_entries (G_ACTION_MAP (app), app_entries, G_N_ELEMENTS (app_entries), app);


    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    return g_application_run(G_APPLICATION(app), argc, argv);
}
