/**
 * @file   engine.h
 * @author Zen <zen@wunder.local>
 * @date   Thu Feb 12 16:34:11 2015
 *
 * @brief
 *
 *
 */

#ifndef _LUA_ENGINE_
#define _LUA_ENGINE_

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class LuaEngine {
 public: // constructor destructor
    LuaEngine();
    ~LuaEngine();

 public: // execute function
    bool execFunc(int args);
    bool execCode(const char* codes);
    bool execFile(const char* path);

 public: // access function
    lua_State* getState();
    void addPackagePath(const char* path);

 private:
    lua_State* _L;

};


#endif
