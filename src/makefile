# the type of compiler to use
CC=g++ -ID:/dev/vs_dev_lib/include -LD:/dev/vs_dev_lib/lib/x86

# the flags to pass to the compiler
CFLAGS=-std=c++14 -c -Wall -w

# the flags to pass to the linker
LFLAGS=-Wall

# libraries to link with
LIBS=-lSDL2main -lSDL2 -lSDL2_image

# project directories
BUILD_DIR=../build
CLIENT_DIR=StandoffClient
SERVER_DIR=StandoffServer
SDLF_DIR=ResourceManager
NET_DIR=ConnectHandler
GAME_DIR=Game

# relevant subdirectories grouped by generated executable
CORE_SUBDIRS=$(NET_DIR) $(GAME_DIR)
CLIENT_SUBDIRS=$(CLIENT_DIR) $(SDLF_DIR) $(CORE_SUBDIRS)
SERVER_SUBDIRS=$(SERVER_DIR) $(CORE_SUBDIRS)

# pattern match for source code files
CLIENT_SRCS=$(foreach dir,$(CLIENT_SUBDIRS),$(wildcard $(dir)/*.cpp))
SERVER_SRCS=$(foreach dir,$(SERVER_SUBDIRS),$(wildcard $(dir)/*.cpp))

# pattern match search for built objects
CLIENT_OBJS=$(CLIENT_SRCS:.cpp=.o)
SERVER_OBJS=$(SERVER_SRCS:.cpp=.o)

# source paths used to build specific objects
CLIENT=StandoffClient/StandoffClient.cpp
SERVER=StandoffServer/StandoffServer.cpp
APP=StandoffClient/StandoffApp.cpp
SDLF=ResourceManager/ResourceManager.cpp
LTEXT=ResourceManager/LTexture.cpp
NET=ConnectHandler/ConnectHandler.cpp
LSOCK=ConnectHandler/LSocket.cpp
ADDR=ConnectHandler/Address.cpp
GAME=Game/Game.cpp
PIECE=Game/Piece.cpp

# header paths used to build specific objects
CLIENTH=StandoffClient/StandoffClient.hpp
SERVERH=StandoffServer/StandoffServer.hpp
APPH=StandoffClient/StandoffApp.hpp
SDLFH=ResourceManager/ResourceManager.hpp
LTEXTH=ResourceManager/LTexture.hpp
NETH=ConnectHandler/ConnectHandler.hpp
LSOCKH=ConnectHandler/LSocket.hpp
ADDRH=ConnectHandler/Address.hpp
GAMEH=Game/Game.hpp
PIECEH=Game/Piece.hpp

all: $(BUILD_DIR)/client $(BUILD_DIR)/server

$(BUILD_DIR)/client: $(CLIENT_OBJS)
	$(CC) $(LFLAGS) -o $@ $(CLIENT_OBJS) $(LIBS)

$(BUILD_DIR)/server: $(SERVER_OBJS)
	$(CC) $(LFLAGS) -o $@ $(SERVER_OBJS)  $(LIBS)

$(CLIENT_DIR)/StandoffClient.o: $(CLIENT) $(CLIENTH) $(APPH) $(NETH)
	$(CC) $(CFLAGS) $< -o $@

$(SERVER_DIR)/StandoffServer.o: $(SERVER) $(SERVERH) $(NETH) $(GAMEH)
	$(CC) $(CFLAGS) $< -o $@

$(CLIENT_DIR)/StandoffApp.o: $(APP) $(APPH) $(NETH) $(GAMEH) $(SDLFH) 
	$(CC) $(CFLAGS) $< -o $@

$(SDLF_DIR)/ResourceManager.o: $(SDLF) $(SDLFH) $(LTEXTH)
	$(CC) $(CFLAGS) $< -o $@

$(SDLF_DIR)/LTexture.o: $(LTEXT) $(LTEXTH)
	$(CC) $(CFLAGS) $< -o $@

$(NET_DIR)/ConnectHandler.o: $(NET) $(NETH) $(LSOCKH)
	$(CC) $(CFLAGS) $< -o $@

$(NET_DIR)/LSocket.o: $(LSOCK) $(LSOCKH) $(ADDRH)
	$(CC) $(CFLAGS) $< -o $@

$(NET_DIR)/Address.o: $(ADDR) $(ADDRH)
	$(CC) $(CFLAGS) $< -o $@

$(GAME_DIR)/Game.o: $(GAME) $(GAMEH) $(PIECEH)
	$(CC) $(CFLAGS) $< -o $@

$(GAME_DIR)/Piece.o: $(PIECE) $(PIECEH)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(BUILD_DIR)/client $(BUILD_DIR)/server