// 2021/02/25 21:40:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Objects/Units/Ground/Tank/Tank_.h"
#include "Scene/Objects/Units/Ground/Tank/TankC.h"
#include "Scene/Objects/Units/UnitC.h"


void TankSpecificC::Update(float timeStep)
{
    UnitSpecificC::Update(timeStep);
}


void Tank::CreateSpecific()
{
    specific = new TankSpecificC(this);
}
