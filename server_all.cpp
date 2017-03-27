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

#define PORT 5500

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
  
  char buffor[256];
  int  Size_Msg;
//  int  yes = 1;
 socklen_t AddrLen;
  int  NrDesc, NrDescSEND;
  void SetAddr();
  void Settime();
  void Init();
};

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
          if( (Size_Msg = recv(NrDesc, buffor, sizeof(buffor)-1, 0)
              )  <= 0)
          {
            close(NrDesc);
            FD_CLR(NrDesc, &master);
          }
          else
          {
            /*
            buffor[255] = '\0';
            cout<<buffor<<endl;
            bzero(buffor, sizeof(buffor));
            cin.getline(buffor, sizeof(buffor));
            send(NrDesc, buffor, sizeof(buffor),0);
            */

            for(NrDescSEND=0; NrDescSEND <= fdmax; NrDescSEND++)
            {
              if(FD_ISSET(NrDescSEND, &master) &&
                 NrDescSEND != DescL && NrDescSEND != NrDesc)
                send(NrDescSEND, buffor, sizeof(buffor), 0);
            }
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



  
