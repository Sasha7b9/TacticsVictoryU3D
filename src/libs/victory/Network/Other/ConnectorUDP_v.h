// 2021/04/27 16:29:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ConnectorUDP
{
public:

    void AcceptServer(pchar ip, uint16 port);

    void SendMessage(pchar message);

private:

    pchar ip = "";
    uint16 port = 0;

    sockpp::udp_socket sock;
    sockpp::inet_address address;
};
