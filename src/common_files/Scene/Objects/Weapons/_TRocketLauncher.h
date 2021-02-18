// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Scene/Objects/Weapons/_TWeaponObject.h"


class Tank;


class RocketLauncher : public WeaponObject
{
    URHO3D_OBJECT(RocketLauncher, WeaponObject);
public:
    RocketLauncher(Context *context = TheContext, Tank *tank = nullptr);

    void Init();
    void Update(float deltaStep);

    static void RegisterObject();

private:
    Tank *tank = nullptr; //-V122
    float radiusDetect = 50.0f;
    float timeRecharge = 50.0f;
    float timeElapsedAfterShoot = 0.0f;
    float timePrevRaycast = 0.0f;

    bool TargetInPointView(Tank* tank);

    static void RegisterInAS();
};
