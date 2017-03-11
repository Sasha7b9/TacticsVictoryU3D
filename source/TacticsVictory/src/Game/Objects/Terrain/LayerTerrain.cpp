// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdafx.h>
#include "CubeTerrain.h"
#include "LayerTerrain.h"
#include "Core/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LayerTerrain::LayerTerrain(Context *context) : Object(context)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
LayerTerrain::~LayerTerrain()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LayerTerrain::AddCube(SharedPtr<CubeTerrain> &cube)
{
    cubes.Push(cube);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LayerTerrain::Create()
{
    for(auto cube : cubes)
    {
        cube->Create();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LayerTerrain::Build()
{
    PODVector<float> vertexes;
    PODVector<uint> indexes;

    for(CubeTerrain *cube : cubes)
    {
        cube->BuildVertexes(vertexes, indexes);
    }

    SharedPtr<VertexBuffer> vb(new VertexBuffer(gContext));
    SharedPtr<IndexBuffer> ib(new IndexBuffer(gContext));
    SharedPtr<Geometry> geom(new Geometry(gContext));

    uint numVert = vertexes.Size();
    uint numInd = indexes.Size();

    float *bufVert = new float[(size_t)numVert];
    uint *bufInd = new uint[(size_t)numInd];

    for (uint i = 0; i < numVert; i++)
    {
        bufVert[(size_t)i] = vertexes[i];
    }

    for (uint i = 0; i < numInd; i++)
    {
        bufInd[(size_t)i] = indexes[i];
    }

    vb->SetShadowed(true);
    vb->SetSize(vertexes.Size() / 8, MASK_POSITION | MASK_NORMAL | MASK_TEXCOORD1);
    vb->SetData(bufVert);

    ib->SetShadowed(true);
    ib->SetSize((uint)numInd, true);
    ib->SetData(bufInd);

    geom->SetVertexBuffer(0, vb);
    geom->SetIndexBuffer(ib);
    geom->SetDrawRange(TRIANGLE_LIST, 0, ib->GetIndexCount());

    model = new Model(gContext);
    Node *node = gScene->CreateChild(NODE_TERRAIN);
    object = node->CreateComponent<StaticModel>();
    object->SetViewMask(VIEW_MASK_FOR_MISSILE);

    Vector<SharedPtr<VertexBuffer>> vbVector;
    Vector<SharedPtr<IndexBuffer>> ibVector;

    model->SetNumGeometries(1);
    model->SetNumGeometryLodLevels(0, 1);
    model->SetGeometry(0, 0, geom);

    PODVector<uint> morphRange;

    vbVector.Push(vb);
    ibVector.Push(ib);

    model->SetVertexBuffers(vbVector, morphRange, morphRange);
    model->SetIndexBuffers(ibVector);

    model->SetBoundingBox(Math::CalculateBoundingBox(bufVert, (uint)numVert / 8));

    object->SetModel(model);
    object->SetMaterial(gCache->GetResource<Material>("Materials/TVTerrain.xml"));
    object->SetCastShadows(true);

    SAFE_DELETE_ARRAY(bufVert); //-V809
    SAFE_DELETE_ARRAY(bufInd); //-V809
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
CubeTerrain* LayerTerrain::GetCube(uint row, uint col)
{
    for(auto cube : cubes)
    {
        if(cube->row == row && cube->col == col)
        {
            return cube;
        }
    }

    return nullptr;
}
