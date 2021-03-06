// Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifdef WIN32
#pragma warning(push, 0)
#define _WINSOCKAPI_
#include <windows.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

#include <Urho3D/Container/Str.h>

using namespace Urho3D;

#undef Bool
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#ifdef WIN32
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

#include "defines.h"
#include "globals.h"
#include "Utils/Log_v.h"
