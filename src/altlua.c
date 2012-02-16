

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
	lua_pushinteger(L, 404);
	return 1;
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
  
  luaopen_base(L);
  luaopen_table(L);
  luaopen_io(L);
  luaopen_string(L);
  luaopen_math(L);

	lua_atpanic(L, luaAlt_panic);

  luaL_requiref(L, "alt", luaopen_alt, 1);
	lua_pop(L, 1);  
  //char pwd[255];
	//getcwd(pwd, 255);
	//pwd += "/start.lua";
	
  if (luaL_dofile (L, "/Users/teemumerikoski/dev/git/alt/start.lua") != 0) 
	{
		printf("failed\n");
	}

  /*
  luaL_dostring(L, "a=alt.print()");
	lua_getglobal(L, "a");
	int i = lua_tointeger(L, -1);
	printf("Got %u\n", i);
	lvm = L;
	*/
  lua_getglobal(L,"alt");
  lua_pushstring(L,"syntax");
  lua_gettable(L,-2);
  lua_remove(L,-2);
  lua_pushstring(L,"highlighting");
  lua_gettable(L,-2);
  lua_remove(L,-2);
  for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) 
  {
    const char *c = lua_tostring(L, -2);
		printf("%s\n", c);
  	for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) 
		{
      const char *c = lua_tostring(L, -2);
		  printf(" - %s\n", c);
			if (lua_istable(L, -1)) 
			{
  	    for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) 
				{
          const char *c = lua_tostring(L, -2);
		      printf("   - %s\n", c);
    			if (lua_istable(L, -1)) 
		    	{
  	        for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) 
					  {
          		int i = lua_tointeger(L, -2);
		      		printf("   - %u\n", i);
    					if (lua_istable(L, -1)) 
		    			{
		  	        for (lua_pushnil(L); lua_next(L, -2); lua_pop(L, 1)) 
								{
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

  lua_remove(L,-2);

	return 1;
}
