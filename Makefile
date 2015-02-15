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
LUA_ENGINE_LIB = $(INSTALL)/lib/liblua_engine.so

all:depend binary library summary

clean:
	rm -rf $(INSTALL)/bin
	rm -rf $(INSTALL)/lib
	rm -rf $(BUILD)/*.o

cleanall : clean
	make -C $(DEPEND)/lua clean

binary:$(LUA_ENGINE_BIN)

library:$(LUA_ENGINE_LIB)

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

LUA_ENGINE_BIN_OBJ = $(BUILD)/lua_engine.o \
                     $(BUILD)/main.o

LUA_ENGINE_LIB_OBJ = $(BUILD)/base_object.o \
                     $(BUILD)/object_proxy.o \
                     $(BUILD)/libentry.o

$(LUA_ENGINE_BIN): $(LUA_ENGINE_BIN_OBJ) $(LUA_LIB)
	mkdir -p $(INSTALL)/bin
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAGS)

$(LUA_ENGINE_LIB): $(LUA_ENGINE_LIB_OBJ) $(LUA_LIB)
	mkdir -p $(INSTALL)/lib
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(SHARED)

### LUA ENGINE ###
$(BUILD)/lua_engine.o: src/lua_engine.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)

$(BUILD)/main.o: src/main.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)

$(BUILD)/base_object.o: src/base_object.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)

$(BUILD)/object_proxy.o: src/object_proxy.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)

$(BUILD)/libentry.o: src/libentry.cc
	$(CC) -c -o $@ $^  $(CFLAGS)  $(INCLUDE)
