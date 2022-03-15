//
//  MazeBuilder.hpp
//  gl-maze-demo
//
//  Created by Jake Pauls on 2022-03-14.
//

#ifndef MazeBuilder_hpp
#define MazeBuilder_hpp

#include <vector>

#include "Maze.h"

#include "Mesh.hpp"
#include "Wall.hpp"

class MazeBuilder {
public:
    const int MAZE_COLUMNS = 4;
    const int MAZE_ROWS = 4;
    
    MazeBuilder();
    MazeBuilder(Mesh* planeMesh, Mesh* cubeMesh, GLuint t1, GLuint t2, GLuint t3);
    
    void PrintMazeDebug();
    
    // Get Wall Textures
    int GetSouthWallCount(MazeCell currentCell, int i, int j);
    int GetNorthWallCount(MazeCell currentCell, int i, int j);
    int GetEastWallCount(MazeCell currentCell, int i, int j);
    int GetWestWallCount(MazeCell currentCell, int i, int j);
    
    GLuint GetWallTexture(int wallCount);
    
    void DrawWalls();
    
    std::vector<Wall *> WallList;
private:
    bool CanCheckCell(int i, int j);
    
    Mesh* _planeMesh;
    Mesh* _cubeMesh;
    
    GLuint _wall0Texture;
    GLuint _wall1Texture;
    GLuint _wall2Texture;
    
    Maze* _maze;
};

#endif /* MazeBuilder_hpp */
