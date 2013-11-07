
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
* changes the Creature's species 
* @param spc is the new species
*/
void Creature::infection(Species* spc) {
  _spc = *spc;
  _pc = 0;
}

/*
*
* adds a creature to the world and to the list
* of creatures in the world
* @param crt is the creature to be added
* @param x is the desired column
* @param y is the desired row
*/
void Grid::addCreature(Creature& crt, int x, int y) {
  _grid[y][x]=(crt._spc)._rep;
  crt._x = x;
  crt._y = y;
  _crts.push_back(&crt);
}

/*
*
* finds a Creature in the grid's list based on coordinates
* @param x is the desired column
* @param y is the desired row
* @returns the Creature at the desired coordinates
*/
Creature* Grid::findCreature(int x, int y) {
  for(int i = 0; i < _crts.size(); ++i) {
    if( (_crts[i]->_x) == x && (_crts[i]->_y) == y )
      return _crts[i];
  }
  return NULL;
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
  if(dir == 0 && y == 0)
    return true;
  if(dir == 1 && x == (_c - 1) )
    return true;
  if(dir == 2 && y == (_r - 1) )
    return true;
  if(dir == 3 && x == 0)
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
  if(dir == 0 && y != 0 && _grid[y-1][x] != rep && _grid[y-1][x] != '.')
    return true;
  if(dir == 1 && x != (_c - 1) && _grid[y][x+1] != rep && _grid[y][x+1] != '.')
    return true;
  if(dir == 2 && y != (_r - 1) && _grid[y+1][x] != rep && _grid[y+1][x] != '.')
    return true;
  if(dir == 3 && x != 0 && _grid[y][x-1] != rep && _grid[y][x-1] != '.')
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
  if(dir == 0 && y != 0 && _grid[y-1][x] == '.')
    return true;
  if(dir == 1 && x != (_c - 1) && _grid[y][x+1] == '.')
    return true;
  if(dir == 2 && y != (_r - 1) && _grid[y+1][x] == '.')
    return true;
  if(dir == 3 && x != 0 && _grid[y][x-1] == '.')
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
void Grid::infect(int x, int y, int dir, Species* spc) {
  Creature* crt;
  int x1 = x;
  int y1 = y;
  if(dir == 0)
    --y1;
  if(dir == 1)
    ++x1;
  if(dir == 2)
    ++y1;
  if(dir == 3)
    --x1;
  crt = findCreature(x1, y1);
  (*crt).infection( spc);
}

/*
*
* moves a creature one position over in direction faced
* @param x is the creature's column position
* @param y is the creature's row position
* @param dir is the creature's direction
* @param crt is the creature
*/
void Grid::hop(int x, int y, int dir, Creature* crt) {
  int x1 = x;
  int y1 = y;
  Creature& crt1 = *crt;
  if(dir == 0 && y1 > 0)
    --y1;
  if(dir == 1 && x1 < (_c - 1))
    ++x1;
  if(dir == 2 && y1 < (_r - 1))
    ++y1;
  if(dir == 3 && x1 > 0)
    --x1;
  crt1._x = x1;
  crt1._y = y1;
}

/*
*
* executes one action instruction for Creature
* @param x is the desired column
* @param y is the desired row
* @param crt is the Creature awaiting instructions
*/
void Grid::moveCreature(int x, int y, Creature& crt) {
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
        hop(col, row, tmpdir, &crt);
        ++tmpPC;
      }
      if(tmpInst ==  1) {   //left
        tmpdir = (++tmpdir) % 4;
        ++tmpPC;
      }
      if(tmpInst == 2) {    //right
        tmpdir = (--tmpdir) % 4;
        ++tmpPC;
      }
      if(tmpInst == 3) {    //infect
        infect(col, row, tmpdir, &(crt._spc));
        ++tmpPC;
      }
      tmpPC = tmpPC % totInst;
      crt._dir = tmpdir;
      crt._pc = tmpPC;
      crt._tc += 1;
    }
  }
}

/*
*
* executes a turn for the entire grid/world
*/
void Grid::turn(){
  Creature* crt;
  for(int i = 0; i < _r; ++i) {
    for(int j = 0; j < _c; ++j) {
      if(_grid[i][j] != '.' ) {
        crt = findCreature(i, j);
        moveCreature(j, i, *crt);
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


int main () {
  return 0;
}
