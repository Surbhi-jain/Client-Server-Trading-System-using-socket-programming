#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
static char id_trader[100];
static char pass[100];
static struct sockaddr_in S;
static int trader_number;

//Putting all data in char * and sending to server and receiving the response from server
void send_request(const char * request, char * response)
{
	int s=socket(AF_INET,SOCK_STREAM,0);

	if(s==-1)
	{
		printf("Socket not created\n");
		exit(-1);
	}

	int t;
	t= connect(s,(struct sockaddr*) &S, sizeof(S));
	if(t<0)
	{
		printf("Connection to server could not be established\n");
		exit(-1);
	}

	t= send(s,request,strlen(request),0);
	if(t<0)
	{
		printf("Request could not be sent\n");
		printf("Please send again\n");
		exit(-1);
	}

	char * buffer[1024];
	int offset=0;
	while(1)
	{
		int tmp=recv(s,buffer,1024,0);
		if(tmp>0)
		{
			memcpy(response+offset,buffer,tmp);
			offset+=tmp;
		}
		else
		{
			break;
		}
	}

	response[offset]='\0';

}

//sending username and password to server to authenticate the user
int check_login (char * response)
{
	char request[65536];
	memset(request,'\0',sizeof(request));

	strcat(request,id_trader); strcat(request," ");
	strcat(request,pass); strcat(request," ");
	strcat(request,"L");
	strcat(request," #$@");
	send_request(request,response);
	//if user exist then server will send ACCEPTED message
	char temp[] = "ACCEPTED";
	int flag = 1;
	for(int i = 0 ; i < 8 ; i++){
		if(temp[i] != response[i]){
			flag = 0;
			break;
		}
	}
	return flag?1:0;

}

//Sending the buying request to server with item_number , quantity and unitprice. and getting response from server
int buy(int item_number, int quantity, int unit_price)
{
	char request[65536];
	char response[65536];
	memset(response,'\0',sizeof(response));
	memset(request,'\0',sizeof(request));
	char tmp[32];
	sprintf(tmp,"%d",trader_number);
	strcat(request,tmp); strcat(request," ");
	strcat(request," aaa B ");
	sprintf(tmp,"%d",item_number);
	strcat(request,tmp); strcat(request," ");
	sprintf(tmp,"%d",quantity);
	strcat(request,tmp); strcat(request," ");
	sprintf(tmp,"%d",unit_price);
	strcat(request,tmp); strcat(request," #$@");

	send_request(request,response);
	//If Successfully requested then server will send ACCEPTED
	char temp[] = "ACCEPTED";
	int flag = 1;
	for(int i = 0 ; i < 8 ; i++){
		if(temp[i] != response[i]){
			flag = 0;
			break;
		}
	}
	return flag?1:0;
}

//Sending the selling request to server with item_number , quantity and unitprice. and getting response from server
int sell(int item_number, int quantity, int unit_price)
{
	char request[65536];
	char response[65536];
	memset(response,'\0',sizeof(response));
	memset(request,'\0',sizeof(request));

	char tmp[32];
	sprintf(tmp,"%d",trader_number);
	strcat(request,tmp);
	strcat(request," aaa S ");
	sprintf(tmp,"%d",item_number);
	strcat(request,tmp); strcat(request," ");
	sprintf(tmp,"%d",quantity);
	strcat(request,tmp); strcat(request," ");
	sprintf(tmp,"%d",unit_price);
	strcat(request,tmp); strcat(request," #$@");

	send_request(request,response);
	//If Successfully requested then server will send ACCEPTED
	char temp[] = "ACCEPTED";
	int flag = 1;
	for(int i = 0 ; i < 8 ; i++){
		if(temp[i] != response[i]){
			flag = 0;
			break;
		}
	}
	return flag?1:0;
}

int main(int argc , char const *argv[])
{

	if(argc < 3){
		printf("Please compile the code in below format : \n");
		printf("========./File_name server_IPAddress server_portnumber ===========\n \n");
		exit(1);
	}
	struct hostent *server;
	server = gethostbyname(argv[1]);
  if(server == NULL){
    fprintf(stderr, "Error, no such host");
  }
	S.sin_addr.s_addr = inet_addr(argv[1]);
	S.sin_family = AF_INET;
	S.sin_port = htons(atoi(argv[2]));

	printf("\n \t===================Trading System designed by Ritik(214101065) Surbhi(214101066) Siddharth(216201004)======================= \n \n");
	int opt, log;
	int I; int Q; int P;
	do{
			printf("\nPlease Select below options : (1 or 2)\n");
			printf("1: Login to the System\n");
			printf("2: Exit\n");
			scanf("%d",&log);
			if(log == 1)
			{
				 		int login = 1;
						char response[65536];
						memset(response,'\0',sizeof(response));
						printf("Enter your id:\n");
						scanf("%s",id_trader);
						printf("Please enter your password \n");
						scanf("%s",pass);
						char * check;
						//authenticate the user 
						if(check_login(response))
						{
							login = 1;
							check=response;
							while(*check != '\n')
							{
								check=check+1;
							}
							check++;
							printf("%s","======Login Successfully========");
							trader_number = check[20]-'0';

						}
						else
						{
							check=response;
							while(*check != '\n')
							{
								check=check+1;
							}
							check++;
							printf("Either Username or Password is incorrect.");
							printf("%s",check);
							login = 0;
						}

					if(login == 1){
								do
								{
									printf("\nPlease Select below options : \n");
									printf("1: Send Buy Request\n");
									printf("2: Send Sell Request\n");
									printf("3: View Order Status\n");
									printf("4: View Trade Status\n");

									printf("\nPlease Select any number(except 1 to 4) to exit from the menu\n");

									scanf("%d",&opt);
									//Sending buy request
									if(opt == 1){
											printf("\t \t ======================Buy the item=================\n");
											printf("\n=======NOTE : There are ten items which you can trade in with their codes from 0 to 9=======\n\n");
											printf("Enter the item number\n");
											scanf("%d",&I);
											if(I > 10){
												printf("Please Select Item code from 1 to 10 only\n");
											}
											else{
													printf("Enter quantity\n");
													scanf("%d",&Q);
													printf("Enter the unit Price\n");
													scanf("%d",&P);
													//Sending request to server
													if(buy(I,Q,P)){
														printf("\n \t Buy Request has been sent Successfully\n");
													}

											}
									}
									//Sending sell request
									else if(opt == 2){
											printf("\t \t ======================Sell the item=================\n");
										  printf("\n=======NOTE : There are ten items which you can trade in with their codes from 0 to 9=======\n\n");
											printf("Enter the item number\n");
											scanf("%d",&I);
											if(I > 10){
												printf("Please Select Item code from 1 to 10 only\n");
											}
											else{
												printf("Enter quantity\n");
												scanf("%d",&Q);
												printf("Enter the unit price\n");
												scanf("%d",&P);
												//Sending request to server
												if(sell(I,Q,P)){
													printf("\n \t Sell Request has been sent Successfully\n");
												}
											}
									}
									//Sending view order request
									else if(opt == 3){
											char request[65536]; //to sent the request to server
											char response[65536]; //to get the response from server
											memset(request,'\0',sizeof(request));
											char tmp[32];
											sprintf(tmp,"%d",trader_number);
											strcat(request,tmp); strcat(request," ");
											strcat(request,pass); strcat(request," ");
											strcat(request,"VO #$@");
											//Sending request to server
											send_request(request,response);
											//Checking request Successfully received or not
											char temp[] = "ACCEPTED";
											int flag = 1;
											for(int i = 0 ; i < 8 ; i++){
												if(temp[i] != response[i]){
													flag = 0;
													break;
												}
											}
											if(flag == 0)
											{
												printf("Request Failed\nTry again\n");
											}
											//Printing response
											else{
												char * check;
												check=response;
												while(*check != '\n')
												{
													check=check+1;
												}
												check++;
												printf("%s",check);
												printf("\n \n");
										 }
									}
									//Sending view trades request
									else if(opt == 4){
											char request[65536];  //to sent the request to server
											char response[65536]; //to get the response from server
											memset(request,'\0',sizeof(request));
											char tmp[32];
											sprintf(tmp,"%d",trader_number);
											strcat(request,tmp); strcat(request," ");
											strcat(request,pass); strcat(request," ");
											strcat(request,"VT #$@");
											//Sending request to server
											send_request(request,response);
											//Checking request Successfully received or not
											char temp[] = "ACCEPTED";
											int flag = 1;
											for(int i = 0 ; i < 8 ; i++){
												if(temp[i] != response[i]){
													flag = 0;
													break;
												}
											}
											if(flag == 0)
											{
												printf("Request Failed\nTry again\n");
											}
											//Printing response
											else{
												char * check;
												check=response;
												while(*check != '\n')
												{
													check=check+1;
												}
												check++;
												printf("%s",check);
										 	}
									}
								}while(opt <= 4);
					}

			}
			else{
				exit(1);
			}
	}while(log >= 1);

	return 0;
}
