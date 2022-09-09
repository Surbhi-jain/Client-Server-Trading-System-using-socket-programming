

struct struct_authorization{  
  char user_name[50];             
  int number;                
  int result;                
};                           

struct struct_trade_info{         
    char buyer[50];       
    char seller[50];      
    int buyerid;          
    int sellerid;         
    int itemnumber;       
    int price;            
    int qty;              
};

struct struct_buy_sell_request{     
    char user[50];     
    int itemnumber;    
    int id;            
    int qty;           
    int price;         
    char type;         
};

struct struct_trade_info trade_info[1000];  

int tradeno = 0;    

struct struct_buy_sell_request buy_req_queue[1000][10];   
struct struct_buy_sell_request sell_req_queue[1000][10];  

int buy_queue_head_tail[10][2]={0};    
int sell_queue_head_tail[10][2]={0};   
