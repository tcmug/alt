
#include "lua.hpp"

using alt::lua;



class Foo {
  public:

    Foo(lua_State *L) {
      printf("in constructor %s\n", lua_tostring(L, -1));
    }

    ~Foo() {
      printf("in destructor\n");
    }

    int foo(lua_State *L) {
      printf("in foo\n");
    }

    static const char className[];
    static const Luna<Foo>::RegType Register[];

};

const char Foo::className[] = "Foo";
const Luna<Foo>::RegType Foo::Register[] = {
  { "foo", &Foo::foo },
  { 0 }
};


lua::lua() {

	L = luaL_newstate();

	if (L)
		printf("Lua VM started\n");


	luaL_requiref(L, "io", luaopen_io, 1);
	luaL_requiref(L, "base", luaopen_base, 1);
	luaL_requiref(L, "math", luaopen_math, 1);
	luaL_requiref(L, "table", luaopen_table, 1);
	luaL_requiref(L, "string", luaopen_string, 1);

	Luna <Foo>::Register(L);
	// luaopen_base(L);
	// luaopen_string(L);
	// luaopen_math(L);
	// luaopen_package(L);
	// luaopen_coroutine(L);
	// luaopen_table(L);
	// luaopen_io(L);
	// luaopen_os(L);
	// luaopen_utf8(L);
	// luaopen_debug(L);

	if (luaL_dofile(L, "preferences.lua") != 0) {
		printf("%s\n", lua_tostring(L, -1));
		return;
	}

	// luaL_requiref(L, "alt", luaopen_alt, 1);
	// lua_pop(L, 1);

	// return 1;
	// /*
	// luaL_dostring(L, "a=alt.print()");
	// lua_getglobal(L, "a");
	// int i = lua_tointeger(L, -1);
	// printf("Got %u\n", i);
	// lvm = L;

/*	lua_getglobal(L,"alt");
	if (lua_isnil(L, -1)) {
		printf("No alt\n");
		return;
	}
	lua_pushstring(L,"syntax");
	lua_gettable(L,-2);
	if (lua_isnil(L, -1)) {
		printf("No alt.syntax\n");
		return;
	}
	lua_remove(L,-2);
	lua_pushstring(L,"highlighting");
	lua_gettable(L,-2);
	lua_remove(L,-2);
	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
		const char *c = lua_tostring(L, -2);
		printf("%s\n", c);
		for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
			const char *c = lua_tostring(L, -2);
			printf(" - %s\n", c);
			if (lua_istable(L, -1)) {
				for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
					const char *c = lua_tostring(L, -2);
					printf("   - %s\n", c);
					if (lua_istable(L, -1)) {
						for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
							int i = lua_tointeger(L, -2);
							printf("   - %u\n", i);
							if (lua_istable(L, -1)) {
								for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) {
									const char *c = lua_tostring(L, -2);
									const char *p = lua_tostring(L, -1);
									printf("       - %s %s\n", c, p);
								}
							}
						}
					}
				}
			}
		}
	}
	lua_pop( L, 1 );
	lua_remove(L,-2);*/
}

lua::~lua() {
	lua_close(L);
}




std::string lua::get_table_string(const char *key) {
	std::string value;
	lua_pushstring(L, key);
	lua_gettable(L, -2);
	if (lua_isstring(L, -1)) {
		value = lua_tostring(L, -1);
	}
	lua_pop(L, 1);
	return value;
}

