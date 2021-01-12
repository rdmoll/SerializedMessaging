#include <iostream>
#include <zmq.h>
#include "zmq.hpp"

#include "TableDefs.pb.h"

int main()
{
  std::cout << "Communicator 1 started" << std::endl;

  zmq::context_t context( 1 );

  zmq::socket_t publisher( context, ZMQ_PUB );
  zmq::socket_t subscriber( context, ZMQ_SUB );
  zmq::socket_t request( context, ZMQ_REQ );
  zmq::socket_t reply( context, ZMQ_REP );

  publisher.bind( "tcp://*:5556" );
  subscriber.connect( "tcp://localhost:5555" );
  subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0 );
  request.connect( "tcp://localhost:5558" );
  reply.bind( "tcp://*:5557" );

  TestPackage::messageTable1 testTable;
  testTable.set_var1( 3.14159 );
  testTable.set_testitems( TestPackage::messageTable1::FIRST_ITEM );
  testTable.add_array1( 1.0 );
  testTable.add_array1( 2.0 );
  testTable.add_array1( 3.0 );
  testTable.set_note( "This is a test." );

  sleep( 2 );

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
