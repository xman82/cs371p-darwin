#include <cassert>
#include <iostream>
#include <vector>

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
};

struct Grid {
  private:
    int _c;                   //columns/width
    int _r;                   //rows/height
    int _twc;                 //total (completed) turns counter
    vector<Creature*> _crts; //list of all creatures on world

  public:
    vector< vector<char> > _grid;

    Grid(int c, int r) {
      _c = c;
      _r = r;
      for(int i=0; i<_r; ++i){
        vector<char>row(_c);
        fill(row.begin(), row.end(), '.');
        _grid.push_back(row);
      }
    }

    void print() {
      for(int i = 0; i < _r; ++i) {
        for(int j = 0; j < _c; ++j)
          cout << _grid[i][j];
        cout << endl;
      }
      cout << endl;
    }
    Creature* findCreature(int x, int y) {
      for(int i = 0; i < _crts.size(); ++i) {
        if( (_crts[i]->_x) == x && (_crts[i]->_y) == y )
          return _crts[i];
      }
      return NULL;
    }

    void addCreature(Creature& crt, int x, int y) {
      _grid[y][x]=(crt._spc)._rep;
      crt._x = x;
      crt._y = y;
      _crts.push_back(&crt);
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

    void moveCreature(Creature& crt) {
      if(crt._tc == _twc) {
        int tmpPC = crt._pc;
        vector<int> tmpInstPair = crt._spc._prog[tmpPC];
        int totInst = crt._spc._totInst;
        int tmpdir = crt._dir;
        int tmpInst = tmpInstPair[0];
        int tmpJPC = tmpInstPair[1];
        switch(tmpInst) {
          case 0:   //hop
            
          case 1:   //turn left
            tmpdir = (++tmpdir) % 4;
            ++tmpPC;
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
        }
      }
    }

    void turn(){
      Creature* crt;
      for(int i = 0; i < _r; ++i) {
        for(int j = 0; j < _c; ++j) {
          if(_grid[i][j] != '.' ) {
            crt = findCreature(i, j);
            moveCreature(*crt);
          }

        }
      }

    }


};

int main () {
  return 0;
}
