#include <iostream>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

class Client
{
  struct sockaddr_in SockTarget;
  int Dtarg;
  std::string Message_box;
  char* text;

 public:
  Client(short int, unsigned short, char*);
  void Client_socket();
  void Client_connect();
  void Client_send();
  void Client_recv();
  void Client_shutdown();
};

Client::Client(short int _sin_family, unsigned short _sin_port,
               char* adress_ip)
{
  bzero(&SockTarget, sizeof(SockTarget));
  SockTarget.sin_family = _sin_family;
  SockTarget.sin_port   = htons(_sin_port);
  inet_pton(_sin_family, adress_ip, &SockTarget.sin_addr);
}

void Client::Client_socket()
{
  
  Dtarg = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  //fcntl(, F_SETFL, O_NONBLOCK);
 
}

void Client::Client_connect()
{
  connect(Dtarg, (struct sockaddr*)&SockTarget, sizeof(SockTarget));
  std::cout<<"CONNECT"<<std::endl;
}

void Client::Client_send()
{
  char text[20]; 
  //bzero(text, 100);
  std::cin.getline(text, 20);
  send(Dtarg, text, 20,0);
  std::cout<<"SEND"<<std::endl;
}
 
void Client::Client_recv()
{
  uint16_t n=0;
 // text = new char[100];;
  char text1[100];
  bzero(text1, sizeof(text1));
  n = read(Dtarg, text1, sizeof(text)-1);
  
  std::cout<<text1<<std::endl;
  std::cout<<"RECV "<<n<<std::endl;
};

void Client::Client_shutdown()
{
  shutdown(Dtarg, SHUT_RDWR);
};

int main()
{
  Client My_client(AF_INET, 5500, "127.0.0.1");
  My_client.Client_socket(); 
 while(1)
    My_client.Client_connect();
    My_client.Client_send();
    My_client.Client_recv();
    My_client.Client_shutdown();
  
  return 0;
}

