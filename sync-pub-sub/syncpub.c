#include "zhelpers.h"

#define SUBSCRIBERS_EXPECTED 2

int main() {
  void* context = zmq_ctx_new();
  void* publisher = zmq_socket(context, ZMQ_PUB);

  /*
   * HWM High-Water Marks
   * PUB and ROUTER sockets will drop data if they reach their HWM.
   */
  int sndhwm = 10;
  zmq_setsockopt(publisher, ZMQ_SNDHWM, &sndhwm, sizeof(int));

  zmq_bind(publisher, "tcp://*:5561");

  void* syncservice = zmq_socket(context, ZMQ_REP);
  zmq_bind(syncservice, "tcp://*:5562");

  printf("Waiting for subscribers\n");

  int subscribers = 0;
  while (subscribers < SUBSCRIBERS_EXPECTED) {
    char* string = s_recv(syncservice);
    free(string);
    s_send(syncservice, "");
    subscribers++;
  }

  printf("Broadcasting messages\n");

  for (int i = 0; i < 10; i++) {
    char acMsg[1024] = {0};
    sprintf(acMsg, "Rhubarb_%d", i);
    s_send(publisher, acMsg);
  }
  s_send(publisher, "END");

  zmq_close(publisher);
  zmq_close(syncservice);
  zmq_ctx_destroy(context);
  return 0;
}
