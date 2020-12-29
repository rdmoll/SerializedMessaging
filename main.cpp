#include <iostream>
#include <zmq.h>
#include "zmq.hpp"

int main()
{
  zmq::context_t context( 1 );
  zmq::socket_t subscriber( context, ZMQ_SUB );
  subscriber.connect( "tcp://localhost:10000" );
  subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0 );

  //while(true)
  //{
  std::cout << "Getting data" << std::endl;
  zmq::message_t recv_msg;
  subscriber.recv( &recv_msg, ZMQ_NOBLOCK );
  std::cout << "Data received" << std::endl;
  //}

  return 0;
}
