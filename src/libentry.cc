#include "object_proxy.h"

extern "C"{

    int luaopen_liblua_engine(lua_State *L) {
        ObjectProxy::Register(L);
        return 1;
    }

}
