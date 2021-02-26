// 2021/02/24 16:50:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Tile : public LogicComponent
{
    URHO3D_OBJECT(Tile, LogicComponent);

public:

    Tile(Context *context);

    static void RegisterObject();

    // �������� �����������
    void Enable();

    // ��������� �����������
    void Disable();

    bool IsEnabled() const;

protected:

    virtual ~Tile();

private:

    virtual void OnNodeSet(Node *node) override;

    virtual void Start() override;
};