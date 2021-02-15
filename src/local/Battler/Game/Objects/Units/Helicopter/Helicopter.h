#pragma once
#include "Game/Objects/Units/UnitObject.h"



class Helicopter : public UnitObject
{
public:
    Helicopter(Context *context = TheContext);

private:
    DEFAULT_MEMBERS(Helicopter);
};
