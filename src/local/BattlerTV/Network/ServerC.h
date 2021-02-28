// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  ����� ��� ������������ � ��������
*/

class ServerC : public Object
{
    URHO3D_OBJECT(ServerC, Object);

public:

    ServerC(Context *context = TheContext);

    void Connect(const String &address, uint16 port);

private:

    void HandleMessage(StringHash, VariantMap &);

    void HandleServerConnected(StringHash, VariantMap &);

    void HandleServerDisconnected(StringHash, VariantMap &);

    void HandleConnectFailed(StringHash, VariantMap &);
};
