
#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GtkWidget *entry1,*entry2,*label;
char *text;
void project__(GtkWidget *widget,gpointer data)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("cloud.png", NULL);

  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Software Sharing over Private Cloud");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0"); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
      "(c) United Cloud");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),"This Project is an attempt to share resources that in this case are Software on a cloud.An adhoc overlay network is created when a node-server is started. Every node carries a list nearest nodes. Nearest nodes are identified on the basis of cost associated to them.");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),"http://zeeshanakhtar.tk/unitedcloud");
  gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
	
}
void team__(GtkWidget *widget,gpointer data)
{
	//GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("team.png", NULL);

  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Team Members");
  //gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0"); 
  //gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
     // "(c) Zee Cloud");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
     "Zeeshan Akhtar (0701010120)\nSachin Srivastava (0701010080)\nSaurabh Gupta  (0701010088)\nSudhanshu Gupta (0701010101)");
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), 
      "United College of Engineering and Research");
  //gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  //g_object_unref(pixbuf), pixbuf = NULL;
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
	
}
void combo_selected(GtkWidget *widget,gpointer data)
{
	gchar *ctext =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
	text=malloc(100);
	memcpy(text,"\0",50);
	if(strncmp(ctext,"Multiplication",sizeof("Multiplication"))==0)
	{
		strcat(text,"48");
	}
	if(strncmp(ctext,"Addition",sizeof("Addition"))==0)
	{
		strcat(text,"42");
	}if(strncmp(ctext,"Subtract",sizeof("Subtract"))==0)
	{
		strcat(text,"43");
	}if(strncmp(ctext,"Division",sizeof("Division"))==0)
	{
		strcat(text,"45");
	}
}
void setting_1(GtkWidget * widget,gpointer data)
{
	if(!system("./con"))
			return;
}
void setting_2(GtkWidget * widget,gpointer data)
{
	if(!system("./soft"))
			return;
}
void setting_3(GtkWidget * widget,gpointer data)
{
	if(!system("./node"))
			return;
}
void send_request(GtkWidget * widget,gpointer data)
{
	char *buff;
	buff=malloc(300);
	memcpy(buff,"\0",200);
	strcat(buff,"./main send ");
	strcat(buff,(char*)text);
	strcat(buff," ");
	strcat(buff,gtk_entry_get_text((GtkEntry*)entry1));
	strcat(buff," ");
	strcat(buff,gtk_entry_get_text((GtkEntry*)entry2));
	printf("\n buffer:%s\n",buff);
	if(!system(buff))
		return;
}
void start_server(GtkWidget * widget,gpointer data)
{
	gtk_label_set_text((GtkLabel*)label,"\t\tSharing of Software started");
	if(!system("./main start"))
		return;
	}
int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *table;

	GtkWidget *menubar;
  GtkWidget *filemenu,*filemenu2;
  GtkWidget *file;
  GtkWidget *quit,*about,*project,*team;
   
	GtkWidget *button1,*button2,*button3,*button4,*button5;
	GtkWidget *label1,*label2,*label3,*label4,*label5,*label6;
   GtkWidget *vbox,*hbox1,*hbox2,*vbox1,*hbox3;
   GtkWidget *fixed1,*fixed2;
   GtkWidget * image;
   GtkWidget *frame1;
   GtkWidget *combo;
   
    gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Software Sharing over Private Cloud");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 300);
  //gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  menubar = gtk_menu_bar_new();
  filemenu = gtk_menu_new();

  file = gtk_menu_item_new_with_label("File");
  quit = gtk_menu_item_new_with_label("Quit");

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
  
  filemenu2 = gtk_menu_new();

  about = gtk_menu_item_new_with_label("About");
	project= gtk_menu_item_new_with_label("Projet");
	team = gtk_menu_item_new_with_label("Team");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(about), filemenu2);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu2), project);
  gtk_menu_shell_append(GTK_MENU_SHELL(filemenu2), team);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), about);
	
  
   gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
   
    hbox1 = gtk_hbox_new(FALSE, 3);
     vbox1 = gtk_vbox_new(FALSE, 0);
     label1=gtk_label_new(" ");
     label2=gtk_label_new("Settings");
      button1=gtk_button_new_with_label("      Configuration     	 ");
       button2=gtk_button_new_with_label("Softwares");
        button3=gtk_button_new_with_label("Neighbours");
         gtk_box_pack_start(GTK_BOX(vbox1), label1, FALSE, FALSE, 0);
          gtk_box_pack_start(GTK_BOX(vbox1), label2, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vbox1), button1, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vbox1), button2, FALSE, FALSE, 0);
     gtk_box_pack_start(GTK_BOX(vbox1), button3, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(hbox1), vbox1, FALSE, FALSE, 0);
   
   image = gtk_image_new_from_file("Cloud1.jpg");
   gtk_box_pack_start(GTK_BOX(hbox1), image, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox1, FALSE, FALSE, 0);
   
   fixed1 = gtk_fixed_new();
    
	combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Subtract");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Addition");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Multiplication");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "Division");
 // gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "gcc Compiler");
 
	entry1=gtk_entry_new();
	entry2=gtk_entry_new();
	label3=gtk_label_new("Select Software:");
	label4=gtk_label_new("Argument1:");
	label5=gtk_label_new("Argument2:");
	
	 gtk_fixed_put(GTK_FIXED(fixed1), label3, 10, 5);
		gtk_widget_set_size_request(label3, 110, 25);
		gtk_fixed_put(GTK_FIXED(fixed1), combo, 121, 5);
		gtk_widget_set_size_request(combo, 109, 25);
		gtk_fixed_put(GTK_FIXED(fixed1), label4, 230, 5);
		gtk_widget_set_size_request(label4, 80, 25);
		gtk_fixed_put(GTK_FIXED(fixed1), entry1, 310, 5);
		gtk_widget_set_size_request(entry1, 80, 25);
		gtk_fixed_put(GTK_FIXED(fixed1), label5, 400, 5);
		gtk_widget_set_size_request(label5, 80, 25);
		gtk_fixed_put(GTK_FIXED(fixed1), entry2, 475, 5);
		gtk_widget_set_size_request(entry2, 80, 25);
		g_signal_connect(G_OBJECT(combo), "changed", 
        G_CALLBACK(combo_selected), NULL);
		gtk_box_pack_start(GTK_BOX(vbox), fixed1, FALSE, FALSE, 10);
		
		
		/* hbox2= gtk_hbox_new(FALSE, 10);
		 entry3=gtk_entry_new();
		label6=gtk_label_new("                                            Output:");
	
		//gtk_widget_set_size_request(label6, 150, 25);
		
		//gtk_widget_set_size_request(combo, 200, 25);
	
	 
		gtk_box_pack_start(GTK_BOX(hbox2), label6, FALSE, FALSE, 10);
		gtk_box_pack_start(GTK_BOX(hbox2), entry3, FALSE, FALSE, 10);
		gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 0);*/
		 hbox3= gtk_hbox_new(FALSE, 10);
		  button4=gtk_button_new_with_label("\t\t\t Send Request\t\t\t");
		   button5=gtk_button_new_with_label("\t\t\t Start Sharing\t\t\t");
		   gtk_box_pack_start(GTK_BOX(hbox3), button4, FALSE, FALSE, 10);
		gtk_box_pack_start(GTK_BOX(hbox3), button5, FALSE, FALSE, 10);
		gtk_box_pack_start(GTK_BOX(vbox), hbox3, FALSE, FALSE, 0);
		label=gtk_label_new("\t\t\t\t\t\tNot yet Sharing");
		gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE,0);
		
		gtk_entry_set_text((GtkEntry*)entry1,"0");
		gtk_entry_set_text((GtkEntry*)entry2,"0");
	gtk_widget_show_all(window);
	 g_signal_connect(G_OBJECT(button1), "clicked", 
      G_CALLBACK(setting_1),NULL);
      	 g_signal_connect(G_OBJECT(button2), "clicked", 
      G_CALLBACK(setting_2),NULL);
      	 g_signal_connect(G_OBJECT(button3), "clicked", 
      G_CALLBACK(setting_3),NULL);
      	 g_signal_connect(G_OBJECT(button4), "clicked", 
      G_CALLBACK(send_request),NULL);
      	 g_signal_connect(G_OBJECT(button5), "clicked", 
      G_CALLBACK(start_server),NULL);


 g_signal_connect(G_OBJECT(project), "activate",
        G_CALLBACK(project__), NULL);
         g_signal_connect(G_OBJECT(team), "activate",
        G_CALLBACK(team__), NULL);

      g_signal_connect(G_OBJECT(quit), "activate",
        G_CALLBACK(gtk_main_quit), NULL);
         g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), NULL);
 

  gtk_main();

  return 0;
}
