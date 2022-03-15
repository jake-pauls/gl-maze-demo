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

MazeBuilder::MazeBuilder(Mesh* planeMesh, Mesh* cubeMesh, GLuint t0, GLuint t1, GLuint t2)
    : _planeMesh(planeMesh), _cubeMesh(cubeMesh), _wall0Texture(t0), _wall1Texture(t1), _wall2Texture(t2)
{
    _maze = new Maze(MAZE_COLUMNS, MAZE_ROWS);
    _maze->Create();
}

void MazeBuilder::DrawWalls()
{
    MazeCell currentCell;
    
    // Positioning Data
    glm::vec3 cellPosition, wallPosition;
    float wallOffset = 0.5f, wallRotation = 0.0f;
    
    // Texture Data
    int wallCount;
    GLuint wallTexture;
    
    for (int i = 0; i < _maze->rows; i++) {
        for (int j = 0; j < _maze->cols; j++) {
            currentCell = _maze->GetCell(i, j);
            wallCount = 0;
            
            cellPosition = glm::vec3(j, 0.0f, i);
            
            if (currentCell.northWallPresent) {
                wallTexture = GetWallTexture(GetNorthWallCount(currentCell, i, j));
                
                wallPosition = cellPosition;
                wallRotation = 0.0f;
                wallPosition.z -= wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation, wallTexture));
            }
            if (currentCell.southWallPresent) {
                wallTexture = GetWallTexture(GetSouthWallCount(currentCell, i, j));
                
                wallPosition = cellPosition;
                wallRotation = 180.0f;
                wallPosition.z += wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation, wallTexture));
            }
            if (currentCell.eastWallPresent) {
                wallTexture = GetWallTexture(GetEastWallCount(currentCell, i, j));
                
                wallPosition = cellPosition;
                wallRotation = -90.0f;
                wallPosition.x += wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation, wallTexture));
            }
            if (currentCell.westWallPresent) {
                wallTexture = GetWallTexture(GetWestWallCount(currentCell, i, j));
                
                wallPosition = cellPosition;
                float wallRotation = 90.0f;
                wallPosition.x -= wallOffset;
                
                WallList.push_back(new Wall(_planeMesh, wallPosition, wallRotation, wallTexture));
            }
        }
    }
}

/**
 * Wall counts are checked differently based on the direction
 * but each direction follows similar steps.
 *
 * For context, it helps to draw this out.
 *
 * 1. Check if adjacent walls exist in the same cell
 * 2. Check if adjacent walls exist in the neighboring cells
 * 3. Check if adjacent walls exist 'behind' the wall (L-Shape)
 */

int MazeBuilder::GetSouthWallCount(MazeCell currentCell, int i, int j)
{
    MazeCell tempCell;
    int wallCount = 0;
    
    // Check for walls in same cell
    if (currentCell.eastWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i, j+1)) {
            // Check for walls beside
            tempCell = _maze->GetCell(i, j+1);
            if (tempCell.southWallPresent) {
                wallCount++;
            } else {
                // Check walls behind
                // Go up a cell, check the correct side
                if (CanCheckCell(i+1, j+1)) {
                    tempCell = _maze->GetCell(i+1, j+1);
                    if (tempCell.westWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    
    if (currentCell.westWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i, j-1)) {
            tempCell = _maze->GetCell(i, j-1);
            if (tempCell.southWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i+1, j-1)) {
                    tempCell = _maze->GetCell(i+1, j-1);
                    if (tempCell.eastWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    
    return wallCount;
}

int MazeBuilder::GetNorthWallCount(MazeCell currentCell, int i, int j)
{
    MazeCell tempCell;
    int wallCount = 0;
    
    if (currentCell.eastWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i, j+1)) {
            tempCell = _maze->GetCell(i, j+1);
            if (tempCell.northWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i-1, j+1)) {
                    tempCell = _maze->GetCell(i-1, j+1);
                    if (tempCell.westWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    
    if (currentCell.westWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i, j-1)) {
            tempCell = _maze->GetCell(i, j-1);
            if (tempCell.northWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i-1, j-1)) {
                    tempCell = _maze->GetCell(i-1, j-1);
                    if (tempCell.eastWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
   
    return wallCount;
}

int MazeBuilder::GetEastWallCount(MazeCell currentCell, int i, int j)
{
    MazeCell tempCell;
    int wallCount = 0;
    
    if (currentCell.southWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i+1, j)) {
            tempCell = _maze->GetCell(i+1, j);
            if (tempCell.eastWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i+1, j+1)) {
                    tempCell = _maze->GetCell(i+1, j+1);
                    if (tempCell.northWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    
    if (currentCell.northWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i-1, j)) {
            tempCell = _maze->GetCell(i-1, j);
            if (tempCell.eastWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i-1, j+1)) {
                    tempCell = _maze->GetCell(i-1, j+1);
                    if (tempCell.southWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
   
    return wallCount;
}

int MazeBuilder::GetWestWallCount(MazeCell currentCell, int i, int j)
{
    MazeCell tempCell;
    int wallCount = 0;
    
    if (currentCell.southWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i+1, j)) {
            tempCell = _maze->GetCell(i+1, j);
            if (tempCell.westWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i+1, j-1)) {
                    tempCell = _maze->GetCell(i+1, j-1);
                    if (tempCell.northWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
    
    if (currentCell.northWallPresent) {
        wallCount++;
    } else {
        if (CanCheckCell(i-1, j)) {
            tempCell = _maze->GetCell(i-1, j);
            if (tempCell.westWallPresent) {
                wallCount++;
            } else {
                if (CanCheckCell(i-1, j-1)) {
                    tempCell = _maze->GetCell(i-1, j-1);
                    if (tempCell.southWallPresent) {
                        wallCount++;
                    }
                }
            }
        }
    }
   
    return wallCount;
}

/// Retrieves wall texture for a given wall count
GLuint MazeBuilder::GetWallTexture(int wallCount)
{
    switch (wallCount) {
        case 0:
            return _wall0Texture;
            break;
        case 1:
            return _wall1Texture;
            break;
        case 2:
            return _wall2Texture;
            break;
    }

    return _wall0Texture;
}

/// Prints an ASCII dump of the generated maze
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

/// Validates that a MazeCell can be checked with the given indices
bool MazeBuilder::CanCheckCell(int i, int j)
{
    return i <= MAZE_COLUMNS - 1 && j <= MAZE_ROWS - 1 && i >= 0 && j >= 0;
}
