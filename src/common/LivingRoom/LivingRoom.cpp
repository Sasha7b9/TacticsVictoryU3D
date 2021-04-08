// 2021/04/08 16:17:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "LivingRoom.h"


LivingRoom::LivingRoom()
{

}


int LivingRoom::Run(pchar address)
{
    TheMasterServer.SetAddress(address);

    TheMasterServer.SetCallbacks
    (
        []()
        {
            LOGERROR("Can't connect to master server");
            TheMasterServer.Connect();
            LOGWRITE("Attempt connection to master-server");
        },
        []()
        {
            LOGWRITE("Connection to master server established");

            SetTasks();
        },
        []()
        {
            TheMasterServer.Connect();
            LOGWRITE("The master server is down. Attempting to connect");
        },
        [](int)
        {
        }
    );

    return 0;
}


void LivingRoom::SetTasks()
{

}
