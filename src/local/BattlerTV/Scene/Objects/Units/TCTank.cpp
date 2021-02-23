// 2021/02/22 15:40:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by 
#include "stdafx.h"
#include "Scene/Objects/Units/TCTank.h"


CTank::CTank() : CTankObject()
{

}


SharedPtr<CTank> CTank::Create(uint /*row*/, uint /*col*/)
{
    SharedPtr<CTank> tank(new CTank());

    return tank;
}
