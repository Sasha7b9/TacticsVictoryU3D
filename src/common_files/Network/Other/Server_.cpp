// 2021/04/09 14:45:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "FileSystem/ConfigurationFile_.h"
#include "Network/Other/NetworkTypes_.h"
#include "Network/Other/Server_.h"
#include "Utils/StringUtils_.h"


static const char MESSAGE[] = "Hello, World!";

// ���������� ��� ����� ����������
static void CallbackRead(struct bufferevent *, void *);
static void CallbackWrite(struct bufferevent *, void *);
static void CallbackAccept(evutil_socket_t listener, short event, void *arg);
static void CallbackError(struct bufferevent *bev, short what, void *ctx);
static void CallbackLog(int, const char *);


#define MAX_LINE 16384


std::map<void *, ClientInfo> clients;


static void ProcessClient(ClientInfo &info);


void Server::Run()
{
    event_set_log_callback(CallbackLog);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(40000);

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

#ifndef WIN32
    {
        int one = 1;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    }
#endif

    if (bind((SOCKET)listener, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        LOGERROR("Can not bind to port");
    }

    if (listen((SOCKET)listener, 100) < 0)
    {
        LOGERROR("Can not call listen()");
    }

    struct event *listener_event = event_new(base, listener, EV_READ | EV_PERSIST, CallbackAccept, (void *)base);

    event_add(listener_event, NULL);

    event_base_dispatch(base);
}


static void CallbackLog(int, const char *message)
{
    LOGERROR(message);
}


static void CallbackAccept(evutil_socket_t listener, short, void *arg)
{
    struct event_base *base = (struct event_base *)arg;

    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);

    int fd = (int)accept((SOCKET)listener, (struct sockaddr *)&ss, &slen);

    if (fd < 0)
    {
        LOGERROR("Error accepted");
    }
    else
    {
        evutil_make_socket_nonblocking(fd);
        struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        bufferevent_setcb(bev, CallbackRead, CallbackWrite, CallbackError, NULL);
//        bufferevent_setwatermark(bev, EV_READ | EV_WRITE, 0, 2);
        bufferevent_enable(bev, EV_READ | EV_WRITE);

        ClientInfo info;
        info.address.sin = *((sockaddr_in *)&ss);
        info.buffer = bev;

        LOGWRITEF("Client %s connected", info.address.ToString().c_str());

        clients[bev] = info;
    }
}


void Server::SendAnswer(void *bev, uint id, pchar message)
{
    bufferevent_write((struct bufferevent *)bev, &id, 4);

    SendString(bev, message);
}


void Server::SendString(void *bufevnt, pchar message)
{
    uint size = (uint)std::strlen(message);

    bufferevent_write((struct bufferevent *)bufevnt, &size, 4);

    bufferevent_write((struct bufferevent *)bufevnt, message, size);
}


static void CallbackRead(struct bufferevent *bev, void *)
{
    std::vector<uint8> &data = clients[bev].data;

#define SIZE_CHUNK 1024

    uint8 buffer[SIZE_CHUNK];

    size_t readed = bufferevent_read(bev, buffer, SIZE_CHUNK);

    while (readed)
    {
        data.insert(data.end(), &buffer[0], &buffer[readed]);

        readed = bufferevent_read(bev, buffer, SIZE_CHUNK);
    }

    ProcessClient(clients[bev]);
}


static void CallbackWrite(struct bufferevent *, void *)
{
    LOGWRITE(__FUNCTION__);
}


static void ProcessClient(ClientInfo &info)
{
    std::vector<uint8> &data = info.data;

    while (data.size() > 8)         // ���� ������� ������ ������, ��� �������� id � ������ ������
    {
        uint id = *(uint *)data.data();

        uint size = *(uint *)(data.data() + 4);

        if (data.size() >= 4 + size)
        {
            std::vector<char> buffer(size + 1);

            std::memcpy(buffer.data(), data.data() + 8, size);
            buffer[size] = '\0';

            data.erase(data.begin(), data.begin() + 8 + size);

            SU::SplitToWords(buffer.data(), size, Server::words);

            auto it = Server::handlers.find(Server::words[0]);

            if (it != Server::handlers.end())
            {
                it->second(id, &info);
            }
        }
    }
}


static void CallbackError(struct bufferevent *bev, short error, void *)
{
    if (error & BEV_EVENT_READING)
    {
        LOGWRITEF("Client %s disconnected", clients[bev].address.ToString().c_str());

        clients.erase(bev);
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


void Server::AppendHandler(pchar command, pFuncVUpV handler)
{
    handlers[command] = handler;
}
