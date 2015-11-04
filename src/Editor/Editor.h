#pragma once


#include "Graphics/Objects/Plane.h"
#include "Game/Objects/Terrain.h"
#include "Graphics/Objects/Line.h"


class Editor : public UObject
{
    OBJECT(Editor);
public:
    Editor(UContext *context);
    void Run();
    void ClearScene();

private:
    Plane currentPlane;
    Plane selectedPlane;

    Line currentEdge;
    Line selectedEdge;

    SharedPtr<Node> lightNode;

    void HandlePostRenderUpdate(StringHash, VariantMap&);
    void HandleMouseDown(StringHash, VariantMap&);
    void HandleKeyDown(StringHash, VariantMap&);

    Editor& operator=(const Editor&)
    {};
};