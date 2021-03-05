// 2021/02/22 15:46:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by 
#pragma once
#include "Scene/Objects/Object_.h"
#include "Scene/Objects/Units/Accessories/Engine/Engine_.h"


//----------------------------------------------------------------------------------------------------------------------
class Unit : public ObjectT
{
    URHO3D_OBJECT(Unit, ObjectT);

public:

    static Vector<Unit *> storage;

    virtual void Update(float timeStep) override;

    SharedPtr<EngineT> engine;

    virtual void Compress(VectorBuffer &buffer, bool log = false) override;

    virtual void Decompress(MemoryBuffer &buffer, bool log = false) override;

protected:

    Unit(Context *);

    virtual void Start() override;
};