#include "messageQueueBroker.h"
#include <unistd.h>
#include <thread>

class consumerDB : public mqConsumerObserver {

    public:
        void startThread()  {
            std::thread thread1(idle);
            thread1.detach();
        }

        //simulate database query processing
        static void writeToDB(mqMessage *msg)  {
            int sleepVal = 10;
            if(msg == nullptr)
                printf("--consumerDB idle--\n");
            else
            {
                printf("MsgHandlerA rec: %s \n", msg->sData.c_str());
                sleepVal = msg->iData;
            }

            usleep(100000 * sleepVal); //mock processing time via sleep
        }

        static void idle() { while(true) writeToDB(nullptr); }

        virtual void notify_messageReceived( mqMessage *msg )  {
            writeToDB(msg);
        }
};


class consumerCalculation : public mqConsumerObserver {

    public:
        void startThread()  {
            std::thread thread1(idle);
            thread1.detach();
        }

        //simulate calculation processing
        static void runCalculations(mqMessage *msg) {
            int sleepVal = 10;
            if(msg == nullptr)
                printf("--consumerCalculation idle--\n");
            else
            {
                printf("MsgHandlerB rec: %s \n", msg->sData.c_str());
                sleepVal = msg->iData;
            }
                
            usleep(100000 * sleepVal); //mock processing time via sleep
        }

        static void idle() { while(true) runCalculations(nullptr); }

        virtual void notify_messageReceived( mqMessage *msg ) {
            runCalculations(msg);
        }
};