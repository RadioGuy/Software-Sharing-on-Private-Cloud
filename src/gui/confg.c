#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define C_FILE "cloud_configuration"

 GtkWidget *ip,*port,*ttl;
void initialise()
{

	FILE* fp;
	char ch;
	int i=0,j=0;
	char buff[50];
	char cuff[50];
	
	if((fp=fopen(C_FILE,"r"))==NULL) perror("file not found");
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch=='#')
		{	
			while((ch=fgetc(fp))!='\n')
			{}
		}
		else if(ch!='\n')
		{
			j=0;
			while(ch!=':')
			{
				buff[j++]=ch;
				ch=fgetc(fp);
				}
				buff[j]='\0';
			
				
			j=0;
			while((ch=fgetc(fp))!='\n')
			{
				cuff[j++]=ch;
				}	
			cuff[j]='\0';
			printf("\n%s\n",cuff);
			if(strncmp(buff,"reciever_port",sizeof("reciever_port"))==0)
			{
					gtk_entry_set_text((GtkEntry*)port,cuff);			}
			else if(strncmp(buff,"reciever_ip",sizeof("reciever_ip"))==0)
			{
					gtk_entry_set_text((GtkEntry*)ip,cuff);
			}
			else if(strncmp(buff,"ttl",sizeof("ttl"))==0)
			{
					gtk_entry_set_text((GtkEntry*)ttl,cuff);
			}
			else if(strncmp(buff,"type",sizeof("type"))==0)
			{
				
			}
		}
		
	}
	
}
void refresh(GtkWidget * widget,gpointer data)
{
	initialise();
}
void save(GtkWidget * widget,gpointer data)
{
	char *buff;
	char *msg="# This is configuration file for the Cloud\n# configure the Cloud client and Server\n# Warning: Noobs are adviced to use their head before applying any random update";
	FILE *fp=fopen(C_FILE,"w");
	if(fp==NULL)
	{
		perror("\n file not found\n");
		exit(1);
	}
	buff=malloc(1000);
	
	memcpy(buff,"\0",700);
	memcpy(buff,msg,strlen(msg));
	strcat(buff,"\n");
	strcat(buff,"reciever_port:");
	
	strcat(buff,gtk_entry_get_text((GtkEntry*)port));
		strcat(buff,"\n");
		strcat(buff,"reciever_ip:");
		
		strcat(buff,gtk_entry_get_text((GtkEntry*)ip));
		strcat(buff,"\n");
		strcat(buff,"ttl:");
		strcat(buff,gtk_entry_get_text((GtkEntry*)ttl));
		strcat(buff,"\n");
		strcat(buff,"type:1\n");
		printf("\nbuff: %s\n",buff);
		fprintf(fp,"%s",buff);	
		fclose(fp);
}
int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *table;

  GtkWidget *label1,*label2,*label3;
  GtkWidget *button1;
  GtkWidget *button2;
   GtkWidget *button3;
   

   GtkWidget *vbox;
   GtkWidget *fixed;
   
    gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Configuration");
  gtk_window_set_default_size(GTK_WINDOW(window), 290, 150);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  
  vbox = gtk_vbox_new(TRUE, 1);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  
	table = gtk_table_new(5,2, FALSE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 3);
  gtk_table_set_col_spacings(GTK_TABLE(table), 4);
  
 
	  label1=gtk_label_new("IP address:");
  label2=gtk_label_new("port:");
  label3=gtk_label_new("ttl(hopecount):");
   gtk_table_attach(GTK_TABLE(table),label1, 0,1,0,1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 4, 4);
  gtk_table_attach(GTK_TABLE(table), label2, 0,1,1,2,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
gtk_table_attach(GTK_TABLE(table), label3, 0,1,2,3,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
      
      
      ip=gtk_entry_new();
		  port=gtk_entry_new();
		  ttl=gtk_entry_new();
      gtk_table_attach(GTK_TABLE(table),ip, 1,2,0,1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 4, 4);
  gtk_table_attach(GTK_TABLE(table), port, 1,2,1,2,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
gtk_table_attach(GTK_TABLE(table), ttl,1,2,2,3,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
      
      gtk_box_pack_start(GTK_BOX(vbox), table, TRUE, TRUE, 0);
      
       button1=gtk_button_new_with_label("Save");
   button2=gtk_button_new_with_label("Refresh");
   button3=gtk_button_new_with_label("Cancel");
      
      
       fixed = gtk_fixed_new();
    
    gtk_fixed_put(GTK_FIXED(fixed), button1, 100, 5);
  gtk_widget_set_size_request(button1, 85, 25);
 // gtk_fixed_put(GTK_FIXED(fixed), button2, 100, 5);
  gtk_widget_set_size_request(button2, 80, 25);
  gtk_fixed_put(GTK_FIXED(fixed), button3, 190, 5);
  gtk_widget_set_size_request(button3, 85, 25);
  
  gtk_box_pack_start(GTK_BOX(vbox), fixed, TRUE, TRUE, 0);
  
  initialise();
     
      gtk_widget_show_all(window);
      
      
      
 g_signal_connect(G_OBJECT(button1), "clicked", 
      G_CALLBACK(save),NULL);
       g_signal_connect(G_OBJECT(button2), "clicked", G_CALLBACK(refresh),NULL);
      
      g_signal_connect(G_OBJECT(button3), "clicked", 
        G_CALLBACK(refresh), (gpointer) label1);
  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

  return 0;
}

      
