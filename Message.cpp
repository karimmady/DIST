#include "Message.h"
#include <iostream>
using namespace std;
Message::Message(int op, char * p_message, size_t p_message_size,int p_rpc_id,MessageType mess_type)
{
	operation=op;
	s=to_string(op) + " " + to_string(p_rpc_id)+ " " + p_message;
	message=(char *)(s.c_str());
	message_size=p_message_size;
	rpc_id=p_rpc_id;
	message_type=mess_type;
}

Message::Message(char * marshalled_base64)
{
	s=marshalled_base64;
	message_size=sizeof(marshalled_base64);
	message=marshalled_base64;
}


std::string Message::marshal(int length){
	s = base64_encode(reinterpret_cast<const unsigned char*>(message), length);
	message=(char *)(s.c_str());
	return s;
}

std::string Message::demarshal()
{
	s = base64_decode(s);
	message=(char *)(s.c_str());
	return s;
}

string Message::getMessage()
{
	return s;
}

int Message::getOperation()
{
	return operation;
}

int Message::getRPCId()
{
	return rpc_id;
}

size_t Message::getMessageSize()
{
	return message_size;
}

MessageType Message::getMessageType()
 {
 	return message_type;
 }

void Message::setMessageType(MessageType mess_type)
{
	message_type=mess_type;
}

void Message::setOperation(int _operation)
{
	operation=_operation;
}

Message::~Message()
{
}
