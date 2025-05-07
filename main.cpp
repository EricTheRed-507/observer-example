
#include <cstring>
#include <cstdio>
#include "consumers.h"
#include <math.h>

int main(int argc, char *argv[]) {

    consumerDB dataSinkA; dataSinkA.sName = "A"; dataSinkA.start();
    consumerCalculation dataSinkB; dataSinkB.sName = "B"; dataSinkB.start();

    mqConsumer messageBroker;
    messageBroker.addListener(&dataSinkA);
    messageBroker.addListener(&dataSinkB);

    for (size_t i = 0; i < 100; i++)
    {
        int iRand = std::rand() % 10;
        std::string dest = iRand % 2 ? "A" : "B";
        messageBroker.newMessage(mqMessage(dest + " message " + std::to_string(iRand), iRand));
    }
    

    return 0;
  } // end main