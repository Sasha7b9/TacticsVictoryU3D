#pragma once


class PointPlane
{
public:
    Vector3 coord;
    Vector3 normal;
    Vector2 texCoord;
};


class PlaneCube : public Object
{
    URHO3D_OBJECT(PlaneCube, Object);

public:
    PlaneCube(Context *context = gContext);

    PointPlane point[4];

private:

    CONSTRUCTORS(PlaneCube, Object);
};
