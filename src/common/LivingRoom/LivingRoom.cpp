// 2021/04/08 16:17:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "LivingRoom.h"
#include "FileSystem/ConfigurationFile_v.h"
#include "Network/Other/NetworkTypes_v.h"
#include "Network/Other/ConnectorTCP_v.h"
#include <sstream>


int LivingRoom::Run(pchar ip)
{
    TheConnMaster.Init(ip, (uint16)TheSettings.GetInt("master_server", "port"));

    TheConnMaster.SetCallbacks
    (
        [](pchar file, int line)
        {
            LOGWRITEF("Can not connect to master server. Connect to remote from %s %d", file, line);
            TheConnMaster.Connect();
        },
        OnConnect,
        OnDisconnect
    );

    TheConnMaster.Connect();

    return MainCycle();
}


void LivingRoom::OnConnect()
{
    LOGWRITE("Connection to master server established");

    TheLivingRoom.SendNameToMasterServer();

    TheConnMaster.SetTasks();

    static TaskMasterServer taskPort =
    {
        []()
        {
            return TheConnMaster.SendRequest(MSG_NTW_GET_PORT_LIVINGROOM_BROADCAST_UDP);
        },
        [](pchar, void *data, uint)
        {
            int delta = *((int *)data);

            LOGWRITEF("Number port for connection %d", TheSettings.GetInt("master_server", "port") + delta);

//            static SocketConfig config;
//
//            TheServer.Init(config, (int)std::thread::hardware_concurrency());
        }
    };
        
    TheConnMaster.RunTask(&taskPort);
}


void LivingRoom::OnDisconnect()
{
    TheConnMaster.Connect();
    LOGWRITE("The master server is down. Attempting to connect");
}


int LivingRoom::MainCycle()
{
    while (true)
    {
        TheConnMaster.Update();
    }

    return 0;
}


void LivingRoom::SendNameToMasterServer()
{
    std::string name;

    std::stringstream stream(name);

    std::srand((uint)time(nullptr));

    stream << "LivingRoom " << std::rand();

    TheConnMaster.SendRequest(MSG_NTW_SET_NAME_LIVINGROOM, stream.str().c_str());
}
