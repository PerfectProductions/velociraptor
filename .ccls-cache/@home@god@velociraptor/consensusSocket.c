#include "miniraptor.h"

void CreateDataSock(void) {

  short DataSocket;
  printf("%s\n","Initializing Consensus Data Retrieval...");
  DataSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (DataSocket > 0 ) { printf("Socket Descriptor = %s\n",DataSocket); }

  return DataSocket;

}

int ConnectDataSocket(int authority_ip, int authority_port) {

  int *DataSocket;
  int ServerIP = authority_ip;
  int ServerPort = authority_port;

  struct sockaddr_in DataStats = {0};

  DataStats.sin_addr.s_addr = inet_addr(ServerIP);
  DataStats.sin_family = AF_INET;
  DataStats.sin_port = ServerPort;

  *DataSocket = socket(AF_INET, SOCK_STREAM, 0);
  connect(*DataSocket, (struct sockaddr *)&DataStats,sizeof (struct sockaddr_in));

  //ConnectDataSocket(DataSocket, ServerIP, ServerPort);
  printf("Socket Descriptor = %s\n",&DataSocket);

  return DataSocket;

}
