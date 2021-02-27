// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Window_.h"


class GameObject;
class Label;


class ContextMenuUnit : public TWindow
{
    URHO3D_OBJECT(ContextMenuUnit, TWindow);

public:
    ContextMenuUnit(Context *context = TheContext);
    void Create(GameObject *object);

private:
    SharedPtr<Label> title;
    GameObject *object = nullptr; //-V122

    void CreateForUnit();
    void HandleToggledFiedView(StringHash, VariantMap&);
};