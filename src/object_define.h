/**
 * @file   object_define.h
 * @author Zen <zen@wunder.local>
 * @date   Fri Feb 13 16:36:06 2015
 *
 * @brief
 *
 *
 */

#ifndef _OBJECT_DEFINE_
#define _OBJECT_DEFINE_

extern "C"{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class BaseObjectWrapper;
typedef int (BaseObjectWrapper::*lua_function)(lua_State* L);

struct ObjectContext{
    ObjectContext(const char* n, lua_function f){
        func_name = n;
        func_body = f;
    }
    const char*   func_name;
    lua_function  func_body;
};


#endif
