
CC := gcc
# CCFLAGS := -std=c11 -Wfatal-errors -Wall -Wextra -Os -s
CCFLAGS := -std=c11 -Wfatal-errors -Wall -Wextra -O3
LDFLAGS += -lm -lSDL2

OUTPUT := engine
$(OUTPUT): $(wildcard src/*.c)
	$(CC) $(CCFLAGS) $^ -o $@ $(LDFLAGS)

