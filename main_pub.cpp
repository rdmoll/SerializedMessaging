#include <iostream>
#include <unistd.h>
#include <zmq.h>
#include "zmq.hpp"

int main()
{
  std::cout << "Establishing publisher..." << std::endl;
  zmq::context_t context( 1 );
  zmq::socket_t publisher( context, ZMQ_PUB );
  publisher.bind( "tcp://*:5555" );

  sleep( 2 );

  for( size_t i = 0; i < 10; i++ )
  {
    std::string msgString = "test";
    size_t msgSize = msgString.size() + 1;
    zmq::message_t testMsg( msgString.size() + 1 );
    memcpy( testMsg.data(), msgString.c_str(), msgString.size() + 1 );

    std::cout << "Sending data : " << i << std::endl;
    publisher.send( testMsg );

    sleep( 2 );
  }

  publisher.close();

  return 0;
}
