#include <iostream> // cout, endl
#include <sstream>  // istringtstream, ostringstream
#include <string>   // ==
#include <vector>
#include <cassert>

#include "gtest/gtest.h"

#include "Darwin.h"

TEST(Darwin, instructions) {
  Species proto('p');
  proto.addInstruction(3, 0);
  vector<int> progI = proto._prog[0];
  ASSERT_EQ(progI[0], 1);
}

TEST(Darwin, instructions2) {
  Species proto('p');
  proto.addInstruction(8, 0);
  vector<int> progI = proto._prog[0];
  ASSERT_EQ(progI[1], 0);
}

TEST(Darwin, instructions3) {
  Species proto('p');
  proto.addInstruction(2, 0);
  vector<int> progI = proto.progInstruction(0);
  ASSERT_EQ(progI[0], 1);
}

Test(Darwin, turn) {
  Species p('p');
  Creature c(p, 0);
  c.left();
  ASSERT_EQ(c._dir, 3);
}

Test(Darwin, turn2) {
  Species p('p');
  Creature c(p, 0);
  c.right();
  ASSERT_EQ(c._dir, 1);
}


Test(Darwin, turn3) {
  Species p('p');
  Creature c(p, 0);
  c.left();
  c.right();
  ASSERT_EQ(c._dir, 0);
}

Test(Darwin, infect) {
  Species p('p');
  Creature c(p, 0);
  Species l('l');
  c.infection(l);
  ASSERT_EQ(c._spc._rep, l._rep);
}

Test(Darwin, infect2) {
  Species p('p');
  Creature c(p, 0);
  Species l('l');
  c.infection(l);
  c.infection(p);
  ASSERT_EQ(c._spc._rep, p._rep);
}

Test(Darwin, addCreature) {
  Species p('p');
  Creature c(p, 0);
  Grid g(3, 3);
  g.addCreature(c, 1, 1);
  ASSERT_EQ(g._crts[0]._spc._rep, c._spc._rep);
}

Test(Darwin, addCreature2) {
  Species p('p');
  Species k('k');
  Creature c(p, 0);
  Creature c2(k, 0);
  Grid g(3, 3);
  g.addCreature(c, 1, 1);
  g.addCreature(c2, 2, 2);
  ASSERT_EQ(g._crts[1]._spc._rep, c2._spc._rep);
}

Test(Darwin, addCreature3) {
  Species p('p');
  Creature c(p, 0);
  Grid g(3, 3);
  g.addCreature(c, 1, 1);
  ASSERT_EQ(g._crts[0]._dir, c._dir);
}

Test(Darwin, findCreature) {
  Species p('p');
  Creature c(p, 0);
  Grid g(3, 3);
  g.addCreature(c, 1, 1);
  int index = g.findCreature(1, 1);
  ASSERT_EQ(index, 0);
}

Test(Darwin, findCreature2) {
  Species p('p');
  Creature c(p, 0);
  Creature c2(p, 3);
  Creature c3(p, 2);
  Grid g(8, 8);
  g.addCreature(c, 0, 0);
  g.addCreature(c2, 4, 4);
  g.addCreature(c3, 0, 7);
  int index = g.findCreature(0, 1);
  ASSERT_EQ(index, (-1));
}

Test(Darwin, findCreature3) {
  Species p('p');
  Creature c(p, 0);
  Creature c2(p, 3);
  Creature c3(p, 2);
  Grid g(8, 8);
  g.addCreature(c, 0, 0);
  g.addCreature(c2, 4, 4);
  g.addCreature(c3, 0, 7);
  int index = g.findCreature(4, 4);
  ASSERT_EQ(g._crts[index]._dir, c2._dir);
}

