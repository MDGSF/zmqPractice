#include "zhelpers.h"
#include <unistd.h>

int main() {
  void* context = zmq_ctx_new();

  void* subscriber = zmq_socket(context, ZMQ_SUB);

  int iRecvTimeout = 5000; //millsecond
  zmq_setsockopt(subscriber, ZMQ_RCVTIMEO, &iRecvTimeout, sizeof(iRecvTimeout));

  zmq_connect(subscriber, "tcp://localhost:5561");

  zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);


  sleep(1);

  void* syncclient = zmq_socket(context, ZMQ_REQ);
  zmq_connect(syncclient, "tcp://localhost:5562");

  s_send(syncclient, "");

  char* string = s_recv(syncclient);
  free(string);

  int i = 0;
  while (1) {
    char *string = s_recv(subscriber);
    if (string == NULL) {
      break;
    }

    if (strcmp(string, "END") == 0) {
      free(string);
      break;
    }

    printf("Received %d %s\n", i, string);

    free(string);
    i++;

    sleep(1);
  }
  printf("Received %d updates\n", i);

  zmq_close(subscriber);
  zmq_close(syncclient);
  zmq_ctx_destroy(context);
  return 0;
}
