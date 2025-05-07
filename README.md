### Observer Design Pattern Example

## Background to the application of interest
This document describes a simplified version of a message queue system which implements event driven design through message-based events. My company has an application that has thousands of distributed systems that can message our server at any time, and up to millions of messages a day. These messages all go through a single message broker which maintains a list of dependents and notifies these other components on the server to do certain actions. 

 ## Why this design pattern fits
The _observer pattern_ is a software design pattern in which an object, named the subject, **maintains a list of its dependents**, called observers, and **notifies them automatically** of any state changes, usually by calling one of their methods. It is often used for implementing **distributed event-handling systems** in event-driven software. 
>[!IMPORTANT]
> The bolded phrases above all align with the needs and intent of this system. The aysnchronous message broker needs to message it's list of dependents and notify them in an event-driven way.

## UML Diagram
![image](https://github.com/user-attachments/assets/18b3caa1-0d86-4b7c-9b8a-ac948aebccc9)

The above class diagram depicts the four classes in this system and relates them to the nominal design pattern. The methods in $${\color{red}red}$$ are the observer typical methods, and the methods in $${\color{blue}blue}$$ are implementation specific to this implementation of a message queue.

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

The class below is the message broker which has the observers assigned to it. It holds the list of observers, which are added through ```addListener(...)```, and within the ```start()``` method it sends out each message as it's recieved to the observers through the interface ```notify_messageReceived( mqMessage *msg )```. 
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
				listeners.push_back( l );
		}

		void newMessage(mqMessage m)
		{
			messageQ.push_back( m );
			start(); //note, this would normally be a separate thread
		}

		void start( void ) {
			for (auto & msg : messageQ)
				for (auto & element : listeners)
					element->notify_messageReceived(&msg);
		};
};
```
>[!NOTE]
> The ```start()``` method above would realistically happen on a separate thread in a real application unlike this simplified example. Notice how it pushes out the messages to each consumer/observer and allows them to decide which messages they need to act on. This message queue broker simply notifies observers when it gets new messages, it doesn't manage what observers it has or what they do.

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
The other observer is very similar, and thus I won't show it here.

## What if the observer pattern wasn't used?
The most substantial change, if we didn't have the observer pattern, would be the ```mqConsumer``` class.
```cpp
class mqConsumer {
    private:
        std::vector<mqMessage> messageQ;
	consumerDB consumerdb;
	consumerCalculation consumercalc;

    public:
        mqConsumer() {
	 }

        void newMessage(mqMessage m) {
            messageQ.push_back(m);
            start();
        }

        void start() {
            for (auto &msg : messageQ)
                dispatchMessage(&msg);
        }

        void dispatchMessage(mqMessage *msg) {
            if (msg->sConsumer == "db") {
                consumerdb.processMessage(msg);
            } else if (msg->sConsumer == "calc") {
                consumercalc.processMessage(msg);
            }
        }
};
```
> [!CAUTION]
> Notice how this class now has direct references to the two observers/consumers. This ultimately means we have **HIGH COUPLING**. This results in **reduced flexibility** and more dificult **extensibility**. Additionally, it would be almost impossible to configure which listeners need to be active dynamically at runtime.

In the actual implementation of this (unlike this simplified version), there are about 30 different message types and roughly 10 observers/consumers. This above non observer implementation would get messy quickly with changes and adding functionality. Additionally, it would be quite complex to attempt to dynamically choose which consumers are going to be allowed at startup and runtime.

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
