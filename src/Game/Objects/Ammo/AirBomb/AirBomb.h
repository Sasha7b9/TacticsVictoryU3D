#pragma once


#include "Game/Objects/Ammo/AmmoObject.h"


class AirBomb : public AmmoObject
{
public:
    AirBomb(Context *context = gContext);

private:
    AirBomb& operator=(const AirBomb&)
    {};
};
