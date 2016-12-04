#ifndef LUA_HPP
#define LUA_HPP

extern "C" {

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

}

#include "luna.hpp"
#include <string>

namespace alt {

class lua {

    private:

        lua_State *L;

    public:

        lua();
        ~lua();
        lua_State *get_state() { return this->L; }

        std::string get_table_string(const char *key);

};

}

#endif
