// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <https://unlicense.org>

#define LUA_IMPL
#include "minilua.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#define LuaPush_int(L, value)           lua_pushinteger(L, value)
#define LuaPush_float(L, value)         lua_pushnumber(L, value);
#define LuaPush_bool(L, value)          lua_pushboolean(L, value)
#define LuaPush_string(L, value)        lua_pushstring(L, value)

static void LuaStartEnum(lua_State* L) {
    lua_newtable(L);
}

static void LuaSetEnum(lua_State* L, const char *name, int value) {
    LuaPush_int(L, value);
    lua_setfield(L, -2, name);
}

static void LuaEndEnum(lua_State* L, const char *name) {
    lua_setglobal(L, name);
}

$

int main(int argc, const char *argv[]) {
    if (argc <= 1)
        return 1;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    initialize_raylua(L);
    luaL_dofile(L, argv[1]);
    lua_close(L);
    return 0;
}