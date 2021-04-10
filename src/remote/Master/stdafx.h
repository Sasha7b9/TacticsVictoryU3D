// Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#ifdef WIN32
#pragma warning(push, 0)
#endif

#include <Urho3D/Math/Vector2.h>

using namespace Urho3D;

#include <cstdarg>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#undef Bool
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <sockpp/tcp_acceptor.h>
#include <sockpp/tcp_connector.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>

#ifdef WIN32
    #pragma warning(pop)
#endif

#include "defines.h"
#include "globals.h"
#include "Utils/Log_.h"
