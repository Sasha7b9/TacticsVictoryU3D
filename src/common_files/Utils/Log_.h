﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class TLog : public Log
{
    URHO3D_OBJECT(TLog, Log);

public:
    TLog(Context *context = TheContext);
    virtual void Write(int level, const String &message, pchar file, pchar func, int numLine);
    void EnableExtendedInfo();
    void DisableExtendedInfo();

protected:
    static bool enabledExtendedInfo;
};


#define LOGINFO(message)        TheLog->Write(LOG_INFO, message, __FILE__, __FUNCTION__, __LINE__)
#define LOGINFOF(format, ...)   TheLog->Write(LOG_INFO, ToString(format, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)
#define LOGERROR(message)       TLog::Write(LOG_ERROR, message, __FILE__, __FUNCTION__, __LINE__)
#define LOGERRORF(format, ...)  TLog::Write(LOG_ERROR, ToString(format, ##__VA_ARGS__), __FILE__, __FUNCTION__, __LINE__)