
#include "lua.hpp"

using alt::lua;

lua::lua() {

	L = luaL_newstate();

	if (L)
		printf("Lua VM started\n");

	luaopen_io(L);
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);

	if (luaL_dofile(L, "preferences.lua") != 0) {
		printf("NO preferences.lua\n");
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
