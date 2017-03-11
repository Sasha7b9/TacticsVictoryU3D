// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdafx.h>
#include "WaveAlgorithm.h"
#include "Game/Objects/Terrain/Terrain.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TERRAIN_HEIGHT_EQUAL(x,y) (fabs(gTerrain->GetHeight(x, y) - heightStart) < M_EPSILON)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WaveAlgorithm::WaveAlgorithm() : Thread()
{
    //passValues.Insert(KeySet(5, )
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
WaveAlgorithm::~WaveAlgorithm()
{
    Stop();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::SetSize(uint rows, uint cols)
{
    numRows = rows;
    numCols = cols;

    cells.Resize(numRows);
    for (auto &row : cells)
    {
        row.Resize(numCols);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::StartFind(Coord start_, Coord end_)
{
    start = start_;
    end = end_;
    Run();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool WaveAlgorithm::PathIsFound()
{
    return pathIsFound;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PODVector<Coord> WaveAlgorithm::GetPath()
{
    Stop();
    return path;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::ThreadFunction()
{
    pathIsFound = false;
    path.Clear();
    FindPath();
    pathIsFound = true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::FindPath()
{
    if (fabs(gTerrain->GetHeight(start.row, start.col) - gTerrain->GetHeight(end.row, end.col)) > M_EPSILON)
    {
        return;
    }

    if (start == end)
    {
        path.Push(start);
        return;
    }

    for (auto &row : cells)
    {
        for (auto &cell : row)
        {
            cell = -1;
        }
    }

    heightStart = gTerrain->GetHeight(start.row, start.col);

    Vector<Wave> waves;

    Wave wave;
    SetCell(wave, start.row, start.col, 0);

    waves.Push(wave);

    do
    {
        NextWave(waves);
    } while(waves[waves.Size() - 1].Size() && !Contain(waves[waves.Size() - 1], end));

    if(Contain(waves[waves.Size() - 1], end))
    {
        path.Push(end);
        while(!(path[0] == start))
        {
            AddPrevWave(path);
        }
    }
    else
    {
        path.Push(start);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool WaveAlgorithm::Contain(Wave &wave, Coord &coord)
{
    for (auto &crd : wave)
    {
        if (crd == coord)
        {
            return true;
        }
    }

    return false;
}


static int dRow[] = {0, -1, 0, 1, -1, -1, 1, 1};
static int dCol[] = {-1, 0, 1, 0, -1, 1, 1, -1};

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::NextWave(Vector<Wave> &waves)
{
    int numWave = (int)waves.Size();
    Wave &prevWave = waves[(uint)numWave - 1];
    Wave wave;

    for (auto coord : prevWave)
    {
        for (int i = 0; i < 8; i++)
        {
            uint row = coord.row;
            uint col = coord.col;

            int dR = dRow[i];
            int dC = dCol[i];

            int iRow = (int)row + dR;
            int iCol = (int)col + dC;

            uint newRow = (uint)iRow;
            uint newCol = (uint)iCol;

            if (newRow < numRows && newCol < numCols && cells[newRow][newCol] == -1 && fabs(gTerrain->GetHeight(newRow, newCol) - heightStart) < M_EPSILON)
            {
                if (i == 4 && (!TERRAIN_HEIGHT_EQUAL(row, col - 1) || !TERRAIN_HEIGHT_EQUAL(row - 1, col)))
                {
                    continue;
                }
                else if (i == 5 && (!TERRAIN_HEIGHT_EQUAL(row - 1, col) || !TERRAIN_HEIGHT_EQUAL(row, col)))
                {
                    continue;
                }
                else if (i == 6 && (!TERRAIN_HEIGHT_EQUAL(row, col + 1) || !TERRAIN_HEIGHT_EQUAL(row + 1, col)))
                {
                    continue;
                }
                else if (i == 7 && (!TERRAIN_HEIGHT_EQUAL(row, col - 1) || !TERRAIN_HEIGHT_EQUAL(row + 1, col)))
                {
                    continue;
                }

                SetCell(wave, newRow, newCol, numWave);
            }
        }
    }

    waves.Push(wave);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::SetCell(Wave &wave, uint row, uint col, int numWave)
{
    wave.Push(Coord(row, col));
    cells[row][col] = numWave;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void WaveAlgorithm::AddPrevWave(PODVector<Coord> &path_)
{
    Coord coord = path_[0];
    uint row = coord.row;
    uint col = coord.col;
    int numWave = cells[row][col];

    for (int i = 0; i < 8; i++)
    {
        int iRow = (int)row + dRow[i];
        int iCol = (int)col + dCol[i];

        uint newRow = (uint)iRow;
        uint newCol = (uint)iCol;

        if (newRow < gTerrain->NumRows() && newCol < gTerrain->NumCols() && cells[newRow][newCol] == numWave - 1)
        {
            path_.Insert(0, Coord(newRow, newCol));
            return;
        }
    }
}
