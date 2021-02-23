// 2021/02/23 11:37:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Ammo/_TAirBomb.h"
#include "Scene/Objects/Ammo/TCAmmoObject.h"


class CAirBomb : public AirBomb, public CAmmoObject
{
    URHO3D_OBJECT(CAirBomb, AirBomb);

public:

    static SharedPtr<CAirBomb> Create();

private:

    CAirBomb();
};
