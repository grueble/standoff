# makes use of implicit rules
# -> https://codereview.stackexchange.com/questions/78855/makefile-for-c-opengl-with-glfw-and-glad
# -> https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html

# the compiler
CXX=g++

# include and lib paths | NOTE: string together unique paths for more flexibility
# -> use x86 for MinGW32 and x64 for cygwin64
INCL_PATH=-ID:/dev/dev_lib/include
LIB_PATH=-LD:/dev/dev_lib/lib/x64 

# the flags to pass to the compiler/linker | NOTE: using standard names to leverage implicit rules
CXXFLAGS=-std=c++14 -w -Wall -g -ggdb3 $(INCL_PATH)
LDFLAGS=$(LIB_PATH) -Wl,-subsystem,windows -lSDL2main -lSDL2 -lSDL2_image -mwindows

BUILD_DIR=../build

NET_OBJS=$(BUILD_DIR)/Address.o $(BUILD_DIR)/ConnectHandler.o $(BUILD_DIR)/LSocket.o
GAME_OBJS=$(BUILD_DIR)/Game.o $(BUILD_DIR)/Piece.o
SDL_OBJS=$(BUILD_DIR)/LTexture.o $(BUILD_DIR)/ResourceManager.o

CLIENT=$(BUILD_DIR)/client.exe
CLI_OBJS=$(BUILD_DIR)/StandoffClient.o $(BUILD_DIR)/StandoffApp.o $(NET_OBJS) $(GAME_OBJS) $(SDL_OBJS)

SERVER=$(BUILD_DIR)/server.exe
SERV_OBJS=$(BUILD_DIR)/StandoffServer.o $(NET_OBJS) $(GAME_OBJS)

TARGET=$(CLIENT) $(SERVER)
OBJS=$(wildcard *.o)

all: $(TARGET)

$(CLIENT): $(CLI_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(SERVER): $(SERV_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	-rm $(CLI_OBJS) $(BUILD_DIR)/StandoffServer.o
