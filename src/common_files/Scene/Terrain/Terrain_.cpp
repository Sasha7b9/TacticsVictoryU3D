// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "stdafx.h"
#include "Scene/Objects/Object_.h"
#include "Scene/Terrain/SegmentTerrain_.h"


Vector<Vector<PODVector<CubeTerrain*>>> TerrainT::columnsCubes;


TerrainT::TerrainT(Context *context) : Object(context)
{
}


TerrainT::~TerrainT()
{
    for(auto &row : columnsCubes)
    {
        for(auto &col : row)
        {
            for(auto &cube : col)
            {
                cube = nullptr;
            }
        }
    }
}


void TerrainT::Level::CreateFromVector(const Vector<Vector<float>> &lev)
{
    Clear();

    for (uint row = 0; row < lev.Size(); row++)
    {
        Vector<LogicCell> layer;
        layer.Resize(lev[row].Size());
        level.Push(layer);

        for (uint col = 0; col < lev[row].Size(); col++)
        {
            level[row][col].height = lev[row][col];
        }
    }
}


void TerrainT::CreateFromVector(const Vector<Vector<float>> &lev)
{
    float time = TheTime->GetElapsedTime();

    level.CreateFromVector(lev);

    CubeTerrain::terrain = this;

    uint heightX = SegmentTerrain::HEIGHT_X;
    uint widthZ = SegmentTerrain::WIDTH_Z;

    uint allRows = HeightX();
    uint allCols = WidthZ();

    uint segmentsInX = allRows / heightX + ((allRows % heightX) == 0 ? 0 : 1);  // ������� ��������� �� X (� ������)

    uint segmentsInZ = allCols / widthZ + ((allCols % widthZ) == 0 ? 0 : 1);    // ������� ��������� �� Z (� ������)

    columnsCubes.Resize(allRows);

    for(auto &row : columnsCubes)
    {
        row.Resize(allCols);
    }

    segments.Resize(segmentsInZ);

    for(uint row0 = 0; row0 < allRows; row0 += widthZ)
    {
        uint i = row0 / widthZ;

        segments[i].Resize(segmentsInX);

        for(uint col0 = 0; col0 < allCols; col0 += heightX)
        {
            uint numRows = (row0 + widthZ > allRows) ? (allRows - row0) : widthZ;
            uint numCols = (col0 + heightX > allCols) ? (allCols - col0) : heightX;

            uint j = col0 / heightX;

            segments[i][j] = new SegmentTerrain();

            segments[i][j]->CreateFromVector(level.level, row0, col0, numRows, numCols);
        }
    }

    for(uint z = 0; z < segmentsInZ; z++)
    {
        for(uint x = 0; x < segmentsInX; x++)
        {
            SegmentTerrain *segment = segments[z][x];
            if(x > 0)
            {
                segment->neighbours[SegmentTerrain::LEFT] = segments[z][x - 1];
            }
            if(z > 0)
            {
                segment->neighbours[SegmentTerrain::TOP] = segments[z - 1][x];
            }
            if(x < segmentsInX - 1)
            {
                segment->neighbours[SegmentTerrain::RIGHT] = segments[z][x + 1];
            }
            if(z < segmentsInZ - 1)
            {
                segment->neighbours[SegmentTerrain::BOTTOM] = segments[z + 1][x];
            }
        }
    }

    for(auto row : segments)
    {
        for(SegmentTerrain* segment : row)
        {
            segment->Build();
        }
    }

    URHO3D_LOGINFOF("time create terrain %f sec", TheTime->GetElapsedTime() - time);
}


float TerrainT::GetHeight(uint rowX, uint colZ) const
{
    return level.GetHeight(rowX, colZ);
}


float TerrainT::GetHeight(float rowX, float colZ) const
{
    return GetHeight((uint)rowX, (uint)colZ);
}


float TerrainT::GetHeight(const Vector2 coord) const
{
    return GetHeight(coord.x_, coord.y_);
}


void TerrainT::SetHeight(uint row, uint col, float height)
{
    level.level[row][col].height = height;
}


uint TerrainT::HeightX() const
{
    return level.level.Size();
}


uint TerrainT::WidthZ() const
{
    return level.level[0].Size();
}


bool TerrainT::IsEmpty() const
{
    return level.level.Empty();
}


TPlane TerrainT::GetIntersectionPlane(Ray& /*ray*/)
{
    return TPlane::ZERO;
}


Line TerrainT::GetIntersectionEdge(Ray &/*ray*/)
{
    return Line::ZERO;
}


TPlane TerrainT::GetPlane(uint /*row*/, uint /*col*/)
{
    return TPlane::ZERO;
}


Vector<Vector<float>> TerrainT::GetHeightMap()
{
    Vector<Vector<float>> result;

    result.Resize(level.level.Size());

    for (uint rowX = 0; rowX < level.level.Size(); rowX++)
    {
        result[rowX].Resize(level.level[rowX].Size());

        for (uint colZ = 0; colZ < level.level[rowX].Size(); colZ++)
        {
            result[rowX][colZ] = level.level[rowX][colZ].height;
        }
    }

    return result;
}


PODVector<CubeTerrain*>* TerrainT::GetColumnCubes(const CubeTerrain *cube, DIR::E dir)
{
    const int dRow[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
    const int dCol[8] = {-1, -1,  0,  1, 1, 1, 0, -1};

    uint row = (uint)((int)cube->row + dRow[dir]);
    uint col = (uint)((int)cube->col + dCol[dir]);

    if(row > HeightX() - 1 || col > WidthZ() - 1)
    {
        return nullptr;
    }

    return &columnsCubes[row][col];
}


SegmentTerrain* TerrainT::GetSegmentForCoord(uint row, uint col)
{
    return segments[row / SegmentTerrain::WIDTH_Z][col / SegmentTerrain::HEIGHT_X];
}


void TerrainT::PutIn(ObjectT *object, uint rowX, uint colZ)
{
    float height = GetHeight(rowX, colZ);
    
    if (object->IsFlying())
    {
        height += object->physics->min.altitude;
    }

    object->physics->pos.SetWorld({ (float)rowX, height, (float)colZ });
}


void TerrainT::Update(float /*dT*/)
{
    
}
