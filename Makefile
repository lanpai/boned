ifeq ($(CXX),)
	CXX := clang++
endif
ifeq ($(BUILD_DIR),)
	BUILD_DIR := build
endif
ifeq ($(OBJ_DIR),)
	OBJ_DIR := obj
endif

BINARY := skeleton
OBJS := SkeletonChess.o SkeletonShip.o SkeletonC4.o SkeletonTTT.o BossScreen.o StartScreen.o VictoryScreen.o DefeatScreen.o Map.o Game.o Graphics.o main.o

output: $(OBJS)
	@mkdir -p $(BUILD_DIR)
	cd $(OBJ_DIR); $(CXX) $(CCFLAGS) -std=c++17 -Wall -o ../$(BUILD_DIR)/$(BINARY) $(OBJS)

$(OBJS): %.o: %.cpp
	@mkdir -p ${OBJ_DIR};
	cd $(OBJ_DIR); $(CXX) $(CCFLAGS) -std=c++17 -Wall -c ../$<

run:
	$(BUILD_DIR)/$(BINARY)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
