// 2021/04/02 17:15:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifdef WIN32
#pragma warning(push, 0)
#else
#endif

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#define _WINSOCKAPI_
#include <windows.h>


#include <Urho3D/Container/Str.h>

using namespace Urho3D;


#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

#ifdef WIN32
#pragma warning(pop)
#else
#endif

#include "defines.h"
#include "globals.h"
#include "Utils/Log_v.h"
