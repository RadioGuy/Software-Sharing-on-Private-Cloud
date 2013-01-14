#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct swlist
{
	GtkWidget *name;
	GtkWidget *id;
	GtkWidget *des;
};
void initialise__();
void initialise_list();
struct swlist list[6];


void refresh(GtkWidget* widget,gpointer data)
{
	initialise__();
	initialise_list();
	
}
void savetofile(GtkWidget* widget,gpointer data)
{
	
	int i=0;
	char *buff;
	FILE *fp=fopen("available_soft_list","w");
	if(fp==NULL)
	{
		perror("\n file not found\n");
		exit(1);
	}
	buff=malloc(500);
	
	memcpy(buff,"\0",100);
	for(i=0;i<6;i++)
	{
		printf("\n\n%s\n",gtk_entry_get_text((GtkEntry*)list[i].name));
		//memcpy(buff,'\0',300);
	
		
		printf("\n\n11000\n\n\n ");
		
		if((strncmp(gtk_entry_get_text((GtkEntry*)list[i].name),"NULL",sizeof("NULL"))==0))
			break;
		
		strcat(buff,gtk_entry_get_text((GtkEntry*)list[i].id));
		strcat(buff," ");
		
		strcat(buff,gtk_entry_get_text((GtkEntry*)list[i].name));
		strcat(buff," ");
		strcat(buff,gtk_entry_get_text((GtkEntry*)list[i].des));
		strcat(buff,"\n");
		printf("\nbuff: %s\n",buff);
		
		
		
}
printf("\n\n\nhello\n\n");
fprintf(fp,"%s",buff);	
printf("\nbuff: %s\n",buff);

fclose(fp);
}
void combo_selected(GtkWidget* widget,gpointer data)
{
	int i=0;
	char id[5];
	char des[100];
	gchar *text =  gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
	if(strncmp(text,"sum",sizeof("sum"))==0)
	{
		memcpy(id,"42",sizeof("42"));
		id[sizeof("42")]='\0';
		memcpy(des,"additionofinteger",sizeof("additionofintegers"));
		id[sizeof("additionofintegers")]='\0';
	}
	else if(strncmp(text,"sub",sizeof("sub"))==0)
	{
		memcpy(id,"43",sizeof("43"));
		id[sizeof("43")]='\0';
		memcpy(des,"subtractionofinteger",sizeof("subtactionofintegers"));
		des[sizeof("subtractionofintegers")]='\0';
	}
	else if(strncmp(text,"mul",sizeof("mul"))==0)
	{
		memcpy(id,"48",sizeof("48"));
		id[sizeof("48")]='\0';
		memcpy(des,"multiplicationofinteger",sizeof("multiplicationofintegers"));
		des[sizeof("multiplicationfintegers")]='\0';
	}
	else if(strncmp(text,"div",sizeof("div"))==0)
	{
		memcpy(id,"45",sizeof("45"));
		id[sizeof("45")]='\0';
		memcpy(des,"divisionofinteger",sizeof("divisionofintegers"));
		des[sizeof("divisionofinteger")]='\0';
	}
	else if(strncmp(text,"gcc",sizeof("gcc"))==0)
	{
		memcpy(id,"55",sizeof("55"));
		id[sizeof("55")]='\0';
		memcpy(des,"gcccompiler",sizeof("gcccompiler"));
		des[sizeof("gcccompiler")]='\0';
	}
		
	
	for(i=0;i<6;i++)
	{
		if(strncmp(gtk_entry_get_text((GtkEntry*)list[i].name),"NULL",sizeof("NULL"))==0)
			{
				break;
			}
	}
	
	gtk_entry_set_text((GtkEntry*)list[i].name,text);
	gtk_entry_set_text((GtkEntry*)list[i].id,id);
	gtk_entry_set_text((GtkEntry*)list[i].des,des);

}

void initialise__()
{
	int i=0;
	for(i=0;i<6;i++)
	{
		gtk_entry_set_text((GtkEntry*)list[i].name,(char*)"NULL");
		gtk_entry_set_text((GtkEntry*)list[i].id,(char*)"NULL");
		gtk_entry_set_text((GtkEntry*)list[i].des,(char*)"NULL");
	}
}
void initialise_list()
{
	
	int i=0,j=0,n=0;
	char ch;
	char output[150];
	FILE* fp;
	if((fp=fopen("available_soft_list","r"))==NULL)
	{
		printf("\nSoftware List not found\n");
		exit(1);
	}
	i=0;
	while(i<6)
	{
		while((ch=fgetc(fp))!='\n'){
               //ch = fgetc(fp); 
              
               if ( ch == EOF )
                      return ;
               else if(ch==' ')
               {
				   
				   n++;
				   output[j]='\0';
				   if(n==1){
					   gtk_entry_set_text((GtkEntry*)list[i].id,output);
					
				}
					
					else if(n==2)
					{
						 gtk_entry_set_text((GtkEntry*)list[i].name,output);
					}
					
				   j=0;
			   }
			   
			   else{
				   
                    output[j++]=ch;
                    
				}
			}output[j]='\0';
			 gtk_entry_set_text((GtkEntry*)list[i].des,output);
			n=0;
			j=0;
			i++;
			 
		}
		//flag=i;
	fclose(fp);
	
}


int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *table,*table2;
	GtkWidget* hbox;
	GtkWidget* vbox;
	GtkWidget* label1;
	GtkWidget* label2;
	GtkWidget* label3;
	GtkWidget* button,*button2;
	GtkWidget* combo;
	GtkWidget* hseparator;
	GtkWidget* wins,*fixed;
	char buff[2];
	int i=0;
	
	
	 gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Software List");
  gtk_window_set_default_size(GTK_WINDOW(window), 290, 300);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  
  vbox = gtk_vbox_new(TRUE, 1);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
  hbox = gtk_hbox_new(TRUE, 1);
  label1=gtk_label_new("software name");
  label2=gtk_label_new("software code");
  label3=gtk_label_new("software specification");
  
  table = gtk_table_new(8,4, FALSE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 3);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);
  
   gtk_table_attach(GTK_TABLE(table),label1, 1,2,0,1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 4, 4);
  gtk_table_attach(GTK_TABLE(table), label2, 2,3,0,1,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
  gtk_table_attach(GTK_TABLE(table),label3, 3,4,0,1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
      
      for(i=0;i<6;i++)
      {
		  list[i].name=gtk_entry_new();
		  list[i].id=gtk_entry_new();
		  list[i].des=gtk_entry_new();
		  sprintf(buff,"%d",i+1);
		  label1=gtk_label_new(buff);
		  gtk_table_attach(GTK_TABLE(table), label1, 0, 1, i+1, i+2, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 3, 3);
		  gtk_table_attach(GTK_TABLE(table), list[i].name, 1, 2, i+1, i+2, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), list[i].id, 2, 3, i+1, i+2, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), list[i].des, 3, 4, i+1, i+2, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	  }
      
  
  gtk_box_pack_start(GTK_BOX(vbox), table, TRUE, TRUE, 0);
  
  //hseparator = gtk_hseparator_new();
   gtk_box_pack_start(GTK_BOX(vbox), hseparator, TRUE, TRUE, 0);

  
  
  combo = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "sum");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "sub");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "div");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "gcc");
  gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "mul");
  
  button=gtk_button_new_with_label("Add");
   button2=gtk_button_new_with_label("Refresh");
  
  
    fixed = gtk_fixed_new();
    
    gtk_fixed_put(GTK_FIXED(fixed), combo, 10, 5);
  gtk_widget_set_size_request(combo, 80, 35);
  gtk_fixed_put(GTK_FIXED(fixed), button, 100, 5);
  gtk_widget_set_size_request(button, 80, 35);
  gtk_fixed_put(GTK_FIXED(fixed), button2, 190, 5);
  gtk_widget_set_size_request(button2, 80, 35);
  
  gtk_box_pack_start(GTK_BOX(vbox), fixed, TRUE, TRUE, 0);
     

  
  initialise__();
initialise_list();
  
  
   
   
   
 gtk_widget_show_all(window);
 g_signal_connect(G_OBJECT(button), "clicked", 
      G_CALLBACK(savetofile),NULL);
       g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(refresh),NULL);
      
      g_signal_connect(G_OBJECT(combo), "changed", 
        G_CALLBACK(combo_selected), (gpointer) label1);
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

  return 0;
}
