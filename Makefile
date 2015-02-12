.PHONY : all clean binary library test summary depend cleanall

AR = ar
CC = g++
CFLAGS = -Wall -fPIC -D__STDC_FORMAT_MACROS
ARFLAGS = r
LDFLAGS = -lpthread -lm
INCLUDE = -Isrc -IDEPEND/include
LIBRARY = -LDEPEND/lib
DEPEND = DEPEND
BUILD = BUILD
INSTALL = INSTALL
GTEST_LIB = -lgtest_main -lgtest
LUA_LIB = $(DEPEND)/lua/src/liblua.a
LUA_FLAGS = MYCFLAGS=-fPIC

### MODE options ###
ifeq ($(mode),release)
    CFLAGS += -O2 -fno-strict-aliasing -DNDEBUG
    LDFLAGS += -O2 -DNDEBUG
else
    mode = debug
    CFLAGS += -g -D_DEBUG
    LDFLAGS += -g -D_DEBUG
endif

###  OTHER options ###
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')
ifeq ($(uname_S), Darwin)
    SHARED = -dynamiclib -fPIC
	LDFLAGS += -Wl,-undefined,dynamic_lookup
	LUA_FLAGS += macosx
else
    SHARED = --shared -fPIC
    LDFLAGS += -lrt -Wl,-E
	LUA_FLAGS += linux
endif

ifneq ($(uname_S), FreeBSD)
    LDFLAGS += -ldl
endif

### BUILD TAGET ###
LUA_ENGINE_BIN = $(INSTALL)/bin/lua_engine
LUA_ENGINE_AR = $(INSTALL)/lib/liblua_engine.a

all:depend binary summary

clean:
	rm -rf $(INSTALL)/bin
	rm -rf $(INSTALL)/lib
	rm -rf $(BUILD)/*.o

cleanall : clean
	make -C $(DEPEND)/lua clean

binary:$(LUA_ENGINE_BIN)

depend:
	make -C $(DEPEND)/lua $(LUA_FLAGS)

summary:
	@echo ----------------- [options] --------------------
	@echo MODE = ${mode}
	@echo CFLAGS = ${CFLAGS}
	@echo LDFLAGS = ${LDFLAGS}
	@echo INCLUDE = ${INCLUDE}
	@echo LIBRARY = ${LIBRARY}
	@echo GTEST_LIB = ${GTEST_LIB}
	@echo LUA_LIB = ${LUA_LIB}
	@echo ------------------------------------------------

LUA_ENGINE_OBJ = $(BUILD)/lua_engine.o
LUA_MAIN_OBJ = $(BUILD)/main.o

$(LUA_ENGINE_BIN): $(LUA_ENGINE_OBJ) $(LUA_MAIN_OBJ) $(LUA_LIB)
	mkdir -p $(INSTALL)/bin
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LIBRARY) $(LDFLAGS)

$(BUILD)/lua_engine.o: src/lua_engine.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)

$(BUILD)/main.o: src/main.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)
