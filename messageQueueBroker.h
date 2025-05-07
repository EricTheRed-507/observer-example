#include <string>
#include <cstdio>
#include <vector>
#include <thread>
#include <unistd.h>

class mqMessage
{
	public:
        mqMessage(std::string message, int i) { sData = message, iData = i; }
		~mqMessage() {};

		std::string sData;
		int iData;
};

class mqConsumerObserver
{
	public:
        mqConsumerListener(){} 
		std::string sName;
        virtual void notify_messageReceived( mqMessage *msg ) = 0;


};

class mqConsumer
{
	private:

		std::vector<mqConsumerObserver*> listeners;
		std::vector<mqMessage> messageQ;

	public:
		mqConsumer() { };
		void addListener( mqConsumerObserver *l )
		{
			if( l != NULL )
			{
				listeners.push_back( l );
			}
		}
		void newMessage(mqMessage m)
		{
			messageQ.push_back( m );
			start();
		}
		void start( void ) {
			if(messageQ.size() && listeners.size())
			{
				mqMessage msg = messageQ.back();
				if(listeners[0]->sName[0] == msg.sData[0])
					listeners[0]->notify_messageReceived(&msg);
				if(listeners[1]->sName[0] == msg.sData[0])
						listeners[1]->notify_messageReceived(&msg);
				messageQ.pop_back();
			}
		};
		void stop( void ) {};

		
};
