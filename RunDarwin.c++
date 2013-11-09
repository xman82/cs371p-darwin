// -----------------------------
// projects/darwin/RunDarwin.c++
// Copyright (C) 2013
// Glenn P. Downing
// -----------------------------

/*
To run the program:
    % g++ -pedantic -std=c++0x -Wall Darwin.c++ RunDarwin.c++ -o RunDarwin
    % valgrind RunDarwin > RunDarwin.out

To configure Doxygen:
    doxygen -g
That creates the file Doxyfile.
Make the following edits:
    EXTRACT_ALL            = YES
    EXTRACT_PRIVATE        = YES
    EXTRACT_STATIC         = YES
    GENERATE_LATEX         = NO

To document the program:
    doxygen Doxyfile
*/

// --------
// includes
// --------

#include <cassert>   // assert
#include <cstdlib>   // rand, srand
#include <iostream>  // cout, endl
#include <stdexcept> // invalid_argument, out_of_range
#include "Darwin.h"

// ----
// main
// ----

int main () {
    using namespace std;

    // ----
    // food
    // ----

    Species food = Species('f');
    /*
     0: left
     1: go 0
    */
    food.addInstruction(1, 0);
    food.addInstruction(8, 0);

    // ------
    // hopper
    // ------

    Species hopper = Species('h');
    /*
     0: hop
     1: go 0
    */
    hopper.addInstruction(0, 0);
    hopper.addInstruction(8, 0);
    // -----
    // rover
    // -----

    Species rover = Species('r');
    /*
     0: if_enemy 9
     1: if_empty 7
     2: if_random 5
     3: left
     4: go 0
     5: right
     6: go 0
     7: hop
     8: go 0
     9: infect
    10: go 0
    */
    rover.addInstruction(7, 9);
    rover.addInstruction(4, 7);
    rover.addInstruction(6, 5);
    rover.addInstruction(1, 0);
    rover.addInstruction(8, 0);
    rover.addInstruction(2, 0);
    rover.addInstruction(8, 0);
    rover.addInstruction(0, 0);
    rover.addInstruction(8, 0);
    rover.addInstruction(3, 0);
    rover.addInstruction(8, 0);
    // ----
    // trap
    // ----

    Species trap = Species('t');
    /*
     0: if_enemy 3
     1: left
     2: go 0
     3: infect
     4: go 0
    */
    trap.addInstruction(7, 3);
    trap.addInstruction(1, 0);
    trap.addInstruction(8, 0);
    trap.addInstruction(3, 0);
    trap.addInstruction(8, 0);

    // ----
    // best
    // ----
    Species best = Species('b');
    /*
     0: if_enemy 10
     1: if_wall 3
     2: if_empty 8
     3: if_random 6
     4: left
     5: go 0
     6: right
     7: go 0
     8: hop
     9: go 0
     10: infect
     11: if_random 4
     12: right
     13: go 0
    */
    best.addInstruction(7, 10);
    best.addInstruction(5, 3);
    best.addInstruction(4, 8);
    best.addInstruction(6, 6);
    best.addInstruction(1, 0);
    best.addInstruction(8, 0);
    best.addInstruction(2, 0);
    best.addInstruction(8, 0);
    best.addInstruction(0, 0);
    best.addInstruction(8, 0);
    best.addInstruction(3, 0);
    best.addInstruction(6, 4);
    best.addInstruction(2, 0);
    best.addInstruction(8, 0);
 
    // ----------
    // darwin 8x8
    // ----------

    try {
        cout << "*** Darwin 8x8 ***" << endl;
        /*
        8x8 Darwin
        Food,   facing east,  at (0, 0)
        Hopper, facing north, at (3, 3)
        Hopper, facing east,  at (3, 4)
        Hopper, facing south, at (4, 4)
        Hopper, facing west,  at (4, 3)
        Food,   facing north, at (7, 7)
        Simulate 5 moves.
        Print every grid.
        */
        Grid world = Grid(8,8);

        Creature f1 = Creature(food, 2);
        world.addCreature(f1, 0, 0);

        Creature h1 = Creature(hopper, 1);
        world.addCreature(h1, 3, 3);

        Creature h2 = Creature(hopper, 2);
        world.addCreature(h2, 4, 3);

        Creature h3 = Creature(hopper, 3);
        world.addCreature(h3, 4, 4);

        Creature h4 = Creature(hopper, 0);
        world.addCreature(h4, 3, 4);

        Creature f2 = Creature(food, 1);
        world.addCreature(f2, 7, 7);

        world.print();
          for(int i=0; i<5; ++i) {
            world.turn();
            world.print();
          }
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    // ----------
    // darwin 7x9
    // ----------

    try {
        cout << "*** Darwin 7x9 ***" << endl;
        srand(0);
        /*
        7x9 Darwin
        Trap,   facing south, at (0, 0)
        Hopper, facing east,  at (3, 2)
        Rover,  facing north, at (5, 4)
        Trap,   facing west,  at (6, 8)
        Simulate 5 moves.
        Print every grid.
        */
        Grid world = Grid(9,7);

        Creature t1 = Creature(trap, 3);
        world.addCreature(t1, 0, 0);

        Creature h1 = Creature(hopper, 2);
        world.addCreature(h1, 2, 3);

        Creature r1 = Creature(rover, 1);
        world.addCreature(r1, 4, 5);

        Creature t2 = Creature(trap, 0);
        world.addCreature(t2, 8, 6);
        
        world.print();
          for(int i=0; i<5; ++i) {
            world.turn();
            world.print();
          }
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    // ------------
    // darwin 72x72
    // without best
    // ------------

    try {
        cout << "*** Darwin 72x72 without Best ***" << endl;
        srand(0);
        /*
        Randomly place the following creatures facing randomly.
        Call rand(), mod it with 5184 (72x72), and use that for the position
        in a row-major order grid.
        Call rand() again, mod it with 4 and use that for it's direction with
        the ordering: west, north, east, south.
        Do that for each kind of creature.
        10 Food
        10 Hopper
        10 Rover
        10 Trap
        Simulate 1000 moves.
        Print the first 10 grids          (i.e. 0, 1, 2...9).
        Print every 100th grid after that (i.e. 100, 200, 300...1000).
        */
        Grid world = Grid(72,72);
        int seed;
        int dir;
        int x;
        int y;
        Creature crt;
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(food, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(hopper, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(rover, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(trap, dir);
          world.addCreature(crt, x, y);
        }
          world.print();
          for(int j=1; j<=1000; ++j) {
            world.turn();
            if(j <= 10)
              world.print();  
            if(j % 100 == 0)
              world.print();
          }        
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    // ------------
    // darwin 72x72
    // without best
    // ------------

    try {
        cout << "*** Darwin 72x72 without Best ***" << endl;
        srand(0);
        /*
        Randomly place the following creatures facing randomly.
        Call rand(), mod it with 5184 (72x72), and use that for the position
        in a row-major order grid.
        Call rand() again, mod it with 4 and use that for it's direction with
        the ordering: west, north, east, south.
        Do that for each kind of creature.
        10 Food
        10 Hopper
        10 Rover
        10 Trap
        Simulate 1000 moves.
        Print the first 10 grids          (i.e. 0, 1, 2...9).
        Print every 100th grid after that (i.e. 100, 200, 300...1000).
        */
        Grid world = Grid(72,72);
        int seed;
        int dir;
        int x;
        int y;
        Creature crt;
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(food, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(hopper, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(rover, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(trap, dir);
          world.addCreature(crt, x, y);
        }
        for(int i=0; i<10; ++i) {
          seed = rand() % 5184;
          y = seed / 72;
          x = seed % 72;
          dir = rand() % 4;
          crt = Creature(best, dir);
          world.addCreature(crt, x, y);
        }
          world.print();
          for(int j=1; j<=1000; ++j) {
            world.turn();
            if(j <= 10)
              world.print();  
            if(j % 100 == 0)
              world.print();
          }        
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    // ----------
    // darwin 8x8
    // ----------

    try {
        cout << "*** Trap Central ( 8 x 8 ) ***" << endl;
        /*
        8x8 Darwin
        rover,  facing east,  at (0, 0)
        rover,  facing south, at (0, 7)
        rover,  facing west,  at (7, 7)
        rover,  facing north, at (7, 0)
        trap,   facing north, at (3, 3)
        trap,   facing east,  at (3, 4)
        trap,   facing south, at (4, 4)
        trap,   facing west,  at (4, 3)
        Simulate 25 moves.
        Print every grid.
        */
        Grid world = Grid(8,8);

        Creature r1 = Creature(rover, 2);
        world.addCreature(r1, 0, 0);

        Creature r2 = Creature(rover, 3);
        world.addCreature(r2, 7,0);

        Creature r3 = Creature(rover, 0);
        world.addCreature(r3, 7,7);

        Creature r4 = Creature(rover, 1);
        world.addCreature(r4, 0, 7);

        Creature t1 = Creature(trap, 1);
        world.addCreature(t1, 3, 3);

        Creature t2 = Creature(trap, 2);
        world.addCreature(t2, 4, 3);

        Creature t3 = Creature(trap, 3);
        world.addCreature(t3, 4, 4);

        Creature t4 = Creature(trap, 0);
        world.addCreature(t4, 3, 4);

        world.print();
          for(int i=0; i<25; ++i) {
            world.turn();
            world.print();
          }
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    // ----------
    // darwin 16x16
    // ----------

    try {
        cout << "*** Trap Apocalypse ( 16 x 16 ) ***" << endl;
        /*
        8x8 Darwin
        rover,  facing east,  at (0, 0)
        rover,  facing south, at (0, 15)
        rover,  facing west,  at (15, 15)
        rover,  facing north, at (15, 0)

        trap,   facing north, at (6, 6)
        trap,   facing north, at (6, 7)
        trap,   facing north, at (6, 8)
        trap,   facing east,  at (6, 9)
        trap,   facing east,  at (7, 9)
        trap,   facing east,  at (8, 9)
        trap,   facing south, at (9, 9)
        trap,   facing south, at (9, 8)
        trap,   facing south, at (9, 7)
        trap,   facing west,  at (9, 6)
        trap,   facing west,  at (8, 6)
        trap,   facing west,  at (7, 6)

        hopper, facing south, at (0, 7)
        hopper, facing west,  at (7, 15)
        hopper, facing north, at (15, 7)
        hopper, facing east,  at (7,0)
        Simulate 50 moves.
        Print every 5th grid.
        */
        Grid world = Grid(16,16);

        Creature r1 = Creature(rover, 2);
        world.addCreature(r1, 0, 0);

        Creature r2 = Creature(rover, 3);
        world.addCreature(r2, 15,0);

        Creature r3 = Creature(rover, 0);
        world.addCreature(r3, 15,15);

        Creature r4 = Creature(rover, 1);
        world.addCreature(r4, 0, 15);

        Creature t1 = Creature(trap, 1);
        world.addCreature(t1, 6, 6);
        Creature t2 = Creature(trap, 1);
        world.addCreature(t2, 7, 6);
        Creature t3 = Creature(trap, 1);
        world.addCreature(t3, 8, 6);

        Creature t4 = Creature(trap, 2);
        world.addCreature(t4, 9, 6);
        Creature t5 = Creature(trap, 2);
        world.addCreature(t5, 9, 7);
        Creature t6 = Creature(trap, 2);
        world.addCreature(t6, 9, 8);

        Creature t7 = Creature(trap, 3);
        world.addCreature(t7, 9, 9);
        Creature t8 = Creature(trap, 3);
        world.addCreature(t8, 8, 9);
        Creature t9 = Creature(trap, 3);
        world.addCreature(t9, 7, 9);

        Creature t10 = Creature(trap, 0);
        world.addCreature(t10, 6, 9);
        Creature t11 = Creature(trap, 0);
        world.addCreature(t11, 6, 8);
        Creature t12 = Creature(trap, 0);
        world.addCreature(t12, 6, 7);

        Creature h1 = Creature(hopper, 3);
        world.addCreature(h1, 7, 0);
        Creature h2 = Creature(hopper, 0);
        world.addCreature(h2, 15, 7);
        Creature h3 = Creature(hopper, 1);
        world.addCreature(h3, 7, 15);
        Creature h4 = Creature(hopper, 2);
        world.addCreature(h4, 0, 7);
       
        world.print();
          for(int i=1; i<=50; ++i) {
            world.turn();
            if(i % 5 == 0)
              world.print();
          }
        }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

  // ------------
  // darwin 2x2: abrame13
  // ------------

  try {
    cout << "*** Darwin 2x2 ***" << endl;
    srand(0);
    /*
                try to place 1000 creatures
                there should only be 4 in the end
                print the result
                */

                Grid world = Grid(2, 2);
                int randN;
                int row, col;
                int dir;
                int i;

                // place traps
                for (i = 0; i < 1000; i++) {
                        randN = rand() % 4;
                        row = randN / 2;
                        col = randN % 2;
                        dir = rand() % 4;
                        world.addCreature(Creature(trap, dir), col, row);
                }

                // print the sim
                world.print();

  }
        catch (const invalid_argument&) {
          assert(false);}
        catch (const out_of_range&) {
                  assert(false);}

    // ----------
    // darwin 10x10: am43299
    // ----------        
        
      try {
        cout << "*** Darwin 10x10 ***" << endl;
        Grid x = Grid(10, 10);
        /*
        10x10 Darwin
        Food,   facing east,  at (0, 0)
        Food,   facing east,  at (9, 9)
        Food,   facing north, at (7, 7)
        Hopper, facing north, at (1, 1)
        Hopper, facing east,  at (3, 4)
        Rover, facing south, at (4, 4)
        Trap, facing west,  at (4, 3)
        Simulate 50 moves.
        Print first 5 then print every 5th grid.
        */
        
        Creature foodC1 = Creature(food, 2);
        Creature foodC2 = Creature(food, 2);
        Creature foodC3 = Creature(food, 1);
        Creature hopperC1 = Creature(hopper, 1);
        Creature hopperC2 = Creature(hopper, 2);        
        Creature roverC1 = Creature(rover, 3);
        Creature trapC1 = Creature(trap, 1);
        
        x.addCreature(foodC1, 0, 0);
        x.addCreature(foodC2, 9, 9);
        x.addCreature(foodC2, 7, 7);
        x.addCreature(hopperC1, 1, 1);
        x.addCreature(hopperC2, 4, 3);
        x.addCreature(roverC1, 4, 4);
        x.addCreature(trapC1, 3, 4);
        
        x.print();
        
        for(int i = 1; i <= 50; i++){
          x.turn();
          if(i <= 5 || (i % 5 == 0)){
            x.print();
          } 
        }
      }
    catch (const invalid_argument&) {
        assert(false);}
    catch (const out_of_range&) {
        assert(false);}

    return 0;}



