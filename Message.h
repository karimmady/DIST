#ifndef MESSAGE_H
#define MESSAGE_H

#include "base64.h"
#include <string.h>
using namespace std;


enum MessageType { Request, Reply, Inquiry,Up};

class Message
{
	private:
		MessageType message_type;
		int operation;
		char * message;
		size_t message_size;
		int rpc_id;
		string s;
	public:
		Message(int op, char * p_message, size_t p_message_size,int p_rpc_id, MessageType mess_type);
		Message(char * marshalled_base64);
		string marshal(int length);
		string demarshal();
		string getMessage();
		int getOperation();
		int getRPCId();
		size_t getMessageSize();
		MessageType getMessageType();
		void setOperation(int _operation);
		//void setMessage(void * message, size_t message_size);
		void setMessageType(MessageType mess_type);
		~Message();
};

#endif // MESSAGE_H
