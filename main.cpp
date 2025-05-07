
#include <cstring>
#include <cstdio>
#include "consumers.h"
#include <math.h>

int main(int argc, char *argv[]) {
    consumerDB db;
    consumerCalculation calc;

    mqConsumer messageBroker;
    messageBroker.addListener(&db);
    messageBroker.addListener(&calc);

    for (size_t i = 0; i < 100; i++)
    {
        int iRand = std::rand() % 10;
        std::string dest = iRand % 2 ? "db" : "calc";
        messageBroker.newMessage(mqMessage("message " + std::to_string(iRand), dest, iRand));
    }
    

    return 0;
  } // end main
