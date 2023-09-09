#include <luaconf.h>
#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <raylib.h>
#include "rayliblua.h"

int main(void) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    initRLLib(L);

    luaL_dofile(L, "./main.lua");

    lua_close(L);
    return 0;
}
