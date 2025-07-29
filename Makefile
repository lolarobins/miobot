# miobot (c) lolasnotunix 2025 under MIT license
#
# uses concord library for using the discord api
# https://github.com/Cogmasters/concord
#
# primarily targetted for darwin, should compile
# for linux, bsd, and wsl just fine
#
# for the sputnik supporters discord server

CC=clang
CFLAGS=-O2 -g -Wall -pthread
LDFLAGS=-lcurl -ldiscord
TARGET=miobot
OBJS=build/main.o

target: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET)

build:
	@-mkdir -p build

build/%.o: build src/%.c
	$(CC) $(CFLAGS) -c $(lastword $^) -o $@

clean:
	rm -rf build
