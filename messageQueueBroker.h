#include <string>
#include <cstdio>
#include <vector>
#include <thread>
#include <unistd.h>

class mqMessage
{
	public:
        mqMessage(std::string message, std::string consumer, int i) { sData = message, sConsumer = consumer, iData = i; }
		~mqMessage() {};

		std::string sConsumer;
		std::string sData;
		int iData;
};

class mqConsumerObserver
{
	public:
		mqConsumerObserver(){}
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
			for (auto & msg : messageQ)
				for (auto & element : listeners)
					element->notify_messageReceived(&msg);
		};

		void stop( void ) {};
};
