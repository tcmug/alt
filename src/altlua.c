

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int luaAlt_panic(lua_State *L)
{
	printf("something happened...\n");
	return 0;
}

int luaAlt_print(lua_State *L) {
  printf("printing\n");
	return 0;
}

static const luaL_Reg altlib[] = {
  {"print", luaAlt_print},
  {NULL, NULL} /* end of array */
};


LUAMOD_API int luaopen_alt(lua_State *L) {
  luaL_newlib(L, altlib);
  return 1;
}


lua_State *L, *lvm;

int luaAlt_init() 
{
	L = luaL_newstate();
	if (L) 
		printf("Lua VM started\n");
	else
		return 0;
	lua_atpanic(L, luaAlt_panic);
  
	printf("%u\n", lua_gettop(L));
  luaL_requiref(L, "alt", luaopen_alt, 1);
	lua_pop(L, 1);  /* remove lib */
	
	printf("%u\n", lua_gettop(L));
  luaL_dostring(L, "alt.print()");
  luaL_dostring(L, "print(\"Hello\")");
  luaL_dostring(L, "a = 2 + 2");
	lua_getglobal(L, "a");
	int i = lua_tointeger(L, -1);
	printf("%u\n", i);
	lvm = L;
	return 1;
}
