#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <cstring>
#include <arpa/inet.h>

using namespace std;
typedef socklen_t slen;
#define PORT 50011

class Server
{
  public:

  fd_set master;
  fd_set readfd;
  struct sockaddr_in My_socket;
  struct sockaddr_in Cl_socket;
  struct timeval timeout;

  int fdmax;
  int DescL;
  int DescC;
  
  string text;
  
  char buffor[32];
  slen AddrLen;
  int  NrDesc, NrDescSEND;
  void SetAddr();
  void Settime();
  void Init();
};

int Sendall(const int& Socket, char *Message, const int& SizeM)
{
  uint16_t sendedB=0;
  uint16_t Bytetosend = SizeM;
  int n;

  while(sendedB < SizeM)
  { 
    n = send(Socket, (Message + sendedB), Bytetosend, 0);
    if(n== -1) return -1;
    sendedB     += n;
    Bytetosend  -= n;  
  }
  
  return (sendedB == SizeM); 
}
int Recvall(const int& _SockT, string& _mg)
{
  char Re[1];
  int R;
  while(1)
  {
    R=recv(_SockT, Re, 1,0);
    _mg.append(Re,1);
    if(Re[0] == '\0' || R == -1)
      break;
  }
  return R;
}

void Server::Settime()
{
  timeout.tv_sec = 10;
} 

void Server::SetAddr()
{
  bzero(&My_socket, sizeof(My_socket));
  bzero(&Cl_socket, sizeof(Cl_socket));
  My_socket.sin_family = AF_INET;
  My_socket.sin_port   = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &My_socket.sin_addr);
  AddrLen = sizeof(My_socket);
}

void Server::Init()
{
  FD_ZERO(&master);
  FD_ZERO(&readfd);
  Settime();

  DescL = socket(PF_INET, SOCK_STREAM, 0);
  SetAddr();
  bind(DescL, (struct sockaddr*)&My_socket, sizeof(My_socket));
  
  listen(DescL, 5);
  FD_SET(DescL, &master);
  fdmax = DescL; //==3
  
  while(1)
  {
    readfd = master;
    select(fdmax+1, &readfd, NULL, NULL, &timeout);
    for(NrDesc=0; NrDesc <= fdmax; NrDesc++)
    {
      if(FD_ISSET(NrDesc, &readfd)) 
    /*if readfd contain a NrDesc then FD_ISSET will 
     * return nonzero value 
     * */
      {
        if(NrDesc == DescL)
        {
          if( (DescC = accept(DescL, (struct sockaddr*)&Cl_socket,
                  &AddrLen)) != -1)
          {
            FD_SET(DescC, &master);
            if(fdmax < DescC)
              fdmax = DescC;
          }
        }
        else
        {
          if( Recvall(NrDesc, text) <= 0)
          {
            close(NrDesc);
            FD_CLR(NrDesc, &master);
          }
          else
          {
            
            buffor[31] = '\0';
            cout<<text<<endl;
            bzero(buffor, sizeof(buffor));
            cin.getline(buffor, sizeof(buffor));

            Sendall(NrDesc, buffor, sizeof(buffor));
            
/*
            for(NrDescSEND=0; NrDescSEND <= fdmax; NrDescSEND++)
            {
              if(FD_ISSET(NrDescSEND, &master) &&
                 NrDescSEND != DescL && NrDescSEND != NrDesc)
                if(Sendall(NrDescSEND, buffor, sizeof(buffor)))
                   cout<<"Sended all"<<endl;
            } */
          }
        }    
      }
    }
  }
}

int main()
{
  Server Start;
  Start.Init();
  return 0;
}  



  
