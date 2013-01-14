#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct node_set
{
	GtkWidget *add;
  GtkWidget *port;
  GtkWidget *cost;
};
struct node_list
{
	char* node_ip;
	char* port;
	char* cost; 
};
struct node_set node[6];
void initialise__();
void get_input(struct node_list*);
void initialise__()
{
	int i=0;
	for(i=0;i<6;i++)
	{
		gtk_entry_set_text((GtkEntry*)node[i].add,(char*)"0");
		gtk_entry_set_text((GtkEntry*)node[i].cost,(char*)"0");
		gtk_entry_set_text((GtkEntry*)node[i].port,(char*)"0");
	}
}
void refresh(GtkWidget* widget, gpointer node1)
{
	
	struct node_list* host;
	 host=malloc(sizeof(struct node_list));
  host->node_ip=malloc(15);
  host->port=malloc(5);
  host->cost=malloc(2);
  initialise__();
  get_input(host);
}
void save_button_clicked(GtkWidget* widget, gpointer node1)
{
	int i=0;
	char *buff;
	FILE *fp=fopen("node_list","w");
	if(fp==NULL)
	{
		perror("\n file not found\n");
		exit(1);
	}
	buff=malloc(400);
	
	memcpy(buff,"\0",100);
	for(i=0;i<6;i++)
	{
		printf("\n\n%s\n",gtk_entry_get_text((GtkEntry*)node[i].add));
		//memcpy(buff,'\0',300);
		
		
		printf("\n\n11000\n\n\n ");
		
		if((strncmp(gtk_entry_get_text((GtkEntry*)node[i].add),"0",sizeof("0"))==0))
			break;
			//printf("\n\n11222 %s",node[i].add);
		strcat(buff,":");
		
		strcat(buff,gtk_entry_get_text((GtkEntry*)node[i].add));
		strcat(buff," ");
		
		strcat(buff,gtk_entry_get_text((GtkEntry*)node[i].port));
		strcat(buff," ");
		strcat(buff,gtk_entry_get_text((GtkEntry*)node[i].cost));
		strcat(buff,"\n");
		printf("\nbuff: %s\n",buff);
		
		
		
}
printf("\n\n\nhello\n\n");
fprintf(fp,"%s",buff);	
printf("\nbuff: %s\n",buff);

fclose(fp);
}

void get_input(struct node_list* host)

{	
	

	char ch;
	char output[50];
	int j=0;
	int n=0;
	int i=0;
	FILE* fp;
	
	if((fp=fopen("node_list","r"))==NULL)
	{
		perror("file not found");
		return;
	}
		
			while ( 1 )
			{
			
				while((ch=fgetc(fp))!='\n'){
         
              
               if ( ch == EOF )
                      return ;
               else if(ch==' ')
               {
				   
				   n++;
				   output[j]='\0';
				   if(n==1){
					memcpy(host->node_ip,output,strlen(output));
					host->node_ip[strlen(output)]='\0';
				}
					
					else if(n==2)
						memcpy(host->port,output,strlen(output));
					host->port[strlen(output)]='\0';
					//else if(n==3)
						//host->cost=atoi(output);
				   j=0;
			   }
			   
			   else{
				   if(ch==':')
				   {ch=fgetc(fp);
				   printf("\nchecking\n");
						}
                    output[j++]=ch;
                    
				}
			}
			
             //ch=fgetc(fp); 
			 output[j]='\0';
			memcpy(host->cost,output,strlen(output));
					host->cost[strlen(output)]='\0';
			n=0;
			j=0;
			
			 
	printf("\n port %s\n",host->port);
	
	printf("\ncost %s\n",host->cost);
	printf("\n node ip:%s\n",host->node_ip);
	gtk_entry_set_text((GtkEntry*)node[i].add,host->node_ip);
	gtk_entry_set_text((GtkEntry*)node[i].port,host->port);
		gtk_entry_set_text((GtkEntry*)node[i].cost,host->cost);

	
	
	i++;
	
	
	if ( ch == EOF )
                      return ;
}
fclose(fp);
	
}
int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *table;

  GtkWidget *label1;
  GtkWidget *button1;
  GtkWidget *button2;
   GtkWidget *button3;
GtkWidget *ok;
  GtkWidget *close;

  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *halign;
  GtkWidget *valign;
  
  struct node_list* host;
  
 
  
  char *values[3] = { "ip address", "port", "cost"
  };
  int i=0,j=0;


	 host=malloc(sizeof(struct node_list));
  host->node_ip=malloc(15);
  host->port=malloc(5);
  host->cost=malloc(2);
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_title(GTK_WINDOW(window), "Neighbour Node List");
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  
   table = gtk_table_new(6,6, TRUE);
  gtk_table_set_row_spacings(GTK_TABLE(table), 2);
  gtk_table_set_col_spacings(GTK_TABLE(table), 2);

  gtk_container_add(GTK_CONTAINER(window), table);

  

for( i=0; i < 6; i++) {
	
    for( j=0; j < 3; j++) {
      label1 = gtk_label_new(values[j]);
      //gtk_table_attach_defaults(GTK_TABLE(table), button, j, j+1, i, i+1 );
      gtk_table_attach(GTK_TABLE(table), label1, (2*j), (2*j)+1, i, i+1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
     
    }
  }

for(i=0;i<6;i++){
	
  node[i].add= gtk_entry_new();
  node[i].port= gtk_entry_new();
  node[i].cost = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), node[i].add, 1, 2, i, i+1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), node[i].port, 3, 4, i, i+1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), node[i].cost, 5, 6, i, i+1, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  }
		button1 = gtk_button_new_with_label("Save");
		button2 = gtk_button_new_with_label("Cancel");
		button3 = gtk_button_new_with_label("Refresh");
  // gtk_widget_set_size_request(button1, 50, 25);
   //gtk_widget_set_size_request(button2, 50, 25);
		gtk_table_attach(GTK_TABLE(table), button1, 3,4,6,7, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
      gtk_table_attach(GTK_TABLE(table), button2, 5,6,6,7, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
      gtk_table_attach(GTK_TABLE(table), button3, 4,5,6,7, 
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
      
    initialise__();          //initialising content
     
    get_input(host); 
    
   gtk_widget_show_all(window);
 g_signal_connect(G_OBJECT(button1), "clicked", 
      G_CALLBACK(save_button_clicked),node);
       g_signal_connect(G_OBJECT(button2), "clicked", 
      G_CALLBACK(refresh),node);
       g_signal_connect(G_OBJECT(button3), "clicked", 
      G_CALLBACK(refresh),node);

  g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

  return 0;
}
