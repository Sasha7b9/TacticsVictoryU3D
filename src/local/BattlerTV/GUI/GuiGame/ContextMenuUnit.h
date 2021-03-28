// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/Window_.h"


class ObjectT;
class Label;


class ContextMenuUnit : public WindowT
{
    URHO3D_OBJECT(ContextMenuUnit, WindowT);

public:
    ContextMenuUnit();
    void Create(ObjectT *object);

private:
    SharedPtr<Label> title;
    ObjectT *object = nullptr;

    void CreateForUnit();
    void HandleToggledFiedView(StringHash, VariantMap&);
};
