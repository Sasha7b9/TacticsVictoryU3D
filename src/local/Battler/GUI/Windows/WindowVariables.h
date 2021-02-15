﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "GUI/Controls/WindowRTS.h"



class GovernorFloat;



class WindowVariables : public WindowRTS
{
    URHO3D_OBJECT(WindowVariables, WindowRTS);

public:
    WindowVariables(Context *context = TheContext);

    void SetVar(float *var, char *title);
    void AddFunctionFloat(char *title, pFuncFV funcRead, pFuncVF funcWrite);

private:
    float *var = nullptr; //-V122
    char *title = nullptr; //-V122

    SharedPtr<Label> label;
    SharedPtr<GovernorFloat> governor;

    DEFAULT_MEMBERS(WindowVariables);
};
