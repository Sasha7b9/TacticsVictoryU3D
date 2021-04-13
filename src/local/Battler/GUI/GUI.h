﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/GUI_.h"
#include "GUI/Controls/Window_.h"


class GUI : public GUIT
{
    URHO3D_OBJECT(GUI, GUIT);

public:
    GUI(GUI **self);
    ~GUI();

    bool GheckOnDeadZoneForCursorBottomScreen(int x);
    void SetVisibleWindow(WindowT *window, bool visible);
    void SetUnvisibleAllWindows();

private:
    void Create();
    void RegistrationObjects();
};
