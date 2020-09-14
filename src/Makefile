CXX = g++
CXXFLAGS = --std=c++11 -g -Wall -MMD -I/opt/X11/include -L/usr/X11/lib
OBJECTS = main.o CommandController.o Game.o TextDisplay.o Subject.o Observer.o Cell.o Level.o Board.o Block.o BlockFactory.o OBlock.o JBlock.o SBlock.o ZBlock.o TBlock.o LBlock.o IBlock.o Block1X1.o GraphicDisplay.o GraphicsWindow.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = quadris

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11
clean : 
	rm -rd ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}