#include <iostream>
#include <unistd.h>
#include <zmq.h>
#include "zmq.hpp"

#include "TableDefs.pb.h"

int main()
{
  std::cout << "Communicator 2 started" << std::endl;
  std::cout << std::endl;

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
    std::cout << "PUBLISHER" << std::endl;

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

    publisher.send( pubMsg1, ZMQ_SNDMORE );
    publisher.send( pubMsg2 );
    std::cout << "PUB message sent : Index = " << i << std::endl;
    std::cout << std::endl;

    // SUB
    std::cout << "SUBSCRIBER" << std::endl;

    zmq::message_t recvMsg;
    subscriber.recv( &recvMsg, ZMQ_NOBLOCK );   // receive first frame
    if( recvMsg.size() > 0 )
    {
      subscriber.recv( &recvMsg );  // receive second frame

      TestPackage::messageTable1 testTableRecv;
      bool parseSuccessful = testTableRecv.ParseFromArray( recvMsg.data(), recvMsg.size() );
      std::cout << "SUB message received : Index = " << testTableRecv.index() << std::endl;
    }
    else
    {
      std::cout << "No message" << std::endl;
    }
    std::cout << std::endl;

    // REP
    std::cout << "REPLY" << std::endl;

    // Receive request
    zmq::message_t reqMsg1;
    reply.recv( &reqMsg1, ZMQ_NOBLOCK );
    std::cout << "test1" << std::endl;
    if( reqMsg1.size() > 0 )
    {
      std::cout << "test2" << std::endl;
      zmq::message_t reqMsg2;
      //reply.recv( &reqMsg2 );

      //TestPackage::messageTable1 receivedTable;
      //success = receivedTable.ParseFromArray( reqMsg2.data(), reqMsg2.size() );
      std::cout << "REQ message received : Index = " << std::endl; //receivedTable.index() << std::endl;
    }

    // Message frame 1
    zmq::message_t repMsg1( msgString.size() + 1 );
    memcpy( repMsg1.data(), msgString.c_str(), msgString.size() + 1 );

    // Message frame 2
    testTable.set_index( i );
    zmq::message_t repMsg2( tableSize );
    success = testTable.SerializeToArray( repMsg2.data(), tableSize );

    // Send reply messages
    reply.send( repMsg1, ZMQ_SNDMORE );
    reply.send( repMsg2 );
    std::cout << "REP message sent : Index = " << i << std::endl;
    std::cout << std::endl;

    sleep( 2 );
  }

  publisher.close();
  subscriber.close();
  request.close();
  reply.close();

  return 0;
}
