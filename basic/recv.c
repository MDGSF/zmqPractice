#include <zmq.h>
#include <stdio.h>

int main() {

  void* pCtx = zmq_ctx_new();
  if (pCtx == NULL) {
    return 0;
  }

  void* pSock = zmq_socket(pCtx, ZMQ_DEALER);
  if (pSock == NULL) {
    zmq_ctx_destroy(pCtx);
    return 0;
  }

  int iRecvTimeout = 5000; //millsecond
  int iRet = zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRecvTimeout, sizeof(iRecvTimeout));
  if (iRet < 0) {
    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
  }

  if (zmq_bind(pSock, "tcp://*:7766") < 0) {
    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
  }

  printf("bind at: *:7766\n");

  while (1) {
    errno = 0;
    char acMsg[1024] = {0};
    if (zmq_recv(pSock, acMsg, sizeof(acMsg), 0) < 0) {
      printf("error = %s\n", zmq_strerror(errno));
      continue;
    }
    printf("received message: %s\n", acMsg);
  }

  return 0;
}
