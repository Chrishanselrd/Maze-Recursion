//
// Created by Yusuf Pisan on 4/18/18.
//

#include "maze.h"
#include <algorithm>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// print maze object
ostream &operator<<(ostream &out, const Maze &maze) {
  for (int row = 0; row < maze.height; ++row) {
    for (int col = 0; col < maze.width; ++col) {
      out << maze.field[row][col];
    }
    out << endl;
  }
  out << endl;
  return out;
}

// default constructor
Maze::Maze() = default;

// Load maze file from current directory
bool Maze::load(const string &fileName) {
  ifstream inFile;
  inFile.open(fileName);
  if (!inFile.is_open()) {
    cerr << "Unable to open file: " << fileName << endl;
    // next line is just to satisfy clang-tidy
    // otherwise it complains saying function can be const
    width = height = 0;
    return false;
  }
  inFile >> width >> height;
  inFile >> exitRow >> exitColumn;
  inFile >> startRow >> startColumn;
  // ignore leftover charcaters up to newline
  inFile.ignore(INT_MAX, '\n');
  string line;
  for (int row = 0; row < height; ++row) {
    getline(inFile, line);
    field.push_back(line);
  }
  return true;
}

// // true if maze can be solved
bool Maze::solve() {
  // pluging in the starting value
  currR = startRow;
  currC = startColumn;

  // if the point is at the end
  if (atExit(currR, currC)) {
    return true;
  }

  // checks if the box is empty or not
  if (!isClear(currR, currC)) {
    return false;
  }

  // mark starting path
  markAsPath(currR, currC);

  // returns if one of the path is true
  return (goNorth(currR, currC) || goSouth(currR, currC) ||
          goEast(currR, currC) || goWest(currR, currC));
}

// path to exit
string Maze::getPath() const { return path; }

// true if row, column is the exit
bool Maze::atExit(int row, int column) const {
  return row == exitRow && column == exitColumn;
}

// true if row, column is inside the maze
bool Maze::isInside(int row, int col) const {
  return row >= 0 && col >= 0 && row < field.size() && col < field[row].size();
}

// true if row, column is clear to move
bool Maze::isClear(int row, int col) const {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  return field[row][col] == CLEAR;
}

// mark location as part of the path to exit
void Maze::markAsPath(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = PATH;
}

// mark location as visited, not part of the path to exit
void Maze::markAsVisited(int row, int col) {
  assert(row >= 0 && col >= 0 && row < field.size() && col < field[row].size());
  field[row][col] = VISITED;
}

// the path will go north or up
bool Maze::goNorth(int row, int col) {
  // checks if north is inside the maze and empty
  // if true move north and mark as path
  if (isInside(row - 1, col) && isClear(row - 1, col)) {
    currR -= 1;
    markAsPath(currR, currC);
    path.push_back('N');

    // if north is at the exit
    if (atExit(currR, currC)) {
      return true;
    }

    // recursion for finding a dead end
    //  and will back track if it is a dead end
    success = goNorth(currR, currC);
    if (!success) {
      success = goWest(currR, currC);
      if (!success) {
        success = goEast(currR, currC);
        if (!success) {
          markAsVisited(currR, currC);
          path.pop_back();
          currR += 1;
          return false;
        }
      }
    }
    return true;
  }
  return false;
}

// the path will go south or down
bool Maze::goSouth(int row, int col) {
  // checks if south is inside the maze and empty
  // if true move south and mark as path
  if (isInside(row + 1, col) && isClear(row + 1, col)) {
    currR += 1;
    markAsPath(currR, currC);
    path.push_back('S');

    // if south is an exit
    if (atExit(currR, currC)) {
      return true;
    }

    // recursion for finding a dead end
    //  and will back track if it is a dead end
    success = goSouth(currR, currC);
    if (!success) {
      success = goWest(currR, currC);
      if (!success) {
        success = goEast(currR, currC);
        if (!success) {
          markAsVisited(currR, currC);
          path.pop_back();
          currR -= 1;
          return false;
        }
      }
    }
    return true;
  }
  return false;
}

// the path will go west or left
bool Maze::goWest(int row, int col) {
  // checks if west is inside the maze and empty
  // if true move west and mark as path
  if (isInside(row, col - 1) && isClear(row, col - 1)) {
    currC -= 1;
    markAsPath(currR, currC);
    path.push_back('W');

    // if east is an exit
    if (atExit(currR, currC)) {
      return true;
    }

    // recursion for finding a dead end
    //  and will back track if it is a dead end
    success = goWest(currR, currC);
    if (!success) {
      success = goNorth(currR, currC);
      if (!success) {
        success = goSouth(currR, currC);
        if (!success) {
          markAsVisited(currR, currC);
          path.pop_back();
          currC += 1;
          return false;
        }
      }
    }
    return true;
  }
  return false;
}

// the path will go east or right
bool Maze::goEast(int row, int col) {
  // checks if east is inside the maze and empty
  // if true move east and mark as path
  if (isInside(row, col + 1) && isClear(row, col + 1)) {
    currC += 1;
    markAsPath(currR, currC);
    path.push_back('E');

    // if east is an exit
    if (atExit(currR, currC)) {
      success = true;
    }

    // recursion for finding a dead end
    //  and will back track if it is a dead end
    success = goEast(currR, currC);
    if (!success) {
      success = goNorth(currR, currC);
      if (!success) {
        success = goSouth(currR, currC);
        if (!success) {
          markAsVisited(currR, currC);
          path.pop_back();
          currC -= 1;
          return false;
        }
      }
    }
    return true;
  }
  return false;
}
