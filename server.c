#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<unistd.h>
#include"server.h"
char message_to_send[1000];
void maxheapify(struct struct_buy_sell_request arr[] , int n , int i) {
  //helper fucntion used in max heap sort
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  struct struct_buy_sell_request swap ;

  if (l < n && arr[l].price < arr[largest].price)
    largest = l;
  if (r < n && arr[r].price < arr[largest].price)
    largest = r;
  if (largest != i)
  {
    swap = arr[i];
    arr[i] = arr[largest];
    arr[largest] = swap;

    maxheapify(arr, n, largest);
  }
}
void maxheapsort(int item , int start , int end){
  //max heap sort used for sorting buy queue

  int length = end - start;
  struct struct_buy_sell_request buy[length];
  struct struct_buy_sell_request swap ;
  for(int i = 0 ; i < length ; i++)
  buy[i] = buy_req_queue[start+i][item];
  for (int i = length / 2 - 1; i >= 0; i--)
    maxheapify(buy, length, i);
  for (int i = length - 1; i > 0; i--) {

    swap = buy[0];
    buy[0] = buy[i];
    buy[i] = swap;

    maxheapify(buy, i, 0);
  }
  for(int i = 0 ; i < length ; i++)
    buy_req_queue[start+i][item] = buy[i];
}


void minheapify(struct struct_buy_sell_request arr[] , int n , int i) {
  //helper function used in min heap sort
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  struct struct_buy_sell_request swap ;

  if (l < n && arr[l].price > arr[largest].price)
    largest = l;
  if (r < n && arr[r].price > arr[largest].price)
    largest = r;
  if (largest != i)
  {
    swap = arr[i];
    arr[i] = arr[largest];
    arr[largest] = swap;

    minheapify(arr, n, largest);
  }
}
void minheapsort(int item , int start , int end){
  //used to sort sell queue`
  int length = end - start;
  struct struct_buy_sell_request buy[length];
  struct struct_buy_sell_request swap ;
  for(int i = 0 ; i < length ; i++)
    buy[i] = sell_req_queue[start+i][item];

  for (int i = length / 2 - 1; i >= 0; i--)
    minheapify(buy, length, i);
  for (int i = length - 1; i > 0; i--) {

    swap = buy[0];
    buy[0] = buy[i];
    buy[i] = swap;

    minheapify(buy, i, 0);
  }
  for(int i = 0 ; i < length ; i++){

        sell_req_queue[start+i][item] = buy[i];
        printf("printing: %d ",buy[i].price);
  }
  printf("\n");

}


int same_string(const char *a, const char *b) {
  //checks if two strings are same
    while (*a == *b) {
        if (*a == '\0')
            return 1;
        a++;
        b++;
    }
    return 0;
}




char **get_message(char *input_message){
  //helper function to extract incoming messages

  int num_words=0;
  int buffer_size=strlen(input_message);
  int i=0;

  while(i<buffer_size){
    char check=input_message[i++];
    if(isspace(check)) num_words=num_words+1;
  }

  char **return_message=(char **)malloc(num_words*sizeof(char *));

  int word_num=0;

  char *token=strtok(input_message," ");

  while(1){
    if(!token)break;
    char *word=token;
    return_message[word_num]=(char *)malloc(strlen(word)*sizeof(char));
    strcpy(return_message[word_num],token);
    token=strtok(NULL," ");
    word_num=word_num+1;
  }
  return return_message;
}

void sendmessage(int sock,char *texttosend){
  //Utility function to send message
  printf("%s \n",texttosend);
  if(write(sock,texttosend,strlen(texttosend))<0){
    printf("Error seding message\n");
  }

}

struct struct_authorization login_credential_check(char *uname,char *ps){
  //helper function to authorize username and password and return the message  during login
  printf("Starting authorization\n");
  FILE *fptr=fopen("credentials.txt","r");
  if(!fptr){
    printf("Error in opening the Credentials File\n");
  }
  char read_line[200];
  struct struct_authorization ret;
  while(fgets(read_line,sizeof(read_line),fptr)!=NULL){

    char pres_line[200];
    strcpy(pres_line,read_line);
    pres_line[strlen(pres_line)-1]='\0';
    //printf("%s\n",pres_line);
    char * pres_id=strtok(pres_line,":");
    char * pres_uname=strtok(NULL,":");
    char * pres_password=strtok(NULL,"\0");
    //printf("%s %s %s \n",pres_id,pres_uname,pres_password);
    if(same_string(pres_uname,uname)){
      strcpy(ret.user_name,pres_uname);
      ret.number=atoi(pres_id);
      if(same_string(pres_password,ps)){
          sprintf(message_to_send,"ACCEPTED! Successfully logged in!!\nYour USER NUMBER IS %s",pres_id);
      }
      else{
        sprintf(message_to_send,"Password Incorrect\nPlease try again\n\n");
      }
      return ret;
    }
  }
  printf(message_to_send,"Wrong Credentials Please try again\n\n");


  return ret;
}

struct struct_authorization req_credential_check(char *unum,char *ps){
  //helper fucntion to authenticate during requests
   printf("Starting authorization\n");
  FILE *fptr=fopen("credentials.txt","r");
  if(!fptr){
    printf("Error in opening the Credentials File\n");
  }
  char read_line[200];
  struct struct_authorization ret;
  while(fgets(read_line,sizeof(read_line),fptr)!=NULL){

    char pres_line[200];
    strcpy(pres_line,read_line);
    pres_line[strlen(pres_line)-1]='\0';
    printf("%s\n",pres_line);
    char * pres_id=strtok(pres_line,":");
    char * pres_uname=strtok(NULL,":");
    char * pres_password=strtok(NULL,"\0");
    printf("%s %s %s \n",pres_id,pres_uname,pres_password);
    if(same_string(pres_id,unum)){
      strcpy(ret.user_name,pres_uname);
      ret.number=atoi(pres_id);
      if(same_string(pres_password,ps)){
          ret.result=1;
      }
      else{
        ret.result=1;
      }
      return ret;
    }
  }
  ret.result=3;
  return ret;
}
void insertBuyInQueue(struct struct_buy_sell_request req){
  //helper function to insert  new request in buy queue
      int ind_item=req.itemnumber;
      int t=buy_queue_head_tail[ind_item][1];
      if(t>=1000){
        printf("Queue has reached 1000. Please restart the program \n");
      }
      else{
        buy_req_queue[t++][ind_item]=req;
        buy_queue_head_tail[ind_item][1]=t;
        maxheapsort(ind_item , buy_queue_head_tail[ind_item][0] , buy_queue_head_tail[ind_item][1]);

      }
      printf("head %d tail %d itemno: %d",buy_queue_head_tail[ind_item][0],buy_queue_head_tail[ind_item][1],ind_item);

  }
void insertSellInQueue(struct struct_buy_sell_request req){
  // helper function to insert new request in sell queue
  int ind_item=req.itemnumber;
  int t=sell_queue_head_tail[ind_item][1];
  if(t>=1000){
    printf("Queue has reached 1000. Please restart the program \n");
  }
  else{
    sell_req_queue[t++][ind_item]=req;
    sell_queue_head_tail[ind_item][1]=t;
    minheapsort(ind_item , sell_queue_head_tail[ind_item][0] , sell_queue_head_tail[ind_item][1]);


  }

   printf("head %d tail %d itemno: %d",sell_queue_head_tail[ind_item][0],sell_queue_head_tail[ind_item][1],ind_item);
}
int is_buy_queue_empty(int item_num){
  //helper function to check if buy queue is empty
  return buy_queue_head_tail[item_num][0]==buy_queue_head_tail[item_num][1];
}
int is_sell_queue_empty(int item_num){
  //helper function to check if sell queue is empty
  return sell_queue_head_tail[item_num][0]==sell_queue_head_tail[item_num][1];
}







int main(int argc,char const *argv[]){

  if(argc<2 || argc>2 || (atoi(argv[1])<1024)){
    printf("Please give a valid PORT NUMBER\nWhen Executing, Follow the format : \n\n./a.out <PORT NUMBER>\n\nPort Number should be a number between 1024 and 65535\n");
    return 0;
  }

  int servfd;
  if((servfd=socket(AF_INET,SOCK_STREAM,0))<0){
    printf("Socket Creation Failed\n");
    return 0;
  }

  struct sockaddr_in servaddr;
  bzero((char *)&servaddr,sizeof(servaddr));
  int servportnumber=atoi(argv[1]);
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  servaddr.sin_port=htons(servportnumber);


  if(bind(servfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
      printf("Socket Binding Failed\n");
      return 0;
    }

    if(listen(servfd,5)<0){
      printf("Socket Listening Failed\n");
      return 0;
    }

    int newsockfd;
    struct sockaddr_in cliaddr;
    int clientaddrsize=sizeof(cliaddr);

    char primarybuffer[65536];


    while(1){

      newsockfd=accept(servfd,(struct sockaddr *)&cliaddr,(socklen_t*)&clientaddrsize);

      if(newsockfd<0){
        printf("Client Accept Failed\n");
        continue;
      }

      char secondarybuffer[512];
      int pbofpos=0;

      while(1){
        //server will keep running
        memset(secondarybuffer,'\0',sizeof(secondarybuffer));

        int sizeread=read(newsockfd,secondarybuffer,sizeof(secondarybuffer));
        if(sizeread<0){
          printf("Read from Client Failed\n");
          exit(-1);
        }

        memcpy(primarybuffer+pbofpos,secondarybuffer,sizeread);
        pbofpos=pbofpos+sizeread;

        char *delimiter="#$@";
        int isended=0;

        int pos=0;
        int lendelim=strlen(delimiter);
        int length=sizeof(secondarybuffer);
        for(pos=0;pos<length-lendelim;pos++){
            if(memcmp(secondarybuffer+pos,delimiter,lendelim)==0)  {isended=1; break;}
        }
        if(isended==1) break;

      }


      primarybuffer[pbofpos]='\0';

      printf("The new message received at the Server is - \n\n%s\n\n",primarybuffer);
      char **req=get_message(primarybuffer);
      char *option=req[2];
      if(sizeof(option)!=8){
        //if size is not 8bytes the return invalid option
        printf("size is %ld\n",sizeof(option));
        char message[]=" INVALID OPTION!";
        sendmessage(newsockfd,message);
      }
      else if(!same_string(option,"L") && !same_string(option,"S") && !same_string(option,"B") && !same_string(option,"VO") && !same_string(option,"VT")){
        //if option is not in L,B,S,VO,VT then return invalid option
        char message[]="INVALID OPTION!";
        sendmessage(newsockfd,message);
      }
      else if(same_string(option,"L")){
        //if user wants to login then check credentials and accept
        char *uname=req[0];
        char *ps=req[1];
        struct struct_authorization auth_result=login_credential_check(uname,ps);
        printf("\n%s\n",message_to_send);
        sendmessage(newsockfd,message_to_send);
      }

      else if (same_string(option,"S")){
        //if user wants to sell


          char *uid=req[0];
          char *pass=req[1];
          char username[50];
          struct struct_authorization authcheck=req_credential_check(uid,pass);
          int validation=authcheck.result;
          strcpy(username,authcheck.user_name);
          int userid=atoi(uid);

          if(validation!=1){
              sendmessage(newsockfd,"REJECTED \n\nLOG IN Unsuccessful\n\n");
              close(newsockfd);
              continue;
          }
          int item = atoi( req[3] );
          int qty =  atoi( req[4] );
          int unitprice = atoi( req[5] );

          struct struct_buy_sell_request sell;
          strcpy(sell.user,username);
          sell.itemnumber = item;
          sell.qty = qty;
          sell.price = unitprice;
          sell.id = userid;
          sell.type = 'S';

          int i;
          while(1){
            //while sell quantity is not 0
            int i=buy_queue_head_tail[item][0];
            if(sell.qty<=0){
              //break if everything is sold
              break;

            }
            else if(is_buy_queue_empty(item) || (buy_req_queue[i][item].price<unitprice )){
              //insert in sell queue if buy queue is empty
              insertSellInQueue(sell);
              break;
            }
            else if(buy_req_queue[i][item].price>=unitprice ){
              //if max buy price is more than sell price
              struct struct_trade_info temp;
              strcpy(temp.seller,username );
              strcpy(temp.buyer, buy_req_queue[i][item].user);
              temp.itemnumber = item;
              temp.price = buy_req_queue[i][item].price;
              temp.sellerid = sell.id;
              temp.buyerid = buy_req_queue[i][item].id;

              if(sell.qty==buy_req_queue[i][item].qty){
                //if quantity of selling is equal to buying qty
                buy_queue_head_tail[item][0]++;
                temp.qty=sell.qty;
                trade_info[tradeno++] = temp;
                sell.qty=0;
                break;
              }
              else if(sell.qty<buy_req_queue[i][item].qty){
                //if sell quantity is smaller than buy
                buy_req_queue[i][item].qty-=sell.qty;
                temp.qty=sell.qty;
                trade_info[tradeno++] = temp;
                sell.qty=0;
                break;
              }
              else{

                buy_queue_head_tail[item][0]++;
                temp.qty=buy_req_queue[i][item].qty;
                sell.qty-=buy_req_queue[i][item].qty;
                trade_info[tradeno++] = temp;
              }

          }
        }
          //send that it is accepted
          sendmessage(newsockfd,"ACCEPTED\n\n");

          }



      else if(same_string(option,"B")){
        //if user wants to buy
        char *uid=req[0];
        char *pass=req[1];
        char username[50];
        struct struct_authorization authcheck=req_credential_check(uid,pass);
        int validation=authcheck.result;
        strcpy(username,authcheck.user_name);
        int userid=atoi(uid);

        if(validation!=1){
            sendmessage(newsockfd,"REJECTED \n\nLOG IN Unsuccessful\n\n");
            close(newsockfd);
            continue;
        }
        int item = atoi( req[3] );
        int qty =  atoi( req[4] );
        int unitprice = atoi( req[5] );

        struct struct_buy_sell_request buy;
        strcpy(buy.user,username);
        buy.itemnumber = item;
        buy.qty = qty;
        buy.price = unitprice;
        buy.id = userid;
        buy.type = 'B';

        int check=0;

        while(1){
          int i=sell_queue_head_tail[item][0];
          if(buy.qty<=0){
            //if buy qty is 0 break the queue
            break;
          }
          else if(is_sell_queue_empty(item) || (sell_req_queue[i][item].price > unitprice)){
            //if sell queue empty insert the request in buy queue
            insertBuyInQueue(buy);
            break;
          }

          else if(sell_req_queue[i][item].price<=unitprice ){
            //if min sell price is less than buy price
            struct struct_trade_info temp;
            strcpy(temp.buyer,username );
            strcpy(temp.seller, sell_req_queue[i][item].user);
            temp.itemnumber = item;
            temp.price = sell_req_queue[i][item].price;
            temp.buyerid = buy.id;
            temp.sellerid = sell_req_queue[i][item].id;

            if(buy.qty==sell_req_queue[i][item].qty){
              //if sell qty is equal to buy qty
              sell_queue_head_tail[item][0]++;
              temp.qty=buy.qty;
              trade_info[tradeno++] = temp;
              break;
            }
            else if(buy.qty<sell_req_queue[i][item].qty){
              //if buy qty is less than sell
              sell_req_queue[i][item].qty-=buy.qty;
              temp.qty=buy.qty;
              trade_info[tradeno++] = temp;

              break;
            }
            else{
              sell_queue_head_tail[item][0]++;
              temp.qty=sell_req_queue[i][item].qty;
              buy.qty-=sell_req_queue[i][item].qty;
              trade_info[tradeno++] = temp;

            }

          }
        }
        sendmessage(newsockfd,"ACCEPTED\n\n");

    }

    else if(same_string(option,"VO")){
      //if option is View Orders

        int i,j;
        char temp[200];
        sprintf(message_to_send,"%s","");
        for(i=0; i<10; i++)
        {//go over all the items and check thier orders
                sprintf(temp, "\nItem: %d\n", i);
                strcat(message_to_send,temp);
                sprintf(temp, "   Sell for this item:\n ");
                strcat(message_to_send,temp);
                if( !is_sell_queue_empty(i) )
                {

                    for(int start=sell_queue_head_tail[i][0];start<sell_queue_head_tail[i][1];start++){
                      if(start==sell_queue_head_tail[i][0]){
                        sprintf(temp,"Minimum Selling price Item::quantity- %d, price- %d\n",sell_req_queue[start][i].qty,sell_req_queue[start][i].price );
                        strcat(message_to_send,temp);
                      }

                      else{
                        sprintf(temp, "       quantity- %d, price- %d\n",sell_req_queue[start][i].qty,sell_req_queue[start][i].price );
                        strcat(message_to_send,temp);
                      }
                    }
                }
                else{
                  sprintf(temp, "       No sell available for this item\n");
                  strcat(message_to_send,temp);
                }


                sprintf(temp, "   Buy for this item:\n ");
                strcat(message_to_send,temp);

                if( !is_buy_queue_empty(i) )
                {
                    for(int start=buy_queue_head_tail[i][0];start<buy_queue_head_tail[i][1];start++){
                      if(start==buy_queue_head_tail[i][0]){
                        sprintf(temp, "Maximum buying price for Item::quantity- %d price- %d\n",buy_req_queue[start][i].qty,buy_req_queue[start][i].price);
                        strcat(message_to_send,temp);
                      }
                      else{
                        sprintf(temp, "       quantity- %d price- %d\n",buy_req_queue[start][i].qty,buy_req_queue[start][i].price);
                        strcat(message_to_send,temp);
                      }

                    }
                }
                else{
                  sprintf(temp, "       No buy available for this item");
                  strcat(message_to_send,temp);
                }

        }
        sendmessage(newsockfd,"ACCEPTED\n\n");
        sendmessage(newsockfd, message_to_send);

    }

    else if(same_string(option,"VT")){
      //if user wants to view trades
      char *uid=req[0];
      char *pass=req[1];
      char username[50];
      struct struct_authorization authcheck=req_credential_check(uid,pass);
      strcpy(username,authcheck.user_name);
      switch (authcheck.result)
      {
      case 1:
        {int userid=atoi(uid);
        sendmessage(newsockfd,"ACCEPTED\n\n");
        int ind=0,j;

        sprintf(message_to_send, "ITEM_NO\t\tBUYER_NAME\tSELLER_NAME\tQUANTITY\tPRICE\tBUYER_ID\tSELLER_ID\n\n");
        sendmessage(newsockfd, message_to_send);
        while(ind<tradeno){
          if(trade_info[ind].buyerid==userid || trade_info[ind].sellerid==userid){
                //if user sold or bought the the item display
                sprintf(message_to_send, "%d\t\t%s\t\t%s\t\t%d\t\t%d\t\t%d\t\t%d\n", trade_info[ind].itemnumber, trade_info[ind].buyer, trade_info[ind].seller, trade_info[ind].qty, trade_info[ind].price,trade_info[ind].buyerid,trade_info[ind].sellerid);
                sendmessage(newsockfd, message_to_send);
          }
          ind++;
        }
        break;
        }
      default:
        printf(message_to_send,"Rejected\n Log in not success!");
        sendmessage(newsockfd,message_to_send);
        break;
      }
    }
    close(newsockfd);

   }
   return 0;
}
