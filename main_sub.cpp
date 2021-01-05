#include <iostream>
#include <zmq.h>
#include "zmq.hpp"

#include "TableDefs.pb.h"

int main()
{
  zmq::context_t context( 1 );
  zmq::socket_t subscriber( context, ZMQ_SUB );
  subscriber.connect( "tcp://localhost:5555" );
  subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0 );

  std::cout << "Waiting for messages..." << std::endl;

  size_t count = 0;
  while( 1 )
  {
    zmq::message_t recv_msg;
    subscriber.recv( &recv_msg, ZMQ_NOBLOCK );
    if( recv_msg.size() > 0 )
    {
      std::cout << "Message received : " << count++ << std::endl;
    }
  }

  subscriber.close();

  return 0;
}
