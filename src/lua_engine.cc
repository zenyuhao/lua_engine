#include <string>
#include "lua_engine.h"

LuaEngine::LuaEngine():_L(NULL){
    // preload
    _L = luaL_newstate();
    luaL_openlibs(_L);
    //lua_close(_L);
    //_L = luaL_newstate();
}

LuaEngine::~LuaEngine(){
    if(_L){
        lua_close(_L);
        _L = NULL;
    }
}


bool LuaEngine::execFunc(int args){

    int func_index = - ( args + 1 );

    if (!lua_isfunction(_L, func_index)){
        fprintf(stderr,"first stack is not a function\n");
        lua_pop(_L, args + 1);                // pop func and args
        return false;
    }

    int traceback = 0;
    lua_getglobal(_L, "__TRACKBACK__");       /* L: ... func arg1 arg2 ... G */
    if (!lua_isfunction(_L, -1)){
        fprintf(stderr,"not found trackback %s \n", lua_tostring(_L, -1));
        lua_pop(_L, 1);                       /* L: ... func arg1 arg2 ... */
    }
    else{
        fprintf(stderr,"found trackback\n");
        lua_insert(_L, func_index - 1);       /* L: ... G func arg1 arg2 ... */
        traceback = func_index - 1;
    }

    int error = 0;
    error = lua_pcall(_L, args, 1, traceback);   /* L: ... [G] ret */
    if (error){
        if (traceback == 0){
            fprintf(stderr,"execute func error [%s].\n", lua_tostring(_L, - 1));
            lua_pop(_L, 1); // remove error message from stack
        }
        else{                                    /* L: ... [G] error */
            lua_pop(_L, 2);
        }
        return false;
    }

    // get return value , with no return
    int ret = 0;
    if (lua_isnumber(_L, -1)){
        ret = (int)lua_tointeger(_L, -1);
    }
    else if (lua_isboolean(_L, -1)){
        ret = (int)lua_toboolean(_L, -1);
    }

    // remove return value from stack
    lua_pop(_L, 1);                                                /* L: ... [G] */
    if (traceback){
        lua_pop(_L, 1); // remove __TRACKBACK__ from stack      /* L: ... */
    }

    return true;
}

bool LuaEngine::execCode(const char* codes){
    luaL_loadstring(_L, codes);
    return execFunc(0); // no arguments
}

bool LuaEngine::execFile(const char* path){
    std::string codes("require \"");
    codes.append(path);
    codes.append("\"");
    return execCode(codes.c_str());
}

void LuaEngine::addPackagePath(const char* path){

    lua_getglobal(_L, "package");
    lua_getfield(_L, -1, "path");
    const char* cur_path =  lua_tostring(_L, -1);
    lua_pushfstring(_L, "%s;%s/?.lua", cur_path, path);
    lua_setfield(_L, -3, "path");
    lua_pop(_L, 2);

}
