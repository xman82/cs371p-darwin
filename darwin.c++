#include <cassert>
#include <iostream>
#include <vector>
#include <stdlib.h>

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

struct Species {
  public:
    char _rep;                    //one character representation for species
    int _totInst;                 //total instruction number
    vector< vector<int>  > _prog; //species' program/instruction set

    Species() {
      _rep = ' ';
      _totInst = 0;
    }
    Species(char* name) {
      _rep=*name;
      _totInst = 0;
    }
    void addInstruction(int inst, int line=0) {
      vector<int> temp(2);
      temp = { inst, line };
      _prog.push_back(temp);
      ++_totInst;
    }
    vector<int> progInstruction(int& curr_line) {
      return  _prog[curr_line - 1];
    }   
};

struct Creature {
  public: 
    Species _spc;
    int _dir;	  //Direction faced: 0)North/UP 1)East/RIGHT 2)South/DOWN 3)West/LEFT
    int _pc;  	//current instruction number
    int _x;	  	//column/x-coordinate
    int _y; 		//row/y-coordinate
    int _tc;    //number of turns
    
    Creature() {
      _spc = Species();
      _dir = 1;
      _pc = 0;
    }

    Creature(Species* spc, int dir) {
      _spc = *spc;
      _dir = dir;
      _pc = 0;
      _tc = 0;
    }
    void infection(Species* spc) {
      _spc = *spc;
      _pc = 0;
    }
};

struct Grid {
  private:
    int _c;                   //columns/width
    int _r;                   //rows/height
    int _twc;                 //total (completed) turns counter
    vector<Creature*> _crts;  //list of all creatures on world

  public:
    vector< vector<char> > _grid;

    Grid(int c, int r) {
      _c = c;
      _r = r;
      _twc = 0;
      for(int i=0; i<_r; ++i){
        vector<char>row(_c);
        fill(row.begin(), row.end(), '.');
        _grid.push_back(row);
      }
    }

    void addCreature(Creature& crt, int x, int y) {
      _grid[y][x]=(crt._spc)._rep;
      crt._x = x;
      crt._y = y;
      _crts.push_back(&crt);
    }

    Creature* findCreature(int x, int y) {
      for(int i = 0; i < _crts.size(); ++i) {
        if( (_crts[i]->_x) == x && (_crts[i]->_y) == y )
          return _crts[i];
      }
      return NULL;
    }

    bool ifWall(int x, int y, int dir) {
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

    bool ifEnemy(int x, int y, int dir, char rep) {
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

    bool ifEmpty(int x, int y, int dir) {
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

    bool ifRandom() {
      int jmp = rand();
      if(jmp % 2 == 0)
        return false;
      return true;
    }
    void infect(int x, int y, int dir, Species* spc) {
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
    void hop(int x, int y, int dir, Creature* crt) {
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

    void moveCreature(int x, int y, Creature& crt) {
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

    void turn(){
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

    void print() {
      for(int i = 0; i < _r; ++i) {
        for(int j = 0; j < _c; ++j)
          cout << _grid[i][j];
        cout << endl;
      }
      cout << endl;
    }
};

int main () {
  return 0;
}
