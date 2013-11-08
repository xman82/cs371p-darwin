#ifndef Darwin_h
#define Darwin_h
#include <iostream>
#include <list>
#include <vector>
#include <stdlib.h>

using namespace std;

struct Species {
  public:
    char _rep;                    //one character representation for species
    int _totInst;                 //total instruction number
    vector< vector<int>  > _prog; //species' program/instruction set

    Species() {
      _rep = ' ';
      _totInst = 0;
    }
    Species(char rep) {
      _rep=rep;
      _totInst = 0;
    }

    void addInstruction(int, int);
    vector<int> progInstruction(int&);
};

struct Creature {
  public: 
    Species _spc;
    int _dir;	//Direction faced: 0)North/UP 1)East/RIGHT 2)South/DOWN 3)West/LEFT
    int _pc;  	//current instruction number
    int _x;	  	//column/x-coordinate
    int _y;		//row/y-coordinate
    int _tc;	//number of turns
    
    Creature() {
      _spc = Species();
      _dir = 1;
      _pc = 0;
    }
    Creature(Species& spc, int dir) {
      _spc = spc;
      _dir = dir;
      _pc = 0;
      _tc = 0;
    }
    bool match(int, int);
    void hop();
    void left();
    void right();
    void infection(Species&);
    void turn(int);
};

struct Grid {
  private:
    int _c;                   //columns/width
    int _r;                   //rows/height
    int _twc;                 //total (completed) turns counter
    vector<Creature> _crts;  //list of all creatures on world

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
    void addCreature(Creature, int, int);
    int findCreature(int, int);

    bool ifWall(int, int, int);
    bool ifEnemy(int, int, int, char);
    bool ifEmpty(int, int, int);
    bool ifRandom();
    void infect(int, int, int, Species&);
    void hop(int, int, int, int);

    void moveCreature(int, int, int);
    void turn();
    void print();
};

#endif

