//
//  MazeBuilder.cpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-14.
//

#include "MazeBuilder.hpp"

#include "Assert.hpp"

MazeBuilder::MazeBuilder()
{ }

MazeBuilder::MazeBuilder(Mesh* planeMesh, Mesh* cubeMesh)
    : _planeMesh(planeMesh), _cubeMesh(cubeMesh)
{
    _maze = new Maze(MAZE_COLUMNS, MAZE_ROWS);
    _maze->Create();
}

void MazeBuilder::DrawWalls()
{
    MazeCell currentCell;
    glm::vec3 cellPosition, wallPosition;
    float wallOffset = 0.5f, wallRotation = 0.0f;
    
    for (int i = 0; i < _maze->rows; i++) {
        for (int j = 0; j < _maze->cols; j++) {
            currentCell = _maze->GetCell(i, j);
            GetWallCount(currentCell, i, j);
            
            cellPosition = glm::vec3(j, 0.0f, i);
            
            if (currentCell.northWallPresent) {
                wallPosition = cellPosition;
                wallRotation = 0.0f;
                wallPosition.z -= wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation));
            }
            if (currentCell.southWallPresent) {
                wallPosition = cellPosition;
                wallRotation = 180.0f;
                wallPosition.z += wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation));
            }
            if (currentCell.eastWallPresent) {
                wallPosition = cellPosition;
                wallRotation = -90.0f;
                wallPosition.x += wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation));
            }
            if (currentCell.westWallPresent) {
                wallPosition = cellPosition;
                float wallRotation = 90.0f;
                wallPosition.x -= wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation));
            }
        }
    }
}

void MazeBuilder::GetWallCount(MazeCell currentCell, int i, int j)
{
    // For each cell, count the number of walls adjacent to the cell
    // 1. Check in the cell against each wall
    MazeCell tempCell;
    int wallCount = 0;
    
    LOG("[" << i << "," << j << "] South Check");
    if (currentCell.southWallPresent) {
        // Check for walls in same cell
        if (currentCell.eastWallPresent) {
            wallCount++;
        } else {
            if (CanCheckCell(i+1, j+1)) {
                // Check for walls beside
                tempCell = _maze->GetCell(i, j+1);
                if (tempCell.southWallPresent) {
                    wallCount++;
                } else {
                    // Check walls behind
                    // Go up a cell, check the correct side
                    tempCell = _maze->GetCell(i+1, j+1);
                    if (tempCell.westWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
        
        if (currentCell.westWallPresent) {
            wallCount++;
        } else {
            if (CanCheckCell(i+1, j-1)) {
                tempCell = _maze->GetCell(i, j-1);
                if (tempCell.southWallPresent) {
                    wallCount++;
                } else {
                    tempCell = _maze->GetCell(i+1, j-1);
                    if (tempCell.eastWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    LOG("COUNT: " << wallCount);
}

void MazeBuilder::PrintMazeDebug()
{
    int i, j;
    
    for (i = MAZE_ROWS - 1; i >= 0; i--)
    {
        for (j = MAZE_COLUMNS - 1; j >= 0; j--) {    // top
            printf(" %c ", _maze->GetCell(i, j).southWallPresent ? '-' : ' ');
        }
        
        printf("\n");
        
        for (j = MAZE_COLUMNS - 1; j >= 0; j--) {    // left/right
            printf("%c", _maze->GetCell(i, j).eastWallPresent ? '|' : ' ');
            printf("%c", ((i+j) < 1) ? '*' : ' ');
            printf("%c", _maze->GetCell(i, j).westWallPresent ? '|' : ' ');
        }
        
        printf("\n");
        
        for (j = MAZE_COLUMNS - 1; j >= 0; j--) {    // bottom
            printf(" %c ", _maze->GetCell(i, j).northWallPresent ? '-' : ' ');
        }
        
        printf("\n");
    }
}

bool MazeBuilder::CanCheckCell(int i, int j)
{
    return i <= MAZE_COLUMNS - 1 && j <= MAZE_ROWS - 1 && i >= 0 && j >= 0;
}
