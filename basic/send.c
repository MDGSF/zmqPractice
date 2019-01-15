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

  int iSendTimeout = 5000; //millsecond
  int iRet = zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iSendTimeout, sizeof(iSendTimeout));
  if (iRet < 0) {
    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
  }

  iRet = zmq_connect(pSock, "tcp://127.0.0.1:7766");
  if (iRet < 0) {
    zmq_close(pSock);
    zmq_ctx_destroy(pCtx);
    return 0;
  }

  while (1) {
    static int i = 0;
    char acMsg[1024] = {0};
    snprintf(acMsg, sizeof(acMsg), "hello world: %3d", i++);
    printf("Enter to send...\n");
    if (zmq_send(pSock, acMsg, sizeof(acMsg), 0) < 0) {
      fprintf(stderr, "send message failed\n");
      continue;
    }
    printf("send message: [%s] succeed\n", acMsg);
    getchar();
  }

  return 0;
}
