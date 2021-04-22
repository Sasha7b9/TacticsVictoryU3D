// 2021/04/12 20:50:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Network/Other/NetworkTypes_.h"
#include "Network/Other/ConnectorTCP_.h"
#include "Utils/GlobalFunctions_.h"


static TaskMasterServer taskPing =
{
    []()
    {
        int64 now = GF::Timer::TimeMS();

        return TheMaster.SendRequest(MSG_NTW_PING, &now, 4);
    },
    [](pchar, void *data, uint)
    {
        (void)data;
//        uint prev_time = *((uint *)data);

//        LOGWRITEF("ping = %d ms", GF::Timer::TimeMS() - prev_time);
    }
};


void ConnectorTCP::SetTasks()
{
    TheMaster.SetTask(1000, &taskPing);
}