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
    const int MAZE_COLUMNS = 10;
    const int MAZE_ROWS = 10;
    
    MazeBuilder();
    MazeBuilder(Mesh* planeMesh, Mesh* cubeMesh);
    
    void PrintMazeDebug();
    void GetWallCount(MazeCell currentCell, int i, int j);
    void DrawWalls();
    
    std::vector<Wall *> WallList;
private:
    bool CanCheckCell(int i, int j);
    
    Mesh* _planeMesh;
    Mesh* _cubeMesh;
    
    Maze* _maze;
};

#endif /* MazeBuilder_hpp */
