/**
 * @file   base_object.h
 * @author Zen <zen@wunder.local>
 * @date   Fri Feb 13 15:59:03 2015
 *
 * @brief
 *
 *
 */

#ifndef _BASE_OBJECT_
#define _BASE_OBJECT_

#include <string>
#include "object_define.h"

class BaseObject {

 public:

    void setV(std::string v){
        _v = v;
    }

    std::string getV(){
        return _v;
    }

    void print(){
        fprintf(stderr, "v is %s\n", _v.c_str());
    }


 private:
    std::string _v;
};

class BaseObjectWrapper : public BaseObject{

 public:
    int setV(lua_State* L){
        const char* v = luaL_checkstring(L, -1);
        BaseObject::setV(std::string(v));
        return 0;
    }

    int getV(lua_State* L){
        lua_pushstring(L, BaseObject::getV().c_str());
        return 1;
    }

    int print(lua_State* L){
        BaseObject::print();
        return 0;
    }

 public:
    static ObjectContext Functions[3];

};

#endif
