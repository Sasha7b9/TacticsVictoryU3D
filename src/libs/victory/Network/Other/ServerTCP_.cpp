// 2021/04/09 14:45:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_.h"
#include "FileSystem/FileSystem_.h"
#include "Network/Other/NetworkTypes_.h"
#include "Network/Other/ServerTCP_.h"
#include "Utils/StringUtils_.h"
#ifdef WIN32
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif


static const char MESSAGE[] = "Hello, World!";


#define MAX_LINE 16384


std::string ClientInfo::SocketAddress::ToStringFull() const
{
    char buffer[100];

#ifdef WIN32
    sprintf_s(buffer, 100, "%d.%d.%d.%d:%d", sin.sin_addr.S_un.S_un_b.s_b1,
        sin.sin_addr.S_un.S_un_b.s_b2,
        sin.sin_addr.S_un.S_un_b.s_b3,
        sin.sin_addr.S_un.S_un_b.s_b4,
        sin.sin_port);
#else
    sprintf(buffer, "%d.%d.%d.%d:%d", (uint8)sin.sin_addr.s_addr,
        (uint8)(sin.sin_addr.s_addr >> 8),
        (uint8)(sin.sin_addr.s_addr >> 16),
        (uint8)(sin.sin_addr.s_addr >> 24),
        sin.sin_port);
#endif

    return std::string(buffer);
}


std::string ClientInfo::SocketAddress::ToStringHost() const
{
    char buffer[100];

#ifdef WIN32
    sprintf_s(buffer, 100, "%d.%d.%d.%d", sin.sin_addr.S_un.S_un_b.s_b1,
        sin.sin_addr.S_un.S_un_b.s_b2,
        sin.sin_addr.S_un.S_un_b.s_b3,
        sin.sin_addr.S_un.S_un_b.s_b4);
#else
    sprintf(buffer, "%d.%d.%d.%d", (uint8)sin.sin_addr.s_addr,
        (uint8)(sin.sin_addr.s_addr >> 8),
        (uint8)(sin.sin_addr.s_addr >> 16),
        (uint8)(sin.sin_addr.s_addr >> 24));
#endif

    return std::string(buffer);
}


void ClientInfo::SocketAddress::SetHostIP(void *ip)
{
    sin = *((sockaddr_in *)ip);

#ifdef WIN32
    if (sin.sin_addr.S_un.S_un_b.s_b1 == 127 &&
        sin.sin_addr.S_un.S_un_b.s_b2 == 0 &&
        sin.sin_addr.S_un.S_un_b.s_b3 == 0 &&
        sin.sin_addr.S_un.S_un_b.s_b4 == 1)
    {
        system("ipconfig > address.txt");
    }
#else
    if ((uint8)(sin.sin_addr.s_addr >> 0) == 127 &&
        (uint8)(sin.sin_addr.s_addr >> 8) == 0 &&
        (uint8)(sin.sin_addr.s_addr >> 16) == 0 &&
        (uint8)(sin.sin_addr.s_addr >> 24) == 1)
    {
        [[maybe_unused]] auto result = system("wget -qO- eth0.me > address.txt");

        FS::File file;

        if(!file.Open("address.txt", __FILE__, __LINE__))
        {
            LOGERROR("Can not open file with address");
        }

        std::string ip;

        file.ReadString(ip);

        inet_aton(ip.c_str(), &sin.sin_addr);

        FS::RemoveFile("address.txt");
    }
#endif
}


void ServerTCP::Run(uint16 port)
{
    Prepare();

    event_set_log_callback(CallbackLog);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(port);

#ifdef WIN32
    WSADATA wsa_data;
    if (WSAStartup(0x0201, &wsa_data) != 0)
    {
        LOGERROR("Can not execute WSAStartup(0x0201, &wsa_data)");
        return;
    };
#endif

    struct event_base *base = event_base_new();

    evutil_socket_t listener = (evutil_socket_t)socket(AF_INET, SOCK_STREAM, 0);

    evutil_make_socket_nonblocking(listener);

#ifdef WIN32
#else
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

#ifdef WIN32
    if (bind((SOCKET)listener, (struct sockaddr *)&sin, sizeof(sin)) < 0)
#else
    if (bind((int)listener, (struct sockaddr *)&sin, sizeof(sin)) < 0)
#endif
    {
        LOGERROR("Can not bind to port");
    }
    else
    {
        LOGWRITEF("Bind to port %d is Ok! Wait connections ...", port);
    }

#ifdef WIN32
    if (listen((SOCKET)listener, 100) < 0)
#else
    if (listen((int)listener, 100) < 0)
#endif
    {
        LOGERROR("Can not call listen()");
    }

    CallbackArgs args = { this, base };

    struct event *listener_event = event_new(base, listener, EV_READ | EV_PERSIST, CallbackAccept, &args);

    event_add(listener_event, NULL);

    event_base_dispatch(base);
}


void ServerTCP::CallbackLog(int, const char *message)
{
    LOGERROR(message);
}


void ServerTCP::CallbackAccept(evutil_socket_t listener, short, void *_args)
{
    CallbackArgs *args = (CallbackArgs *)_args;

    struct event_base *base = args->base;

    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);

#ifdef WIN32
    int fd = (int)accept((SOCKET)listener, (struct sockaddr *)&ss, &slen);
#else
    int fd = (int)accept((int)listener, (struct sockaddr *)&ss, &slen);
#endif

    sockaddr addr;
    socklen_t len = sizeof(addr);

#ifdef WIN32
    getsockname((SOCKET)fd, &addr, &len);
#else
    getsockname(fd, &addr, &len);
#endif

    if (fd < 0)
    {
        LOGERROR("Error accepted");
    }
    else
    {
        evutil_make_socket_nonblocking(fd);
        struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, CallbackRead, CallbackWrite, CallbackError, args);
//        bufferevent_setwatermark(bev, EV_READ | EV_WRITE, 0, 2);
        bufferevent_enable(bev, EV_READ | EV_WRITE);

        ClientInfo info;
        info.address.SetHostIP(&ss);
        info.benv = bev;

        LOGWRITEF("Client %s connected", info.address.ToStringFull().c_str());

        args->server->clients[bev] = info;
    }
}


void ServerTCP::SendAnswer(void *bev, uint id, pchar message, void *data, uint size_data)
{
    bufferevent_write((struct bufferevent *)bev, &id, 4);

    uint full_size = (uint)std::strlen(message) + 1 + size_data;

    bufferevent_write((struct bufferevent *)bev, &full_size, 4);

    bufferevent_write((struct bufferevent *)bev, message, std::strlen(message) + 1);

    if (data)
    {
        bufferevent_write((struct bufferevent *)bev, data, size_data);
    }
}


void ServerTCP::SendAnswer(void *bev, uint id, pchar message, pchar data)
{
    SendAnswer(bev, id, message, (void *)data, (uint)std::strlen(data) + 1);
}


void ServerTCP::CallbackRead(struct bufferevent *bev, void *_args)
{
    CallbackArgs *args = (CallbackArgs *)_args;

    std::vector<uint8> &data = args->server->clients[bev].bindata;

#define SIZE_CHUNK 1024

    uint8 buffer[SIZE_CHUNK];

    size_t readed = bufferevent_read(bev, buffer, SIZE_CHUNK);

    while (readed)
    {
        data.insert(data.end(), &buffer[0], &buffer[readed]);

        readed = bufferevent_read(bev, buffer, SIZE_CHUNK);
    }

    ProcessClient(args->server->clients[bev], args->server);
}


void ServerTCP::CallbackWrite(struct bufferevent *, void *)
{
}


static uint GetID(std::vector<uint8> &received)
{
    return *(uint *)received.data();
}


static uint GetSize(std::vector<uint8> &received)
{
    return *(uint *)(received.data() + 4);
}


// ���������� ����� ������� �� received � data. ��� ���� �� ����������� ������� ������������ ������ ���������
static void MoveData(std::vector<uint8> &received, std::vector<uint8> &data)
{
    uint size = GetSize(received);

    data.resize(size);

    std::memcpy(data.data(), received.data() + 8, size);

    received.erase(received.begin(), received.begin() + 8 + size);
}


void ServerTCP::ProcessClient(ClientInfo &info, ServerTCP *server)
{
    std::vector<uint8> &received = info.bindata;

    while (received.size() > 4 + 4)         // ���� ������� ������ ������, ��� �������� id � ������ ������
    {
        uint id = GetID(received);

        uint size = GetSize(received);

        if (received.size() >= 4 + 4 + size)
        {
            MoveData(received, info.message);

            SU::SplitToWords((char *)info.message.data(), info.words);

            auto it = server->handlers.find(info.words[0]);

            if (it != server->handlers.end())
            {
                it->second(id, info);
            }
        }
        else
        {
            break;      // ���� ������� �� ��� ����� �������
        }
    }
}


void ServerTCP::CallbackError(struct bufferevent *bev, short error, void *_args)
{
    ServerTCP *server = ((CallbackArgs *)_args)->server;

    if (error & BEV_EVENT_READING)
    {
        LOGWRITEF("Client %s disconnected", server->clients[bev].address.ToStringFull().c_str());

        server->clients.erase(bev);
    }
    else if (error & BEV_EVENT_WRITING)
    {
        LOGERROR("BEV_EVENT_WRITING");
    }
    else if (error & BEV_EVENT_EOF)
    {
        LOGERROR("BEV_EVENT_EOF");
    }
    else if (error & BEV_EVENT_TIMEOUT)
    {
        LOGERROR("BEV_EVENT_TIMEOUT");
    }
    else if (error & BEV_EVENT_CONNECTED)
    {
        LOGERROR("BEV_EVENT_TIMEOUT");
    }
    else
    {
        LOGERROR("Unknown error occured");
    }

    bufferevent_free(bev);
}


void ServerTCP::AppendHandler(pchar command, handlerClient handler)
{
    handlers[command] = handler;
}


void *ClientInfo::GetRawData()
{
    char *string = (char *)message.data();

    if (std::strlen(string) + 1 == message.size())
    {
        return nullptr;
    }

    return message.data() + std::strlen(string) + 1;
}