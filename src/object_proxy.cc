#include "object_proxy.h"
#include "base_object.h"


void ObjectProxy::Register(lua_State* L){

    /*
       sp()
    */
    lua_pushcfunction(L, ObjectProxy::Create);
    lua_setglobal(L,  "SP");

    /*
       sp = { }
    */
    luaL_newmetatable(L, "SP");

    /*
       sp = {
       setV = &ObjectProxy::CallPorxy,
       getV = &ObjectProxy::CallPorxy,
       print = &ObjectProxy::CallPorxy
       }

     */
    lua_pushstring(L, "setV");
    lua_pushnumber(L, 0);
    lua_pushcclosure(L, &ObjectProxy::CallProxy, 1);
    lua_settable(L, -3);

    lua_pushstring(L, "getV");
    lua_pushnumber(L, 1);
    lua_pushcclosure(L, &ObjectProxy::CallProxy, 1);
    lua_settable(L, -3);

    lua_pushstring(L, "print");
    lua_pushnumber(L, 2);
    lua_pushcclosure(L, &ObjectProxy::CallProxy, 1);
    lua_settable(L, -3);

    /*
      sp_meta.__index = sp_meta
     */
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);
}

int ObjectProxy::Create(lua_State* L){

    BaseObjectWrapper* obj = new BaseObjectWrapper();

    /* ... userdata ...*/
    BaseObjectWrapper** a = (BaseObjectWrapper**)lua_newuserdata(L, sizeof(BaseObjectWrapper*));
    *a = obj;

    /* ... sp_mata, userdata ...*/
    luaL_getmetatable(L, "SP");

    /* ... userdata ...*/
    lua_setmetatable(L, -2);
    return 1;
}

int ObjectProxy::CallProxy(lua_State* L){

    int i = (int)lua_tonumber(L, lua_upvalueindex(1));
    BaseObjectWrapper** obj = (BaseObjectWrapper**)luaL_checkudata(L, 1, "SP");
    return ((*obj)->*(BaseObjectWrapper::Functions[i].func_body))(L);

}
