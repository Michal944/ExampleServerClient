#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

int recvall(const int& _SockT, std::string& _text)
{
    char Re[1];
    int R;
    while(1)
    {
      R=recv(_SockT, Re, 1, 0);
      _text.append(Re,1);
      if(Re[0] == '\0' || R == -1)
        break;
    }
    std::cout<<_text<<std::endl;
    return R;
}

int main()
{
  struct sockaddr_in Sv;
  bzero(&Sv, sizeof(Sv));
  Sv.sin_family = AF_INET;
  Sv.sin_port   = htons(50011);
  Sv.sin_addr.s_addr = inet_addr("127.0.0.1");

 
  int SockT;
  if( (SockT = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
    printf("ERRR");

  if(connect(SockT, (struct sockaddr*)&Sv, sizeof(Sv)) < 0)
    printf("connect error");

  
  char Mg[32] = "ABCDE5FGIJK10L";
  std::string text; 
       
  send(SockT, Mg, sizeof(Mg), 0);
  recvall(SockT, text);

  close(SockT);

  return 0;
}
  

