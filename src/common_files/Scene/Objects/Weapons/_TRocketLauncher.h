// 2021/02/22 21:17:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Weapons/_TWeaponObject.h"


class RocketLauncher : public WeaponObject
{
    URHO3D_OBJECT(RocketLauncher, WeaponObject);

public:

protected:

    RocketLauncher(Context *context);
};
