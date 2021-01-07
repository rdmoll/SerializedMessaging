#include <iostream>
#include <unistd.h>
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

  publisher.bind( "tcp://*:5555" );
  subscriber.connect( "tcp://localhost:5556" );
  subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0 );
  request.connect( "tcp://localhost:5557" );
  reply.bind( "tcp://*:5558" );

  TestPackage::messageTable1 testTable;
  testTable.set_var1( 3.14159 );
  testTable.set_testitems( TestPackage::messageTable1::FIRST_ITEM );
  testTable.add_array1( 1.0 );
  testTable.add_array1( 2.0 );
  testTable.add_array1( 3.0 );
  testTable.set_note( "This is a test." );

  sleep( 2 );

  for( size_t i = 0; i < 10; i++ )
  {
    // PUB
    // Message frame 1
    std::string msgString = "test";
    size_t msgSize = msgString.size() + 1;
    zmq::message_t pubMsg1( msgString.size() + 1 );
    memcpy( pubMsg1.data(), msgString.c_str(), msgString.size() + 1 );

    // Message frame 2
    testTable.set_index( i );
    size_t tableSize = testTable.ByteSizeLong();
    zmq::message_t pubMsg2( tableSize );
    bool success = testTable.SerializeToArray( pubMsg2.data(), tableSize );

    std::cout << "Publishing data : " << i << std::endl;
    publisher.send( pubMsg1, ZMQ_SNDMORE );
    publisher.send( pubMsg2 );

    // SUB
    zmq::message_t recvMsg;
    subscriber.recv( &recvMsg, ZMQ_NOBLOCK );
    if( recvMsg.size() > 0 )
    {
      TestPackage::messageTable1 testTableRecv;
      bool parseSuccessful = testTableRecv.ParseFromArray( recvMsg.data(), recvMsg.size() );
      std::cout << "Sub message received : " << testTableRecv.index() << std::endl;
    }

    sleep( 2 );
  }

  publisher.close();

  return 0;
}
