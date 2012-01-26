

#include "lua.h"
#include "lauxlib.h"


int luaAlt_panic(lua_State *L)
{
	printf("something happened...\n");
	return 0;
}

int luaAlt_print (lua_State *L) {
  printf("printing\n");
	return 0;
}

static const luaL_Reg luaAlt_funcs[] = {
  {"print", luaAlt_print},
  {NULL, NULL} /* end of array */
};

lua_State *lvm;

int luaAlt_init() 
{
	lvm = luaL_newstate();
	if (lvm) 
		printf("Lua VM started\n");
	else
		return 0;
	lua_atpanic(lvm, luaAlt_panic);
	//lua_getglobal(lvm, "_G");
	//luaL_setfuncs(lvm, luaAlt_funcs, 0);
  //lua_pop(lvm, 1);

  luaL_dostring(lvm, "print(\"Hello\")");
  luaL_dostring(lvm, "a = 2 + 2");
	lua_getglobal(lvm, "a");
	int i = lua_tointeger(lvm, -1);
	printf("%u\n", i);
	return 1;
}
