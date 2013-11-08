
#include <cassert>
#include <iostream>
#include <vector>
#include <stdlib.h>

#include "Darwin.h"

using namespace std;

/*
 * 0: hop
 * 1: turn left
 * 2: turn right
 * 3: infect
 *
 * 4: if_empty n
 * 5: if_wall n
 * 6: if_random n
 * 7: if_enemy n
 * 8: goto n
 *
 */

/*
* 
* adds an instruction to the Species program
* @param inst is the instruction number
* @param line is the secondary instruction if 
* the instruction is between [4,8]
*/
void Species::addInstruction(int inst, int line=0) {
  vector<int> temp(2);
  temp = { inst, line };
  _prog.push_back(temp);
  ++_totInst;
}

/*
*
* retrives an instruction from the Species program
* @param curr_line is the current instruction number
* @returns the instruction pair at curr_line
*/
vector<int> Species::progInstruction(int& curr_line) {
  return  _prog[curr_line - 1];
}

/*
*
* given x, y coordinates, checks to see if the creature is a match
* @returns true when creature _x _y match x y
*/
bool Creature::match(int x, int y) {
  if(_x == x && _y == y )
    return true;
  else
   return false;
}

/*
*
* moves creature one space in direction faced
*/
void Creature::hop() {
  if(_dir == 0)
    --_x;
  if(_dir == 1)
    --_y;
  if(_dir == 2)
    ++_x;
  if(_dir == 3)
    ++_y;
}

/*
*
* alters _dir to represent the creature turning left
* (counterclockwise)
*/
void Creature::left() {
  _dir = (_dir - 1) % 4;
}

/*
*
* alters _dir to represent the creature turning right
* (clockwise)
*/
void Creature::right() {
  _dir = (_dir + 1) % 4;
}

/*
*
* changes the Creature's species 
* @param spc is the new species
*/
void Creature::infection(Species& spc) {
  _spc = spc;
  _pc = 0;
}

void Creature::turn(int tmpPC) {
  _pc = tmpPC;
  ++_tc;
}

/*
*
* adds a creature to the world and to the list
* of creatures in the world
* @param crt is the creature to be added
* @param x is the desired column
* @param y is the desired row
*/
void Grid::addCreature(Creature crt, int x, int y) {
  _grid[y][x]=(crt._spc)._rep;
  crt._x = x;
  crt._y = y;
  _crts.push_back(crt);
}

/*
*
* finds a Creature in the grid's list based on coordinates
* @param x is the desired column
* @param y is the desired row
* @returns the index of Creature
*/
int Grid::findCreature(int x, int y) {
  for(int i = 0; i < _crts.size(); ++i) {
    if( _crts[i].match(x, y) ) {
      return i;
    }
  }
  return (-1);
}

/*
*
* detects if creature is facing a wall
* @param x is the desired column
* @param y is the desired row
* @param dir is the creature's direction
* @returns true if creature if facing wall
*/
bool Grid::ifWall(int x, int y, int dir) {
  if(dir == 0 && x == 0)		//facing west
    return true;
  if(dir == 1 && y == 0)		//facing north
    return true;
  if(dir == 2 && x == (_c - 1) )	//facing east
    return true;
  if(dir == 3 && y == (_r - 1) )	//facing south
    return true;
  return false;
}

/*
*
* detects if creature is facing an Enemy
* @param x is the desired column
* @param y is the desired row
* @param dir is the creature's direction
* @returns true if creature if facing enemy
*/
bool Grid::ifEnemy(int x, int y, int dir, char rep) {
  if(dir == 0 && x != 0 && _grid[y][x-1] != rep && _grid[y][x-1] != '.')
    return true;
  if(dir == 1 && y != 0 && _grid[y-1][x] != rep && _grid[y-1][x] != '.')
    return true;
  if(dir == 2 && x != (_c - 1) && _grid[y][x+1] != rep && _grid[y][x+1] != '.')
    return true;
  if(dir == 3 && y != (_r - 1) && _grid[y+1][x] != rep && _grid[y+1][x] != '.')
    return true;
  return false;
}

/*
*
* detects if creature is facing an empty space
* @param x is the desired column
* @param y is the desired row
* @param dir is the creature's direction
* @returns true if creature if facing empty space
*/
bool Grid::ifEmpty(int x, int y, int dir) {
  if(dir == 0 && x > 0 && _grid[y][x-1] == '.')
    return true;
  if(dir == 1 && y > 0 && _grid[y-1][x] == '.')
    return true;
  if(dir == 2 && x < (_c - 1) && _grid[y][x+1] == '.')
    return true;
  if(dir == 3 && y < (_r - 1) && _grid[y+1][x] == '.')
    return true;
  return false;
}

/*
* @returns true if integer generated is odd
*/
bool Grid::ifRandom() {
  int jmp = rand();
  if(jmp % 2 == 0)
    return false;
  return true;
}

/*
*
* infects the enemy the creature is facing
* @param x is the creature's column position
* @param y is the creature's row position
* @param dir is the creature's direction
* @param spc is the species the enemy will be converted to
*/
void Grid::infect(int x, int y, int dir, Species& spc) {
  int x1 = x;
  int y1 = y;
  if(dir == 0)
    --x1;
  if(dir == 1)
    --y1;
  if(dir == 2)
    ++x1;
  if(dir == 3)
    ++y1;
  int crtInd = findCreature(x1, y1);
  if(crtInd != (-1)) {
    _crts[crtInd].infection(spc);
    _grid[y1][x1]=_crts[crtInd]._spc._rep;
  }
}

/*
*
* moves a creature one position over in direction faced
* @param x is the creature's column position
* @param y is the creature's row position
* @param dir is the creature's direction
* @param crt is the creature
*/
void Grid::hop(int x, int y, int dir, int crtInd) {
  int x1 = x;
  int y1 = y;
  bool canHop = false;
  if(dir == 0 && x1 > 0) {
    canHop = true;
    --x1;
  }
  if(dir == 1 && y1 > 0) {
    canHop = true;
    --y1;
  }
  if(dir == 2 && x1 < (_c - 1)){
    canHop = true;
    ++x1;
  }
  if(dir == 3 && y1 < (_r - 1)){
    canHop = true;
    ++y1;
  }
  if(canHop && ifEmpty(x, y, dir)) {
    _crts[crtInd].hop();
    _grid[y][x] = '.';
    _grid[y1][x1] = _crts[crtInd]._spc._rep;
  }
}

/*
*
* executes one action instruction for Creature
* @param x is the desired column
* @param y is the desired row
* @param crt is the Creature awaiting instructions
*/
void Grid::moveCreature(int x, int y, int crtInd) {
  Creature& crt = _crts[crtInd];
  if(crt._tc == _twc) {
    int col = x;
    int row = y;
    int tmpPC = crt._pc;
    vector<int> tmpInstPair = crt._spc._prog[tmpPC];
    int totInst = crt._spc._totInst;
    int tmpdir = crt._dir;
    int tmpInst = tmpInstPair[0];
    int tmpJPC = tmpInstPair[1];
    bool jump = false;

    //CONTROL FLOW INSTRUCTIONS
    while(tmpInst >= 4 ) {
      switch(tmpInst) {
        case 4:   //if_empty N
          if(ifEmpty(col, row, tmpdir))
            jump = true; 
        case 5:   //if_wall N
          if(ifWall(col, row, tmpdir))
            jump = true; 
        case 6:   //if_random N
          if(ifRandom())
            jump = true; 
        case 7:   //if_enemy N
          if(ifEnemy(col, row, tmpdir, crt._spc._rep))
            jump = true;
        case 8:   //go N
          jump = true;
      }
      if(jump == true)
        tmpPC = tmpJPC;
      else
        ++tmpPC;
      
      tmpPC = tmpPC % totInst;
      tmpInstPair = crt._spc._prog[tmpPC];
      tmpInst = tmpInstPair [0];
      tmpJPC = tmpInstPair [1];
      jump = false;
    }

    //ACTION INSTRUCTIONS
    if(tmpInst < 4  && tmpInst >= 0) {
      if(tmpInst == 0) {    //hop
        hop(col, row, tmpdir, crtInd);
      }
      if(tmpInst ==  1) {   //left
        _crts[crtInd].left();
      }
      if(tmpInst == 2) {    //right
        _crts[crtInd].right();
      }
      if(tmpInst == 3) {    //infect
        infect(col, row, tmpdir, crt._spc);
      }
      ++tmpPC;
      tmpPC = tmpPC % totInst;
      _crts[crtInd].turn(tmpPC);
    }
  }
}

/*
*
* executes a turn for the entire grid/world
*/
void Grid::turn(){
  int crtInd;
  for(int i = 0; i < _r; ++i) {
    for(int j = 0; j < _c; ++j) {
      if(_grid[i][j] != '.' ) {
        crtInd = findCreature(j, i);
        if(crtInd != (-1))
          moveCreature(j, i, crtInd);
      }
    }
  }
  ++_twc;
}

/*
* prints out the grid/world
*/
void Grid::print() {
  for(int i = 0; i < _r; ++i) {
    for(int j = 0; j < _c; ++j)
      cout << _grid[i][j];
    cout << endl;
  }
  cout << endl;
}

