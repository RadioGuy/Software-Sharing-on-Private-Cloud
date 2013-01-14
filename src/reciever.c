#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOFT_LIST "available_soft_list"
#define OUT_FILE "output.html"
#define SOURCE_FILE "source_file"

char* gip="127.0.0.1";
int gport;

struct request_data
{
	int request_id;
	char argument[50];
	int software_id;
	int req_code;      //req/res type type1 or type 2
	int type;          //primary or secondary request- 1 or 2
	int ttl;
	char source_address[20];
	int source_server_port;
};
struct response_data
{
	int req_id;
	int type;
	int status;
	char* output;
	char* address;
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
struct software_info
{
	int software_id;
	char process_name[10];  //with full path
	char process_info[20];
};



struct raw_packet
{
	int status;     //available=1 not available=2 ; earlier = code
	int type;		
	char* output;
	int req_id;
	char* address;
};

struct software_info* soft_list[10];

void push_queue(struct queue*,struct queue_node);
void pop_queue(struct queue*,struct queue_node*,int);  //0 for regular pop  and 1 for selected pop
int request_reciever(struct queue*,int);
int check_availability(int);
void process_request(int,char*,char*);
void create_send_packet(struct response_data*,char*);
void set_response(struct response_data*,int,int,int,char*,char*);
void response_pend_req(struct request_data*);
void read_output(struct request_data);
void create_send_packet_second(char*,struct request_data);
struct raw_packet parse1(char*);
int create_presentation__(char*,int);        //to create a presentation for a particular request\
void parse_argument__(char*,char*,char*);
void write_into_file(char*,char*);

void parse_argument__(char* arg1,char* arg2,char* argument)
{
	int i=0,j=0;
	while(argument[i]!=' ')
	{
		arg1[j++]=argument[i++];
	}
	arg1[j]='\0';
	j=0;
	while(argument[i]!='\0')
	{
		arg2[j++]=argument[++i];
	}
	arg2[j]='\0';
	printf("\narg1:%s\narg2:%s\n",arg1,arg2);
	
}
void write_into_file(char* arg2,char* source_file)
{
	FILE* fp;
	 if((fp=fopen(source_file,"w"))==NULL)
		perror("file open error");
	fprintf(fp,"%s",arg2);
}

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



void create_send_packet_second(char* send_data,struct request_data req_data)
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
struct raw_packet parse1(char* raw_data)
{
	struct raw_packet packet;
	int i=0,j=0,flag=0;
	//char c;
	char code[10];
	char* data;
	data=malloc(500);
	
	
	// printf("\n \n inside parse\n\n");
	 //printf("\n rawdata: %s\n",raw_data);
	
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
void read_output(struct request_data parsed_req_data)
{
	
	int sockfd;
	struct sockaddr_in server_add;
	struct hostent *server;
	char send_data[100],recv_data[1000];
	int bytes_recieved=0;
	struct request_data req_data;
	struct raw_packet recv_data_info;
	int i=0;
	
	req_data.software_id=parsed_req_data.software_id;
	req_data.request_id=parsed_req_data.request_id;
	req_data.type=1;
	req_data.req_code=1;
	memcpy(req_data.argument,parsed_req_data.argument,strlen(parsed_req_data.argument));
	req_data.argument[strlen(parsed_req_data.argument)]='\0';
	req_data.ttl=4;
	
	
	create_send_packet_second(send_data,req_data);
	//memcpy(send_data,msg,strlen(msg));
	//send_data[strlen(msg)]='\0';
	
	printf("\n send data:%s \n \n",send_data);
	server= gethostbyname(parsed_req_data.source_address);
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("\nconnection error in socket creation %d",sockfd);
		exit(1);
	}
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(parsed_req_data.source_server_port);
	server_add.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(server_add.sin_zero),8); 

	//printf("\n check\n");
                
      if(connect(sockfd,(struct sockaddr*)&server_add,sizeof(struct sockaddr))<0)
      {
		  printf("\n connect error\n");
		  return;
	  }
	  if(write(sockfd,send_data,strlen(send_data))<0)	
	  {
		  perror("\n send error\n");
		  return;
		  //exit(1);
	  }
	  // printf("\n check3\n");
	  if((bytes_recieved=read(sockfd,recv_data,100))==0)
	  {
		  printf("\n data not recieved from server\n");   //alternate:another try to resend the data can be attempted
		  return;
	  } 
	//  printf("\n check2\n");
          recv_data[bytes_recieved] = '\0';
          printf("\n recv data: %s \n",recv_data);
          
          
          // printf("\n check4\n");
         // recv_data_info=parse2(recv_data);   //return parsed structure
          
        //  printf("\n check parse \n \n");
          
          //recv_code=atoi(recv_data);
          //if(recv_data_info.type==1 && recv_data_info.status==1)          //type =1  maens ack/nack type   statu=1 means s/w available
          //{
			  printf("\n software available\n");
			   bytes_recieved=read(sockfd,recv_data,100);    //recieving executed output
				recv_data[bytes_recieved] = '\0';
				 recv_data_info=parse1(recv_data);   //return parsed structure
				 
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
		  //}
}
void response_pend_req(struct request_data* request)
{
	int port=request->source_server_port;
	int sockfd;
	struct sockaddr_in server_add;
	struct hostent *server;
	char* own_ip;    //server own ip address
	int  own_port;     //server own port
	//to be sent to the sender
	char* send_data;
	struct request_data req_data;
	
	send_data=malloc(500);
	own_ip=malloc(20);
	req_data.software_id=request->software_id;
	req_data.request_id=request->request_id;
	req_data.type=2;
	req_data.req_code=2;
	req_data.ttl=request->ttl;
	memcpy(req_data.argument,request->argument,strlen(request->argument));
	req_data.argument[strlen(request->argument)]='\0';
	//req_data.source_address=own_ip;
	
	printf("\nsending secondary response to primary client\n");
	memcpy(req_data.source_address,gip,strlen(gip));
				req_data.source_address[strlen(gip)]='\0';
				req_data.source_server_port=gport; 
	create_send_packet_second(send_data,req_data);
	
	printf("\nsecondary response send packet is:%s\n",send_data);
	
	server= gethostbyname(request->source_address);
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("\nconnection error in socket creation %d",sockfd);
		return ;
	}
	server_add.sin_family=AF_INET;
	server_add.sin_port=htons(request->source_server_port);
	server_add.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(server_add.sin_zero),8); 

	//printf("\n check\n");
                
      if(connect(sockfd,(struct sockaddr*)&server_add,sizeof(struct sockaddr))<0)
      {
		  printf("\n connect error\n");
		  return ;
	  }
	  if(write(sockfd,send_data,strlen(send_data))<0)	
	  {
		  perror("\n send error\n");
		  return;
		  //exit(1);
	  }
}

void set_response(struct response_data* response,int req_id,int type,int status,char* output,char* address)
{
	response->type=type;
	response->req_id=req_id;
	response->status=status;
	response->output=output;
	response->address=address;
}
void create_send_packet(struct response_data* response, char* buffer1)
{
	
	char* buff=":";
	char buffer[10];

	sprintf(buffer,"%d",response->req_id);
	memcpy(buffer1,buffer,sizeof(buffer));
	//buffer1[sizeof(buffer)+1]=':';
	buffer1[sizeof(buffer)]='\0';
	strcat(buffer1,buff);
	
	sprintf(buffer,"%d",response->type);
	strcat(buffer1,buffer);
	strcat(buffer1,buff);
	
	sprintf(buffer,"%d",response->status);
	strcat(buffer1,buffer);
	strcat(buffer1,buff);
	
	strcat(buffer1,response->output);
	strcat(buffer1,buff);
	
	strcat(buffer1,response->address);
	strcat(buffer1,buff);
	

	
}

void initialise_list()
{
	
	int i=0,j=0,n=0;
	struct software_info* soft;
	char ch;
	char output[20];
	FILE* fp;
	if((fp=fopen(SOFT_LIST,"r"))==NULL)
	{
		printf("\nSoftware List not found\n");
		exit(1);
	}
	for(i=0;i<10;i++)
	{
		soft=malloc(sizeof(struct software_info));
		soft->software_id=0;
		memcpy(soft->process_name,"0",10);
		soft_list[i]=soft;
	}
	i=0;
	while(i<10)
	{
		soft=soft_list[i++];
		while((ch=fgetc(fp))!='\n'){
               //ch = fgetc(fp); 
              
               if ( ch == EOF )
                      return ;
               else if(ch==' ')
               {
				   
				   n++;
				   output[j]='\0';
				   if(n==1){
					soft->software_id=atoi(output);
				}
					
					else if(n==2)
					{
						memcpy(soft->process_name,output,strlen(output));
						soft->process_name[strlen(output)]='\0';
					}
					
				   j=0;
			   }
			   
			   else{
				   
                    output[j++]=ch;
                    
				}
			}output[j]='\0';
			memcpy(soft->process_info,output,strlen(output));
						soft->process_info[strlen(output)]='\0';
			n=0;
			j=0;
			//soft_list[i++]=soft; 
			
			printf("\n softid: %d\n soft_name:%s",soft->software_id,soft->process_name);
			
		}
	
	/*soft_list[0]->software_id=42;
	soft_list[0]->process_name="sum";
	soft_list[1]->software_id=43;
	soft_list[1]->process_name="sub";*/
	
	
	
}

int check_availability(int software_id)
{
	int i=0;
	printf("\n inside availability\n");
	for(i=0;i<10;i++)
	{
		if(soft_list[i]->software_id==software_id)
		{
			return 1;
		}
	}
	
	return 0;
}
void process_request(int software_id,char* arg,char* output)
{
	int i=0,j=0;
	char process[30];
	char command[30];
	char* buffer=" >out_file";
	char* buff="";
	char ch;
	//char output[100];
	FILE* fp;
	 printf(" \n check softid %d arg %s\n",software_id,arg);
	for(i=0;i<10;i++)
	{
		printf("\n check 9 \n\n");
		
		if(soft_list[i]->software_id==software_id)
		{
			//printf("\n check10\n\n");
			memcpy(process,soft_list[i]->process_name,strlen(soft_list[i]->process_name));
			process[strlen(soft_list[i]->process_name)]=' ';
			process[strlen(soft_list[i]->process_name)+1]='\0';
			//printf("\n check11\n\n");
			//arg[strlen(arg)]=' ';
			arg[strlen(arg)]='\0';
			//printf("\n check12\n\n");
			strcat(process,arg);
			//printf("\n check13\n\n");
			memcpy(command,buff,strlen(buff));
			command[strlen(buff)]='\0';
			
			strcat(command,process);
			//buffer=" >out_file";
			
			
			//printf("\n check14\n\n");
			strcat(command,buffer);
			printf("\ncommand: %s\n",command);
			system(command);
			if((fp=fopen("out_file","r"))==NULL)
			{
				perror("\n file not opened:");
			}
			
			while ( 1 )
			{
               ch = fgetc (fp); 
               if ( ch == EOF )
                      break ;
               else
                    output[j++]=ch;
			}
			output[j]='\0';
			printf("\n output is: %s\n",output);
			//return output;
		}
	}
	
}
struct request_data parse(char* raw_data)
{
	struct request_data packet;
	int i=0,j=0,n=0;
	//char c;
	//char code[4];
	char data[100];
	
	
	// printf("\n \n inside parse\n\n");
	
	while(raw_data[i]!='\0')
	{
	
		data[j++]=raw_data[i++];
		if(raw_data[i]==':')
		{
			i++;
			data[j]='\0';
			//printf("\n parsed data:%s\n",data);
			
			n++;
			if(n==1)
			packet.req_code=atoi(data);
			else if(n==2)
			packet.type=atoi(data);
			else if(n==3)
				packet.request_id=atoi(data);
			else if(n==4)
				packet.software_id=atoi(data);
			else if(n==5){
				memcpy(packet.argument,data,strlen(data));
				packet.argument[strlen(data)]='\0';}
			else if(n==6)
				packet.ttl=atoi(data);
			else if(n==7){
				memcpy(packet.source_address,data,strlen(data));
				packet.source_address[strlen(data)]='\0';}
			else if(n==8)
				packet.source_server_port=atoi(data);
				
			
		j=0;
		}
	}
	printf("\nttl:%d",packet.ttl);
	printf("\naddress:%s",packet.source_address);
	printf("\nport:%d",packet.source_server_port);
	printf("\nsoftwareid:%d",packet.software_id);
	printf("\nargument:%s",packet.argument);
	printf("\n req_code: %d \n tupe : %d argument:%s\n",packet.req_code,packet.type,packet.argument);
	return packet;
	
	
}
int request_reciever(struct queue* pending_queue,int port)
{
	int sockfd,clientfd;
	int size,recv_size;
	int pid;
	int temp;
	struct sockaddr_in server_add,client_add;
	char recmsg[1024];
	char* msg;
	char buff1[5000];
	char output[100];
	char buff[20];
	char* buf=" ";
	char send_packet[500];
	struct request_data parsed_req_data;
	struct response_data* response;
	int new_pid=0,new_pid2=0;
	int flag;
	int npid=0;
	char* arg1;
	char* arg2;
	char* source_file="source_file";
	
	
	arg1=malloc(10);
	arg2=malloc(500);
	
	
	msg=malloc(5000);
	response=malloc(sizeof(struct response_data));
	memcpy(&server_add,"0",sizeof(server_add)); 
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("\nsocket error\n");
		exit(1);
	}
	server_add.sin_family = AF_INET;  
	server_add.sin_port=htons(port);
	server_add.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(sockfd,(struct sockaddr*)&server_add,sizeof(server_add))<0)
	{
		printf("\nbind error\n");
		exit(1);
	}
	 if(listen(sockfd,5)<0)
	{
		printf("listen error");
		exit(1);
	}
	//printf("\n inside reciever\n");
	while(1)
	{
		//printf("\n inside loop\n");
		size=sizeof(client_add);
		if((clientfd=accept(sockfd,(struct sockaddr*)&client_add,&size))<0)
		{
			perror("\n accept error\n");
		} 
		//printf("\n accepting\n");
		if((pid=fork())<0)
		{
			perror("\n fork error");
		}
		if(pid!=0)
		{
			//printf("\n inside child process\n");
			close(sockfd);
			if((recv_size=read(clientfd,recmsg,1024))<0)
			{
				perror("\n recv error\n");
			}	
			recmsg[recv_size]='\0';
			printf("%s",recmsg);
			parsed_req_data=parse(recmsg);
			printf("\n%d \n%d \n%d\n",parsed_req_data.type,parsed_req_data.request_id,parsed_req_data.software_id);
			
			if(parsed_req_data.req_code==1)       //SOFFTWARE REQUEST FROM THE CLIENT
			{
				printf("\n check1\n");
				
				if(check_availability(parsed_req_data.software_id)==1)   //available
				{
					printf("\n check2\n");
					
					//memcpy(send_packet,buff,strlen(buff));
					//send_packet[strlen(buff)]='\0';
					set_response(response,parsed_req_data.request_id,1,1,"0","0");  //sending ack for availability
					create_send_packet(response,send_packet);
					if(write(clientfd,send_packet,strlen(send_packet))<0)
					{
						perror("\n send eror\n");
					}		
					
					parse_argument__(arg1,arg2,parsed_req_data.argument);
					
					//printf("\n check3\n");
					if(arg1<0)
					{
						write_into_file(arg2,source_file);
						process_request(parsed_req_data.software_id,source_file,output);
					}
					else
					{
					process_request(parsed_req_data.software_id,parsed_req_data.argument,output);
					}
					//printf("\n check5\n");
						
					
						//memcpy(send_packet,buff,strlen(buff));
						//send_packet[strlen(buff)]='\0';
					
					if(parsed_req_data.type==1)
					{
						set_response(response,parsed_req_data.request_id,2,5,output,"0");
						create_send_packet(response,send_packet);
						printf("\noutput is: %s",output);
						//strcat(send_packet,output);
						printf("\n packet send: %s",send_packet);
						
						if(write(clientfd,send_packet,strlen(send_packet))<0)
						{
							perror("\n send eror\n");
						}
						exit(1);
					}
					else if(parsed_req_data.type==2)
					{
						
						set_response(response,parsed_req_data.request_id,3,5,"0","0");
						create_send_packet(response,send_packet);
						printf("\noutput is: %s",output);
						//strcat(send_packet,output);
						printf("\n packet send: %s",send_packet);
						
						if(write(clientfd,send_packet,strlen(send_packet))<0)
						{
							perror("\n send eror\n");
						}
						//creating connection on source address and writing output
						
							
						
							response_pend_req(&parsed_req_data);
							exit(1);
							
					}
				}
				else
				{	parsed_req_data.ttl=parsed_req_data.ttl -1;
					//insert_into_record();
					
						printf("\n requested software not available\n");
						//msg="2:";
						//memcpy(output,"0",100);
						set_response(response,parsed_req_data.request_id,1,2,"0","0");
						create_send_packet(response,send_packet);
					
						//printf("\n\n hello 1\n\n");
						if(write(clientfd,send_packet,strlen(send_packet))<0)
						{
						
							perror("\n send eror\n");
						}
						
					if(parsed_req_data.ttl!=0)
					{
						//printf("\n\n hello 2\n\n");
						sprintf(buff,"%d",parsed_req_data.software_id);
						memcpy(msg,buff,strlen(buff));
						msg[strlen(buff)]='\0';
						//printf("\n\n hello 4\n\nmsg:%s",msg);
						strcat(msg,buf);
						strcat(msg,parsed_req_data.argument);
						strcat(msg,buf);
						//printf("\n\n hello 5\n\nmsg:%s",msg);
						sprintf(buff,"%d",parsed_req_data.ttl);
						strcat(msg,buff);
						strcat(msg,buf);
						//printf("\n\n hello 6\n\nmsg:%s",msg);
						strcat(msg,parsed_req_data.source_address);
						strcat(msg,buf);
						sprintf(buff,"%d",parsed_req_data.source_server_port);
						strcat(msg,buff);
						strcat (msg,buf);
						
						buff[0]='2';   //set type =2   :secondary request
						buff[1]='\0';
						strcat(msg,buff);
						//printf("\n\n hello 3\n\n");
					
						printf("\n\nThe sender argument is:%s",msg);
						memcpy(buff1,"./sender",sizeof("./sender"));
						buff1[sizeof("./sender")]='\0';
						strcat(buff1,buf);
						strcat(buff1,msg);
						free(msg);
						
						
						
						system(buff1);
						printf("\n\nThe sender argument is:%s",buff1);
						exit(1);
					
					}
						
					
				}
			}
		
			else if(parsed_req_data.req_code==2)   //recieved from an availble reciever in secondary stage
			{
				
				//read output from the client
				
				printf("\nreceiving response from secondary server only address\n");
				
				
				
					//flag=1;
					read_output(parsed_req_data);  //reading pending output
					exit(1);
				
				
			}
			
			//sleep(1);
			close(clientfd);
			exit(1);
		}
		close(clientfd);
	}
	//close(clientfd);
	close(sockfd);
	
		   
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
struct queue_node get_node_input__()
{
	struct queue_node* qnode;
	struct request_data r_data;
	qnode=malloc(sizeof(struct queue_node));
	memcpy(qnode,"0",sizeof(struct queue_node));
	printf("\n enter request _id\n");
	scanf("%d",&r_data.request_id);
	printf("\n enter software id\n");
	scanf("%d",&r_data.software_id);
	printf("\nenter argument\n");
	scanf("%s",r_data.argument);
	printf("enter ttl");
	scanf("%d",&qnode->ttl);
	qnode->req_info=r_data;
	qnode->req_id=r_data.request_id;
	//free(&r_data);
	return *qnode;
}
void display_queue__(struct queue* pending_queue)
{
	
	int temp1=0;
	struct queue_node* previous;
	
	
	printf("\n list\n");
	
	//previous=&list->node_index[1];
	//printf("\n list %d",previous->cost);

	if(pending_queue->head==NULL)
	{
		printf("\n queue empty\n");
		return;
	}
	
	printf("\n check5\n");
			previous=pending_queue->head;
			printf("\n head info: %d\n",previous->ttl);
			do
			{
				printf("\n request id: %d \nttl: %d \n",previous->req_id,previous->ttl);
				previous=previous->next;
				
			}while(previous!=pending_queue->head);
	
}
	
int main(int argc,char* argv[])
{
	struct queue_node qnode;
	struct queue pending_queue;
	int port;
	
	initialise_list();
	/*Checking Queue*/

/*	pending_queue.head=NULL;
	pending_queue.tail=NULL;
	int i=0;
	for(i=0;i<3;i++)
	{
		qnode=get_node_input__();
		printf("\ncheck 1\n");
		push_queue(&pending_queue,qnode);
		printf("\ncheck2\n");
		//display_queue__(&pending_queue);
		printf("\n check3 \n");
	}
	display_queue__(&pending_queue);
	pop_queue(&pending_queue,&qnode,0);
		display_queue__(&pending_queue);
*/
		
		/*main reciever code*/
	port=atoi(argv[1]);
	gport=port;
	if(request_reciever(&pending_queue,port)<0)
	{
		perror("\nrequest_reciever error\n");
		exit(1);
	}	
	
	return 0;
}
