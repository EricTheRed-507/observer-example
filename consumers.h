#include "messageQueueBroker.h"
#include <unistd.h>

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


class consumerCalculation : public mqConsumerObserver {
    private:
        //simulate calculation processing
        static void runCalculations(mqMessage *msg) {
            printf("MsgHandlerCalc rec: %s \n", msg->sData.c_str());
            usleep(100000 * msg->iData); //mock processing time via sleep
        }
    public:
        virtual void notify_messageReceived( mqMessage *msg ) {
            if(msg != nullptr && msg->sConsumer == "calc") //only do something if the message is meant for this consumer
                runCalculations(msg);
        }
};
