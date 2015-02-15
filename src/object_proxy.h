/**
 * @file   object_proxy.h
 * @author Zen <zen@wunder.local>
 * @date   Fri Feb 13 16:01:03 2015
 *
 * @brief
 *
 *
 */

#ifndef _OBJECT_PROXY_
#define _OBJECT_PROXY_

#include "object_define.h"
    struct lua_State;

class ObjectProxy {
 public:
    static void Register(lua_State* L);
    static int CallProxy(lua_State* L);
    static int Create(lua_State* L);
};

#endif
