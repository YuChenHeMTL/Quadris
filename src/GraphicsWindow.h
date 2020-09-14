#ifndef _GRAPHICSWINDOW_
#define _GRAPHICSWINDOW_

#include <X11/Xlib.h>
#include <string.h>

#include <map>
#include <string>

class GraphicsWindow {
 public:
  GraphicsWindow();
  ~GraphicsWindow();

  void drawCell(int, int, std::string);
  void drawCurrentScore(int);
  void drawHighScore(int);
  void drawCurrentLevel(int);
  void drawNextBlock(char);
  void drawGridLines();

  void drawString(int, int, std::string, std::string);

 private:
  // Private helpers
  void createWindow();
  void createGC();
  void drawLine(int, int, int, int, std::string);
  void addToColourMap(std::string);
  unsigned long getColour(std::string);

  GC gc_;  // Graphical Context
  Display* display_;
  Window win_;

  int screenNum_;  // Current screen

  // Width and height of each cell in pixels
  int cellWidth_;
  int cellHeight_;

  // Window width and height
  int winHeight_;
  int winWidth_;

  std::map<std::string, unsigned long> colourMap_;
};

#endif