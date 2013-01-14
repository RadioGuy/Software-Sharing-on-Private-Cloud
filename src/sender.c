#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#define MAX_COST 5
#define NODE_LIST "node_list"
#define REQUEST_LIST "pending_queue"
#define TTL_MAX 4
#define OUT_FILE "output.html"


struct raw_packet
{
	int status;     //available=1 not available=2 ; earlier = code
	int type;		
	char* output;
	int req_id;
	char* address;
};
struct request_data
{
	int request_id;
	char argument[5000];
	int software_id;
	int type;
	int req_code;
	int ttl;
	char source_address[20];
	int source_server_port;
};

struct queue_node
{
	struct queue_node* next;
	struct queue_node* previous;
	int ttl;
	int req_id;         //unique id number =proceess id
	int req_type;        //1= primary 2=secondary
	struct request_data req_info;
};
struct queue
{
	struct queue_node* head;
	struct queue_node* tail;
};

struct global_software_info
{
	struct global_software_info* next;
	int id;
};
struct node
{
	struct node* next;
	int cost;
	char* node_ip;
	int port;
	char* node_name;  //url
	
};

struct list_of_nodes
{
	
	int i;
	struct node node_index[MAX_COST+1]; 
	
};

struct software_list
{
	char software[15];
	int aval;  
};          //availability status: 1=available 0= software unavailable
	

int get_software_request__();            // Obtain the global s/w id for the requested s/w.
void send_request__(struct list_of_nodes,struct request_data);
void update_pending_queue(int,char*);
struct raw_packet parse(char*);
void push_queue(struct queue*,struct queue_node);
void pop_queue(struct queue*,struct queue_node*,int);  //0 for regular pop  and 1 for selected pop
int create_presentation__(char*,int);        //to create a presentation for a particular request

int create_presentation__(char* output,int req_id)
{
	
	char* buff;
	char* buff1="<html><center><h1>OUTPUT from the remote HOST</h1><br>";
	char* file;
	char* rs;
	
	buff=malloc(500);
	file=malloc(50);
	rs=malloc(10);
	sprintf(rs,"%d",req_id);
	memcpy(buff,"0",50);
	
	memcpy(file,OUT_FILE,strlen(OUT_FILE));
	file[strlen(OUT_FILE)]='\0';
	strcat(file,rs);
	
	memcpy(buff,"touch ",sizeof("touch "));
	buff[sizeof("touch ")]='\0';
	strcat(buff,OUT_FILE);
	system(buff);
	FILE* fp=fopen(OUT_FILE,"w");
	if(fp==NULL)
	 return -1;
	 
	
	
	//memcpy(buff,"0",500);
	memcpy(buff,buff1,strlen(buff1));
	buff[strlen(buff1)]='\0';
	strcat(buff,output);
	fprintf(fp,"%s",buff);
	system("firefox output.html");
	return 1;
	
}



void push_queue(struct queue* pen_queue,struct queue_node temp_node)
{
	struct queue_node* qnode;
	struct queue_node* pre;
	qnode=malloc(sizeof(struct queue_node));
	memcpy(qnode,"0",sizeof(struct queue_node));
	qnode->previous=NULL;
	qnode->next=NULL;
	memcpy(qnode,&temp_node,sizeof(struct queue_node));
	if(pen_queue->head==NULL)
	{
		pen_queue->head=qnode;
		pen_queue->tail=qnode;
		qnode->next=qnode;
		qnode->previous=qnode;
	}
	else
	{
		//pen_queue.tail=qnode;
		
		qnode->previous=pen_queue->tail;
		pre=qnode->previous;
		pre->next=qnode;
		qnode->next=pen_queue->head;
		pen_queue->tail=qnode;
		//pre=pen_queue->tail;
		//pre->next=pen_queue->head;
	
		
	}
	temp_node=*pen_queue->tail;
	printf("\ntail info ttl %d",temp_node.ttl);
	printf("\n node info ttl %d\n",qnode->ttl);
	
}
void pop_queue(struct queue* pen_queue,struct queue_node* qnode,int flag)
{
	struct queue_node* temp_node;
	struct queue_node* x;
	int id;
	if(pen_queue->head==NULL)
		return;
	else if(flag==0)
	{
		temp_node=pen_queue->head;
		pen_queue->head=temp_node->next;
		temp_node->previous=pen_queue->tail;
		temp_node=pen_queue->tail;
		temp_node->next=pen_queue->head; 
		//free(temp_node);
	}
	else
	{
		
		temp_node=pen_queue->head;
		while(temp_node!=NULL){
		//id=temp_node->req_id;
		if(temp_node->req_id==qnode->req_id)
		{
			x=temp_node->previous;
			x->next=temp_node->next;
			x=temp_node->next;
			x->previous=temp_node->previous;
			free(qnode);
			return;
			
			//remove
		}
		temp_node=temp_node->next;
		
	}
		
	}
}

void get_input__(struct list_of_nodes* list,FILE* fp)

{	
	
	struct node* host,*previous;
	char ch;
	char output[50];
	int j=0;
	int n=0;
	
	
		
			while ( 1 )
			{
				
	host=malloc(sizeof(struct node));
	host->node_ip=malloc(50);
	host->node_name=malloc(50);
	host->next=NULL;
		//ch=fgetc(fp);
				while((ch=fgetc(fp))!='\n'){
               //ch = fgetc(fp); 
              
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
						host->port=atoi(output);
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
			host->cost=atoi(output);
			n=0;
			j=0;
			 
	printf("\n port %d\n",host->port);
	
	printf("\ncost %d\n",host->cost);
	printf("\n node ip:%s\n",host->node_ip);
	
	if(host->cost>MAX_COST)
	{	
		printf("exception");
		exit(1);
	}
	previous=&list->node_index[host->cost];
	if(previous->cost==10)
	{
		printf("\n success2\n");
		list->node_index[host->cost]=*host;
		free(host);
		printf("\n success1  %d cost\n",previous->cost);
		
	}
	else
	{	
		
		while(previous->next!=NULL)
		{
			previous=previous->next;
			printf("\n success\n");
		}
		previous->next=host;
	}
	if ( ch == EOF )
                      return ;
}
	
}
void display_list__(struct list_of_nodes* list)
{
	int temp1=0;
	struct node* previous;
	printf("\n\n list");
	previous=&list->node_index[1];
	printf("\n list %d",previous->cost);
	for(temp1=0;temp1<MAX_COST;temp1++)
	{
			previous=&list->node_index[temp1];
			do
			{
				printf("\n node ip: %s \nnode cost: %d \nnode port: %d\n",previous->node_ip,previous->cost,previous->port);
				previous=previous->next;
				
			}while(previous!=NULL);
	}
}

struct raw_packet parse(char* raw_data)
{
	struct raw_packet packet;
	int i=0,j=0,flag=0;
	//char c;
	char code[10];
	char* data;
	data=malloc(500);
	
	
	 printf("\n \n inside parse\n\n");
	 printf("\n rawdata: %s\n",raw_data);
	
	while(raw_data[i]!='\0')
	{
		if(flag==0){
		code[j++]=raw_data[i++];
		if(raw_data[i]==':')
		{
			code[j]='\0';
			flag=1;
			printf("\n code:%s",code);
			packet.req_id=atoi(code);
			j=0;
			i++;
		}
		}
		else if(flag==1){
			data[j++]=raw_data[i++];
			if(raw_data[i]==':')
		{
			data[j]='\0';
			flag=2;
			packet.type=atoi(data);
			j=0;
			i++;
		}
		}
		else if(flag==2){
			data[j++]=raw_data[i++];
			if(raw_data[i]==':')
		{
			data[j]='\0';
			flag=3;
			packet.status=atoi(data);
			j=0;
			i++;
		}
		}
		else if(flag==3){
			data[j++]=raw_data[i++];
			if(raw_data[i]==':')
		{
			printf("\nout:%s",data);
			data[j]='\0';
			flag=4;
			printf("\ndata:%s",data);
			packet.output=malloc(strlen(data));
			memcpy(packet.output,data,strlen(data));
			packet.output[strlen(data)]='\0';
			j=0;
			i++;
		}
		}
		else if(flag==4){
			data[j++]=raw_data[i++];
			if(raw_data[i]==':')
		{
			data[j]='\0';
			flag=5;
			packet.address=malloc(strlen(data));
			memcpy(packet.address,data,strlen(data));
			packet.address[strlen(data)]='\0';
			j=0;
			i++;
		}
		}
		
		else{
			printf("\n rid:%d ",packet.req_id);
			printf("\n type:%d",packet.type);
			printf("\n status:%d",packet.status);
			printf("\n output:%s \n",packet.output);
			
		 break;
	 }
		
	}
	//data[j]='\0';
	
	//packet.code=atoi(code);
	//packet.output=data;
	 printf("\n rid:%d ",packet.req_id);
			printf("\n type:%d",packet.type);
			printf("\n status:%d",packet.status);
			printf("\n output:%s",packet.output);
			printf("\n address:%s \n",packet.address);
	//printf("\n type: %d \n output : %s\n",packet.type,packet.output);
	return packet;
	
	
}
struct list_of_nodes* initialise__(struct list_of_nodes* list)
{
	int temp1=0,temp2=0;
	struct node* ntemp;
	list->i=0;
	for(temp1=0;temp1<=MAX_COST;temp1++)
	{
		ntemp=&list->node_index[temp1];
		ntemp->node_ip=malloc(50);
		ntemp->node_name=malloc(50);
		ntemp->cost=10;
		ntemp->port=0;
		ntemp->next=NULL;
	}
	return list;
}
struct node* getnextnode(struct list_of_nodes* list)
{
	
}
int get_software_request__()
{
	int software_id=0;
	
	printf("\n Enter the requested sofware id \n");
	scanf("%d",&software_id);
	return software_id;
}


void update_pending_queue(int flag, char* new_request)
{
}

void create_send_packet(char* send_data,struct request_data req_data)
{
	char buffer[50];
	char* buff=":";
	
	sprintf(buffer,"%d",req_data.req_code);
	memcpy(send_data,buffer,strlen(buffer));
	send_data[strlen(buffer)]=':';
	send_data[strlen(buffer)+1]='\0';
	
	sprintf(buffer,"%d",req_data.type);
	strcat(send_data,buffer);
	strcat(send_data,buff);
	
	sprintf(buffer,"%d",req_data.request_id);
	strcat(send_data,buffer);
	strcat(send_data,buff);
	
	sprintf(buffer,"%d",req_data.software_id);
	strcat(send_data,buffer);
	strcat(send_data,buff);
	
	strcat(send_data,req_data.argument);
	strcat(send_data,buff);
	
	sprintf(buffer,"%d",req_data.ttl);
	strcat(send_data,buffer);
	strcat(send_data,buff);
	
	strcat(send_data,req_data.source_address);
	strcat(send_data,buff);
	
	sprintf(buffer,"%d",req_data.source_server_port);
	strcat(send_data,buffer);
	strcat(send_data,buff);
	
	//strcat(send_data,req_data.argument);
	//strcat(send_data,buff);
	
	printf("\n send data packet:%s\n",send_data);
}
int connect_over_cloud(struct node min_cost_node,struct request_data req_data)
{
	
	int sockfd;
	struct sockaddr_in server_add;
	struct hostent *server;
	char send_data[100],recv_data[1000];
	int bytes_recieved=0;
	int recv_code;
	char *msg="1:1:42:42:10 8:";
	struct raw_packet recv_data_info;
	int port=0;
	struct queue pending_queue;
	struct queue_node qnode;
	int i=0;
	
	port=min_cost_node.port;
	printf("\n\n\n node_ip:%s \nport:%d \n",min_cost_node.node_ip,min_cost_node.port);
	
	create_send_packet(send_data,req_data);
	//memcpy(send_data,msg,strlen(msg));
	//send_data[strlen(msg)]='\0';
	
	printf("\n send data:%s \n \n",send_data);
	printf("\n \n\ncheck4\n");
	printf("\n\n\n node_ip:%s \n \n",min_cost_node.node_ip);
		
	server= gethostbyname(min_cost_node.node_ip);
	printf("\n \n\ncheck1\n");
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("\nconnection error in socket creation %d",sockfd);
		return -1;
	}
	printf("\n \n\ncheck1\n");
	
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(port);
	server_add.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(server_add.sin_zero),8); 

	printf("\n \n\ncheck\n");
                
      if(connect(sockfd,(struct sockaddr*)&server_add,sizeof(struct sockaddr))<0)
      {
		  printf("\n connect error\n");
		  return -1;
	  }
	  if(write(sockfd,send_data,strlen(send_data))<0)	
	  {
		  perror("\n send error\n");
		  return 0;
		  //exit(1);
	  }
	   printf("\n check3\n");
	  if((bytes_recieved=read(sockfd,recv_data,100))==0)
	  {
		  printf("\n data not recieved from server\n");   //alternate:another try to resend the data can be attempted
		  return 3;
	  } 
	  printf("\n check2\n");
          recv_data[bytes_recieved] = '\0';
          printf("\n recv data: %s \n",recv_data);
          
          
           printf("\n check4\n");
          recv_data_info=parse(recv_data);   //return parsed structure
          
          printf("\n check parse \n \n");
          
          //recv_code=atoi(recv_data);
          
          if(recv_data_info.type==1 && recv_data_info.status==1)          //type =1  maens ack/nack type   statu=1 means s/w available
          {
			  printf("\n software available\n");
			   bytes_recieved=read(sockfd,recv_data,100);    //recieving executed output
				recv_data[bytes_recieved] = '\0';
				 recv_data_info=parse(recv_data);   //return parsed structure
				 if( recv_data_info.type==3)   // if type != 2  means not containing the output
				 {
					 //printf("\nerror\n");
					
	  
						printf("recieved secondary response");
						exit(1);
	  
					// return 0;
				 }
				printf("\n The output obtained from the cloud is:\n************************\n %s\n********************\n",recv_data);
				for(i=0;i<20;i++)
					printf("\n");
				printf("\n\n\t\t****************************\n\t\t\toutput\n\t\t****************************\n\n\t\t%s\n\n\n\n\n\r",recv_data_info.output);
				//pop_queue(&pending_queue,&qnode);
				//update_pending_queue(0,"0");  //0=requested granted--remove from pending queue
				
				for(i=0;i<10;i++)
					printf("\n");
					
					if(create_presentation__(recv_data_info.output,recv_data_info.req_id)<0)
						perror("\nerror in file\n");
				exit(1);       //exiting the child proceess, since requested is granted and output is obtained
				//return 1;
		  }
		  else if(recv_data_info.type==1 && recv_data_info.status==2)   //sofware unavailable code
 		  {
			  printf("\n software not avilable at server \n");
			  //exit(1);       //exiting the child proceess, since requested is granted and output is obtained
			   
			  // push_queue(&pending_queue,&qnode);
			  return 2;
		  }
	 
          

	  return 0;

}

void send_request__(struct list_of_nodes list, struct request_data req_data)
{
	int temp1=0;
	int temp2=0;
	struct node min_cost_node;
	
	for(temp1=0;temp1<MAX_COST;temp1++)
	{
		min_cost_node=list.node_index[temp1];
		printf("\n node cost: %d\n",min_cost_node.cost);
		if(min_cost_node.cost!=10){
		
		if(connect_over_cloud(min_cost_node,req_data)<0)
		{
			perror("\n connect oer cloud error\n");
			//exit(1);
		}
		while(min_cost_node.next!=NULL)
		{
			
					if(connect_over_cloud(min_cost_node,req_data)<0)
					{
						perror("\n connect over cloud error\n");
						//exit(1);
					}

			min_cost_node=*(min_cost_node.next);
			printf("\n node cost: %d\n",min_cost_node.cost);
		}
	}
	}
	 printf("\n Requested Software not found on all server \n");
	 exit(1);  //exiting the child process
	
}
int main(int argc,char* argv[])
{
	int temp1,temp2=0;
	struct list_of_nodes* list;
	struct node* next_node;
	int software_id=0;
	char flag=' ';
	int pid=0;
	struct request_data req_data;
	struct node test_min_node;   //testing purpose only
	struct queue_node qnode;
	struct queue pending_queue;
	char arg1[10];
	char* arg2;
	char* buff=" ";
	
	
	FILE* fp;
	FILE* fp1;
	FILE* input_file;
	char ch;
	int i=0;
	//struct hostent* hostn; //test
	if((fp=fopen(NODE_LIST,"r"))==NULL)
	{
		perror("\n file of node list not found\n");
		exit(1);
	}
	if((fp1=fopen(REQUEST_LIST,"w"))==NULL)
	{
		perror("\n file of node list not found\n");
		exit(1);
	}
	
	printf("\nTest success5 maxcount %d\n",MAX_COST);
	list=malloc(sizeof(struct list_of_nodes));
	
	/*Initialising the list_of_nodes data structure with 0 value at each node in the array*/
	list=initialise__(list);
	
	for(temp1=0;temp1<3;temp1++)
	{
		printf("cost %d  ip %s\n",list->node_index[temp1].cost,list->node_index[temp1].node_ip);
	}
		
		get_input__(list,fp);
	
	
		display_list__(list);
		printf("\n hello\n");
		
		
		/*Testing Connect over cloud*/
	/*	test_min_node.cost=1;
		test_min_node.next=NULL;
		//test_min_node.node_ip=malloc(30);
		test_min_node.node_ip="127.0.0.1";
		req_data.i=0;
		req_data.software_id=2434;
		//req_data.software_id[4]='\0';
		printf("\ndata ok\n");
		printf("\n%shello",test_min_node.node_ip);
		if(connect_over_cloud(test_min_node,req_data)<0)  //Only for Testing
		{
			perror("\n connect over cloud errror");
			exit(1);
		}
		printf("\n connected\n");  */
		/*Testing End*/
		
		
		/*Getting request Info*/
		
		
		while(1)
		{
			
			
			printf("helloout\n");
			flag='w';
			//software_id=get_software_request__();
			software_id=atoi(argv[1]);
			printf("\nSoftware ID: %d\n",software_id);
			if((pid=fork())<0)
			{
				perror("fork error");
				exit(1);
			}
			if(pid!=0)
			{
				//req_data.i=0;
				req_data.software_id=software_id;
				req_data.request_id=pid;
				//req_data.type=1;
				req_data.req_code=1;
				
				
				memcpy(arg1,argv[2],strlen(argv[2]));
				arg1[strlen(argv[2])]='\0';
				
				arg2=argv[3];
				printf("%s\n",arg1);
				
				if(atoi(arg1)<0){
					
					input_file=fopen(arg2,"r");
					if(input_file==NULL)
						exit(1);
					while((ch=fgetc(input_file))!=EOF)
					{
						req_data.argument[i++]=ch;
					}
					req_data.argument[i]='\0';
					
					
				}
				else{
				
			    strcat(arg1,buff);
				printf("%s\n",arg1);
				//arg1[strlen(arg1)+1]='\0';
				//printf("%s\n",arg1);
				//printf("%s\n",arg1);
				strcat(arg1,arg2);
				printf("%s\n",arg1);
				
				memcpy(req_data.argument,arg1,strlen(arg1));
				req_data.argument[strlen(arg1)]='\0';
				}
				printf("\nargumentis:%s \n",req_data.argument);
				
				req_data.ttl=atoi(argv[4]);  //ttl value given by the calling program :user or reciever
				memcpy(req_data.source_address,argv[5],strlen(argv[5]));
				req_data.source_address[strlen(argv[5])]='\0';
				req_data.source_server_port=atoi(argv[6]);   //given by the calling program : user or reciever
				
				req_data.type=atoi(argv[7]);
				
				send_request__(*list,req_data);
				
				exit(1);
				
			}
			else if(pid==0) //temp code to take single request
			{
				exit(1);
				}
		}

}
