
CXXFLAGS = -w -I ./inc
LINKER_FLAGS = -lSDL2 -lSDL2_ttf

VPATH = ./src

obj = main.o Game.o GamePlay.o
exec = game

$(exec): $(obj)
	$(CXX) $(obj) $(LINKER_FLAGS) -o $@

$(obj): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -f $(obj) $(exec)