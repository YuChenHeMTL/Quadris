#include "GraphicsWindow.h"

#include <stdio.h>

#include <iostream>

GraphicsWindow::GraphicsWindow() {
  // Opens a connection to the server from the current environment
  display_ = XOpenDisplay(nullptr);
  if (display_ == nullptr) {
    std::cerr << "Connection to server failed" << std::endl;
    std::cerr << "If you see this message, please enter 'export DISPLAY=:0' into the terminal and run again" << std::endl;
    std::cerr << "In addition, if you are windows, ensure that both Xming and Xming fonts are installed" << std::endl;
    std::cerr << "You may also find important information on: http://www.straightrunning.com/XmingNotes/trouble.php" << std::endl;
    exit(1);
  }
  // Instantiates screenNum_, window_, and gc_ (Graphical context)
  // Using helper functions
  screenNum_ = DefaultScreen(display_);
  createWindow();
  createGC();
  drawGridLines();
}

GraphicsWindow::~GraphicsWindow() {
  // Free's the gc which prevents memory leaks
  XFreeGC(display_, gc_);
  // Close the connection
  XCloseDisplay(display_);
}

// Instantiates the window
void GraphicsWindow::createWindow() {
  // Gets value of white and black pixels
  unsigned long whitePixel = WhitePixel(display_, screenNum_);
  unsigned long blackPixel = BlackPixel(display_, screenNum_);

  // Sets the window width, height, and border width to be drawn
  // In pixels
  winWidth_ = 297;
  winHeight_ = 702;
  int winBorderWidth = 2;

  // Sets the width and height of each cell
  cellWidth_ = winWidth_ / 11;
  cellHeight_ = winHeight_ / 26;

  // Position of left top of window initially
  int topLeftStartX = 150;
  int topLeftStartY = 150;

  // Creates the window using the supplied parameters
  win_ = XCreateSimpleWindow(display_, RootWindow(display_, screenNum_),
                             topLeftStartX, topLeftStartY,
                             winWidth_, winHeight_, winBorderWidth,
                             blackPixel, whitePixel);

  // Makes window appear on screen
  XMapRaised(display_, win_);
}

// Instantiates the graphical context
void GraphicsWindow::createGC() {
  unsigned long valuemask = 0;
  XGCValues values;

  gc_ = XCreateGC(display_, win_, valuemask, &values);
  if (gc_ == nullptr) {
    std::cerr << "XCreateGC Fails" << std::endl;
    exit(1);
  }

  // Sets the foreground as black
  // All drawn objects will be black unless the foreground colour is changed
  XSetForeground(display_, gc_, BlackPixel(display_, screenNum_));
  // Sets the background as white
  XSetBackground(display_, gc_, WhitePixel(display_, screenNum_));
}

void GraphicsWindow::drawGridLines() {
  // Draws Horizontal Grid Lines on the Board
  for (int i = 4; i <= 22; i++) {
    drawLine(0, i * cellHeight_, winWidth_, i * cellHeight_, "Black");
  }

  // Draws Vertial Grid Lines on the Board
  for (int i = 0; i <= 12; i++) {
    drawLine(i * cellWidth_, 4 * cellHeight_, i * cellWidth_, 22 * cellHeight_, "Black");
  }
}

// Helper that draws a line from (x1,y1) to (x2,y2)
void GraphicsWindow::drawLine(int x1, int y1, int x2, int y2, std::string colour) {
  // Sets to current fill colour to the specified colour
  XSetForeground(display_, gc_, getColour(colour));

  // Draws the line
  XDrawLine(display_, win_, gc_, x1, y1, x2, y2);

  // Updates the display
  XFlush(display_);
}

// Adds the specified colour to the colourMap
void GraphicsWindow::addToColourMap(std::string colour) {
  XColor col;

  XParseColor(display_, DefaultColormap(display_, screenNum_), colour.c_str(), &col);
  XAllocColor(display_, DefaultColormap(display_, screenNum_), &col);

  // Sets the corresponding entry in the map to the colour's unsigned long id
  colourMap_[colour] = col.pixel;
}

// Helper that draws the cells at row,column using the specified colour
void GraphicsWindow::drawCell(int row, int column, std::string colour = "Black") {
  // If the colour is not currently in the colourMap, add it

  // Sets to current fill colour to the specified colour
  XSetForeground(display_, gc_, getColour(colour));

  // Draws the rectangle
  XFillRectangle(display_, win_, gc_, column * cellWidth_, (row + 4) * cellHeight_, cellWidth_, cellHeight_);

  // Updates the display
  XFlush(display_);
}

// Helper that draws the specified string at the specified x and y coordinates in the window
void GraphicsWindow::drawString(int x, int y, std::string s, std::string colour) {
  // Sets to current fill colour to the specified colour
  XSetForeground(display_, gc_, getColour(colour));

  // Draws the string
  XDrawString(display_, win_, gc_, x, y, s.c_str(), strlen(s.c_str()));

  // Updates the display
  XFlush(display_);
}

// Draws the current level on the screen
void GraphicsWindow::drawCurrentLevel(int level) {
  // Clears the previously drawn level
  for (int column = 0; column < 11; column++) {
    drawCell(-4, column, "White");
  }

  // Draws the level string
  drawString(cellWidth_ / 4, cellHeight_, "Level: " + std::to_string(level), "Black");
}

void GraphicsWindow::drawCurrentScore(int score) {
  // Clears the previously drawn score
  for (int column = 0; column < 11; column++) {
    drawCell(-3, column, "White");
  }

  // Draws the current score string
  drawString(cellWidth_ / 4, cellHeight_ * 2, "Score: " + std::to_string(score), "Black");
}

// Draws the high score on the screen
void GraphicsWindow::drawHighScore(int score) {
  for (int column = 0; column < 11; column++) {
    drawCell(-2, column, "White");
  }
  drawString(cellWidth_ / 4, cellHeight_ * 3, "High Score: " + std::to_string(score), "Black");
}

unsigned long GraphicsWindow::getColour(std::string colour) {
  // If the colour is not currently in the colourMap, add it
  if (!colourMap_.count(colour)) {
    addToColourMap(colour);
  }
  return colourMap_[colour];
}

// Drwas the next block on the screen
void GraphicsWindow::drawNextBlock(char c) {
  drawString(cellWidth_ / 4, cellHeight_ * 23, "Next Block: ", "Black");

  // Clears cells that display the next block
  drawCell(19, 3, "White");
  drawCell(19, 4, "White");
  drawCell(19, 5, "White");
  drawCell(19, 6, "White");
  drawCell(20, 3, "White");
  drawCell(20, 4, "White");
  drawCell(20, 5, "White");
  drawCell(20, 6, "White");

  // Draws next block
  switch (c) {
    case 'I':
      drawCell(20, 3, "Cyan");
      drawCell(20, 4, "Cyan");
      drawCell(20, 5, "Cyan");
      drawCell(20, 6, "Cyan");
      break;
    case 'O':
      drawCell(19, 3, "Yellow");
      drawCell(19, 4, "Yellow");
      drawCell(20, 3, "Yellow");
      drawCell(20, 4, "Yellow");
      break;
    case 'J':
      drawCell(19, 3, "Dark Blue");
      drawCell(20, 3, "Dark Blue");
      drawCell(20, 4, "Dark Blue");
      drawCell(20, 5, "Dark Blue");
      break;
    case 'Z':
      drawCell(19, 3, "Red");
      drawCell(19, 4, "Red");
      drawCell(20, 4, "Red");
      drawCell(20, 5, "Red");
      break;
    case 'S':
      drawCell(20, 3, "Green");
      drawCell(20, 4, "Green");
      drawCell(19, 4, "Green");
      drawCell(19, 5, "Green");
      break;
    case 'L':
      drawCell(20, 3, "Dark Orange");
      drawCell(20, 4, "Dark Orange");
      drawCell(20, 5, "Dark Orange");
      drawCell(19, 5, "Dark Orange");
      break;
    case 'T':
      drawCell(19, 3, "Purple");
      drawCell(19, 4, "Purple");
      drawCell(19, 5, "Purple");
      drawCell(20, 4, "Purple");
      break;
    default:
      std::cout << "Invalid next block in graphicsWindow" << std::endl;
      break;
  }
}