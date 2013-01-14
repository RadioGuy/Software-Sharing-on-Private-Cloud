#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define C_FILE "cloud_configuration"

struct configuration
{
	char port[10];
	char ip[20];
	char type[3];
	char ttl[3];
};
void get_configuration(struct configuration*);
void get_command_rec(struct configuration*,char*);
void get_command_send(struct configuration*,char*,char*,char*,char*);

void get_configuration(struct configuration* conf)
{
	FILE* fp;
	char ch;
	int i=0,j=0;
	char buff[100];
	char cuff[100];
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
				//printf("\n %s\n",buff);
				
			j=0;
			while((ch=fgetc(fp))!='\n')
			{
				cuff[j++]=ch;
				}	
			cuff[j]='\0';
			//printf("\n%s\n",cuff);
			if(strncmp(buff,"reciever_port",sizeof("reciever_port"))==0)
			{
				memcpy(conf->port,cuff,j);
				conf->port[j]='\0';
			}
			else if(strncmp(buff,"reciever_ip",sizeof("reciever_ip"))==0)
			{
				memcpy(conf->ip,cuff,j);
				conf->ip[j]='\0';
			}
			else if(strncmp(buff,"ttl",sizeof("ttl"))==0)
			{
				memcpy(conf->ttl,cuff,j);
				conf->ttl[j]='\0';
			}
			else if(strncmp(buff,"type",sizeof("type"))==0)
			{
				memcpy(conf->type,cuff,j);
				conf->type[j]='\0';
			}
		}
		
	}
	printf("\n reciever port:%s",conf->port);
	printf("\n reciever ip:%s",conf->ip);
	printf("\n reciever type:%s",conf->type);
	printf("\n reciever ttl:%s\n",conf->ttl);
}
void get_command_rec(struct configuration* conf,char* cmd)
{
	
	memcpy(cmd,"./reciever ",sizeof("./reciever "));
	cmd[sizeof("./reciever ")]='\0';
	strcat(cmd,conf->port);
	printf("\n%s\n",cmd);
}
void get_command_send(struct configuration* conf,char* cmd,char* soft, char* arg1,char* arg2)
{
	memcpy(cmd,"./sender ",sizeof("./sender "));
	cmd[sizeof("./sender ")]='\0';
	strcat(cmd,soft);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,arg1);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,arg2);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,conf->ttl);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,conf->ip);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,conf->port);
	strcat(cmd," ");
	printf("\n%s\n",cmd);
	strcat(cmd,conf->type);
	printf("\n%s\n",cmd);
}
int main(int argc,char* argv[])
{
	
	struct configuration* conf;
	char* cmd_rec,*cmd_send;
	
	conf=malloc(sizeof(struct configuration));
	cmd_rec=malloc(100);
	cmd_send=malloc(100);
	get_configuration(conf);
	
	if(strncmp(argv[1],"start",sizeof("start"))==0)
	{
		printf("\n start detected\n");
		get_command_rec(conf,cmd_rec);
		if(!system(cmd_rec))
			exit(1);
		//if(!system("firefox output.html"))
			//exit(1);	
	}
	else if(strncmp(argv[1],"send",sizeof("send"))==0)
	{
		get_command_send(conf,cmd_send,argv[2],argv[3],argv[4]);
		if(!system(cmd_send))
			exit(1);
		
		
	}
	
	
	
}
