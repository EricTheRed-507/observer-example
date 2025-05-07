### Observer Design Pattern Example

## Background to the application of interest
This document describes a simplified version of a message queue system which implements event driven design through message-based events. My company has an application that has thousands of distributed systems that can message our server at any time, and up to millions of messages a day. These messages all go through a single message broker which maintains a list of dependents and notifies these other components on the server to do certain actions. 

 ## Why this design pattern fits
The _observer pattern_ is a software design pattern in which an object, named the subject, **maintains a list of its dependents**, called observers, and **notifies them automatically** of any state changes, usually by calling one of their methods. It is often used for implementing **distributed event-handling systems** in event-driven software. 


## UML Diagram
![image](https://github.com/user-attachments/assets/18b3caa1-0d86-4b7c-9b8a-ac948aebccc9)

The above class diagram depicts the four classes in this system and relates them to the nominal design pattern. The methods in red are the observer typical methods, and the methods in blue are implementation specific to this implementation of a message queue.

## Code Description
### main.cpp
```main.cpp``` is the client code in this example. It instantiates the message queue, the consumers (which are the observers), and it sends messages to the message queue. The code exerpt below depicts the creation of the main classes, and where the listener assignment occurs:
```cpp
consumerDB db;
consumerCalculation calc;

mqConsumer messageBroker;
messageBroker.addListener(&db);
messageBroker.addListener(&calc);
```
Below shows how this client code creates messages and passes them to the message queue.
```cpp
for (size_t i = 0; i < 100; i++)
{
    int iRand = std::rand() % 10;
    std::string dest = iRand % 2 ? "db" : "calc";
    messageBroker.newMessage(mqMessage("message " + std::to_string(iRand), dest, iRand));
}
```
>[!NOTE]
>The sample code above simply sends a random number 1-10 as part of the message, then randomly assigns "db" or "calc" as the destination for the message

### messageQueueBroker.h
This file defines all of the classes and interfaces for the observer pattern.
The class below is the interface for the observers:
```cpp
class mqConsumerObserver
{
	public:
		mqConsumerObserver(){}
        virtual void notify_messageReceived( mqMessage *msg ) = 0;
};
```

The class below is the message broker which has the observers assigned to it. It holds the list of observers, which are added through ```addListener(...)```, and within the ```start()``` method it sends out each message as it's recieved to the observers through the interface ```notify_messageReceived( mqMessage *msg )```
```cpp
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
```

### consumers.h
This file has two implementation examples of the ```mqConsumerObserver```, which are the ```consumerDB``` and the ```consumerCalculation```. Below is the former, which has the notify method from the observer design pattern and a simulated workload function, which has a sleep method to mock processing time where it would be interacting with a database.
```cpp
class consumerDB : public mqConsumerObserver {
    private:
        //simulate database query processing
        static void writeToDB(mqMessage *msg)  {
            printf("MsgHandlerDB rec: %s \n", msg->sData.c_str());
            usleep(100000 * msg->iData); //mock processing time via sleep
        }
    public:
        virtual void notify_messageReceived( mqMessage *msg )  {
            if(msg != nullptr && msg->sConsumer == "db") //only do something if the message is meant for this consumer
                writeToDB(msg);
        }
};
```


## Code Compilation and Usage

To compile the program with g++, type the following:
```
g++ main.cpp -o observerEx
```

Then simply run the application with:
```
./observerEx
```

Below is an example output from running the application:
![image](https://github.com/user-attachments/assets/a7a6eff6-04d8-41df-a31f-a8fbadb79ac2)
