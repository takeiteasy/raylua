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
static void lua_pushVector2(lua_State *L, Vector2* val) {
  lua_newtable(L);
  lua_pushstring(L, "x");
  lua_pushnumber(L, val->x);
  lua_settable(L, -3);
  lua_pushstring(L, "y");
  lua_pushnumber(L, val->y);
  lua_settable(L, -3);
}
static Vector2 lua_checkVector2(lua_State* L, int index) {
  Vector2 ret;
  lua_pushstring(L, "x");
  lua_gettable(L, index);
  ret.x = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "y");
  lua_gettable(L, index);
  ret.y = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushVector3(lua_State *L, Vector3* val) {
  lua_newtable(L);
  lua_pushstring(L, "x");
  lua_pushnumber(L, val->x);
  lua_settable(L, -3);
  lua_pushstring(L, "y");
  lua_pushnumber(L, val->y);
  lua_settable(L, -3);
  lua_pushstring(L, "z");
  lua_pushnumber(L, val->z);
  lua_settable(L, -3);
}
static Vector3 lua_checkVector3(lua_State* L, int index) {
  Vector3 ret;
  lua_pushstring(L, "x");
  lua_gettable(L, index);
  ret.x = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "y");
  lua_gettable(L, index);
  ret.y = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "z");
  lua_gettable(L, index);
  ret.z = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushVector4(lua_State *L, Vector4* val) {
  lua_newtable(L);
  lua_pushstring(L, "x");
  lua_pushnumber(L, val->x);
  lua_settable(L, -3);
  lua_pushstring(L, "y");
  lua_pushnumber(L, val->y);
  lua_settable(L, -3);
  lua_pushstring(L, "z");
  lua_pushnumber(L, val->z);
  lua_settable(L, -3);
  lua_pushstring(L, "w");
  lua_pushnumber(L, val->w);
  lua_settable(L, -3);
}
static Vector4 lua_checkVector4(lua_State* L, int index) {
  Vector4 ret;
  lua_pushstring(L, "x");
  lua_gettable(L, index);
  ret.x = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "y");
  lua_gettable(L, index);
  ret.y = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "z");
  lua_gettable(L, index);
  ret.z = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "w");
  lua_gettable(L, index);
  ret.w = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushMatrix(lua_State *L, Matrix* val) {
  lua_newtable(L);
  lua_pushstring(L, "m0");
  lua_pushnumber(L, val->m0);
  lua_settable(L, -3);
  lua_pushstring(L, "m4");
  lua_pushnumber(L, val->m4);
  lua_settable(L, -3);
  lua_pushstring(L, "m8");
  lua_pushnumber(L, val->m8);
  lua_settable(L, -3);
  lua_pushstring(L, "m12");
  lua_pushnumber(L, val->m12);
  lua_settable(L, -3);
  lua_pushstring(L, "m1");
  lua_pushnumber(L, val->m1);
  lua_settable(L, -3);
  lua_pushstring(L, "m5");
  lua_pushnumber(L, val->m5);
  lua_settable(L, -3);
  lua_pushstring(L, "m9");
  lua_pushnumber(L, val->m9);
  lua_settable(L, -3);
  lua_pushstring(L, "m13");
  lua_pushnumber(L, val->m13);
  lua_settable(L, -3);
  lua_pushstring(L, "m2");
  lua_pushnumber(L, val->m2);
  lua_settable(L, -3);
  lua_pushstring(L, "m6");
  lua_pushnumber(L, val->m6);
  lua_settable(L, -3);
  lua_pushstring(L, "m10");
  lua_pushnumber(L, val->m10);
  lua_settable(L, -3);
  lua_pushstring(L, "m14");
  lua_pushnumber(L, val->m14);
  lua_settable(L, -3);
  lua_pushstring(L, "m3");
  lua_pushnumber(L, val->m3);
  lua_settable(L, -3);
  lua_pushstring(L, "m7");
  lua_pushnumber(L, val->m7);
  lua_settable(L, -3);
  lua_pushstring(L, "m11");
  lua_pushnumber(L, val->m11);
  lua_settable(L, -3);
  lua_pushstring(L, "m15");
  lua_pushnumber(L, val->m15);
  lua_settable(L, -3);
}
static Matrix lua_checkMatrix(lua_State* L, int index) {
  Matrix ret;
  lua_pushstring(L, "m0");
  lua_gettable(L, index);
  ret.m0 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m4");
  lua_gettable(L, index);
  ret.m4 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m8");
  lua_gettable(L, index);
  ret.m8 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m12");
  lua_gettable(L, index);
  ret.m12 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m1");
  lua_gettable(L, index);
  ret.m1 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m5");
  lua_gettable(L, index);
  ret.m5 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m9");
  lua_gettable(L, index);
  ret.m9 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m13");
  lua_gettable(L, index);
  ret.m13 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m2");
  lua_gettable(L, index);
  ret.m2 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m6");
  lua_gettable(L, index);
  ret.m6 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m10");
  lua_gettable(L, index);
  ret.m10 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m14");
  lua_gettable(L, index);
  ret.m14 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m3");
  lua_gettable(L, index);
  ret.m3 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m7");
  lua_gettable(L, index);
  ret.m7 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m11");
  lua_gettable(L, index);
  ret.m11 = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "m15");
  lua_gettable(L, index);
  ret.m15 = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushColor(lua_State *L, Color* val) {
  lua_newtable(L);
  lua_pushstring(L, "r");
  lua_pushinteger(L, val->r);
  lua_settable(L, -3);
  lua_pushstring(L, "g");
  lua_pushinteger(L, val->g);
  lua_settable(L, -3);
  lua_pushstring(L, "b");
  lua_pushinteger(L, val->b);
  lua_settable(L, -3);
  lua_pushstring(L, "a");
  lua_pushinteger(L, val->a);
  lua_settable(L, -3);
}
static Color lua_checkColor(lua_State* L, int index) {
  Color ret;
  lua_pushstring(L, "r");
  lua_gettable(L, index);
  ret.r = (unsigned char)luaL_checkinteger(L, -1);
  lua_pushstring(L, "g");
  lua_gettable(L, index);
  ret.g = (unsigned char)luaL_checkinteger(L, -1);
  lua_pushstring(L, "b");
  lua_gettable(L, index);
  ret.b = (unsigned char)luaL_checkinteger(L, -1);
  lua_pushstring(L, "a");
  lua_gettable(L, index);
  ret.a = (unsigned char)luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushRectangle(lua_State *L, Rectangle* val) {
  lua_newtable(L);
  lua_pushstring(L, "x");
  lua_pushnumber(L, val->x);
  lua_settable(L, -3);
  lua_pushstring(L, "y");
  lua_pushnumber(L, val->y);
  lua_settable(L, -3);
  lua_pushstring(L, "width");
  lua_pushnumber(L, val->width);
  lua_settable(L, -3);
  lua_pushstring(L, "height");
  lua_pushnumber(L, val->height);
  lua_settable(L, -3);
}
static Rectangle lua_checkRectangle(lua_State* L, int index) {
  Rectangle ret;
  lua_pushstring(L, "x");
  lua_gettable(L, index);
  ret.x = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "y");
  lua_gettable(L, index);
  ret.y = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "width");
  lua_gettable(L, index);
  ret.width = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "height");
  lua_gettable(L, index);
  ret.height = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushImage(lua_State *L, Image* val) {
  lua_newtable(L);
  lua_pushstring(L, "data");
  lua_pushlightuserdata(L, val->data);
  lua_settable(L, -3);
  lua_pushstring(L, "width");
  lua_pushinteger(L, val->width);
  lua_settable(L, -3);
  lua_pushstring(L, "height");
  lua_pushinteger(L, val->height);
  lua_settable(L, -3);
  lua_pushstring(L, "mipmaps");
  lua_pushinteger(L, val->mipmaps);
  lua_settable(L, -3);
  lua_pushstring(L, "format");
  lua_pushinteger(L, val->format);
  lua_settable(L, -3);
}
static Image lua_checkImage(lua_State* L, int index) {
  Image ret;
  lua_pushstring(L, "data");
  lua_gettable(L, index);
  ret.data = lua_touserdata(L, -1);
  lua_pushstring(L, "width");
  lua_gettable(L, index);
  ret.width = luaL_checkinteger(L, -1);
  lua_pushstring(L, "height");
  lua_gettable(L, index);
  ret.height = luaL_checkinteger(L, -1);
  lua_pushstring(L, "mipmaps");
  lua_gettable(L, index);
  ret.mipmaps = luaL_checkinteger(L, -1);
  lua_pushstring(L, "format");
  lua_gettable(L, index);
  ret.format = luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushTexture(lua_State *L, Texture* val) {
  lua_newtable(L);
  lua_pushstring(L, "id");
  lua_pushinteger(L, val->id);
  lua_settable(L, -3);
  lua_pushstring(L, "width");
  lua_pushinteger(L, val->width);
  lua_settable(L, -3);
  lua_pushstring(L, "height");
  lua_pushinteger(L, val->height);
  lua_settable(L, -3);
  lua_pushstring(L, "mipmaps");
  lua_pushinteger(L, val->mipmaps);
  lua_settable(L, -3);
  lua_pushstring(L, "format");
  lua_pushinteger(L, val->format);
  lua_settable(L, -3);
}
static Texture lua_checkTexture(lua_State* L, int index) {
  Texture ret;
  lua_pushstring(L, "id");
  lua_gettable(L, index);
  ret.id = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "width");
  lua_gettable(L, index);
  ret.width = luaL_checkinteger(L, -1);
  lua_pushstring(L, "height");
  lua_gettable(L, index);
  ret.height = luaL_checkinteger(L, -1);
  lua_pushstring(L, "mipmaps");
  lua_gettable(L, index);
  ret.mipmaps = luaL_checkinteger(L, -1);
  lua_pushstring(L, "format");
  lua_gettable(L, index);
  ret.format = luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushRenderTexture(lua_State *L, RenderTexture* val) {
  lua_newtable(L);
  lua_pushstring(L, "id");
  lua_pushinteger(L, val->id);
  lua_settable(L, -3);
  lua_pushstring(L, "texture");
  lua_pushTexture(L, &(val->texture));
  lua_settable(L, -3);
  lua_pushstring(L, "depth");
  lua_pushTexture(L, &(val->depth));
  lua_settable(L, -3);
}
static RenderTexture lua_checkRenderTexture(lua_State* L, int index) {
  RenderTexture ret;
  lua_pushstring(L, "id");
  lua_gettable(L, index);
  ret.id = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "texture");
  lua_gettable(L, index);
  ret.texture = lua_checkTexture(L, -1);
  lua_pushstring(L, "depth");
  lua_gettable(L, index);
  ret.depth = lua_checkTexture(L, -1);
  return ret;
}

static void lua_pushNPatchInfo(lua_State *L, NPatchInfo* val) {
  lua_newtable(L);
  lua_pushstring(L, "source");
  lua_pushRectangle(L, &(val->source));
  lua_settable(L, -3);
  lua_pushstring(L, "left");
  lua_pushinteger(L, val->left);
  lua_settable(L, -3);
  lua_pushstring(L, "top");
  lua_pushinteger(L, val->top);
  lua_settable(L, -3);
  lua_pushstring(L, "right");
  lua_pushinteger(L, val->right);
  lua_settable(L, -3);
  lua_pushstring(L, "bottom");
  lua_pushinteger(L, val->bottom);
  lua_settable(L, -3);
  lua_pushstring(L, "layout");
  lua_pushinteger(L, val->layout);
  lua_settable(L, -3);
}
static NPatchInfo lua_checkNPatchInfo(lua_State* L, int index) {
  NPatchInfo ret;
  lua_pushstring(L, "source");
  lua_gettable(L, index);
  ret.source = lua_checkRectangle(L, -1);
  lua_pushstring(L, "left");
  lua_gettable(L, index);
  ret.left = luaL_checkinteger(L, -1);
  lua_pushstring(L, "top");
  lua_gettable(L, index);
  ret.top = luaL_checkinteger(L, -1);
  lua_pushstring(L, "right");
  lua_gettable(L, index);
  ret.right = luaL_checkinteger(L, -1);
  lua_pushstring(L, "bottom");
  lua_gettable(L, index);
  ret.bottom = luaL_checkinteger(L, -1);
  lua_pushstring(L, "layout");
  lua_gettable(L, index);
  ret.layout = luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushGlyphInfo(lua_State *L, GlyphInfo* val) {
  lua_newtable(L);
  lua_pushstring(L, "value");
  lua_pushinteger(L, val->value);
  lua_settable(L, -3);
  lua_pushstring(L, "offsetX");
  lua_pushinteger(L, val->offsetX);
  lua_settable(L, -3);
  lua_pushstring(L, "offsetY");
  lua_pushinteger(L, val->offsetY);
  lua_settable(L, -3);
  lua_pushstring(L, "advanceX");
  lua_pushinteger(L, val->advanceX);
  lua_settable(L, -3);
  lua_pushstring(L, "image");
  lua_pushImage(L, &(val->image));
  lua_settable(L, -3);
}
static GlyphInfo lua_checkGlyphInfo(lua_State* L, int index) {
  GlyphInfo ret;
  lua_pushstring(L, "value");
  lua_gettable(L, index);
  ret.value = luaL_checkinteger(L, -1);
  lua_pushstring(L, "offsetX");
  lua_gettable(L, index);
  ret.offsetX = luaL_checkinteger(L, -1);
  lua_pushstring(L, "offsetY");
  lua_gettable(L, index);
  ret.offsetY = luaL_checkinteger(L, -1);
  lua_pushstring(L, "advanceX");
  lua_gettable(L, index);
  ret.advanceX = luaL_checkinteger(L, -1);
  lua_pushstring(L, "image");
  lua_gettable(L, index);
  ret.image = lua_checkImage(L, -1);
  return ret;
}

static void lua_pushFont(lua_State *L, Font* val) {
  lua_newtable(L);
  lua_pushstring(L, "baseSize");
  lua_pushinteger(L, val->baseSize);
  lua_settable(L, -3);
  lua_pushstring(L, "glyphCount");
  lua_pushinteger(L, val->glyphCount);
  lua_settable(L, -3);
  lua_pushstring(L, "glyphPadding");
  lua_pushinteger(L, val->glyphPadding);
  lua_settable(L, -3);
  lua_pushstring(L, "texture");
  lua_pushTexture(L, &(val->texture));
  lua_settable(L, -3);
  lua_pushstring(L, "recs");
  lua_pushlightuserdata(L, (void *)val->recs);
  lua_settable(L, -3);
  lua_pushstring(L, "glyphs");
  lua_pushlightuserdata(L, (void *)val->glyphs);
  lua_settable(L, -3);
}
static Font lua_checkFont(lua_State* L, int index) {
  Font ret;
  lua_pushstring(L, "baseSize");
  lua_gettable(L, index);
  ret.baseSize = luaL_checkinteger(L, -1);
  lua_pushstring(L, "glyphCount");
  lua_gettable(L, index);
  ret.glyphCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "glyphPadding");
  lua_gettable(L, index);
  ret.glyphPadding = luaL_checkinteger(L, -1);
  lua_pushstring(L, "texture");
  lua_gettable(L, index);
  ret.texture = lua_checkTexture(L, -1);
  lua_pushstring(L, "recs");
  lua_gettable(L, index);
  ret.recs = (Rectangle *)lua_touserdata(L, -1);
  lua_pushstring(L, "glyphs");
  lua_gettable(L, index);
  ret.glyphs = (GlyphInfo *)lua_touserdata(L, -1);
  return ret;
}

static void lua_pushCamera3D(lua_State *L, Camera3D* val) {
  lua_newtable(L);
  lua_pushstring(L, "position");
  lua_pushVector3(L, &(val->position));
  lua_settable(L, -3);
  lua_pushstring(L, "target");
  lua_pushVector3(L, &(val->target));
  lua_settable(L, -3);
  lua_pushstring(L, "up");
  lua_pushVector3(L, &(val->up));
  lua_settable(L, -3);
  lua_pushstring(L, "fovy");
  lua_pushnumber(L, val->fovy);
  lua_settable(L, -3);
  lua_pushstring(L, "projection");
  lua_pushinteger(L, val->projection);
  lua_settable(L, -3);
}
static Camera3D lua_checkCamera3D(lua_State* L, int index) {
  Camera3D ret;
  lua_pushstring(L, "position");
  lua_gettable(L, index);
  ret.position = lua_checkVector3(L, -1);
  lua_pushstring(L, "target");
  lua_gettable(L, index);
  ret.target = lua_checkVector3(L, -1);
  lua_pushstring(L, "up");
  lua_gettable(L, index);
  ret.up = lua_checkVector3(L, -1);
  lua_pushstring(L, "fovy");
  lua_gettable(L, index);
  ret.fovy = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "projection");
  lua_gettable(L, index);
  ret.projection = luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushCamera2D(lua_State *L, Camera2D* val) {
  lua_newtable(L);
  lua_pushstring(L, "offset");
  lua_pushVector2(L, &(val->offset));
  lua_settable(L, -3);
  lua_pushstring(L, "target");
  lua_pushVector2(L, &(val->target));
  lua_settable(L, -3);
  lua_pushstring(L, "rotation");
  lua_pushnumber(L, val->rotation);
  lua_settable(L, -3);
  lua_pushstring(L, "zoom");
  lua_pushnumber(L, val->zoom);
  lua_settable(L, -3);
}
static Camera2D lua_checkCamera2D(lua_State* L, int index) {
  Camera2D ret;
  lua_pushstring(L, "offset");
  lua_gettable(L, index);
  ret.offset = lua_checkVector2(L, -1);
  lua_pushstring(L, "target");
  lua_gettable(L, index);
  ret.target = lua_checkVector2(L, -1);
  lua_pushstring(L, "rotation");
  lua_gettable(L, index);
  ret.rotation = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "zoom");
  lua_gettable(L, index);
  ret.zoom = (float)luaL_checknumber(L, -1);
  return ret;
}

static void lua_pushMesh(lua_State *L, Mesh* val) {
  lua_newtable(L);
  lua_pushstring(L, "vertexCount");
  lua_pushinteger(L, val->vertexCount);
  lua_settable(L, -3);
  lua_pushstring(L, "triangleCount");
  lua_pushinteger(L, val->triangleCount);
  lua_settable(L, -3);
  lua_pushstring(L, "vertices");
  lua_pushlightuserdata(L, (void *)val->vertices);
  lua_settable(L, -3);
  lua_pushstring(L, "texcoords");
  lua_pushlightuserdata(L, (void *)val->texcoords);
  lua_settable(L, -3);
  lua_pushstring(L, "texcoords2");
  lua_pushlightuserdata(L, (void *)val->texcoords2);
  lua_settable(L, -3);
  lua_pushstring(L, "normals");
  lua_pushlightuserdata(L, (void *)val->normals);
  lua_settable(L, -3);
  lua_pushstring(L, "tangents");
  lua_pushlightuserdata(L, (void *)val->tangents);
  lua_settable(L, -3);
  lua_pushstring(L, "colors");
  lua_pushstring(L, (const char *)val->colors);
  lua_settable(L, -3);
  lua_pushstring(L, "indices");
  lua_pushlightuserdata(L, (void *)val->indices);
  lua_settable(L, -3);
  lua_pushstring(L, "animVertices");
  lua_pushlightuserdata(L, (void *)val->animVertices);
  lua_settable(L, -3);
  lua_pushstring(L, "animNormals");
  lua_pushlightuserdata(L, (void *)val->animNormals);
  lua_settable(L, -3);
  lua_pushstring(L, "boneIds");
  lua_pushstring(L, (const char *)val->boneIds);
  lua_settable(L, -3);
  lua_pushstring(L, "boneWeights");
  lua_pushlightuserdata(L, (void *)val->boneWeights);
  lua_settable(L, -3);
  lua_pushstring(L, "vaoId");
  lua_pushinteger(L, val->vaoId);
  lua_settable(L, -3);
  lua_pushstring(L, "vboId");
  lua_pushlightuserdata(L, (void *)val->vboId);
  lua_settable(L, -3);
}
static Mesh lua_checkMesh(lua_State* L, int index) {
  Mesh ret;
  lua_pushstring(L, "vertexCount");
  lua_gettable(L, index);
  ret.vertexCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "triangleCount");
  lua_gettable(L, index);
  ret.triangleCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "vertices");
  lua_gettable(L, index);
  ret.vertices = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "texcoords");
  lua_gettable(L, index);
  ret.texcoords = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "texcoords2");
  lua_gettable(L, index);
  ret.texcoords2 = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "normals");
  lua_gettable(L, index);
  ret.normals = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "tangents");
  lua_gettable(L, index);
  ret.tangents = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "colors");
  lua_gettable(L, index);
  ret.colors = (unsigned char *)luaL_checkstring(L, -1);
  lua_pushstring(L, "indices");
  lua_gettable(L, index);
  ret.indices = (unsigned short *)lua_touserdata(L, -1);
  lua_pushstring(L, "animVertices");
  lua_gettable(L, index);
  ret.animVertices = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "animNormals");
  lua_gettable(L, index);
  ret.animNormals = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "boneIds");
  lua_gettable(L, index);
  ret.boneIds = (unsigned char *)luaL_checkstring(L, -1);
  lua_pushstring(L, "boneWeights");
  lua_gettable(L, index);
  ret.boneWeights = (float *)lua_touserdata(L, -1);
  lua_pushstring(L, "vaoId");
  lua_gettable(L, index);
  ret.vaoId = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "vboId");
  lua_gettable(L, index);
  ret.vboId = (unsigned int *)lua_touserdata(L, -1);
  return ret;
}

static void lua_pushShader(lua_State *L, Shader* val) {
  lua_newtable(L);
  lua_pushstring(L, "id");
  lua_pushinteger(L, val->id);
  lua_settable(L, -3);
  lua_pushstring(L, "locs");
  lua_pushlightuserdata(L, (void *)val->locs);
  lua_settable(L, -3);
}
static Shader lua_checkShader(lua_State* L, int index) {
  Shader ret;
  lua_pushstring(L, "id");
  lua_gettable(L, index);
  ret.id = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "locs");
  lua_gettable(L, index);
  ret.locs = (int *)lua_touserdata(L, -1);
  return ret;
}

static void lua_pushMaterialMap(lua_State *L, MaterialMap* val) {
  lua_newtable(L);
  lua_pushstring(L, "texture");
  lua_pushTexture(L, &(val->texture));
  lua_settable(L, -3);
  lua_pushstring(L, "color");
  lua_pushColor(L, &(val->color));
  lua_settable(L, -3);
  lua_pushstring(L, "value");
  lua_pushnumber(L, val->value);
  lua_settable(L, -3);
}
static MaterialMap lua_checkMaterialMap(lua_State* L, int index) {
  MaterialMap ret;
  lua_pushstring(L, "texture");
  lua_gettable(L, index);
  ret.texture = lua_checkTexture(L, -1);
  lua_pushstring(L, "color");
  lua_gettable(L, index);
  ret.color = lua_checkColor(L, -1);
  lua_pushstring(L, "value");
  lua_gettable(L, index);
  ret.value = (float)luaL_checknumber(L, -1);
  return ret;
}

// unknow field type float[4]
static void lua_pushMaterial(lua_State *L, Material* val) {}
static Material lua_checkMaterial(lua_State* L, int index) {Material ret; return ret;}

static void lua_pushTransform(lua_State *L, Transform* val) {
  lua_newtable(L);
  lua_pushstring(L, "translation");
  lua_pushVector3(L, &(val->translation));
  lua_settable(L, -3);
  lua_pushstring(L, "rotation");
  lua_pushVector4(L, &(val->rotation));
  lua_settable(L, -3);
  lua_pushstring(L, "scale");
  lua_pushVector3(L, &(val->scale));
  lua_settable(L, -3);
}
static Transform lua_checkTransform(lua_State* L, int index) {
  Transform ret;
  lua_pushstring(L, "translation");
  lua_gettable(L, index);
  ret.translation = lua_checkVector3(L, -1);
  lua_pushstring(L, "rotation");
  lua_gettable(L, index);
  ret.rotation = lua_checkVector4(L, -1);
  lua_pushstring(L, "scale");
  lua_gettable(L, index);
  ret.scale = lua_checkVector3(L, -1);
  return ret;
}

// unknow field type char[32]
static void lua_pushBoneInfo(lua_State *L, BoneInfo* val) {}
static BoneInfo lua_checkBoneInfo(lua_State* L, int index) {BoneInfo ret; return ret;}

static void lua_pushModel(lua_State *L, Model* val) {
  lua_newtable(L);
  lua_pushstring(L, "transform");
  lua_pushMatrix(L, &(val->transform));
  lua_settable(L, -3);
  lua_pushstring(L, "meshCount");
  lua_pushinteger(L, val->meshCount);
  lua_settable(L, -3);
  lua_pushstring(L, "materialCount");
  lua_pushinteger(L, val->materialCount);
  lua_settable(L, -3);
  lua_pushstring(L, "meshes");
  lua_pushlightuserdata(L, (void *)val->meshes);
  lua_settable(L, -3);
  lua_pushstring(L, "materials");
  lua_pushlightuserdata(L, (void *)val->materials);
  lua_settable(L, -3);
  lua_pushstring(L, "meshMaterial");
  lua_pushlightuserdata(L, (void *)val->meshMaterial);
  lua_settable(L, -3);
  lua_pushstring(L, "boneCount");
  lua_pushinteger(L, val->boneCount);
  lua_settable(L, -3);
  lua_pushstring(L, "bones");
  lua_pushlightuserdata(L, (void *)val->bones);
  lua_settable(L, -3);
  lua_pushstring(L, "bindPose");
  lua_pushlightuserdata(L, (void *)val->bindPose);
  lua_settable(L, -3);
}
static Model lua_checkModel(lua_State* L, int index) {
  Model ret;
  lua_pushstring(L, "transform");
  lua_gettable(L, index);
  ret.transform = lua_checkMatrix(L, -1);
  lua_pushstring(L, "meshCount");
  lua_gettable(L, index);
  ret.meshCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "materialCount");
  lua_gettable(L, index);
  ret.materialCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "meshes");
  lua_gettable(L, index);
  ret.meshes = (Mesh *)lua_touserdata(L, -1);
  lua_pushstring(L, "materials");
  lua_gettable(L, index);
  ret.materials = (Material *)lua_touserdata(L, -1);
  lua_pushstring(L, "meshMaterial");
  lua_gettable(L, index);
  ret.meshMaterial = (int *)lua_touserdata(L, -1);
  lua_pushstring(L, "boneCount");
  lua_gettable(L, index);
  ret.boneCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "bones");
  lua_gettable(L, index);
  ret.bones = (BoneInfo *)lua_touserdata(L, -1);
  lua_pushstring(L, "bindPose");
  lua_gettable(L, index);
  ret.bindPose = (Transform *)lua_touserdata(L, -1);
  return ret;
}

// unknow field type char[32]
static void lua_pushModelAnimation(lua_State *L, ModelAnimation* val) {}
static ModelAnimation lua_checkModelAnimation(lua_State* L, int index) {ModelAnimation ret; return ret;}

static void lua_pushRay(lua_State *L, Ray* val) {
  lua_newtable(L);
  lua_pushstring(L, "position");
  lua_pushVector3(L, &(val->position));
  lua_settable(L, -3);
  lua_pushstring(L, "direction");
  lua_pushVector3(L, &(val->direction));
  lua_settable(L, -3);
}
static Ray lua_checkRay(lua_State* L, int index) {
  Ray ret;
  lua_pushstring(L, "position");
  lua_gettable(L, index);
  ret.position = lua_checkVector3(L, -1);
  lua_pushstring(L, "direction");
  lua_gettable(L, index);
  ret.direction = lua_checkVector3(L, -1);
  return ret;
}

static void lua_pushRayCollision(lua_State *L, RayCollision* val) {
  lua_newtable(L);
  lua_pushstring(L, "hit");
  lua_pushboolean(L, val->hit);
  lua_settable(L, -3);
  lua_pushstring(L, "distance");
  lua_pushnumber(L, val->distance);
  lua_settable(L, -3);
  lua_pushstring(L, "point");
  lua_pushVector3(L, &(val->point));
  lua_settable(L, -3);
  lua_pushstring(L, "normal");
  lua_pushVector3(L, &(val->normal));
  lua_settable(L, -3);
}
static RayCollision lua_checkRayCollision(lua_State* L, int index) {
  RayCollision ret;
  lua_pushstring(L, "hit");
  lua_gettable(L, index);
  ret.hit = lua_toboolean(L, -1);
  lua_pushstring(L, "distance");
  lua_gettable(L, index);
  ret.distance = (float)luaL_checknumber(L, -1);
  lua_pushstring(L, "point");
  lua_gettable(L, index);
  ret.point = lua_checkVector3(L, -1);
  lua_pushstring(L, "normal");
  lua_gettable(L, index);
  ret.normal = lua_checkVector3(L, -1);
  return ret;
}

static void lua_pushBoundingBox(lua_State *L, BoundingBox* val) {
  lua_newtable(L);
  lua_pushstring(L, "min");
  lua_pushVector3(L, &(val->min));
  lua_settable(L, -3);
  lua_pushstring(L, "max");
  lua_pushVector3(L, &(val->max));
  lua_settable(L, -3);
}
static BoundingBox lua_checkBoundingBox(lua_State* L, int index) {
  BoundingBox ret;
  lua_pushstring(L, "min");
  lua_gettable(L, index);
  ret.min = lua_checkVector3(L, -1);
  lua_pushstring(L, "max");
  lua_gettable(L, index);
  ret.max = lua_checkVector3(L, -1);
  return ret;
}

static void lua_pushWave(lua_State *L, Wave* val) {
  lua_newtable(L);
  lua_pushstring(L, "frameCount");
  lua_pushinteger(L, val->frameCount);
  lua_settable(L, -3);
  lua_pushstring(L, "sampleRate");
  lua_pushinteger(L, val->sampleRate);
  lua_settable(L, -3);
  lua_pushstring(L, "sampleSize");
  lua_pushinteger(L, val->sampleSize);
  lua_settable(L, -3);
  lua_pushstring(L, "channels");
  lua_pushinteger(L, val->channels);
  lua_settable(L, -3);
  lua_pushstring(L, "data");
  lua_pushlightuserdata(L, val->data);
  lua_settable(L, -3);
}
static Wave lua_checkWave(lua_State* L, int index) {
  Wave ret;
  lua_pushstring(L, "frameCount");
  lua_gettable(L, index);
  ret.frameCount = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "sampleRate");
  lua_gettable(L, index);
  ret.sampleRate = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "sampleSize");
  lua_gettable(L, index);
  ret.sampleSize = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "channels");
  lua_gettable(L, index);
  ret.channels = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "data");
  lua_gettable(L, index);
  ret.data = lua_touserdata(L, -1);
  return ret;
}

// unknow field type rAudioBuffer *
// unknow field type rAudioProcessor *
static void lua_pushAudioStream(lua_State *L, AudioStream* val) {}
static AudioStream lua_checkAudioStream(lua_State* L, int index) {AudioStream ret; return ret;}

static void lua_pushSound(lua_State *L, Sound* val) {
  lua_newtable(L);
  lua_pushstring(L, "stream");
  lua_pushAudioStream(L, &(val->stream));
  lua_settable(L, -3);
  lua_pushstring(L, "frameCount");
  lua_pushinteger(L, val->frameCount);
  lua_settable(L, -3);
}
static Sound lua_checkSound(lua_State* L, int index) {
  Sound ret;
  lua_pushstring(L, "stream");
  lua_gettable(L, index);
  ret.stream = lua_checkAudioStream(L, -1);
  lua_pushstring(L, "frameCount");
  lua_gettable(L, index);
  ret.frameCount = (unsigned int)luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushMusic(lua_State *L, Music* val) {
  lua_newtable(L);
  lua_pushstring(L, "stream");
  lua_pushAudioStream(L, &(val->stream));
  lua_settable(L, -3);
  lua_pushstring(L, "frameCount");
  lua_pushinteger(L, val->frameCount);
  lua_settable(L, -3);
  lua_pushstring(L, "looping");
  lua_pushboolean(L, val->looping);
  lua_settable(L, -3);
  lua_pushstring(L, "ctxType");
  lua_pushinteger(L, val->ctxType);
  lua_settable(L, -3);
  lua_pushstring(L, "ctxData");
  lua_pushlightuserdata(L, val->ctxData);
  lua_settable(L, -3);
}
static Music lua_checkMusic(lua_State* L, int index) {
  Music ret;
  lua_pushstring(L, "stream");
  lua_gettable(L, index);
  ret.stream = lua_checkAudioStream(L, -1);
  lua_pushstring(L, "frameCount");
  lua_gettable(L, index);
  ret.frameCount = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "looping");
  lua_gettable(L, index);
  ret.looping = lua_toboolean(L, -1);
  lua_pushstring(L, "ctxType");
  lua_gettable(L, index);
  ret.ctxType = luaL_checkinteger(L, -1);
  lua_pushstring(L, "ctxData");
  lua_gettable(L, index);
  ret.ctxData = lua_touserdata(L, -1);
  return ret;
}

// unknow field type float[4]
// unknow field type float[4]
static void lua_pushVrDeviceInfo(lua_State *L, VrDeviceInfo* val) {}
static VrDeviceInfo lua_checkVrDeviceInfo(lua_State* L, int index) {VrDeviceInfo ret; return ret;}

// unknow field type Matrix[2]
// unknow field type Matrix[2]
// unknow field type float[2]
// unknow field type float[2]
// unknow field type float[2]
// unknow field type float[2]
// unknow field type float[2]
// unknow field type float[2]
static void lua_pushVrStereoConfig(lua_State *L, VrStereoConfig* val) {}
static VrStereoConfig lua_checkVrStereoConfig(lua_State* L, int index) {VrStereoConfig ret; return ret;}

static void lua_pushFilePathList(lua_State *L, FilePathList* val) {
  lua_newtable(L);
  lua_pushstring(L, "capacity");
  lua_pushinteger(L, val->capacity);
  lua_settable(L, -3);
  lua_pushstring(L, "count");
  lua_pushinteger(L, val->count);
  lua_settable(L, -3);
  lua_pushstring(L, "paths");
  lua_pushlightuserdata(L, (void *)val->paths);
  lua_settable(L, -3);
}
static FilePathList lua_checkFilePathList(lua_State* L, int index) {
  FilePathList ret;
  lua_pushstring(L, "capacity");
  lua_gettable(L, index);
  ret.capacity = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "count");
  lua_gettable(L, index);
  ret.count = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "paths");
  lua_gettable(L, index);
  ret.paths = (char **)lua_touserdata(L, -1);
  return ret;
}

// unknow field type int[4]
static void lua_pushAutomationEvent(lua_State *L, AutomationEvent* val) {}
static AutomationEvent lua_checkAutomationEvent(lua_State* L, int index) {AutomationEvent ret; return ret;}

static void lua_pushAutomationEventList(lua_State *L, AutomationEventList* val) {
  lua_newtable(L);
  lua_pushstring(L, "capacity");
  lua_pushinteger(L, val->capacity);
  lua_settable(L, -3);
  lua_pushstring(L, "count");
  lua_pushinteger(L, val->count);
  lua_settable(L, -3);
  lua_pushstring(L, "events");
  lua_pushlightuserdata(L, (void *)val->events);
  lua_settable(L, -3);
}
static AutomationEventList lua_checkAutomationEventList(lua_State* L, int index) {
  AutomationEventList ret;
  lua_pushstring(L, "capacity");
  lua_gettable(L, index);
  ret.capacity = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "count");
  lua_gettable(L, index);
  ret.count = (unsigned int)luaL_checkinteger(L, -1);
  lua_pushstring(L, "events");
  lua_gettable(L, index);
  ret.events = (AutomationEvent *)lua_touserdata(L, -1);
  return ret;
}

static int lua_InitWindow(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  const char * title = luaL_checkstring(L, 3);
  InitWindow(width, height, title);
  return 0;
}
static int lua_CloseWindow(lua_State *L) {
  CloseWindow();
  return 0;
}
static int lua_WindowShouldClose(lua_State *L) {
  bool ret = WindowShouldClose();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowReady(lua_State *L) {
  bool ret = IsWindowReady();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowFullscreen(lua_State *L) {
  bool ret = IsWindowFullscreen();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowHidden(lua_State *L) {
  bool ret = IsWindowHidden();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowMinimized(lua_State *L) {
  bool ret = IsWindowMinimized();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowMaximized(lua_State *L) {
  bool ret = IsWindowMaximized();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowFocused(lua_State *L) {
  bool ret = IsWindowFocused();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowResized(lua_State *L) {
  bool ret = IsWindowResized();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsWindowState(lua_State *L) {
  unsigned int flag = (unsigned int)luaL_checkinteger(L, 1);
  bool ret = IsWindowState(flag);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_SetWindowState(lua_State *L) {
  unsigned int flags = (unsigned int)luaL_checkinteger(L, 1);
  SetWindowState(flags);
  return 0;
}
static int lua_ClearWindowState(lua_State *L) {
  unsigned int flags = (unsigned int)luaL_checkinteger(L, 1);
  ClearWindowState(flags);
  return 0;
}
static int lua_ToggleFullscreen(lua_State *L) {
  ToggleFullscreen();
  return 0;
}
static int lua_ToggleBorderlessWindowed(lua_State *L) {
  ToggleBorderlessWindowed();
  return 0;
}
static int lua_MaximizeWindow(lua_State *L) {
  MaximizeWindow();
  return 0;
}
static int lua_MinimizeWindow(lua_State *L) {
  MinimizeWindow();
  return 0;
}
static int lua_RestoreWindow(lua_State *L) {
  RestoreWindow();
  return 0;
}
static int lua_SetWindowIcon(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  SetWindowIcon(image);
  return 0;
}
static int lua_SetWindowIcons(lua_State *L) {
  Image * images = (Image *)lua_touserdata(L, 1);
  int count = luaL_checkinteger(L, 2);
  SetWindowIcons(images, count);
  return 0;
}
static int lua_SetWindowTitle(lua_State *L) {
  const char * title = luaL_checkstring(L, 1);
  SetWindowTitle(title);
  return 0;
}
static int lua_SetWindowPosition(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  SetWindowPosition(x, y);
  return 0;
}
static int lua_SetWindowMonitor(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  SetWindowMonitor(monitor);
  return 0;
}
static int lua_SetWindowMinSize(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  SetWindowMinSize(width, height);
  return 0;
}
static int lua_SetWindowMaxSize(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  SetWindowMaxSize(width, height);
  return 0;
}
static int lua_SetWindowSize(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  SetWindowSize(width, height);
  return 0;
}
static int lua_SetWindowOpacity(lua_State *L) {
  float opacity = (float)luaL_checknumber(L, 1);
  SetWindowOpacity(opacity);
  return 0;
}
static int lua_SetWindowFocused(lua_State *L) {
  SetWindowFocused();
  return 0;
}
static int lua_GetWindowHandle(lua_State *L) {
  void * ret = GetWindowHandle();
  lua_pushlightuserdata(L, ret);
  return 1;
}
static int lua_GetScreenWidth(lua_State *L) {
  int ret = GetScreenWidth();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetScreenHeight(lua_State *L) {
  int ret = GetScreenHeight();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetRenderWidth(lua_State *L) {
  int ret = GetRenderWidth();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetRenderHeight(lua_State *L) {
  int ret = GetRenderHeight();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorCount(lua_State *L) {
  int ret = GetMonitorCount();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetCurrentMonitor(lua_State *L) {
  int ret = GetCurrentMonitor();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorPosition(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  Vector2 ret = GetMonitorPosition(monitor);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetMonitorWidth(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  int ret = GetMonitorWidth(monitor);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorHeight(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  int ret = GetMonitorHeight(monitor);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorPhysicalWidth(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  int ret = GetMonitorPhysicalWidth(monitor);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorPhysicalHeight(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  int ret = GetMonitorPhysicalHeight(monitor);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMonitorRefreshRate(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  int ret = GetMonitorRefreshRate(monitor);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetWindowPosition(lua_State *L) {
  Vector2 ret = GetWindowPosition();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetWindowScaleDPI(lua_State *L) {
  Vector2 ret = GetWindowScaleDPI();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetMonitorName(lua_State *L) {
  int monitor = luaL_checkinteger(L, 1);
  const char * ret = GetMonitorName(monitor);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_SetClipboardText(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  SetClipboardText(text);
  return 0;
}
static int lua_GetClipboardText(lua_State *L) {
  const char * ret = GetClipboardText();
  lua_pushstring(L, ret);
  return 1;
}
static int lua_EnableEventWaiting(lua_State *L) {
  EnableEventWaiting();
  return 0;
}
static int lua_DisableEventWaiting(lua_State *L) {
  DisableEventWaiting();
  return 0;
}
static int lua_ShowCursor(lua_State *L) {
  ShowCursor();
  return 0;
}
static int lua_HideCursor(lua_State *L) {
  HideCursor();
  return 0;
}
static int lua_IsCursorHidden(lua_State *L) {
  bool ret = IsCursorHidden();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_EnableCursor(lua_State *L) {
  EnableCursor();
  return 0;
}
static int lua_DisableCursor(lua_State *L) {
  DisableCursor();
  return 0;
}
static int lua_IsCursorOnScreen(lua_State *L) {
  bool ret = IsCursorOnScreen();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_ClearBackground(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  ClearBackground(color);
  return 0;
}
static int lua_BeginDrawing(lua_State *L) {
  BeginDrawing();
  return 0;
}
static int lua_EndDrawing(lua_State *L) {
  EndDrawing();
  return 0;
}
static int lua_BeginMode2D(lua_State *L) {
  Camera2D camera = lua_checkCamera2D(L, 1);
  BeginMode2D(camera);
  return 0;
}
static int lua_EndMode2D(lua_State *L) {
  EndMode2D();
  return 0;
}
static int lua_BeginMode3D(lua_State *L) {
  Camera3D camera = lua_checkCamera3D(L, 1);
  BeginMode3D(camera);
  return 0;
}
static int lua_EndMode3D(lua_State *L) {
  EndMode3D();
  return 0;
}
static int lua_BeginTextureMode(lua_State *L) {
  RenderTexture2D target = lua_checkRenderTexture(L, 1);
  BeginTextureMode(target);
  return 0;
}
static int lua_EndTextureMode(lua_State *L) {
  EndTextureMode();
  return 0;
}
static int lua_BeginShaderMode(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  BeginShaderMode(shader);
  return 0;
}
static int lua_EndShaderMode(lua_State *L) {
  EndShaderMode();
  return 0;
}
static int lua_BeginBlendMode(lua_State *L) {
  int mode = luaL_checkinteger(L, 1);
  BeginBlendMode(mode);
  return 0;
}
static int lua_EndBlendMode(lua_State *L) {
  EndBlendMode();
  return 0;
}
static int lua_BeginScissorMode(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  BeginScissorMode(x, y, width, height);
  return 0;
}
static int lua_EndScissorMode(lua_State *L) {
  EndScissorMode();
  return 0;
}
static int lua_BeginVrStereoMode(lua_State *L) {
  VrStereoConfig config = lua_checkVrStereoConfig(L, 1);
  BeginVrStereoMode(config);
  return 0;
}
static int lua_EndVrStereoMode(lua_State *L) {
  EndVrStereoMode();
  return 0;
}
static int lua_LoadVrStereoConfig(lua_State *L) {
  VrDeviceInfo device = lua_checkVrDeviceInfo(L, 1);
  VrStereoConfig ret = LoadVrStereoConfig(device);
  lua_pushVrStereoConfig(L, &ret);
  return 1;
}
static int lua_UnloadVrStereoConfig(lua_State *L) {
  VrStereoConfig config = lua_checkVrStereoConfig(L, 1);
  UnloadVrStereoConfig(config);
  return 0;
}
static int lua_LoadShader(lua_State *L) {
  const char * vsFileName = luaL_checkstring(L, 1);
  const char * fsFileName = luaL_checkstring(L, 2);
  Shader ret = LoadShader(vsFileName, fsFileName);
  lua_pushShader(L, &ret);
  return 1;
}
static int lua_LoadShaderFromMemory(lua_State *L) {
  const char * vsCode = luaL_checkstring(L, 1);
  const char * fsCode = luaL_checkstring(L, 2);
  Shader ret = LoadShaderFromMemory(vsCode, fsCode);
  lua_pushShader(L, &ret);
  return 1;
}
static int lua_IsShaderReady(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  bool ret = IsShaderReady(shader);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetShaderLocation(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  const char * uniformName = luaL_checkstring(L, 2);
  int ret = GetShaderLocation(shader, uniformName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetShaderLocationAttrib(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  const char * attribName = luaL_checkstring(L, 2);
  int ret = GetShaderLocationAttrib(shader, attribName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_SetShaderValue(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  int locIndex = luaL_checkinteger(L, 2);
  const void * value = lua_touserdata(L, 3);
  int uniformType = luaL_checkinteger(L, 4);
  SetShaderValue(shader, locIndex, value, uniformType);
  return 0;
}
static int lua_SetShaderValueV(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  int locIndex = luaL_checkinteger(L, 2);
  const void * value = lua_touserdata(L, 3);
  int uniformType = luaL_checkinteger(L, 4);
  int count = luaL_checkinteger(L, 5);
  SetShaderValueV(shader, locIndex, value, uniformType, count);
  return 0;
}
static int lua_SetShaderValueMatrix(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  int locIndex = luaL_checkinteger(L, 2);
  Matrix mat = lua_checkMatrix(L, 3);
  SetShaderValueMatrix(shader, locIndex, mat);
  return 0;
}
static int lua_SetShaderValueTexture(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  int locIndex = luaL_checkinteger(L, 2);
  Texture2D texture = lua_checkTexture(L, 3);
  SetShaderValueTexture(shader, locIndex, texture);
  return 0;
}
static int lua_UnloadShader(lua_State *L) {
  Shader shader = lua_checkShader(L, 1);
  UnloadShader(shader);
  return 0;
}
static int lua_GetMouseRay(lua_State *L) {
  Vector2 mousePosition = lua_checkVector2(L, 1);
  Camera camera = lua_checkCamera3D(L, 2);
  Ray ret = GetMouseRay(mousePosition, camera);
  lua_pushRay(L, &ret);
  return 1;
}
static int lua_GetCameraMatrix(lua_State *L) {
  Camera camera = lua_checkCamera3D(L, 1);
  Matrix ret = GetCameraMatrix(camera);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_GetCameraMatrix2D(lua_State *L) {
  Camera2D camera = lua_checkCamera2D(L, 1);
  Matrix ret = GetCameraMatrix2D(camera);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_GetWorldToScreen(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  Camera camera = lua_checkCamera3D(L, 2);
  Vector2 ret = GetWorldToScreen(position, camera);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetScreenToWorld2D(lua_State *L) {
  Vector2 position = lua_checkVector2(L, 1);
  Camera2D camera = lua_checkCamera2D(L, 2);
  Vector2 ret = GetScreenToWorld2D(position, camera);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetWorldToScreenEx(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  Camera camera = lua_checkCamera3D(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  Vector2 ret = GetWorldToScreenEx(position, camera, width, height);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetWorldToScreen2D(lua_State *L) {
  Vector2 position = lua_checkVector2(L, 1);
  Camera2D camera = lua_checkCamera2D(L, 2);
  Vector2 ret = GetWorldToScreen2D(position, camera);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_SetTargetFPS(lua_State *L) {
  int fps = luaL_checkinteger(L, 1);
  SetTargetFPS(fps);
  return 0;
}
static int lua_GetFrameTime(lua_State *L) {
  float ret = GetFrameTime();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetTime(lua_State *L) {
  double ret = GetTime();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetFPS(lua_State *L) {
  int ret = GetFPS();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_SwapScreenBuffer(lua_State *L) {
  SwapScreenBuffer();
  return 0;
}
static int lua_PollInputEvents(lua_State *L) {
  PollInputEvents();
  return 0;
}
static int lua_WaitTime(lua_State *L) {
  double seconds = (double)luaL_checknumber(L, 1);
  WaitTime(seconds);
  return 0;
}
static int lua_SetRandomSeed(lua_State *L) {
  unsigned int seed = (unsigned int)luaL_checkinteger(L, 1);
  SetRandomSeed(seed);
  return 0;
}
static int lua_GetRandomValue(lua_State *L) {
  int min = luaL_checkinteger(L, 1);
  int max = luaL_checkinteger(L, 2);
  int ret = GetRandomValue(min, max);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_LoadRandomSequence(lua_State *L) {
  unsigned int count = (unsigned int)luaL_checkinteger(L, 1);
  int min = luaL_checkinteger(L, 2);
  int max = luaL_checkinteger(L, 3);
  int * ret = LoadRandomSequence(count, min, max);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_UnloadRandomSequence(lua_State *L) {
  int * sequence = (int *)lua_touserdata(L, 1);
  UnloadRandomSequence(sequence);
  return 0;
}
static int lua_TakeScreenshot(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  TakeScreenshot(fileName);
  return 0;
}
static int lua_SetConfigFlags(lua_State *L) {
  unsigned int flags = (unsigned int)luaL_checkinteger(L, 1);
  SetConfigFlags(flags);
  return 0;
}
static int lua_OpenURL(lua_State *L) {
  const char * url = luaL_checkstring(L, 1);
  OpenURL(url);
  return 0;
}
// unknow param type ...
static int lua_TraceLog(lua_State *L) {return 0;}

static int lua_SetTraceLogLevel(lua_State *L) {
  int logLevel = luaL_checkinteger(L, 1);
  SetTraceLogLevel(logLevel);
  return 0;
}
static int lua_MemAlloc(lua_State *L) {
  unsigned int size = (unsigned int)luaL_checkinteger(L, 1);
  void * ret = MemAlloc(size);
  lua_pushlightuserdata(L, ret);
  return 1;
}
static int lua_MemRealloc(lua_State *L) {
  void * ptr = lua_touserdata(L, 1);
  unsigned int size = (unsigned int)luaL_checkinteger(L, 2);
  void * ret = MemRealloc(ptr, size);
  lua_pushlightuserdata(L, ret);
  return 1;
}
static int lua_MemFree(lua_State *L) {
  void * ptr = lua_touserdata(L, 1);
  MemFree(ptr);
  return 0;
}
// unknow param type TraceLogCallback
static int lua_SetTraceLogCallback(lua_State *L) {return 0;}

// unknow param type LoadFileDataCallback
static int lua_SetLoadFileDataCallback(lua_State *L) {return 0;}

// unknow param type SaveFileDataCallback
static int lua_SetSaveFileDataCallback(lua_State *L) {return 0;}

// unknow param type LoadFileTextCallback
static int lua_SetLoadFileTextCallback(lua_State *L) {return 0;}

// unknow param type SaveFileTextCallback
static int lua_SetSaveFileTextCallback(lua_State *L) {return 0;}

static int lua_LoadFileData(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int * dataSize = (int *)lua_touserdata(L, 2);
  unsigned char * ret = LoadFileData(fileName, dataSize);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_UnloadFileData(lua_State *L) {
  unsigned char * data = (unsigned char *)luaL_checkstring(L, 1);
  UnloadFileData(data);
  return 0;
}
static int lua_SaveFileData(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  void * data = lua_touserdata(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  bool ret = SaveFileData(fileName, data, dataSize);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_ExportDataAsCode(lua_State *L) {
  const unsigned char * data = (const unsigned char *)luaL_checkstring(L, 1);
  int dataSize = luaL_checkinteger(L, 2);
  const char * fileName = luaL_checkstring(L, 3);
  bool ret = ExportDataAsCode(data, dataSize, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_LoadFileText(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  char * ret = LoadFileText(fileName);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_UnloadFileText(lua_State *L) {
  char * text = (char *)luaL_checkstring(L, 1);
  UnloadFileText(text);
  return 0;
}
static int lua_SaveFileText(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  char * text = (char *)luaL_checkstring(L, 2);
  bool ret = SaveFileText(fileName, text);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_FileExists(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  bool ret = FileExists(fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_DirectoryExists(lua_State *L) {
  const char * dirPath = luaL_checkstring(L, 1);
  bool ret = DirectoryExists(dirPath);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsFileExtension(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  const char * ext = luaL_checkstring(L, 2);
  bool ret = IsFileExtension(fileName, ext);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetFileLength(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int ret = GetFileLength(fileName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetFileExtension(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  const char * ret = GetFileExtension(fileName);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetFileName(lua_State *L) {
  const char * filePath = luaL_checkstring(L, 1);
  const char * ret = GetFileName(filePath);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetFileNameWithoutExt(lua_State *L) {
  const char * filePath = luaL_checkstring(L, 1);
  const char * ret = GetFileNameWithoutExt(filePath);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetDirectoryPath(lua_State *L) {
  const char * filePath = luaL_checkstring(L, 1);
  const char * ret = GetDirectoryPath(filePath);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetPrevDirectoryPath(lua_State *L) {
  const char * dirPath = luaL_checkstring(L, 1);
  const char * ret = GetPrevDirectoryPath(dirPath);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetWorkingDirectory(lua_State *L) {
  const char * ret = GetWorkingDirectory();
  lua_pushstring(L, ret);
  return 1;
}
static int lua_GetApplicationDirectory(lua_State *L) {
  const char * ret = GetApplicationDirectory();
  lua_pushstring(L, ret);
  return 1;
}
static int lua_ChangeDirectory(lua_State *L) {
  const char * dir = luaL_checkstring(L, 1);
  bool ret = ChangeDirectory(dir);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsPathFile(lua_State *L) {
  const char * path = luaL_checkstring(L, 1);
  bool ret = IsPathFile(path);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_LoadDirectoryFiles(lua_State *L) {
  const char * dirPath = luaL_checkstring(L, 1);
  FilePathList ret = LoadDirectoryFiles(dirPath);
  lua_pushFilePathList(L, &ret);
  return 1;
}
static int lua_LoadDirectoryFilesEx(lua_State *L) {
  const char * basePath = luaL_checkstring(L, 1);
  const char * filter = luaL_checkstring(L, 2);
  bool scanSubdirs = lua_toboolean(L, 3);
  FilePathList ret = LoadDirectoryFilesEx(basePath, filter, scanSubdirs);
  lua_pushFilePathList(L, &ret);
  return 1;
}
static int lua_UnloadDirectoryFiles(lua_State *L) {
  FilePathList files = lua_checkFilePathList(L, 1);
  UnloadDirectoryFiles(files);
  return 0;
}
static int lua_IsFileDropped(lua_State *L) {
  bool ret = IsFileDropped();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_LoadDroppedFiles(lua_State *L) {
  FilePathList ret = LoadDroppedFiles();
  lua_pushFilePathList(L, &ret);
  return 1;
}
static int lua_UnloadDroppedFiles(lua_State *L) {
  FilePathList files = lua_checkFilePathList(L, 1);
  UnloadDroppedFiles(files);
  return 0;
}
static int lua_GetFileModTime(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  long ret = GetFileModTime(fileName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_CompressData(lua_State *L) {
  const unsigned char * data = (const unsigned char *)luaL_checkstring(L, 1);
  int dataSize = luaL_checkinteger(L, 2);
  int * compDataSize = (int *)lua_touserdata(L, 3);
  unsigned char * ret = CompressData(data, dataSize, compDataSize);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_DecompressData(lua_State *L) {
  const unsigned char * compData = (const unsigned char *)luaL_checkstring(L, 1);
  int compDataSize = luaL_checkinteger(L, 2);
  int * dataSize = (int *)lua_touserdata(L, 3);
  unsigned char * ret = DecompressData(compData, compDataSize, dataSize);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_EncodeDataBase64(lua_State *L) {
  const unsigned char * data = (const unsigned char *)luaL_checkstring(L, 1);
  int dataSize = luaL_checkinteger(L, 2);
  int * outputSize = (int *)lua_touserdata(L, 3);
  char * ret = EncodeDataBase64(data, dataSize, outputSize);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_DecodeDataBase64(lua_State *L) {
  const unsigned char * data = (const unsigned char *)luaL_checkstring(L, 1);
  int * outputSize = (int *)lua_touserdata(L, 2);
  unsigned char * ret = DecodeDataBase64(data, outputSize);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_LoadAutomationEventList(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  AutomationEventList ret = LoadAutomationEventList(fileName);
  lua_pushAutomationEventList(L, &ret);
  return 1;
}
static int lua_UnloadAutomationEventList(lua_State *L) {
  AutomationEventList * list = (AutomationEventList *)lua_touserdata(L, 1);
  UnloadAutomationEventList(list);
  return 0;
}
static int lua_ExportAutomationEventList(lua_State *L) {
  AutomationEventList list = lua_checkAutomationEventList(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportAutomationEventList(list, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_SetAutomationEventList(lua_State *L) {
  AutomationEventList * list = (AutomationEventList *)lua_touserdata(L, 1);
  SetAutomationEventList(list);
  return 0;
}
static int lua_SetAutomationEventBaseFrame(lua_State *L) {
  int frame = luaL_checkinteger(L, 1);
  SetAutomationEventBaseFrame(frame);
  return 0;
}
static int lua_StartAutomationEventRecording(lua_State *L) {
  StartAutomationEventRecording();
  return 0;
}
static int lua_StopAutomationEventRecording(lua_State *L) {
  StopAutomationEventRecording();
  return 0;
}
static int lua_PlayAutomationEvent(lua_State *L) {
  AutomationEvent event = lua_checkAutomationEvent(L, 1);
  PlayAutomationEvent(event);
  return 0;
}
static int lua_IsKeyPressed(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  bool ret = IsKeyPressed(key);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsKeyPressedRepeat(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  bool ret = IsKeyPressedRepeat(key);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsKeyDown(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  bool ret = IsKeyDown(key);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsKeyReleased(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  bool ret = IsKeyReleased(key);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsKeyUp(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  bool ret = IsKeyUp(key);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetKeyPressed(lua_State *L) {
  int ret = GetKeyPressed();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetCharPressed(lua_State *L) {
  int ret = GetCharPressed();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_SetExitKey(lua_State *L) {
  int key = luaL_checkinteger(L, 1);
  SetExitKey(key);
  return 0;
}
static int lua_IsGamepadAvailable(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  bool ret = IsGamepadAvailable(gamepad);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetGamepadName(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  const char * ret = GetGamepadName(gamepad);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_IsGamepadButtonPressed(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int button = luaL_checkinteger(L, 2);
  bool ret = IsGamepadButtonPressed(gamepad, button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsGamepadButtonDown(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int button = luaL_checkinteger(L, 2);
  bool ret = IsGamepadButtonDown(gamepad, button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsGamepadButtonReleased(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int button = luaL_checkinteger(L, 2);
  bool ret = IsGamepadButtonReleased(gamepad, button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsGamepadButtonUp(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int button = luaL_checkinteger(L, 2);
  bool ret = IsGamepadButtonUp(gamepad, button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetGamepadButtonPressed(lua_State *L) {
  int ret = GetGamepadButtonPressed();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetGamepadAxisCount(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int ret = GetGamepadAxisCount(gamepad);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetGamepadAxisMovement(lua_State *L) {
  int gamepad = luaL_checkinteger(L, 1);
  int axis = luaL_checkinteger(L, 2);
  float ret = GetGamepadAxisMovement(gamepad, axis);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_SetGamepadMappings(lua_State *L) {
  const char * mappings = luaL_checkstring(L, 1);
  int ret = SetGamepadMappings(mappings);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_IsMouseButtonPressed(lua_State *L) {
  int button = luaL_checkinteger(L, 1);
  bool ret = IsMouseButtonPressed(button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsMouseButtonDown(lua_State *L) {
  int button = luaL_checkinteger(L, 1);
  bool ret = IsMouseButtonDown(button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsMouseButtonReleased(lua_State *L) {
  int button = luaL_checkinteger(L, 1);
  bool ret = IsMouseButtonReleased(button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_IsMouseButtonUp(lua_State *L) {
  int button = luaL_checkinteger(L, 1);
  bool ret = IsMouseButtonUp(button);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetMouseX(lua_State *L) {
  int ret = GetMouseX();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMouseY(lua_State *L) {
  int ret = GetMouseY();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetMousePosition(lua_State *L) {
  Vector2 ret = GetMousePosition();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetMouseDelta(lua_State *L) {
  Vector2 ret = GetMouseDelta();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_SetMousePosition(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  SetMousePosition(x, y);
  return 0;
}
static int lua_SetMouseOffset(lua_State *L) {
  int offsetX = luaL_checkinteger(L, 1);
  int offsetY = luaL_checkinteger(L, 2);
  SetMouseOffset(offsetX, offsetY);
  return 0;
}
static int lua_SetMouseScale(lua_State *L) {
  float scaleX = (float)luaL_checknumber(L, 1);
  float scaleY = (float)luaL_checknumber(L, 2);
  SetMouseScale(scaleX, scaleY);
  return 0;
}
static int lua_GetMouseWheelMove(lua_State *L) {
  float ret = GetMouseWheelMove();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetMouseWheelMoveV(lua_State *L) {
  Vector2 ret = GetMouseWheelMoveV();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_SetMouseCursor(lua_State *L) {
  int cursor = luaL_checkinteger(L, 1);
  SetMouseCursor(cursor);
  return 0;
}
static int lua_GetTouchX(lua_State *L) {
  int ret = GetTouchX();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetTouchY(lua_State *L) {
  int ret = GetTouchY();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetTouchPosition(lua_State *L) {
  int index = luaL_checkinteger(L, 1);
  Vector2 ret = GetTouchPosition(index);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetTouchPointId(lua_State *L) {
  int index = luaL_checkinteger(L, 1);
  int ret = GetTouchPointId(index);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetTouchPointCount(lua_State *L) {
  int ret = GetTouchPointCount();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_SetGesturesEnabled(lua_State *L) {
  unsigned int flags = (unsigned int)luaL_checkinteger(L, 1);
  SetGesturesEnabled(flags);
  return 0;
}
static int lua_IsGestureDetected(lua_State *L) {
  unsigned int gesture = (unsigned int)luaL_checkinteger(L, 1);
  bool ret = IsGestureDetected(gesture);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetGestureDetected(lua_State *L) {
  int ret = GetGestureDetected();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetGestureHoldDuration(lua_State *L) {
  float ret = GetGestureHoldDuration();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetGestureDragVector(lua_State *L) {
  Vector2 ret = GetGestureDragVector();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetGestureDragAngle(lua_State *L) {
  float ret = GetGestureDragAngle();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetGesturePinchVector(lua_State *L) {
  Vector2 ret = GetGesturePinchVector();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetGesturePinchAngle(lua_State *L) {
  float ret = GetGesturePinchAngle();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_UpdateCamera(lua_State *L) {
  Camera * camera = (Camera *)lua_touserdata(L, 1);
  int mode = luaL_checkinteger(L, 2);
  UpdateCamera(camera, mode);
  return 0;
}
static int lua_UpdateCameraPro(lua_State *L) {
  Camera * camera = (Camera *)lua_touserdata(L, 1);
  Vector3 movement = lua_checkVector3(L, 2);
  Vector3 rotation = lua_checkVector3(L, 3);
  float zoom = (float)luaL_checknumber(L, 4);
  UpdateCameraPro(camera, movement, rotation, zoom);
  return 0;
}
static int lua_SetShapesTexture(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Rectangle source = lua_checkRectangle(L, 2);
  SetShapesTexture(texture, source);
  return 0;
}
static int lua_DrawPixel(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawPixel(posX, posY, color);
  return 0;
}
static int lua_DrawPixelV(lua_State *L) {
  Vector2 position = lua_checkVector2(L, 1);
  Color color = lua_checkColor(L, 2);
  DrawPixelV(position, color);
  return 0;
}
static int lua_DrawLine(lua_State *L) {
  int startPosX = luaL_checkinteger(L, 1);
  int startPosY = luaL_checkinteger(L, 2);
  int endPosX = luaL_checkinteger(L, 3);
  int endPosY = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawLine(startPosX, startPosY, endPosX, endPosY, color);
  return 0;
}
static int lua_DrawLineV(lua_State *L) {
  Vector2 startPos = lua_checkVector2(L, 1);
  Vector2 endPos = lua_checkVector2(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawLineV(startPos, endPos, color);
  return 0;
}
static int lua_DrawLineEx(lua_State *L) {
  Vector2 startPos = lua_checkVector2(L, 1);
  Vector2 endPos = lua_checkVector2(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawLineEx(startPos, endPos, thick, color);
  return 0;
}
static int lua_DrawLineStrip(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawLineStrip(points, pointCount, color);
  return 0;
}
static int lua_DrawLineBezier(lua_State *L) {
  Vector2 startPos = lua_checkVector2(L, 1);
  Vector2 endPos = lua_checkVector2(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawLineBezier(startPos, endPos, thick, color);
  return 0;
}
static int lua_DrawCircle(lua_State *L) {
  int centerX = luaL_checkinteger(L, 1);
  int centerY = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawCircle(centerX, centerY, radius, color);
  return 0;
}
static int lua_DrawCircleSector(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  float startAngle = (float)luaL_checknumber(L, 3);
  float endAngle = (float)luaL_checknumber(L, 4);
  int segments = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCircleSector(center, radius, startAngle, endAngle, segments, color);
  return 0;
}
static int lua_DrawCircleSectorLines(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  float startAngle = (float)luaL_checknumber(L, 3);
  float endAngle = (float)luaL_checknumber(L, 4);
  int segments = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCircleSectorLines(center, radius, startAngle, endAngle, segments, color);
  return 0;
}
static int lua_DrawCircleGradient(lua_State *L) {
  int centerX = luaL_checkinteger(L, 1);
  int centerY = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  Color color1 = lua_checkColor(L, 4);
  Color color2 = lua_checkColor(L, 5);
  DrawCircleGradient(centerX, centerY, radius, color1, color2);
  return 0;
}
static int lua_DrawCircleV(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawCircleV(center, radius, color);
  return 0;
}
static int lua_DrawCircleLines(lua_State *L) {
  int centerX = luaL_checkinteger(L, 1);
  int centerY = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawCircleLines(centerX, centerY, radius, color);
  return 0;
}
static int lua_DrawCircleLinesV(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawCircleLinesV(center, radius, color);
  return 0;
}
static int lua_DrawEllipse(lua_State *L) {
  int centerX = luaL_checkinteger(L, 1);
  int centerY = luaL_checkinteger(L, 2);
  float radiusH = (float)luaL_checknumber(L, 3);
  float radiusV = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawEllipse(centerX, centerY, radiusH, radiusV, color);
  return 0;
}
static int lua_DrawEllipseLines(lua_State *L) {
  int centerX = luaL_checkinteger(L, 1);
  int centerY = luaL_checkinteger(L, 2);
  float radiusH = (float)luaL_checknumber(L, 3);
  float radiusV = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawEllipseLines(centerX, centerY, radiusH, radiusV, color);
  return 0;
}
static int lua_DrawRing(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float innerRadius = (float)luaL_checknumber(L, 2);
  float outerRadius = (float)luaL_checknumber(L, 3);
  float startAngle = (float)luaL_checknumber(L, 4);
  float endAngle = (float)luaL_checknumber(L, 5);
  int segments = luaL_checkinteger(L, 6);
  Color color = lua_checkColor(L, 7);
  DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, color);
  return 0;
}
static int lua_DrawRingLines(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float innerRadius = (float)luaL_checknumber(L, 2);
  float outerRadius = (float)luaL_checknumber(L, 3);
  float startAngle = (float)luaL_checknumber(L, 4);
  float endAngle = (float)luaL_checknumber(L, 5);
  int segments = luaL_checkinteger(L, 6);
  Color color = lua_checkColor(L, 7);
  DrawRingLines(center, innerRadius, outerRadius, startAngle, endAngle, segments, color);
  return 0;
}
static int lua_DrawRectangle(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawRectangle(posX, posY, width, height, color);
  return 0;
}
static int lua_DrawRectangleV(lua_State *L) {
  Vector2 position = lua_checkVector2(L, 1);
  Vector2 size = lua_checkVector2(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawRectangleV(position, size, color);
  return 0;
}
static int lua_DrawRectangleRec(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  Color color = lua_checkColor(L, 2);
  DrawRectangleRec(rec, color);
  return 0;
}
static int lua_DrawRectanglePro(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  Vector2 origin = lua_checkVector2(L, 2);
  float rotation = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawRectanglePro(rec, origin, rotation, color);
  return 0;
}
static int lua_DrawRectangleGradientV(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  Color color1 = lua_checkColor(L, 5);
  Color color2 = lua_checkColor(L, 6);
  DrawRectangleGradientV(posX, posY, width, height, color1, color2);
  return 0;
}
static int lua_DrawRectangleGradientH(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  Color color1 = lua_checkColor(L, 5);
  Color color2 = lua_checkColor(L, 6);
  DrawRectangleGradientH(posX, posY, width, height, color1, color2);
  return 0;
}
static int lua_DrawRectangleGradientEx(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  Color col1 = lua_checkColor(L, 2);
  Color col2 = lua_checkColor(L, 3);
  Color col3 = lua_checkColor(L, 4);
  Color col4 = lua_checkColor(L, 5);
  DrawRectangleGradientEx(rec, col1, col2, col3, col4);
  return 0;
}
static int lua_DrawRectangleLines(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawRectangleLines(posX, posY, width, height, color);
  return 0;
}
static int lua_DrawRectangleLinesEx(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  float lineThick = (float)luaL_checknumber(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawRectangleLinesEx(rec, lineThick, color);
  return 0;
}
static int lua_DrawRectangleRounded(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  float roundness = (float)luaL_checknumber(L, 2);
  int segments = luaL_checkinteger(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawRectangleRounded(rec, roundness, segments, color);
  return 0;
}
static int lua_DrawRectangleRoundedLines(lua_State *L) {
  Rectangle rec = lua_checkRectangle(L, 1);
  float roundness = (float)luaL_checknumber(L, 2);
  int segments = luaL_checkinteger(L, 3);
  float lineThick = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawRectangleRoundedLines(rec, roundness, segments, lineThick, color);
  return 0;
}
static int lua_DrawTriangle(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 v3 = lua_checkVector2(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawTriangle(v1, v2, v3, color);
  return 0;
}
static int lua_DrawTriangleLines(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 v3 = lua_checkVector2(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawTriangleLines(v1, v2, v3, color);
  return 0;
}
static int lua_DrawTriangleFan(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawTriangleFan(points, pointCount, color);
  return 0;
}
static int lua_DrawTriangleStrip(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawTriangleStrip(points, pointCount, color);
  return 0;
}
static int lua_DrawPoly(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  int sides = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  float rotation = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawPoly(center, sides, radius, rotation, color);
  return 0;
}
static int lua_DrawPolyLines(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  int sides = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  float rotation = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawPolyLines(center, sides, radius, rotation, color);
  return 0;
}
static int lua_DrawPolyLinesEx(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  int sides = luaL_checkinteger(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  float rotation = (float)luaL_checknumber(L, 4);
  float lineThick = (float)luaL_checknumber(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawPolyLinesEx(center, sides, radius, rotation, lineThick, color);
  return 0;
}
static int lua_DrawSplineLinear(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineLinear(points, pointCount, thick, color);
  return 0;
}
static int lua_DrawSplineBasis(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineBasis(points, pointCount, thick, color);
  return 0;
}
static int lua_DrawSplineCatmullRom(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineCatmullRom(points, pointCount, thick, color);
  return 0;
}
static int lua_DrawSplineBezierQuadratic(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineBezierQuadratic(points, pointCount, thick, color);
  return 0;
}
static int lua_DrawSplineBezierCubic(lua_State *L) {
  Vector2 * points = (Vector2 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineBezierCubic(points, pointCount, thick, color);
  return 0;
}
static int lua_DrawSplineSegmentLinear(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 p2 = lua_checkVector2(L, 2);
  float thick = (float)luaL_checknumber(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawSplineSegmentLinear(p1, p2, thick, color);
  return 0;
}
static int lua_DrawSplineSegmentBasis(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 p2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float thick = (float)luaL_checknumber(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawSplineSegmentBasis(p1, p2, p3, p4, thick, color);
  return 0;
}
static int lua_DrawSplineSegmentCatmullRom(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 p2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float thick = (float)luaL_checknumber(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawSplineSegmentCatmullRom(p1, p2, p3, p4, thick, color);
  return 0;
}
static int lua_DrawSplineSegmentBezierQuadratic(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 c2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  float thick = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawSplineSegmentBezierQuadratic(p1, c2, p3, thick, color);
  return 0;
}
static int lua_DrawSplineSegmentBezierCubic(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 c2 = lua_checkVector2(L, 2);
  Vector2 c3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float thick = (float)luaL_checknumber(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawSplineSegmentBezierCubic(p1, c2, c3, p4, thick, color);
  return 0;
}
static int lua_GetSplinePointLinear(lua_State *L) {
  Vector2 startPos = lua_checkVector2(L, 1);
  Vector2 endPos = lua_checkVector2(L, 2);
  float t = (float)luaL_checknumber(L, 3);
  Vector2 ret = GetSplinePointLinear(startPos, endPos, t);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetSplinePointBasis(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 p2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float t = (float)luaL_checknumber(L, 5);
  Vector2 ret = GetSplinePointBasis(p1, p2, p3, p4, t);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetSplinePointCatmullRom(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 p2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float t = (float)luaL_checknumber(L, 5);
  Vector2 ret = GetSplinePointCatmullRom(p1, p2, p3, p4, t);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetSplinePointBezierQuad(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 c2 = lua_checkVector2(L, 2);
  Vector2 p3 = lua_checkVector2(L, 3);
  float t = (float)luaL_checknumber(L, 4);
  Vector2 ret = GetSplinePointBezierQuad(p1, c2, p3, t);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetSplinePointBezierCubic(lua_State *L) {
  Vector2 p1 = lua_checkVector2(L, 1);
  Vector2 c2 = lua_checkVector2(L, 2);
  Vector2 c3 = lua_checkVector2(L, 3);
  Vector2 p4 = lua_checkVector2(L, 4);
  float t = (float)luaL_checknumber(L, 5);
  Vector2 ret = GetSplinePointBezierCubic(p1, c2, c3, p4, t);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_CheckCollisionRecs(lua_State *L) {
  Rectangle rec1 = lua_checkRectangle(L, 1);
  Rectangle rec2 = lua_checkRectangle(L, 2);
  bool ret = CheckCollisionRecs(rec1, rec2);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionCircles(lua_State *L) {
  Vector2 center1 = lua_checkVector2(L, 1);
  float radius1 = (float)luaL_checknumber(L, 2);
  Vector2 center2 = lua_checkVector2(L, 3);
  float radius2 = (float)luaL_checknumber(L, 4);
  bool ret = CheckCollisionCircles(center1, radius1, center2, radius2);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionCircleRec(lua_State *L) {
  Vector2 center = lua_checkVector2(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Rectangle rec = lua_checkRectangle(L, 3);
  bool ret = CheckCollisionCircleRec(center, radius, rec);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionPointRec(lua_State *L) {
  Vector2 point = lua_checkVector2(L, 1);
  Rectangle rec = lua_checkRectangle(L, 2);
  bool ret = CheckCollisionPointRec(point, rec);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionPointCircle(lua_State *L) {
  Vector2 point = lua_checkVector2(L, 1);
  Vector2 center = lua_checkVector2(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  bool ret = CheckCollisionPointCircle(point, center, radius);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionPointTriangle(lua_State *L) {
  Vector2 point = lua_checkVector2(L, 1);
  Vector2 p1 = lua_checkVector2(L, 2);
  Vector2 p2 = lua_checkVector2(L, 3);
  Vector2 p3 = lua_checkVector2(L, 4);
  bool ret = CheckCollisionPointTriangle(point, p1, p2, p3);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionPointPoly(lua_State *L) {
  Vector2 point = lua_checkVector2(L, 1);
  Vector2 * points = (Vector2 *)lua_touserdata(L, 2);
  int pointCount = luaL_checkinteger(L, 3);
  bool ret = CheckCollisionPointPoly(point, points, pointCount);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionLines(lua_State *L) {
  Vector2 startPos1 = lua_checkVector2(L, 1);
  Vector2 endPos1 = lua_checkVector2(L, 2);
  Vector2 startPos2 = lua_checkVector2(L, 3);
  Vector2 endPos2 = lua_checkVector2(L, 4);
  Vector2 * collisionPoint = (Vector2 *)lua_touserdata(L, 5);
  bool ret = CheckCollisionLines(startPos1, endPos1, startPos2, endPos2, collisionPoint);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionPointLine(lua_State *L) {
  Vector2 point = lua_checkVector2(L, 1);
  Vector2 p1 = lua_checkVector2(L, 2);
  Vector2 p2 = lua_checkVector2(L, 3);
  int threshold = luaL_checkinteger(L, 4);
  bool ret = CheckCollisionPointLine(point, p1, p2, threshold);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetCollisionRec(lua_State *L) {
  Rectangle rec1 = lua_checkRectangle(L, 1);
  Rectangle rec2 = lua_checkRectangle(L, 2);
  Rectangle ret = GetCollisionRec(rec1, rec2);
  lua_pushRectangle(L, &ret);
  return 1;
}
static int lua_LoadImage(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Image ret = LoadImage(fileName);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageRaw(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int width = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  int format = luaL_checkinteger(L, 4);
  int headerSize = luaL_checkinteger(L, 5);
  Image ret = LoadImageRaw(fileName, width, height, format, headerSize);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageSvg(lua_State *L) {
  const char * fileNameOrString = luaL_checkstring(L, 1);
  int width = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  Image ret = LoadImageSvg(fileNameOrString, width, height);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageAnim(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int * frames = (int *)lua_touserdata(L, 2);
  Image ret = LoadImageAnim(fileName, frames);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageFromMemory(lua_State *L) {
  const char * fileType = luaL_checkstring(L, 1);
  const unsigned char * fileData = (const unsigned char *)luaL_checkstring(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  Image ret = LoadImageFromMemory(fileType, fileData, dataSize);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageFromTexture(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Image ret = LoadImageFromTexture(texture);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_LoadImageFromScreen(lua_State *L) {
  Image ret = LoadImageFromScreen();
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_IsImageReady(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  bool ret = IsImageReady(image);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadImage(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  UnloadImage(image);
  return 0;
}
static int lua_ExportImage(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportImage(image, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_ExportImageToMemory(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  const char * fileType = luaL_checkstring(L, 2);
  int * fileSize = (int *)lua_touserdata(L, 3);
  unsigned char * ret = ExportImageToMemory(image, fileType, fileSize);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_ExportImageAsCode(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportImageAsCode(image, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GenImageColor(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  Image ret = GenImageColor(width, height, color);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageGradientLinear(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  int direction = luaL_checkinteger(L, 3);
  Color start = lua_checkColor(L, 4);
  Color end = lua_checkColor(L, 5);
  Image ret = GenImageGradientLinear(width, height, direction, start, end);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageGradientRadial(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  float density = (float)luaL_checknumber(L, 3);
  Color inner = lua_checkColor(L, 4);
  Color outer = lua_checkColor(L, 5);
  Image ret = GenImageGradientRadial(width, height, density, inner, outer);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageGradientSquare(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  float density = (float)luaL_checknumber(L, 3);
  Color inner = lua_checkColor(L, 4);
  Color outer = lua_checkColor(L, 5);
  Image ret = GenImageGradientSquare(width, height, density, inner, outer);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageChecked(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  int checksX = luaL_checkinteger(L, 3);
  int checksY = luaL_checkinteger(L, 4);
  Color col1 = lua_checkColor(L, 5);
  Color col2 = lua_checkColor(L, 6);
  Image ret = GenImageChecked(width, height, checksX, checksY, col1, col2);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageWhiteNoise(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  float factor = (float)luaL_checknumber(L, 3);
  Image ret = GenImageWhiteNoise(width, height, factor);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImagePerlinNoise(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  int offsetX = luaL_checkinteger(L, 3);
  int offsetY = luaL_checkinteger(L, 4);
  float scale = (float)luaL_checknumber(L, 5);
  Image ret = GenImagePerlinNoise(width, height, offsetX, offsetY, scale);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageCellular(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  int tileSize = luaL_checkinteger(L, 3);
  Image ret = GenImageCellular(width, height, tileSize);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_GenImageText(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  const char * text = luaL_checkstring(L, 3);
  Image ret = GenImageText(width, height, text);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_ImageCopy(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  Image ret = ImageCopy(image);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_ImageFromImage(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  Rectangle rec = lua_checkRectangle(L, 2);
  Image ret = ImageFromImage(image, rec);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_ImageText(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int fontSize = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  Image ret = ImageText(text, fontSize, color);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_ImageTextEx(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const char * text = luaL_checkstring(L, 2);
  float fontSize = (float)luaL_checknumber(L, 3);
  float spacing = (float)luaL_checknumber(L, 4);
  Color tint = lua_checkColor(L, 5);
  Image ret = ImageTextEx(font, text, fontSize, spacing, tint);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_ImageFormat(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int newFormat = luaL_checkinteger(L, 2);
  ImageFormat(image, newFormat);
  return 0;
}
static int lua_ImageToPOT(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Color fill = lua_checkColor(L, 2);
  ImageToPOT(image, fill);
  return 0;
}
static int lua_ImageCrop(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Rectangle crop = lua_checkRectangle(L, 2);
  ImageCrop(image, crop);
  return 0;
}
static int lua_ImageAlphaCrop(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  float threshold = (float)luaL_checknumber(L, 2);
  ImageAlphaCrop(image, threshold);
  return 0;
}
static int lua_ImageAlphaClear(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Color color = lua_checkColor(L, 2);
  float threshold = (float)luaL_checknumber(L, 3);
  ImageAlphaClear(image, color, threshold);
  return 0;
}
static int lua_ImageAlphaMask(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Image alphaMask = lua_checkImage(L, 2);
  ImageAlphaMask(image, alphaMask);
  return 0;
}
static int lua_ImageAlphaPremultiply(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageAlphaPremultiply(image);
  return 0;
}
static int lua_ImageBlurGaussian(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int blurSize = luaL_checkinteger(L, 2);
  ImageBlurGaussian(image, blurSize);
  return 0;
}
static int lua_ImageResize(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int newWidth = luaL_checkinteger(L, 2);
  int newHeight = luaL_checkinteger(L, 3);
  ImageResize(image, newWidth, newHeight);
  return 0;
}
static int lua_ImageResizeNN(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int newWidth = luaL_checkinteger(L, 2);
  int newHeight = luaL_checkinteger(L, 3);
  ImageResizeNN(image, newWidth, newHeight);
  return 0;
}
static int lua_ImageResizeCanvas(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int newWidth = luaL_checkinteger(L, 2);
  int newHeight = luaL_checkinteger(L, 3);
  int offsetX = luaL_checkinteger(L, 4);
  int offsetY = luaL_checkinteger(L, 5);
  Color fill = lua_checkColor(L, 6);
  ImageResizeCanvas(image, newWidth, newHeight, offsetX, offsetY, fill);
  return 0;
}
static int lua_ImageMipmaps(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageMipmaps(image);
  return 0;
}
static int lua_ImageDither(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int rBpp = luaL_checkinteger(L, 2);
  int gBpp = luaL_checkinteger(L, 3);
  int bBpp = luaL_checkinteger(L, 4);
  int aBpp = luaL_checkinteger(L, 5);
  ImageDither(image, rBpp, gBpp, bBpp, aBpp);
  return 0;
}
static int lua_ImageFlipVertical(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageFlipVertical(image);
  return 0;
}
static int lua_ImageFlipHorizontal(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageFlipHorizontal(image);
  return 0;
}
static int lua_ImageRotate(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int degrees = luaL_checkinteger(L, 2);
  ImageRotate(image, degrees);
  return 0;
}
static int lua_ImageRotateCW(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageRotateCW(image);
  return 0;
}
static int lua_ImageRotateCCW(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageRotateCCW(image);
  return 0;
}
static int lua_ImageColorTint(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Color color = lua_checkColor(L, 2);
  ImageColorTint(image, color);
  return 0;
}
static int lua_ImageColorInvert(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageColorInvert(image);
  return 0;
}
static int lua_ImageColorGrayscale(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  ImageColorGrayscale(image);
  return 0;
}
static int lua_ImageColorContrast(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  float contrast = (float)luaL_checknumber(L, 2);
  ImageColorContrast(image, contrast);
  return 0;
}
static int lua_ImageColorBrightness(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  int brightness = luaL_checkinteger(L, 2);
  ImageColorBrightness(image, brightness);
  return 0;
}
static int lua_ImageColorReplace(lua_State *L) {
  Image * image = (Image *)lua_touserdata(L, 1);
  Color color = lua_checkColor(L, 2);
  Color replace = lua_checkColor(L, 3);
  ImageColorReplace(image, color, replace);
  return 0;
}
static int lua_LoadImageColors(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  Color * ret = LoadImageColors(image);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_LoadImagePalette(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  int maxPaletteSize = luaL_checkinteger(L, 2);
  int * colorCount = (int *)lua_touserdata(L, 3);
  Color * ret = LoadImagePalette(image, maxPaletteSize, colorCount);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_UnloadImageColors(lua_State *L) {
  Color * colors = (Color *)lua_touserdata(L, 1);
  UnloadImageColors(colors);
  return 0;
}
static int lua_UnloadImagePalette(lua_State *L) {
  Color * colors = (Color *)lua_touserdata(L, 1);
  UnloadImagePalette(colors);
  return 0;
}
static int lua_GetImageAlphaBorder(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  float threshold = (float)luaL_checknumber(L, 2);
  Rectangle ret = GetImageAlphaBorder(image, threshold);
  lua_pushRectangle(L, &ret);
  return 1;
}
static int lua_GetImageColor(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  int x = luaL_checkinteger(L, 2);
  int y = luaL_checkinteger(L, 3);
  Color ret = GetImageColor(image, x, y);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ImageClearBackground(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Color color = lua_checkColor(L, 2);
  ImageClearBackground(dst, color);
  return 0;
}
static int lua_ImageDrawPixel(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  int posX = luaL_checkinteger(L, 2);
  int posY = luaL_checkinteger(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawPixel(dst, posX, posY, color);
  return 0;
}
static int lua_ImageDrawPixelV(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Vector2 position = lua_checkVector2(L, 2);
  Color color = lua_checkColor(L, 3);
  ImageDrawPixelV(dst, position, color);
  return 0;
}
static int lua_ImageDrawLine(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  int startPosX = luaL_checkinteger(L, 2);
  int startPosY = luaL_checkinteger(L, 3);
  int endPosX = luaL_checkinteger(L, 4);
  int endPosY = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  ImageDrawLine(dst, startPosX, startPosY, endPosX, endPosY, color);
  return 0;
}
static int lua_ImageDrawLineV(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Vector2 start = lua_checkVector2(L, 2);
  Vector2 end = lua_checkVector2(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawLineV(dst, start, end, color);
  return 0;
}
static int lua_ImageDrawCircle(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  int centerX = luaL_checkinteger(L, 2);
  int centerY = luaL_checkinteger(L, 3);
  int radius = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  ImageDrawCircle(dst, centerX, centerY, radius, color);
  return 0;
}
static int lua_ImageDrawCircleV(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Vector2 center = lua_checkVector2(L, 2);
  int radius = luaL_checkinteger(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawCircleV(dst, center, radius, color);
  return 0;
}
static int lua_ImageDrawCircleLines(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  int centerX = luaL_checkinteger(L, 2);
  int centerY = luaL_checkinteger(L, 3);
  int radius = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  ImageDrawCircleLines(dst, centerX, centerY, radius, color);
  return 0;
}
static int lua_ImageDrawCircleLinesV(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Vector2 center = lua_checkVector2(L, 2);
  int radius = luaL_checkinteger(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawCircleLinesV(dst, center, radius, color);
  return 0;
}
static int lua_ImageDrawRectangle(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  int posX = luaL_checkinteger(L, 2);
  int posY = luaL_checkinteger(L, 3);
  int width = luaL_checkinteger(L, 4);
  int height = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  ImageDrawRectangle(dst, posX, posY, width, height, color);
  return 0;
}
static int lua_ImageDrawRectangleV(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Vector2 position = lua_checkVector2(L, 2);
  Vector2 size = lua_checkVector2(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawRectangleV(dst, position, size, color);
  return 0;
}
static int lua_ImageDrawRectangleRec(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Rectangle rec = lua_checkRectangle(L, 2);
  Color color = lua_checkColor(L, 3);
  ImageDrawRectangleRec(dst, rec, color);
  return 0;
}
static int lua_ImageDrawRectangleLines(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Rectangle rec = lua_checkRectangle(L, 2);
  int thick = luaL_checkinteger(L, 3);
  Color color = lua_checkColor(L, 4);
  ImageDrawRectangleLines(dst, rec, thick, color);
  return 0;
}
static int lua_ImageDraw(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Image src = lua_checkImage(L, 2);
  Rectangle srcRec = lua_checkRectangle(L, 3);
  Rectangle dstRec = lua_checkRectangle(L, 4);
  Color tint = lua_checkColor(L, 5);
  ImageDraw(dst, src, srcRec, dstRec, tint);
  return 0;
}
static int lua_ImageDrawText(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  const char * text = luaL_checkstring(L, 2);
  int posX = luaL_checkinteger(L, 3);
  int posY = luaL_checkinteger(L, 4);
  int fontSize = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  ImageDrawText(dst, text, posX, posY, fontSize, color);
  return 0;
}
static int lua_ImageDrawTextEx(lua_State *L) {
  Image * dst = (Image *)lua_touserdata(L, 1);
  Font font = lua_checkFont(L, 2);
  const char * text = luaL_checkstring(L, 3);
  Vector2 position = lua_checkVector2(L, 4);
  float fontSize = (float)luaL_checknumber(L, 5);
  float spacing = (float)luaL_checknumber(L, 6);
  Color tint = lua_checkColor(L, 7);
  ImageDrawTextEx(dst, font, text, position, fontSize, spacing, tint);
  return 0;
}
static int lua_LoadTexture(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Texture2D ret = LoadTexture(fileName);
  lua_pushTexture(L, &ret);
  return 1;
}
static int lua_LoadTextureFromImage(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  Texture2D ret = LoadTextureFromImage(image);
  lua_pushTexture(L, &ret);
  return 1;
}
static int lua_LoadTextureCubemap(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  int layout = luaL_checkinteger(L, 2);
  TextureCubemap ret = LoadTextureCubemap(image, layout);
  lua_pushTexture(L, &ret);
  return 1;
}
static int lua_LoadRenderTexture(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  RenderTexture2D ret = LoadRenderTexture(width, height);
  lua_pushRenderTexture(L, &ret);
  return 1;
}
static int lua_IsTextureReady(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  bool ret = IsTextureReady(texture);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadTexture(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  UnloadTexture(texture);
  return 0;
}
static int lua_IsRenderTextureReady(lua_State *L) {
  RenderTexture2D target = lua_checkRenderTexture(L, 1);
  bool ret = IsRenderTextureReady(target);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadRenderTexture(lua_State *L) {
  RenderTexture2D target = lua_checkRenderTexture(L, 1);
  UnloadRenderTexture(target);
  return 0;
}
static int lua_UpdateTexture(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  const void * pixels = lua_touserdata(L, 2);
  UpdateTexture(texture, pixels);
  return 0;
}
static int lua_UpdateTextureRec(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Rectangle rec = lua_checkRectangle(L, 2);
  const void * pixels = lua_touserdata(L, 3);
  UpdateTextureRec(texture, rec, pixels);
  return 0;
}
static int lua_GenTextureMipmaps(lua_State *L) {
  Texture2D * texture = (Texture2D *)lua_touserdata(L, 1);
  GenTextureMipmaps(texture);
  return 0;
}
static int lua_SetTextureFilter(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  int filter = luaL_checkinteger(L, 2);
  SetTextureFilter(texture, filter);
  return 0;
}
static int lua_SetTextureWrap(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  int wrap = luaL_checkinteger(L, 2);
  SetTextureWrap(texture, wrap);
  return 0;
}
static int lua_DrawTexture(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  int posX = luaL_checkinteger(L, 2);
  int posY = luaL_checkinteger(L, 3);
  Color tint = lua_checkColor(L, 4);
  DrawTexture(texture, posX, posY, tint);
  return 0;
}
static int lua_DrawTextureV(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Vector2 position = lua_checkVector2(L, 2);
  Color tint = lua_checkColor(L, 3);
  DrawTextureV(texture, position, tint);
  return 0;
}
static int lua_DrawTextureEx(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Vector2 position = lua_checkVector2(L, 2);
  float rotation = (float)luaL_checknumber(L, 3);
  float scale = (float)luaL_checknumber(L, 4);
  Color tint = lua_checkColor(L, 5);
  DrawTextureEx(texture, position, rotation, scale, tint);
  return 0;
}
static int lua_DrawTextureRec(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Rectangle source = lua_checkRectangle(L, 2);
  Vector2 position = lua_checkVector2(L, 3);
  Color tint = lua_checkColor(L, 4);
  DrawTextureRec(texture, source, position, tint);
  return 0;
}
static int lua_DrawTexturePro(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  Rectangle source = lua_checkRectangle(L, 2);
  Rectangle dest = lua_checkRectangle(L, 3);
  Vector2 origin = lua_checkVector2(L, 4);
  float rotation = (float)luaL_checknumber(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
  return 0;
}
static int lua_DrawTextureNPatch(lua_State *L) {
  Texture2D texture = lua_checkTexture(L, 1);
  NPatchInfo nPatchInfo = lua_checkNPatchInfo(L, 2);
  Rectangle dest = lua_checkRectangle(L, 3);
  Vector2 origin = lua_checkVector2(L, 4);
  float rotation = (float)luaL_checknumber(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawTextureNPatch(texture, nPatchInfo, dest, origin, rotation, tint);
  return 0;
}
static int lua_Fade(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  float alpha = (float)luaL_checknumber(L, 2);
  Color ret = Fade(color, alpha);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorToInt(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  int ret = ColorToInt(color);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_ColorNormalize(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  Vector4 ret = ColorNormalize(color);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_ColorFromNormalized(lua_State *L) {
  Vector4 normalized = lua_checkVector4(L, 1);
  Color ret = ColorFromNormalized(normalized);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorToHSV(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  Vector3 ret = ColorToHSV(color);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_ColorFromHSV(lua_State *L) {
  float hue = (float)luaL_checknumber(L, 1);
  float saturation = (float)luaL_checknumber(L, 2);
  float value = (float)luaL_checknumber(L, 3);
  Color ret = ColorFromHSV(hue, saturation, value);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorTint(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  Color tint = lua_checkColor(L, 2);
  Color ret = ColorTint(color, tint);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorBrightness(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  float factor = (float)luaL_checknumber(L, 2);
  Color ret = ColorBrightness(color, factor);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorContrast(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  float contrast = (float)luaL_checknumber(L, 2);
  Color ret = ColorContrast(color, contrast);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorAlpha(lua_State *L) {
  Color color = lua_checkColor(L, 1);
  float alpha = (float)luaL_checknumber(L, 2);
  Color ret = ColorAlpha(color, alpha);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_ColorAlphaBlend(lua_State *L) {
  Color dst = lua_checkColor(L, 1);
  Color src = lua_checkColor(L, 2);
  Color tint = lua_checkColor(L, 3);
  Color ret = ColorAlphaBlend(dst, src, tint);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_GetColor(lua_State *L) {
  unsigned int hexValue = (unsigned int)luaL_checkinteger(L, 1);
  Color ret = GetColor(hexValue);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_GetPixelColor(lua_State *L) {
  void * srcPtr = lua_touserdata(L, 1);
  int format = luaL_checkinteger(L, 2);
  Color ret = GetPixelColor(srcPtr, format);
  lua_pushColor(L, &ret);
  return 1;
}
static int lua_SetPixelColor(lua_State *L) {
  void * dstPtr = lua_touserdata(L, 1);
  Color color = lua_checkColor(L, 2);
  int format = luaL_checkinteger(L, 3);
  SetPixelColor(dstPtr, color, format);
  return 0;
}
static int lua_GetPixelDataSize(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  int format = luaL_checkinteger(L, 3);
  int ret = GetPixelDataSize(width, height, format);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetFontDefault(lua_State *L) {
  Font ret = GetFontDefault();
  lua_pushFont(L, &ret);
  return 1;
}
static int lua_LoadFont(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Font ret = LoadFont(fileName);
  lua_pushFont(L, &ret);
  return 1;
}
static int lua_LoadFontEx(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int fontSize = luaL_checkinteger(L, 2);
  int * codepoints = (int *)lua_touserdata(L, 3);
  int codepointCount = luaL_checkinteger(L, 4);
  Font ret = LoadFontEx(fileName, fontSize, codepoints, codepointCount);
  lua_pushFont(L, &ret);
  return 1;
}
static int lua_LoadFontFromImage(lua_State *L) {
  Image image = lua_checkImage(L, 1);
  Color key = lua_checkColor(L, 2);
  int firstChar = luaL_checkinteger(L, 3);
  Font ret = LoadFontFromImage(image, key, firstChar);
  lua_pushFont(L, &ret);
  return 1;
}
static int lua_LoadFontFromMemory(lua_State *L) {
  const char * fileType = luaL_checkstring(L, 1);
  const unsigned char * fileData = (const unsigned char *)luaL_checkstring(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  int fontSize = luaL_checkinteger(L, 4);
  int * codepoints = (int *)lua_touserdata(L, 5);
  int codepointCount = luaL_checkinteger(L, 6);
  Font ret = LoadFontFromMemory(fileType, fileData, dataSize, fontSize, codepoints, codepointCount);
  lua_pushFont(L, &ret);
  return 1;
}
static int lua_IsFontReady(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  bool ret = IsFontReady(font);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_LoadFontData(lua_State *L) {
  const unsigned char * fileData = (const unsigned char *)luaL_checkstring(L, 1);
  int dataSize = luaL_checkinteger(L, 2);
  int fontSize = luaL_checkinteger(L, 3);
  int * codepoints = (int *)lua_touserdata(L, 4);
  int codepointCount = luaL_checkinteger(L, 5);
  int type = luaL_checkinteger(L, 6);
  GlyphInfo * ret = LoadFontData(fileData, dataSize, fontSize, codepoints, codepointCount, type);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_GenImageFontAtlas(lua_State *L) {
  const GlyphInfo * glyphs = (const GlyphInfo *)lua_touserdata(L, 1);
  Rectangle ** glyphRecs = (Rectangle **)lua_touserdata(L, 2);
  int glyphCount = luaL_checkinteger(L, 3);
  int fontSize = luaL_checkinteger(L, 4);
  int padding = luaL_checkinteger(L, 5);
  int packMethod = luaL_checkinteger(L, 6);
  Image ret = GenImageFontAtlas(glyphs, glyphRecs, glyphCount, fontSize, padding, packMethod);
  lua_pushImage(L, &ret);
  return 1;
}
static int lua_UnloadFontData(lua_State *L) {
  GlyphInfo * glyphs = (GlyphInfo *)lua_touserdata(L, 1);
  int glyphCount = luaL_checkinteger(L, 2);
  UnloadFontData(glyphs, glyphCount);
  return 0;
}
static int lua_UnloadFont(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  UnloadFont(font);
  return 0;
}
static int lua_ExportFontAsCode(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportFontAsCode(font, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_DrawFPS(lua_State *L) {
  int posX = luaL_checkinteger(L, 1);
  int posY = luaL_checkinteger(L, 2);
  DrawFPS(posX, posY);
  return 0;
}
static int lua_DrawText(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int posX = luaL_checkinteger(L, 2);
  int posY = luaL_checkinteger(L, 3);
  int fontSize = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawText(text, posX, posY, fontSize, color);
  return 0;
}
static int lua_DrawTextEx(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const char * text = luaL_checkstring(L, 2);
  Vector2 position = lua_checkVector2(L, 3);
  float fontSize = (float)luaL_checknumber(L, 4);
  float spacing = (float)luaL_checknumber(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawTextEx(font, text, position, fontSize, spacing, tint);
  return 0;
}
static int lua_DrawTextPro(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const char * text = luaL_checkstring(L, 2);
  Vector2 position = lua_checkVector2(L, 3);
  Vector2 origin = lua_checkVector2(L, 4);
  float rotation = (float)luaL_checknumber(L, 5);
  float fontSize = (float)luaL_checknumber(L, 6);
  float spacing = (float)luaL_checknumber(L, 7);
  Color tint = lua_checkColor(L, 8);
  DrawTextPro(font, text, position, origin, rotation, fontSize, spacing, tint);
  return 0;
}
static int lua_DrawTextCodepoint(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  int codepoint = luaL_checkinteger(L, 2);
  Vector2 position = lua_checkVector2(L, 3);
  float fontSize = (float)luaL_checknumber(L, 4);
  Color tint = lua_checkColor(L, 5);
  DrawTextCodepoint(font, codepoint, position, fontSize, tint);
  return 0;
}
static int lua_DrawTextCodepoints(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const int * codepoints = (const int *)lua_touserdata(L, 2);
  int codepointCount = luaL_checkinteger(L, 3);
  Vector2 position = lua_checkVector2(L, 4);
  float fontSize = (float)luaL_checknumber(L, 5);
  float spacing = (float)luaL_checknumber(L, 6);
  Color tint = lua_checkColor(L, 7);
  DrawTextCodepoints(font, codepoints, codepointCount, position, fontSize, spacing, tint);
  return 0;
}
static int lua_SetTextLineSpacing(lua_State *L) {
  int spacing = luaL_checkinteger(L, 1);
  SetTextLineSpacing(spacing);
  return 0;
}
static int lua_MeasureText(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int fontSize = luaL_checkinteger(L, 2);
  int ret = MeasureText(text, fontSize);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_MeasureTextEx(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  const char * text = luaL_checkstring(L, 2);
  float fontSize = (float)luaL_checknumber(L, 3);
  float spacing = (float)luaL_checknumber(L, 4);
  Vector2 ret = MeasureTextEx(font, text, fontSize, spacing);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_GetGlyphIndex(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  int codepoint = luaL_checkinteger(L, 2);
  int ret = GetGlyphIndex(font, codepoint);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetGlyphInfo(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  int codepoint = luaL_checkinteger(L, 2);
  GlyphInfo ret = GetGlyphInfo(font, codepoint);
  lua_pushGlyphInfo(L, &ret);
  return 1;
}
static int lua_GetGlyphAtlasRec(lua_State *L) {
  Font font = lua_checkFont(L, 1);
  int codepoint = luaL_checkinteger(L, 2);
  Rectangle ret = GetGlyphAtlasRec(font, codepoint);
  lua_pushRectangle(L, &ret);
  return 1;
}
static int lua_LoadUTF8(lua_State *L) {
  const int * codepoints = (const int *)lua_touserdata(L, 1);
  int length = luaL_checkinteger(L, 2);
  char * ret = LoadUTF8(codepoints, length);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_UnloadUTF8(lua_State *L) {
  char * text = (char *)luaL_checkstring(L, 1);
  UnloadUTF8(text);
  return 0;
}
static int lua_LoadCodepoints(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int * count = (int *)lua_touserdata(L, 2);
  int * ret = LoadCodepoints(text, count);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_UnloadCodepoints(lua_State *L) {
  int * codepoints = (int *)lua_touserdata(L, 1);
  UnloadCodepoints(codepoints);
  return 0;
}
static int lua_GetCodepointCount(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int ret = GetCodepointCount(text);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetCodepoint(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int * codepointSize = (int *)lua_touserdata(L, 2);
  int ret = GetCodepoint(text, codepointSize);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetCodepointNext(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int * codepointSize = (int *)lua_touserdata(L, 2);
  int ret = GetCodepointNext(text, codepointSize);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_GetCodepointPrevious(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int * codepointSize = (int *)lua_touserdata(L, 2);
  int ret = GetCodepointPrevious(text, codepointSize);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_CodepointToUTF8(lua_State *L) {
  int codepoint = luaL_checkinteger(L, 1);
  int * utf8Size = (int *)lua_touserdata(L, 2);
  const char * ret = CodepointToUTF8(codepoint, utf8Size);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextCopy(lua_State *L) {
  char * dst = (char *)luaL_checkstring(L, 1);
  const char * src = luaL_checkstring(L, 2);
  int ret = TextCopy(dst, src);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_TextIsEqual(lua_State *L) {
  const char * text1 = luaL_checkstring(L, 1);
  const char * text2 = luaL_checkstring(L, 2);
  bool ret = TextIsEqual(text1, text2);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_TextLength(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  unsigned int ret = TextLength(text);
  lua_pushinteger(L, ret);
  return 1;
}
// unknow param type ...
static int lua_TextFormat(lua_State *L) {return 0;}

static int lua_TextSubtext(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int position = luaL_checkinteger(L, 2);
  int length = luaL_checkinteger(L, 3);
  const char * ret = TextSubtext(text, position, length);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextReplace(lua_State *L) {
  char * text = (char *)luaL_checkstring(L, 1);
  const char * replace = luaL_checkstring(L, 2);
  const char * by = luaL_checkstring(L, 3);
  char * ret = TextReplace(text, replace, by);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextInsert(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  const char * insert = luaL_checkstring(L, 2);
  int position = luaL_checkinteger(L, 3);
  char * ret = TextInsert(text, insert, position);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextJoin(lua_State *L) {
  const char ** textList = (const char **)lua_touserdata(L, 1);
  int count = luaL_checkinteger(L, 2);
  const char * delimiter = luaL_checkstring(L, 3);
  const char * ret = TextJoin(textList, count, delimiter);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextSplit(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  char delimiter = (char)luaL_checkinteger(L, 2);
  int * count = (int *)lua_touserdata(L, 3);
  const char ** ret = TextSplit(text, delimiter, count);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_TextAppend(lua_State *L) {
  char * text = (char *)luaL_checkstring(L, 1);
  const char * append = luaL_checkstring(L, 2);
  int * position = (int *)lua_touserdata(L, 3);
  TextAppend(text, append, position);
  return 0;
}
static int lua_TextFindIndex(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  const char * find = luaL_checkstring(L, 2);
  int ret = TextFindIndex(text, find);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_TextToUpper(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  const char * ret = TextToUpper(text);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextToLower(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  const char * ret = TextToLower(text);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextToPascal(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  const char * ret = TextToPascal(text);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_TextToInteger(lua_State *L) {
  const char * text = luaL_checkstring(L, 1);
  int ret = TextToInteger(text);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_DrawLine3D(lua_State *L) {
  Vector3 startPos = lua_checkVector3(L, 1);
  Vector3 endPos = lua_checkVector3(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawLine3D(startPos, endPos, color);
  return 0;
}
static int lua_DrawPoint3D(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  Color color = lua_checkColor(L, 2);
  DrawPoint3D(position, color);
  return 0;
}
static int lua_DrawCircle3D(lua_State *L) {
  Vector3 center = lua_checkVector3(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Vector3 rotationAxis = lua_checkVector3(L, 3);
  float rotationAngle = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawCircle3D(center, radius, rotationAxis, rotationAngle, color);
  return 0;
}
static int lua_DrawTriangle3D(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 v3 = lua_checkVector3(L, 3);
  Color color = lua_checkColor(L, 4);
  DrawTriangle3D(v1, v2, v3, color);
  return 0;
}
static int lua_DrawTriangleStrip3D(lua_State *L) {
  Vector3 * points = (Vector3 *)lua_touserdata(L, 1);
  int pointCount = luaL_checkinteger(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawTriangleStrip3D(points, pointCount, color);
  return 0;
}
static int lua_DrawCube(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  float width = (float)luaL_checknumber(L, 2);
  float height = (float)luaL_checknumber(L, 3);
  float length = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawCube(position, width, height, length, color);
  return 0;
}
static int lua_DrawCubeV(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  Vector3 size = lua_checkVector3(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawCubeV(position, size, color);
  return 0;
}
static int lua_DrawCubeWires(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  float width = (float)luaL_checknumber(L, 2);
  float height = (float)luaL_checknumber(L, 3);
  float length = (float)luaL_checknumber(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawCubeWires(position, width, height, length, color);
  return 0;
}
static int lua_DrawCubeWiresV(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  Vector3 size = lua_checkVector3(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawCubeWiresV(position, size, color);
  return 0;
}
static int lua_DrawSphere(lua_State *L) {
  Vector3 centerPos = lua_checkVector3(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawSphere(centerPos, radius, color);
  return 0;
}
static int lua_DrawSphereEx(lua_State *L) {
  Vector3 centerPos = lua_checkVector3(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  int rings = luaL_checkinteger(L, 3);
  int slices = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawSphereEx(centerPos, radius, rings, slices, color);
  return 0;
}
static int lua_DrawSphereWires(lua_State *L) {
  Vector3 centerPos = lua_checkVector3(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  int rings = luaL_checkinteger(L, 3);
  int slices = luaL_checkinteger(L, 4);
  Color color = lua_checkColor(L, 5);
  DrawSphereWires(centerPos, radius, rings, slices, color);
  return 0;
}
static int lua_DrawCylinder(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  float radiusTop = (float)luaL_checknumber(L, 2);
  float radiusBottom = (float)luaL_checknumber(L, 3);
  float height = (float)luaL_checknumber(L, 4);
  int slices = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCylinder(position, radiusTop, radiusBottom, height, slices, color);
  return 0;
}
static int lua_DrawCylinderEx(lua_State *L) {
  Vector3 startPos = lua_checkVector3(L, 1);
  Vector3 endPos = lua_checkVector3(L, 2);
  float startRadius = (float)luaL_checknumber(L, 3);
  float endRadius = (float)luaL_checknumber(L, 4);
  int sides = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCylinderEx(startPos, endPos, startRadius, endRadius, sides, color);
  return 0;
}
static int lua_DrawCylinderWires(lua_State *L) {
  Vector3 position = lua_checkVector3(L, 1);
  float radiusTop = (float)luaL_checknumber(L, 2);
  float radiusBottom = (float)luaL_checknumber(L, 3);
  float height = (float)luaL_checknumber(L, 4);
  int slices = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCylinderWires(position, radiusTop, radiusBottom, height, slices, color);
  return 0;
}
static int lua_DrawCylinderWiresEx(lua_State *L) {
  Vector3 startPos = lua_checkVector3(L, 1);
  Vector3 endPos = lua_checkVector3(L, 2);
  float startRadius = (float)luaL_checknumber(L, 3);
  float endRadius = (float)luaL_checknumber(L, 4);
  int sides = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCylinderWiresEx(startPos, endPos, startRadius, endRadius, sides, color);
  return 0;
}
static int lua_DrawCapsule(lua_State *L) {
  Vector3 startPos = lua_checkVector3(L, 1);
  Vector3 endPos = lua_checkVector3(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  int slices = luaL_checkinteger(L, 4);
  int rings = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCapsule(startPos, endPos, radius, slices, rings, color);
  return 0;
}
static int lua_DrawCapsuleWires(lua_State *L) {
  Vector3 startPos = lua_checkVector3(L, 1);
  Vector3 endPos = lua_checkVector3(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  int slices = luaL_checkinteger(L, 4);
  int rings = luaL_checkinteger(L, 5);
  Color color = lua_checkColor(L, 6);
  DrawCapsuleWires(startPos, endPos, radius, slices, rings, color);
  return 0;
}
static int lua_DrawPlane(lua_State *L) {
  Vector3 centerPos = lua_checkVector3(L, 1);
  Vector2 size = lua_checkVector2(L, 2);
  Color color = lua_checkColor(L, 3);
  DrawPlane(centerPos, size, color);
  return 0;
}
static int lua_DrawRay(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  Color color = lua_checkColor(L, 2);
  DrawRay(ray, color);
  return 0;
}
static int lua_DrawGrid(lua_State *L) {
  int slices = luaL_checkinteger(L, 1);
  float spacing = (float)luaL_checknumber(L, 2);
  DrawGrid(slices, spacing);
  return 0;
}
static int lua_LoadModel(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Model ret = LoadModel(fileName);
  lua_pushModel(L, &ret);
  return 1;
}
static int lua_LoadModelFromMesh(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  Model ret = LoadModelFromMesh(mesh);
  lua_pushModel(L, &ret);
  return 1;
}
static int lua_IsModelReady(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  bool ret = IsModelReady(model);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadModel(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  UnloadModel(model);
  return 0;
}
static int lua_GetModelBoundingBox(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  BoundingBox ret = GetModelBoundingBox(model);
  lua_pushBoundingBox(L, &ret);
  return 1;
}
static int lua_DrawModel(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  Vector3 position = lua_checkVector3(L, 2);
  float scale = (float)luaL_checknumber(L, 3);
  Color tint = lua_checkColor(L, 4);
  DrawModel(model, position, scale, tint);
  return 0;
}
static int lua_DrawModelEx(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  Vector3 position = lua_checkVector3(L, 2);
  Vector3 rotationAxis = lua_checkVector3(L, 3);
  float rotationAngle = (float)luaL_checknumber(L, 4);
  Vector3 scale = lua_checkVector3(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawModelEx(model, position, rotationAxis, rotationAngle, scale, tint);
  return 0;
}
static int lua_DrawModelWires(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  Vector3 position = lua_checkVector3(L, 2);
  float scale = (float)luaL_checknumber(L, 3);
  Color tint = lua_checkColor(L, 4);
  DrawModelWires(model, position, scale, tint);
  return 0;
}
static int lua_DrawModelWiresEx(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  Vector3 position = lua_checkVector3(L, 2);
  Vector3 rotationAxis = lua_checkVector3(L, 3);
  float rotationAngle = (float)luaL_checknumber(L, 4);
  Vector3 scale = lua_checkVector3(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawModelWiresEx(model, position, rotationAxis, rotationAngle, scale, tint);
  return 0;
}
static int lua_DrawBoundingBox(lua_State *L) {
  BoundingBox box = lua_checkBoundingBox(L, 1);
  Color color = lua_checkColor(L, 2);
  DrawBoundingBox(box, color);
  return 0;
}
static int lua_DrawBillboard(lua_State *L) {
  Camera camera = lua_checkCamera3D(L, 1);
  Texture2D texture = lua_checkTexture(L, 2);
  Vector3 position = lua_checkVector3(L, 3);
  float size = (float)luaL_checknumber(L, 4);
  Color tint = lua_checkColor(L, 5);
  DrawBillboard(camera, texture, position, size, tint);
  return 0;
}
static int lua_DrawBillboardRec(lua_State *L) {
  Camera camera = lua_checkCamera3D(L, 1);
  Texture2D texture = lua_checkTexture(L, 2);
  Rectangle source = lua_checkRectangle(L, 3);
  Vector3 position = lua_checkVector3(L, 4);
  Vector2 size = lua_checkVector2(L, 5);
  Color tint = lua_checkColor(L, 6);
  DrawBillboardRec(camera, texture, source, position, size, tint);
  return 0;
}
static int lua_DrawBillboardPro(lua_State *L) {
  Camera camera = lua_checkCamera3D(L, 1);
  Texture2D texture = lua_checkTexture(L, 2);
  Rectangle source = lua_checkRectangle(L, 3);
  Vector3 position = lua_checkVector3(L, 4);
  Vector3 up = lua_checkVector3(L, 5);
  Vector2 size = lua_checkVector2(L, 6);
  Vector2 origin = lua_checkVector2(L, 7);
  float rotation = (float)luaL_checknumber(L, 8);
  Color tint = lua_checkColor(L, 9);
  DrawBillboardPro(camera, texture, source, position, up, size, origin, rotation, tint);
  return 0;
}
static int lua_UploadMesh(lua_State *L) {
  Mesh * mesh = (Mesh *)lua_touserdata(L, 1);
  bool dynamic = lua_toboolean(L, 2);
  UploadMesh(mesh, dynamic);
  return 0;
}
static int lua_UpdateMeshBuffer(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  int index = luaL_checkinteger(L, 2);
  const void * data = lua_touserdata(L, 3);
  int dataSize = luaL_checkinteger(L, 4);
  int offset = luaL_checkinteger(L, 5);
  UpdateMeshBuffer(mesh, index, data, dataSize, offset);
  return 0;
}
static int lua_UnloadMesh(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  UnloadMesh(mesh);
  return 0;
}
static int lua_DrawMesh(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  Material material = lua_checkMaterial(L, 2);
  Matrix transform = lua_checkMatrix(L, 3);
  DrawMesh(mesh, material, transform);
  return 0;
}
static int lua_DrawMeshInstanced(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  Material material = lua_checkMaterial(L, 2);
  const Matrix * transforms = (const Matrix *)lua_touserdata(L, 3);
  int instances = luaL_checkinteger(L, 4);
  DrawMeshInstanced(mesh, material, transforms, instances);
  return 0;
}
static int lua_ExportMesh(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportMesh(mesh, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetMeshBoundingBox(lua_State *L) {
  Mesh mesh = lua_checkMesh(L, 1);
  BoundingBox ret = GetMeshBoundingBox(mesh);
  lua_pushBoundingBox(L, &ret);
  return 1;
}
static int lua_GenMeshTangents(lua_State *L) {
  Mesh * mesh = (Mesh *)lua_touserdata(L, 1);
  GenMeshTangents(mesh);
  return 0;
}
static int lua_GenMeshPoly(lua_State *L) {
  int sides = luaL_checkinteger(L, 1);
  float radius = (float)luaL_checknumber(L, 2);
  Mesh ret = GenMeshPoly(sides, radius);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshPlane(lua_State *L) {
  float width = (float)luaL_checknumber(L, 1);
  float length = (float)luaL_checknumber(L, 2);
  int resX = luaL_checkinteger(L, 3);
  int resZ = luaL_checkinteger(L, 4);
  Mesh ret = GenMeshPlane(width, length, resX, resZ);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshCube(lua_State *L) {
  float width = (float)luaL_checknumber(L, 1);
  float height = (float)luaL_checknumber(L, 2);
  float length = (float)luaL_checknumber(L, 3);
  Mesh ret = GenMeshCube(width, height, length);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshSphere(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  int rings = luaL_checkinteger(L, 2);
  int slices = luaL_checkinteger(L, 3);
  Mesh ret = GenMeshSphere(radius, rings, slices);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshHemiSphere(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  int rings = luaL_checkinteger(L, 2);
  int slices = luaL_checkinteger(L, 3);
  Mesh ret = GenMeshHemiSphere(radius, rings, slices);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshCylinder(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  float height = (float)luaL_checknumber(L, 2);
  int slices = luaL_checkinteger(L, 3);
  Mesh ret = GenMeshCylinder(radius, height, slices);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshCone(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  float height = (float)luaL_checknumber(L, 2);
  int slices = luaL_checkinteger(L, 3);
  Mesh ret = GenMeshCone(radius, height, slices);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshTorus(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  float size = (float)luaL_checknumber(L, 2);
  int radSeg = luaL_checkinteger(L, 3);
  int sides = luaL_checkinteger(L, 4);
  Mesh ret = GenMeshTorus(radius, size, radSeg, sides);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshKnot(lua_State *L) {
  float radius = (float)luaL_checknumber(L, 1);
  float size = (float)luaL_checknumber(L, 2);
  int radSeg = luaL_checkinteger(L, 3);
  int sides = luaL_checkinteger(L, 4);
  Mesh ret = GenMeshKnot(radius, size, radSeg, sides);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshHeightmap(lua_State *L) {
  Image heightmap = lua_checkImage(L, 1);
  Vector3 size = lua_checkVector3(L, 2);
  Mesh ret = GenMeshHeightmap(heightmap, size);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_GenMeshCubicmap(lua_State *L) {
  Image cubicmap = lua_checkImage(L, 1);
  Vector3 cubeSize = lua_checkVector3(L, 2);
  Mesh ret = GenMeshCubicmap(cubicmap, cubeSize);
  lua_pushMesh(L, &ret);
  return 1;
}
static int lua_LoadMaterials(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int * materialCount = (int *)lua_touserdata(L, 2);
  Material * ret = LoadMaterials(fileName, materialCount);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_LoadMaterialDefault(lua_State *L) {
  Material ret = LoadMaterialDefault();
  lua_pushMaterial(L, &ret);
  return 1;
}
static int lua_IsMaterialReady(lua_State *L) {
  Material material = lua_checkMaterial(L, 1);
  bool ret = IsMaterialReady(material);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadMaterial(lua_State *L) {
  Material material = lua_checkMaterial(L, 1);
  UnloadMaterial(material);
  return 0;
}
static int lua_SetMaterialTexture(lua_State *L) {
  Material * material = (Material *)lua_touserdata(L, 1);
  int mapType = luaL_checkinteger(L, 2);
  Texture2D texture = lua_checkTexture(L, 3);
  SetMaterialTexture(material, mapType, texture);
  return 0;
}
static int lua_SetModelMeshMaterial(lua_State *L) {
  Model * model = (Model *)lua_touserdata(L, 1);
  int meshId = luaL_checkinteger(L, 2);
  int materialId = luaL_checkinteger(L, 3);
  SetModelMeshMaterial(model, meshId, materialId);
  return 0;
}
static int lua_LoadModelAnimations(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  int * animCount = (int *)lua_touserdata(L, 2);
  ModelAnimation * ret = LoadModelAnimations(fileName, animCount);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_UpdateModelAnimation(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  ModelAnimation anim = lua_checkModelAnimation(L, 2);
  int frame = luaL_checkinteger(L, 3);
  UpdateModelAnimation(model, anim, frame);
  return 0;
}
static int lua_UnloadModelAnimation(lua_State *L) {
  ModelAnimation anim = lua_checkModelAnimation(L, 1);
  UnloadModelAnimation(anim);
  return 0;
}
static int lua_UnloadModelAnimations(lua_State *L) {
  ModelAnimation * animations = (ModelAnimation *)lua_touserdata(L, 1);
  int animCount = luaL_checkinteger(L, 2);
  UnloadModelAnimations(animations, animCount);
  return 0;
}
static int lua_IsModelAnimationValid(lua_State *L) {
  Model model = lua_checkModel(L, 1);
  ModelAnimation anim = lua_checkModelAnimation(L, 2);
  bool ret = IsModelAnimationValid(model, anim);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionSpheres(lua_State *L) {
  Vector3 center1 = lua_checkVector3(L, 1);
  float radius1 = (float)luaL_checknumber(L, 2);
  Vector3 center2 = lua_checkVector3(L, 3);
  float radius2 = (float)luaL_checknumber(L, 4);
  bool ret = CheckCollisionSpheres(center1, radius1, center2, radius2);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionBoxes(lua_State *L) {
  BoundingBox box1 = lua_checkBoundingBox(L, 1);
  BoundingBox box2 = lua_checkBoundingBox(L, 2);
  bool ret = CheckCollisionBoxes(box1, box2);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_CheckCollisionBoxSphere(lua_State *L) {
  BoundingBox box = lua_checkBoundingBox(L, 1);
  Vector3 center = lua_checkVector3(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  bool ret = CheckCollisionBoxSphere(box, center, radius);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_GetRayCollisionSphere(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  Vector3 center = lua_checkVector3(L, 2);
  float radius = (float)luaL_checknumber(L, 3);
  RayCollision ret = GetRayCollisionSphere(ray, center, radius);
  lua_pushRayCollision(L, &ret);
  return 1;
}
static int lua_GetRayCollisionBox(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  BoundingBox box = lua_checkBoundingBox(L, 2);
  RayCollision ret = GetRayCollisionBox(ray, box);
  lua_pushRayCollision(L, &ret);
  return 1;
}
static int lua_GetRayCollisionMesh(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  Mesh mesh = lua_checkMesh(L, 2);
  Matrix transform = lua_checkMatrix(L, 3);
  RayCollision ret = GetRayCollisionMesh(ray, mesh, transform);
  lua_pushRayCollision(L, &ret);
  return 1;
}
static int lua_GetRayCollisionTriangle(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  Vector3 p1 = lua_checkVector3(L, 2);
  Vector3 p2 = lua_checkVector3(L, 3);
  Vector3 p3 = lua_checkVector3(L, 4);
  RayCollision ret = GetRayCollisionTriangle(ray, p1, p2, p3);
  lua_pushRayCollision(L, &ret);
  return 1;
}
static int lua_GetRayCollisionQuad(lua_State *L) {
  Ray ray = lua_checkRay(L, 1);
  Vector3 p1 = lua_checkVector3(L, 2);
  Vector3 p2 = lua_checkVector3(L, 3);
  Vector3 p3 = lua_checkVector3(L, 4);
  Vector3 p4 = lua_checkVector3(L, 5);
  RayCollision ret = GetRayCollisionQuad(ray, p1, p2, p3, p4);
  lua_pushRayCollision(L, &ret);
  return 1;
}
static int lua_InitAudioDevice(lua_State *L) {
  InitAudioDevice();
  return 0;
}
static int lua_CloseAudioDevice(lua_State *L) {
  CloseAudioDevice();
  return 0;
}
static int lua_IsAudioDeviceReady(lua_State *L) {
  bool ret = IsAudioDeviceReady();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_SetMasterVolume(lua_State *L) {
  float volume = (float)luaL_checknumber(L, 1);
  SetMasterVolume(volume);
  return 0;
}
static int lua_GetMasterVolume(lua_State *L) {
  float ret = GetMasterVolume();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_LoadWave(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Wave ret = LoadWave(fileName);
  lua_pushWave(L, &ret);
  return 1;
}
static int lua_LoadWaveFromMemory(lua_State *L) {
  const char * fileType = luaL_checkstring(L, 1);
  const unsigned char * fileData = (const unsigned char *)luaL_checkstring(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  Wave ret = LoadWaveFromMemory(fileType, fileData, dataSize);
  lua_pushWave(L, &ret);
  return 1;
}
static int lua_IsWaveReady(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  bool ret = IsWaveReady(wave);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_LoadSound(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Sound ret = LoadSound(fileName);
  lua_pushSound(L, &ret);
  return 1;
}
static int lua_LoadSoundFromWave(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  Sound ret = LoadSoundFromWave(wave);
  lua_pushSound(L, &ret);
  return 1;
}
static int lua_LoadSoundAlias(lua_State *L) {
  Sound source = lua_checkSound(L, 1);
  Sound ret = LoadSoundAlias(source);
  lua_pushSound(L, &ret);
  return 1;
}
static int lua_IsSoundReady(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  bool ret = IsSoundReady(sound);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UpdateSound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  const void * data = lua_touserdata(L, 2);
  int sampleCount = luaL_checkinteger(L, 3);
  UpdateSound(sound, data, sampleCount);
  return 0;
}
static int lua_UnloadWave(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  UnloadWave(wave);
  return 0;
}
static int lua_UnloadSound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  UnloadSound(sound);
  return 0;
}
static int lua_UnloadSoundAlias(lua_State *L) {
  Sound alias = lua_checkSound(L, 1);
  UnloadSoundAlias(alias);
  return 0;
}
static int lua_ExportWave(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportWave(wave, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_ExportWaveAsCode(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  const char * fileName = luaL_checkstring(L, 2);
  bool ret = ExportWaveAsCode(wave, fileName);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_PlaySound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  PlaySound(sound);
  return 0;
}
static int lua_StopSound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  StopSound(sound);
  return 0;
}
static int lua_PauseSound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  PauseSound(sound);
  return 0;
}
static int lua_ResumeSound(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  ResumeSound(sound);
  return 0;
}
static int lua_IsSoundPlaying(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  bool ret = IsSoundPlaying(sound);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_SetSoundVolume(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  float volume = (float)luaL_checknumber(L, 2);
  SetSoundVolume(sound, volume);
  return 0;
}
static int lua_SetSoundPitch(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  float pitch = (float)luaL_checknumber(L, 2);
  SetSoundPitch(sound, pitch);
  return 0;
}
static int lua_SetSoundPan(lua_State *L) {
  Sound sound = lua_checkSound(L, 1);
  float pan = (float)luaL_checknumber(L, 2);
  SetSoundPan(sound, pan);
  return 0;
}
static int lua_WaveCopy(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  Wave ret = WaveCopy(wave);
  lua_pushWave(L, &ret);
  return 1;
}
static int lua_WaveCrop(lua_State *L) {
  Wave * wave = (Wave *)lua_touserdata(L, 1);
  int initSample = luaL_checkinteger(L, 2);
  int finalSample = luaL_checkinteger(L, 3);
  WaveCrop(wave, initSample, finalSample);
  return 0;
}
static int lua_WaveFormat(lua_State *L) {
  Wave * wave = (Wave *)lua_touserdata(L, 1);
  int sampleRate = luaL_checkinteger(L, 2);
  int sampleSize = luaL_checkinteger(L, 3);
  int channels = luaL_checkinteger(L, 4);
  WaveFormat(wave, sampleRate, sampleSize, channels);
  return 0;
}
static int lua_LoadWaveSamples(lua_State *L) {
  Wave wave = lua_checkWave(L, 1);
  float * ret = LoadWaveSamples(wave);
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_UnloadWaveSamples(lua_State *L) {
  float * samples = (float *)lua_touserdata(L, 1);
  UnloadWaveSamples(samples);
  return 0;
}
static int lua_LoadMusicStream(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  Music ret = LoadMusicStream(fileName);
  lua_pushMusic(L, &ret);
  return 1;
}
static int lua_LoadMusicStreamFromMemory(lua_State *L) {
  const char * fileType = luaL_checkstring(L, 1);
  const unsigned char * data = (const unsigned char *)luaL_checkstring(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  Music ret = LoadMusicStreamFromMemory(fileType, data, dataSize);
  lua_pushMusic(L, &ret);
  return 1;
}
static int lua_IsMusicReady(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  bool ret = IsMusicReady(music);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  UnloadMusicStream(music);
  return 0;
}
static int lua_PlayMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  PlayMusicStream(music);
  return 0;
}
static int lua_IsMusicStreamPlaying(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  bool ret = IsMusicStreamPlaying(music);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UpdateMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  UpdateMusicStream(music);
  return 0;
}
static int lua_StopMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  StopMusicStream(music);
  return 0;
}
static int lua_PauseMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  PauseMusicStream(music);
  return 0;
}
static int lua_ResumeMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  ResumeMusicStream(music);
  return 0;
}
static int lua_SeekMusicStream(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float position = (float)luaL_checknumber(L, 2);
  SeekMusicStream(music, position);
  return 0;
}
static int lua_SetMusicVolume(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float volume = (float)luaL_checknumber(L, 2);
  SetMusicVolume(music, volume);
  return 0;
}
static int lua_SetMusicPitch(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float pitch = (float)luaL_checknumber(L, 2);
  SetMusicPitch(music, pitch);
  return 0;
}
static int lua_SetMusicPan(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float pan = (float)luaL_checknumber(L, 2);
  SetMusicPan(music, pan);
  return 0;
}
static int lua_GetMusicTimeLength(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float ret = GetMusicTimeLength(music);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_GetMusicTimePlayed(lua_State *L) {
  Music music = lua_checkMusic(L, 1);
  float ret = GetMusicTimePlayed(music);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_LoadAudioStream(lua_State *L) {
  unsigned int sampleRate = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int sampleSize = (unsigned int)luaL_checkinteger(L, 2);
  unsigned int channels = (unsigned int)luaL_checkinteger(L, 3);
  AudioStream ret = LoadAudioStream(sampleRate, sampleSize, channels);
  lua_pushAudioStream(L, &ret);
  return 1;
}
static int lua_IsAudioStreamReady(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  bool ret = IsAudioStreamReady(stream);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_UnloadAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  UnloadAudioStream(stream);
  return 0;
}
static int lua_UpdateAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  const void * data = lua_touserdata(L, 2);
  int frameCount = luaL_checkinteger(L, 3);
  UpdateAudioStream(stream, data, frameCount);
  return 0;
}
static int lua_IsAudioStreamProcessed(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  bool ret = IsAudioStreamProcessed(stream);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_PlayAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  PlayAudioStream(stream);
  return 0;
}
static int lua_PauseAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  PauseAudioStream(stream);
  return 0;
}
static int lua_ResumeAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  ResumeAudioStream(stream);
  return 0;
}
static int lua_IsAudioStreamPlaying(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  bool ret = IsAudioStreamPlaying(stream);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_StopAudioStream(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  StopAudioStream(stream);
  return 0;
}
static int lua_SetAudioStreamVolume(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  float volume = (float)luaL_checknumber(L, 2);
  SetAudioStreamVolume(stream, volume);
  return 0;
}
static int lua_SetAudioStreamPitch(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  float pitch = (float)luaL_checknumber(L, 2);
  SetAudioStreamPitch(stream, pitch);
  return 0;
}
static int lua_SetAudioStreamPan(lua_State *L) {
  AudioStream stream = lua_checkAudioStream(L, 1);
  float pan = (float)luaL_checknumber(L, 2);
  SetAudioStreamPan(stream, pan);
  return 0;
}
static int lua_SetAudioStreamBufferSizeDefault(lua_State *L) {
  int size = luaL_checkinteger(L, 1);
  SetAudioStreamBufferSizeDefault(size);
  return 0;
}
// unknow param type AudioCallback
static int lua_SetAudioStreamCallback(lua_State *L) {return 0;}

// unknow param type AudioCallback
static int lua_AttachAudioStreamProcessor(lua_State *L) {return 0;}

// unknow param type AudioCallback
static int lua_DetachAudioStreamProcessor(lua_State *L) {return 0;}

// unknow param type AudioCallback
static int lua_AttachAudioMixedProcessor(lua_State *L) {return 0;}

// unknow param type AudioCallback
static int lua_DetachAudioMixedProcessor(lua_State *L) {return 0;}





// unknow field type float[3]
static void lua_pushfloat3(lua_State *L, float3* val) {}
static float3 lua_checkfloat3(lua_State* L, int index) {float3 ret; return ret;}

// unknow field type float[16]
static void lua_pushfloat16(lua_State *L, float16* val) {}
static float16 lua_checkfloat16(lua_State* L, int index) {float16 ret; return ret;}

static int lua_Clamp(lua_State *L) {
  float value = (float)luaL_checknumber(L, 1);
  float min = (float)luaL_checknumber(L, 2);
  float max = (float)luaL_checknumber(L, 3);
  float ret = Clamp(value, min, max);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Lerp(lua_State *L) {
  float start = (float)luaL_checknumber(L, 1);
  float end = (float)luaL_checknumber(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  float ret = Lerp(start, end, amount);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Normalize(lua_State *L) {
  float value = (float)luaL_checknumber(L, 1);
  float start = (float)luaL_checknumber(L, 2);
  float end = (float)luaL_checknumber(L, 3);
  float ret = Normalize(value, start, end);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Remap(lua_State *L) {
  float value = (float)luaL_checknumber(L, 1);
  float inputStart = (float)luaL_checknumber(L, 2);
  float inputEnd = (float)luaL_checknumber(L, 3);
  float outputStart = (float)luaL_checknumber(L, 4);
  float outputEnd = (float)luaL_checknumber(L, 5);
  float ret = Remap(value, inputStart, inputEnd, outputStart, outputEnd);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Wrap(lua_State *L) {
  float value = (float)luaL_checknumber(L, 1);
  float min = (float)luaL_checknumber(L, 2);
  float max = (float)luaL_checknumber(L, 3);
  float ret = Wrap(value, min, max);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_FloatEquals(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  int ret = FloatEquals(x, y);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_Vector2Zero(lua_State *L) {
  Vector2 ret = Vector2Zero();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2One(lua_State *L) {
  Vector2 ret = Vector2One();
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Add(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 ret = Vector2Add(v1, v2);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2AddValue(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float add = (float)luaL_checknumber(L, 2);
  Vector2 ret = Vector2AddValue(v, add);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Subtract(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 ret = Vector2Subtract(v1, v2);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2SubtractValue(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float sub = (float)luaL_checknumber(L, 2);
  Vector2 ret = Vector2SubtractValue(v, sub);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Length(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float ret = Vector2Length(v);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2LengthSqr(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float ret = Vector2LengthSqr(v);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2DotProduct(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  float ret = Vector2DotProduct(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2Distance(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  float ret = Vector2Distance(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2DistanceSqr(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  float ret = Vector2DistanceSqr(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2Angle(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  float ret = Vector2Angle(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2LineAngle(lua_State *L) {
  Vector2 start = lua_checkVector2(L, 1);
  Vector2 end = lua_checkVector2(L, 2);
  float ret = Vector2LineAngle(start, end);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector2Scale(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float scale = (float)luaL_checknumber(L, 2);
  Vector2 ret = Vector2Scale(v, scale);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Multiply(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 ret = Vector2Multiply(v1, v2);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Negate(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 ret = Vector2Negate(v);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Divide(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  Vector2 ret = Vector2Divide(v1, v2);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Normalize(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 ret = Vector2Normalize(v);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Transform(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Matrix mat = lua_checkMatrix(L, 2);
  Vector2 ret = Vector2Transform(v, mat);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Lerp(lua_State *L) {
  Vector2 v1 = lua_checkVector2(L, 1);
  Vector2 v2 = lua_checkVector2(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  Vector2 ret = Vector2Lerp(v1, v2, amount);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Reflect(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 normal = lua_checkVector2(L, 2);
  Vector2 ret = Vector2Reflect(v, normal);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Rotate(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float angle = (float)luaL_checknumber(L, 2);
  Vector2 ret = Vector2Rotate(v, angle);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2MoveTowards(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 target = lua_checkVector2(L, 2);
  float maxDistance = (float)luaL_checknumber(L, 3);
  Vector2 ret = Vector2MoveTowards(v, target, maxDistance);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Invert(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 ret = Vector2Invert(v);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Clamp(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  Vector2 min = lua_checkVector2(L, 2);
  Vector2 max = lua_checkVector2(L, 3);
  Vector2 ret = Vector2Clamp(v, min, max);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2ClampValue(lua_State *L) {
  Vector2 v = lua_checkVector2(L, 1);
  float min = (float)luaL_checknumber(L, 2);
  float max = (float)luaL_checknumber(L, 3);
  Vector2 ret = Vector2ClampValue(v, min, max);
  lua_pushVector2(L, &ret);
  return 1;
}
static int lua_Vector2Equals(lua_State *L) {
  Vector2 p = lua_checkVector2(L, 1);
  Vector2 q = lua_checkVector2(L, 2);
  int ret = Vector2Equals(p, q);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_Vector3Zero(lua_State *L) {
  Vector3 ret = Vector3Zero();
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3One(lua_State *L) {
  Vector3 ret = Vector3One();
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Add(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Add(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3AddValue(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  float add = (float)luaL_checknumber(L, 2);
  Vector3 ret = Vector3AddValue(v, add);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Subtract(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Subtract(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3SubtractValue(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  float sub = (float)luaL_checknumber(L, 2);
  Vector3 ret = Vector3SubtractValue(v, sub);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Scale(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  float scalar = (float)luaL_checknumber(L, 2);
  Vector3 ret = Vector3Scale(v, scalar);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Multiply(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Multiply(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3CrossProduct(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3CrossProduct(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Perpendicular(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 ret = Vector3Perpendicular(v);
  lua_pushVector3(L, &ret);
  return 1;
}
// unknow param type const Vector3
static int lua_Vector3Length(lua_State *L) {return 0;}

// unknow param type const Vector3
static int lua_Vector3LengthSqr(lua_State *L) {return 0;}

static int lua_Vector3DotProduct(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  float ret = Vector3DotProduct(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector3Distance(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  float ret = Vector3Distance(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector3DistanceSqr(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  float ret = Vector3DistanceSqr(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector3Angle(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  float ret = Vector3Angle(v1, v2);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_Vector3Negate(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 ret = Vector3Negate(v);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Divide(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Divide(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Normalize(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 ret = Vector3Normalize(v);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Project(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Project(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Reject(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Reject(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3OrthoNormalize(lua_State *L) {
  Vector3 * v1 = (Vector3 *)lua_touserdata(L, 1);
  Vector3 * v2 = (Vector3 *)lua_touserdata(L, 2);
  Vector3OrthoNormalize(v1, v2);
  return 0;
}
static int lua_Vector3Transform(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Matrix mat = lua_checkMatrix(L, 2);
  Vector3 ret = Vector3Transform(v, mat);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3RotateByQuaternion(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Quaternion q = lua_checkVector4(L, 2);
  Vector3 ret = Vector3RotateByQuaternion(v, q);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3RotateByAxisAngle(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 axis = lua_checkVector3(L, 2);
  float angle = (float)luaL_checknumber(L, 3);
  Vector3 ret = Vector3RotateByAxisAngle(v, axis, angle);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Lerp(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  Vector3 ret = Vector3Lerp(v1, v2, amount);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Reflect(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 normal = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Reflect(v, normal);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Min(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Min(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Max(lua_State *L) {
  Vector3 v1 = lua_checkVector3(L, 1);
  Vector3 v2 = lua_checkVector3(L, 2);
  Vector3 ret = Vector3Max(v1, v2);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Barycenter(lua_State *L) {
  Vector3 p = lua_checkVector3(L, 1);
  Vector3 a = lua_checkVector3(L, 2);
  Vector3 b = lua_checkVector3(L, 3);
  Vector3 c = lua_checkVector3(L, 4);
  Vector3 ret = Vector3Barycenter(p, a, b, c);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Unproject(lua_State *L) {
  Vector3 source = lua_checkVector3(L, 1);
  Matrix projection = lua_checkMatrix(L, 2);
  Matrix view = lua_checkMatrix(L, 3);
  Vector3 ret = Vector3Unproject(source, projection, view);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3ToFloatV(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  float3 ret = Vector3ToFloatV(v);
  lua_pushfloat3(L, &ret);
  return 1;
}
static int lua_Vector3Invert(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 ret = Vector3Invert(v);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Clamp(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 min = lua_checkVector3(L, 2);
  Vector3 max = lua_checkVector3(L, 3);
  Vector3 ret = Vector3Clamp(v, min, max);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3ClampValue(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  float min = (float)luaL_checknumber(L, 2);
  float max = (float)luaL_checknumber(L, 3);
  Vector3 ret = Vector3ClampValue(v, min, max);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_Vector3Equals(lua_State *L) {
  Vector3 p = lua_checkVector3(L, 1);
  Vector3 q = lua_checkVector3(L, 2);
  int ret = Vector3Equals(p, q);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_Vector3Refract(lua_State *L) {
  Vector3 v = lua_checkVector3(L, 1);
  Vector3 n = lua_checkVector3(L, 2);
  float r = (float)luaL_checknumber(L, 3);
  Vector3 ret = Vector3Refract(v, n, r);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_MatrixDeterminant(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  float ret = MatrixDeterminant(mat);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_MatrixTrace(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  float ret = MatrixTrace(mat);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_MatrixTranspose(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  Matrix ret = MatrixTranspose(mat);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixInvert(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  Matrix ret = MatrixInvert(mat);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixIdentity(lua_State *L) {
  Matrix ret = MatrixIdentity();
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixAdd(lua_State *L) {
  Matrix left = lua_checkMatrix(L, 1);
  Matrix right = lua_checkMatrix(L, 2);
  Matrix ret = MatrixAdd(left, right);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixSubtract(lua_State *L) {
  Matrix left = lua_checkMatrix(L, 1);
  Matrix right = lua_checkMatrix(L, 2);
  Matrix ret = MatrixSubtract(left, right);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixMultiply(lua_State *L) {
  Matrix left = lua_checkMatrix(L, 1);
  Matrix right = lua_checkMatrix(L, 2);
  Matrix ret = MatrixMultiply(left, right);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixTranslate(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  Matrix ret = MatrixTranslate(x, y, z);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotate(lua_State *L) {
  Vector3 axis = lua_checkVector3(L, 1);
  float angle = (float)luaL_checknumber(L, 2);
  Matrix ret = MatrixRotate(axis, angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotateX(lua_State *L) {
  float angle = (float)luaL_checknumber(L, 1);
  Matrix ret = MatrixRotateX(angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotateY(lua_State *L) {
  float angle = (float)luaL_checknumber(L, 1);
  Matrix ret = MatrixRotateY(angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotateZ(lua_State *L) {
  float angle = (float)luaL_checknumber(L, 1);
  Matrix ret = MatrixRotateZ(angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotateXYZ(lua_State *L) {
  Vector3 angle = lua_checkVector3(L, 1);
  Matrix ret = MatrixRotateXYZ(angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixRotateZYX(lua_State *L) {
  Vector3 angle = lua_checkVector3(L, 1);
  Matrix ret = MatrixRotateZYX(angle);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixScale(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  Matrix ret = MatrixScale(x, y, z);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixFrustum(lua_State *L) {
  double left = (double)luaL_checknumber(L, 1);
  double right = (double)luaL_checknumber(L, 2);
  double bottom = (double)luaL_checknumber(L, 3);
  double top = (double)luaL_checknumber(L, 4);
  double near = (double)luaL_checknumber(L, 5);
  double far = (double)luaL_checknumber(L, 6);
  Matrix ret = MatrixFrustum(left, right, bottom, top, near, far);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixPerspective(lua_State *L) {
  double fovY = (double)luaL_checknumber(L, 1);
  double aspect = (double)luaL_checknumber(L, 2);
  double nearPlane = (double)luaL_checknumber(L, 3);
  double farPlane = (double)luaL_checknumber(L, 4);
  Matrix ret = MatrixPerspective(fovY, aspect, nearPlane, farPlane);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixOrtho(lua_State *L) {
  double left = (double)luaL_checknumber(L, 1);
  double right = (double)luaL_checknumber(L, 2);
  double bottom = (double)luaL_checknumber(L, 3);
  double top = (double)luaL_checknumber(L, 4);
  double nearPlane = (double)luaL_checknumber(L, 5);
  double farPlane = (double)luaL_checknumber(L, 6);
  Matrix ret = MatrixOrtho(left, right, bottom, top, nearPlane, farPlane);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixLookAt(lua_State *L) {
  Vector3 eye = lua_checkVector3(L, 1);
  Vector3 target = lua_checkVector3(L, 2);
  Vector3 up = lua_checkVector3(L, 3);
  Matrix ret = MatrixLookAt(eye, target, up);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_MatrixToFloatV(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  float16 ret = MatrixToFloatV(mat);
  lua_pushfloat16(L, &ret);
  return 1;
}
static int lua_QuaternionAdd(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  Quaternion ret = QuaternionAdd(q1, q2);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionAddValue(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  float add = (float)luaL_checknumber(L, 2);
  Quaternion ret = QuaternionAddValue(q, add);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionSubtract(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  Quaternion ret = QuaternionSubtract(q1, q2);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionSubtractValue(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  float sub = (float)luaL_checknumber(L, 2);
  Quaternion ret = QuaternionSubtractValue(q, sub);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionIdentity(lua_State *L) {
  Quaternion ret = QuaternionIdentity();
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionLength(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  float ret = QuaternionLength(q);
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_QuaternionNormalize(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Quaternion ret = QuaternionNormalize(q);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionInvert(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Quaternion ret = QuaternionInvert(q);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionMultiply(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  Quaternion ret = QuaternionMultiply(q1, q2);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionScale(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  float mul = (float)luaL_checknumber(L, 2);
  Quaternion ret = QuaternionScale(q, mul);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionDivide(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  Quaternion ret = QuaternionDivide(q1, q2);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionLerp(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  Quaternion ret = QuaternionLerp(q1, q2, amount);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionNlerp(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  Quaternion ret = QuaternionNlerp(q1, q2, amount);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionSlerp(lua_State *L) {
  Quaternion q1 = lua_checkVector4(L, 1);
  Quaternion q2 = lua_checkVector4(L, 2);
  float amount = (float)luaL_checknumber(L, 3);
  Quaternion ret = QuaternionSlerp(q1, q2, amount);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionFromVector3ToVector3(lua_State *L) {
  Vector3 from = lua_checkVector3(L, 1);
  Vector3 to = lua_checkVector3(L, 2);
  Quaternion ret = QuaternionFromVector3ToVector3(from, to);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionFromMatrix(lua_State *L) {
  Matrix mat = lua_checkMatrix(L, 1);
  Quaternion ret = QuaternionFromMatrix(mat);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionToMatrix(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Matrix ret = QuaternionToMatrix(q);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_QuaternionFromAxisAngle(lua_State *L) {
  Vector3 axis = lua_checkVector3(L, 1);
  float angle = (float)luaL_checknumber(L, 2);
  Quaternion ret = QuaternionFromAxisAngle(axis, angle);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionToAxisAngle(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Vector3 * outAxis = (Vector3 *)lua_touserdata(L, 2);
  float * outAngle = (float *)lua_touserdata(L, 3);
  QuaternionToAxisAngle(q, outAxis, outAngle);
  return 0;
}
static int lua_QuaternionFromEuler(lua_State *L) {
  float pitch = (float)luaL_checknumber(L, 1);
  float yaw = (float)luaL_checknumber(L, 2);
  float roll = (float)luaL_checknumber(L, 3);
  Quaternion ret = QuaternionFromEuler(pitch, yaw, roll);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionToEuler(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Vector3 ret = QuaternionToEuler(q);
  lua_pushVector3(L, &ret);
  return 1;
}
static int lua_QuaternionTransform(lua_State *L) {
  Quaternion q = lua_checkVector4(L, 1);
  Matrix mat = lua_checkMatrix(L, 2);
  Quaternion ret = QuaternionTransform(q, mat);
  lua_pushVector4(L, &ret);
  return 1;
}
static int lua_QuaternionEquals(lua_State *L) {
  Quaternion p = lua_checkVector4(L, 1);
  Quaternion q = lua_checkVector4(L, 2);
  int ret = QuaternionEquals(p, q);
  lua_pushinteger(L, ret);
  return 1;
}

// unknow field type #if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENunsigned int *
// unknow field type #endif
// unknow field type #if defined(GRAPHICS_API_OPENGL_ES2)
// unknow field type #endif
// unknow field type unsigned int[4]
static void lua_pushrlVertexBuffer(lua_State *L, rlVertexBuffer* val) {}
static rlVertexBuffer lua_checkrlVertexBuffer(lua_State* L, int index) {rlVertexBuffer ret; return ret;}

static void lua_pushrlDrawCall(lua_State *L, rlDrawCall* val) {
  lua_newtable(L);
  lua_pushstring(L, "mode");
  lua_pushinteger(L, val->mode);
  lua_settable(L, -3);
  lua_pushstring(L, "vertexCount");
  lua_pushinteger(L, val->vertexCount);
  lua_settable(L, -3);
  lua_pushstring(L, "vertexAlignment");
  lua_pushinteger(L, val->vertexAlignment);
  lua_settable(L, -3);
  lua_pushstring(L, "textureId");
  lua_pushinteger(L, val->textureId);
  lua_settable(L, -3);
}
static rlDrawCall lua_checkrlDrawCall(lua_State* L, int index) {
  rlDrawCall ret;
  lua_pushstring(L, "mode");
  lua_gettable(L, index);
  ret.mode = luaL_checkinteger(L, -1);
  lua_pushstring(L, "vertexCount");
  lua_gettable(L, index);
  ret.vertexCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "vertexAlignment");
  lua_gettable(L, index);
  ret.vertexAlignment = luaL_checkinteger(L, -1);
  lua_pushstring(L, "textureId");
  lua_gettable(L, index);
  ret.textureId = (unsigned int)luaL_checkinteger(L, -1);
  return ret;
}

static void lua_pushrlRenderBatch(lua_State *L, rlRenderBatch* val) {
  lua_newtable(L);
  lua_pushstring(L, "bufferCount");
  lua_pushinteger(L, val->bufferCount);
  lua_settable(L, -3);
  lua_pushstring(L, "currentBuffer");
  lua_pushinteger(L, val->currentBuffer);
  lua_settable(L, -3);
  lua_pushstring(L, "vertexBuffer");
  lua_pushlightuserdata(L, (void *)val->vertexBuffer);
  lua_settable(L, -3);
  lua_pushstring(L, "draws");
  lua_pushlightuserdata(L, (void *)val->draws);
  lua_settable(L, -3);
  lua_pushstring(L, "drawCounter");
  lua_pushinteger(L, val->drawCounter);
  lua_settable(L, -3);
  lua_pushstring(L, "currentDepth");
  lua_pushnumber(L, val->currentDepth);
  lua_settable(L, -3);
}
static rlRenderBatch lua_checkrlRenderBatch(lua_State* L, int index) {
  rlRenderBatch ret;
  lua_pushstring(L, "bufferCount");
  lua_gettable(L, index);
  ret.bufferCount = luaL_checkinteger(L, -1);
  lua_pushstring(L, "currentBuffer");
  lua_gettable(L, index);
  ret.currentBuffer = luaL_checkinteger(L, -1);
  lua_pushstring(L, "vertexBuffer");
  lua_gettable(L, index);
  ret.vertexBuffer = (rlVertexBuffer *)lua_touserdata(L, -1);
  lua_pushstring(L, "draws");
  lua_gettable(L, index);
  ret.draws = (rlDrawCall *)lua_touserdata(L, -1);
  lua_pushstring(L, "drawCounter");
  lua_gettable(L, index);
  ret.drawCounter = luaL_checkinteger(L, -1);
  lua_pushstring(L, "currentDepth");
  lua_gettable(L, index);
  ret.currentDepth = (float)luaL_checknumber(L, -1);
  return ret;
}

static int lua_rlMatrixMode(lua_State *L) {
  int mode = luaL_checkinteger(L, 1);
  rlMatrixMode(mode);
  return 0;
}
static int lua_rlPushMatrix(lua_State *L) {
  rlPushMatrix();
  return 0;
}
static int lua_rlPopMatrix(lua_State *L) {
  rlPopMatrix();
  return 0;
}
static int lua_rlLoadIdentity(lua_State *L) {
  rlLoadIdentity();
  return 0;
}
static int lua_rlTranslatef(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  rlTranslatef(x, y, z);
  return 0;
}
static int lua_rlRotatef(lua_State *L) {
  float angle = (float)luaL_checknumber(L, 1);
  float x = (float)luaL_checknumber(L, 2);
  float y = (float)luaL_checknumber(L, 3);
  float z = (float)luaL_checknumber(L, 4);
  rlRotatef(angle, x, y, z);
  return 0;
}
static int lua_rlScalef(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  rlScalef(x, y, z);
  return 0;
}
// unknow param type const float *
static int lua_rlMultMatrixf(lua_State *L) {return 0;}

static int lua_rlFrustum(lua_State *L) {
  double left = (double)luaL_checknumber(L, 1);
  double right = (double)luaL_checknumber(L, 2);
  double bottom = (double)luaL_checknumber(L, 3);
  double top = (double)luaL_checknumber(L, 4);
  double znear = (double)luaL_checknumber(L, 5);
  double zfar = (double)luaL_checknumber(L, 6);
  rlFrustum(left, right, bottom, top, znear, zfar);
  return 0;
}
static int lua_rlOrtho(lua_State *L) {
  double left = (double)luaL_checknumber(L, 1);
  double right = (double)luaL_checknumber(L, 2);
  double bottom = (double)luaL_checknumber(L, 3);
  double top = (double)luaL_checknumber(L, 4);
  double znear = (double)luaL_checknumber(L, 5);
  double zfar = (double)luaL_checknumber(L, 6);
  rlOrtho(left, right, bottom, top, znear, zfar);
  return 0;
}
static int lua_rlViewport(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  rlViewport(x, y, width, height);
  return 0;
}
static int lua_rlBegin(lua_State *L) {
  int mode = luaL_checkinteger(L, 1);
  rlBegin(mode);
  return 0;
}
static int lua_rlEnd(lua_State *L) {
  rlEnd();
  return 0;
}
static int lua_rlVertex2i(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  rlVertex2i(x, y);
  return 0;
}
static int lua_rlVertex2f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  rlVertex2f(x, y);
  return 0;
}
static int lua_rlVertex3f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  rlVertex3f(x, y, z);
  return 0;
}
static int lua_rlTexCoord2f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  rlTexCoord2f(x, y);
  return 0;
}
static int lua_rlNormal3f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  rlNormal3f(x, y, z);
  return 0;
}
static int lua_rlColor4ub(lua_State *L) {
  unsigned char r = (unsigned char)luaL_checkinteger(L, 1);
  unsigned char g = (unsigned char)luaL_checkinteger(L, 2);
  unsigned char b = (unsigned char)luaL_checkinteger(L, 3);
  unsigned char a = (unsigned char)luaL_checkinteger(L, 4);
  rlColor4ub(r, g, b, a);
  return 0;
}
static int lua_rlColor3f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  rlColor3f(x, y, z);
  return 0;
}
static int lua_rlColor4f(lua_State *L) {
  float x = (float)luaL_checknumber(L, 1);
  float y = (float)luaL_checknumber(L, 2);
  float z = (float)luaL_checknumber(L, 3);
  float w = (float)luaL_checknumber(L, 4);
  rlColor4f(x, y, z, w);
  return 0;
}
static int lua_rlEnableVertexArray(lua_State *L) {
  unsigned int vaoId = (unsigned int)luaL_checkinteger(L, 1);
  bool ret = rlEnableVertexArray(vaoId);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_rlDisableVertexArray(lua_State *L) {
  rlDisableVertexArray();
  return 0;
}
static int lua_rlEnableVertexBuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableVertexBuffer(id);
  return 0;
}
static int lua_rlDisableVertexBuffer(lua_State *L) {
  rlDisableVertexBuffer();
  return 0;
}
static int lua_rlEnableVertexBufferElement(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableVertexBufferElement(id);
  return 0;
}
static int lua_rlDisableVertexBufferElement(lua_State *L) {
  rlDisableVertexBufferElement();
  return 0;
}
static int lua_rlEnableVertexAttribute(lua_State *L) {
  unsigned int index = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableVertexAttribute(index);
  return 0;
}
static int lua_rlDisableVertexAttribute(lua_State *L) {
  unsigned int index = (unsigned int)luaL_checkinteger(L, 1);
  rlDisableVertexAttribute(index);
  return 0;
}
static int lua_rlActiveTextureSlot(lua_State *L) {
  int slot = luaL_checkinteger(L, 1);
  rlActiveTextureSlot(slot);
  return 0;
}
static int lua_rlEnableTexture(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableTexture(id);
  return 0;
}
static int lua_rlDisableTexture(lua_State *L) {
  rlDisableTexture();
  return 0;
}
static int lua_rlEnableTextureCubemap(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableTextureCubemap(id);
  return 0;
}
static int lua_rlDisableTextureCubemap(lua_State *L) {
  rlDisableTextureCubemap();
  return 0;
}
static int lua_rlTextureParameters(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int param = luaL_checkinteger(L, 2);
  int value = luaL_checkinteger(L, 3);
  rlTextureParameters(id, param, value);
  return 0;
}
static int lua_rlCubemapParameters(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int param = luaL_checkinteger(L, 2);
  int value = luaL_checkinteger(L, 3);
  rlCubemapParameters(id, param, value);
  return 0;
}
static int lua_rlEnableShader(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableShader(id);
  return 0;
}
static int lua_rlDisableShader(lua_State *L) {
  rlDisableShader();
  return 0;
}
static int lua_rlEnableFramebuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlEnableFramebuffer(id);
  return 0;
}
static int lua_rlDisableFramebuffer(lua_State *L) {
  rlDisableFramebuffer();
  return 0;
}
static int lua_rlActiveDrawBuffers(lua_State *L) {
  int count = luaL_checkinteger(L, 1);
  rlActiveDrawBuffers(count);
  return 0;
}
static int lua_rlBlitFramebuffer(lua_State *L) {
  int srcX = luaL_checkinteger(L, 1);
  int srcY = luaL_checkinteger(L, 2);
  int srcWidth = luaL_checkinteger(L, 3);
  int srcHeight = luaL_checkinteger(L, 4);
  int dstX = luaL_checkinteger(L, 5);
  int dstY = luaL_checkinteger(L, 6);
  int dstWidth = luaL_checkinteger(L, 7);
  int dstHeight = luaL_checkinteger(L, 8);
  int bufferMask = luaL_checkinteger(L, 9);
  rlBlitFramebuffer(srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, bufferMask);
  return 0;
}
static int lua_rlEnableColorBlend(lua_State *L) {
  rlEnableColorBlend();
  return 0;
}
static int lua_rlDisableColorBlend(lua_State *L) {
  rlDisableColorBlend();
  return 0;
}
static int lua_rlEnableDepthTest(lua_State *L) {
  rlEnableDepthTest();
  return 0;
}
static int lua_rlDisableDepthTest(lua_State *L) {
  rlDisableDepthTest();
  return 0;
}
static int lua_rlEnableDepthMask(lua_State *L) {
  rlEnableDepthMask();
  return 0;
}
static int lua_rlDisableDepthMask(lua_State *L) {
  rlDisableDepthMask();
  return 0;
}
static int lua_rlEnableBackfaceCulling(lua_State *L) {
  rlEnableBackfaceCulling();
  return 0;
}
static int lua_rlDisableBackfaceCulling(lua_State *L) {
  rlDisableBackfaceCulling();
  return 0;
}
static int lua_rlSetCullFace(lua_State *L) {
  int mode = luaL_checkinteger(L, 1);
  rlSetCullFace(mode);
  return 0;
}
static int lua_rlEnableScissorTest(lua_State *L) {
  rlEnableScissorTest();
  return 0;
}
static int lua_rlDisableScissorTest(lua_State *L) {
  rlDisableScissorTest();
  return 0;
}
static int lua_rlScissor(lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int width = luaL_checkinteger(L, 3);
  int height = luaL_checkinteger(L, 4);
  rlScissor(x, y, width, height);
  return 0;
}
static int lua_rlEnableWireMode(lua_State *L) {
  rlEnableWireMode();
  return 0;
}
static int lua_rlEnablePointMode(lua_State *L) {
  rlEnablePointMode();
  return 0;
}
static int lua_rlDisableWireMode(lua_State *L) {
  rlDisableWireMode();
  return 0;
}
static int lua_rlSetLineWidth(lua_State *L) {
  float width = (float)luaL_checknumber(L, 1);
  rlSetLineWidth(width);
  return 0;
}
static int lua_rlGetLineWidth(lua_State *L) {
  float ret = rlGetLineWidth();
  lua_pushnumber(L, ret);
  return 1;
}
static int lua_rlEnableSmoothLines(lua_State *L) {
  rlEnableSmoothLines();
  return 0;
}
static int lua_rlDisableSmoothLines(lua_State *L) {
  rlDisableSmoothLines();
  return 0;
}
static int lua_rlEnableStereoRender(lua_State *L) {
  rlEnableStereoRender();
  return 0;
}
static int lua_rlDisableStereoRender(lua_State *L) {
  rlDisableStereoRender();
  return 0;
}
static int lua_rlIsStereoRenderEnabled(lua_State *L) {
  bool ret = rlIsStereoRenderEnabled();
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_rlClearColor(lua_State *L) {
  unsigned char r = (unsigned char)luaL_checkinteger(L, 1);
  unsigned char g = (unsigned char)luaL_checkinteger(L, 2);
  unsigned char b = (unsigned char)luaL_checkinteger(L, 3);
  unsigned char a = (unsigned char)luaL_checkinteger(L, 4);
  rlClearColor(r, g, b, a);
  return 0;
}
static int lua_rlClearScreenBuffers(lua_State *L) {
  rlClearScreenBuffers();
  return 0;
}
static int lua_rlCheckErrors(lua_State *L) {
  rlCheckErrors();
  return 0;
}
static int lua_rlSetBlendMode(lua_State *L) {
  int mode = luaL_checkinteger(L, 1);
  rlSetBlendMode(mode);
  return 0;
}
static int lua_rlSetBlendFactors(lua_State *L) {
  int glSrcFactor = luaL_checkinteger(L, 1);
  int glDstFactor = luaL_checkinteger(L, 2);
  int glEquation = luaL_checkinteger(L, 3);
  rlSetBlendFactors(glSrcFactor, glDstFactor, glEquation);
  return 0;
}
static int lua_rlSetBlendFactorsSeparate(lua_State *L) {
  int glSrcRGB = luaL_checkinteger(L, 1);
  int glDstRGB = luaL_checkinteger(L, 2);
  int glSrcAlpha = luaL_checkinteger(L, 3);
  int glDstAlpha = luaL_checkinteger(L, 4);
  int glEqRGB = luaL_checkinteger(L, 5);
  int glEqAlpha = luaL_checkinteger(L, 6);
  rlSetBlendFactorsSeparate(glSrcRGB, glDstRGB, glSrcAlpha, glDstAlpha, glEqRGB, glEqAlpha);
  return 0;
}
static int lua_rlglInit(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  rlglInit(width, height);
  return 0;
}
static int lua_rlglClose(lua_State *L) {
  rlglClose();
  return 0;
}
static int lua_rlLoadExtensions(lua_State *L) {
  void * loader = lua_touserdata(L, 1);
  rlLoadExtensions(loader);
  return 0;
}
static int lua_rlGetVersion(lua_State *L) {
  int ret = rlGetVersion();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlSetFramebufferWidth(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  rlSetFramebufferWidth(width);
  return 0;
}
static int lua_rlGetFramebufferWidth(lua_State *L) {
  int ret = rlGetFramebufferWidth();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlSetFramebufferHeight(lua_State *L) {
  int height = luaL_checkinteger(L, 1);
  rlSetFramebufferHeight(height);
  return 0;
}
static int lua_rlGetFramebufferHeight(lua_State *L) {
  int ret = rlGetFramebufferHeight();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlGetTextureIdDefault(lua_State *L) {
  unsigned int ret = rlGetTextureIdDefault();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlGetShaderIdDefault(lua_State *L) {
  unsigned int ret = rlGetShaderIdDefault();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlGetShaderLocsDefault(lua_State *L) {
  int * ret = rlGetShaderLocsDefault();
  lua_pushlightuserdata(L, (void *)ret);
  return 1;
}
static int lua_rlLoadRenderBatch(lua_State *L) {
  int numBuffers = luaL_checkinteger(L, 1);
  int bufferElements = luaL_checkinteger(L, 2);
  rlRenderBatch ret = rlLoadRenderBatch(numBuffers, bufferElements);
  lua_pushrlRenderBatch(L, &ret);
  return 1;
}
static int lua_rlUnloadRenderBatch(lua_State *L) {
  rlRenderBatch batch = lua_checkrlRenderBatch(L, 1);
  rlUnloadRenderBatch(batch);
  return 0;
}
static int lua_rlDrawRenderBatch(lua_State *L) {
  rlRenderBatch * batch = (rlRenderBatch *)lua_touserdata(L, 1);
  rlDrawRenderBatch(batch);
  return 0;
}
static int lua_rlSetRenderBatchActive(lua_State *L) {
  rlRenderBatch * batch = (rlRenderBatch *)lua_touserdata(L, 1);
  rlSetRenderBatchActive(batch);
  return 0;
}
static int lua_rlDrawRenderBatchActive(lua_State *L) {
  rlDrawRenderBatchActive();
  return 0;
}
static int lua_rlCheckRenderBatchLimit(lua_State *L) {
  int vCount = luaL_checkinteger(L, 1);
  bool ret = rlCheckRenderBatchLimit(vCount);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_rlSetTexture(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlSetTexture(id);
  return 0;
}
static int lua_rlLoadVertexArray(lua_State *L) {
  unsigned int ret = rlLoadVertexArray();
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlLoadVertexBuffer(lua_State *L) {
  const void * buffer = lua_touserdata(L, 1);
  int size = luaL_checkinteger(L, 2);
  bool dynamic = lua_toboolean(L, 3);
  unsigned int ret = rlLoadVertexBuffer(buffer, size, dynamic);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlLoadVertexBufferElement(lua_State *L) {
  const void * buffer = lua_touserdata(L, 1);
  int size = luaL_checkinteger(L, 2);
  bool dynamic = lua_toboolean(L, 3);
  unsigned int ret = rlLoadVertexBufferElement(buffer, size, dynamic);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlUpdateVertexBuffer(lua_State *L) {
  unsigned int bufferId = (unsigned int)luaL_checkinteger(L, 1);
  const void * data = lua_touserdata(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  int offset = luaL_checkinteger(L, 4);
  rlUpdateVertexBuffer(bufferId, data, dataSize, offset);
  return 0;
}
static int lua_rlUpdateVertexBufferElements(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  const void * data = lua_touserdata(L, 2);
  int dataSize = luaL_checkinteger(L, 3);
  int offset = luaL_checkinteger(L, 4);
  rlUpdateVertexBufferElements(id, data, dataSize, offset);
  return 0;
}
static int lua_rlUnloadVertexArray(lua_State *L) {
  unsigned int vaoId = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadVertexArray(vaoId);
  return 0;
}
static int lua_rlUnloadVertexBuffer(lua_State *L) {
  unsigned int vboId = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadVertexBuffer(vboId);
  return 0;
}
static int lua_rlSetVertexAttribute(lua_State *L) {
  unsigned int index = (unsigned int)luaL_checkinteger(L, 1);
  int compSize = luaL_checkinteger(L, 2);
  int type = luaL_checkinteger(L, 3);
  bool normalized = lua_toboolean(L, 4);
  int stride = luaL_checkinteger(L, 5);
  const void * pointer = lua_touserdata(L, 6);
  rlSetVertexAttribute(index, compSize, type, normalized, stride, pointer);
  return 0;
}
static int lua_rlSetVertexAttributeDivisor(lua_State *L) {
  unsigned int index = (unsigned int)luaL_checkinteger(L, 1);
  int divisor = luaL_checkinteger(L, 2);
  rlSetVertexAttributeDivisor(index, divisor);
  return 0;
}
static int lua_rlSetVertexAttributeDefault(lua_State *L) {
  int locIndex = luaL_checkinteger(L, 1);
  const void * value = lua_touserdata(L, 2);
  int attribType = luaL_checkinteger(L, 3);
  int count = luaL_checkinteger(L, 4);
  rlSetVertexAttributeDefault(locIndex, value, attribType, count);
  return 0;
}
static int lua_rlDrawVertexArray(lua_State *L) {
  int offset = luaL_checkinteger(L, 1);
  int count = luaL_checkinteger(L, 2);
  rlDrawVertexArray(offset, count);
  return 0;
}
static int lua_rlDrawVertexArrayElements(lua_State *L) {
  int offset = luaL_checkinteger(L, 1);
  int count = luaL_checkinteger(L, 2);
  const void * buffer = lua_touserdata(L, 3);
  rlDrawVertexArrayElements(offset, count, buffer);
  return 0;
}
static int lua_rlDrawVertexArrayInstanced(lua_State *L) {
  int offset = luaL_checkinteger(L, 1);
  int count = luaL_checkinteger(L, 2);
  int instances = luaL_checkinteger(L, 3);
  rlDrawVertexArrayInstanced(offset, count, instances);
  return 0;
}
static int lua_rlDrawVertexArrayElementsInstanced(lua_State *L) {
  int offset = luaL_checkinteger(L, 1);
  int count = luaL_checkinteger(L, 2);
  const void * buffer = lua_touserdata(L, 3);
  int instances = luaL_checkinteger(L, 4);
  rlDrawVertexArrayElementsInstanced(offset, count, buffer, instances);
  return 0;
}
static int lua_rlLoadTexture(lua_State *L) {
  const void * data = lua_touserdata(L, 1);
  int width = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  int format = luaL_checkinteger(L, 4);
  int mipmapCount = luaL_checkinteger(L, 5);
  unsigned int ret = rlLoadTexture(data, width, height, format, mipmapCount);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlLoadTextureDepth(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  bool useRenderBuffer = lua_toboolean(L, 3);
  unsigned int ret = rlLoadTextureDepth(width, height, useRenderBuffer);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlLoadTextureCubemap(lua_State *L) {
  const void * data = lua_touserdata(L, 1);
  int size = luaL_checkinteger(L, 2);
  int format = luaL_checkinteger(L, 3);
  unsigned int ret = rlLoadTextureCubemap(data, size, format);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlUpdateTexture(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int offsetX = luaL_checkinteger(L, 2);
  int offsetY = luaL_checkinteger(L, 3);
  int width = luaL_checkinteger(L, 4);
  int height = luaL_checkinteger(L, 5);
  int format = luaL_checkinteger(L, 6);
  const void * data = lua_touserdata(L, 7);
  rlUpdateTexture(id, offsetX, offsetY, width, height, format, data);
  return 0;
}
static int lua_rlGetGlTextureFormats(lua_State *L) {
  int format = luaL_checkinteger(L, 1);
  unsigned int * glInternalFormat = (unsigned int *)lua_touserdata(L, 2);
  unsigned int * glFormat = (unsigned int *)lua_touserdata(L, 3);
  unsigned int * glType = (unsigned int *)lua_touserdata(L, 4);
  rlGetGlTextureFormats(format, glInternalFormat, glFormat, glType);
  return 0;
}
static int lua_rlGetPixelFormatName(lua_State *L) {
  unsigned int format = (unsigned int)luaL_checkinteger(L, 1);
  const char * ret = rlGetPixelFormatName(format);
  lua_pushstring(L, ret);
  return 1;
}
static int lua_rlUnloadTexture(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadTexture(id);
  return 0;
}
static int lua_rlGenTextureMipmaps(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int width = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  int format = luaL_checkinteger(L, 4);
  int * mipmaps = (int *)lua_touserdata(L, 5);
  rlGenTextureMipmaps(id, width, height, format, mipmaps);
  return 0;
}
static int lua_rlReadTexturePixels(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int width = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  int format = luaL_checkinteger(L, 4);
  void * ret = rlReadTexturePixels(id, width, height, format);
  lua_pushlightuserdata(L, ret);
  return 1;
}
static int lua_rlReadScreenPixels(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  unsigned char * ret = rlReadScreenPixels(width, height);
  lua_pushstring(L, (const char *)ret);
  return 1;
}
static int lua_rlLoadFramebuffer(lua_State *L) {
  int width = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  unsigned int ret = rlLoadFramebuffer(width, height);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlFramebufferAttach(lua_State *L) {
  unsigned int fboId = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int texId = (unsigned int)luaL_checkinteger(L, 2);
  int attachType = luaL_checkinteger(L, 3);
  int texType = luaL_checkinteger(L, 4);
  int mipLevel = luaL_checkinteger(L, 5);
  rlFramebufferAttach(fboId, texId, attachType, texType, mipLevel);
  return 0;
}
static int lua_rlFramebufferComplete(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  bool ret = rlFramebufferComplete(id);
  lua_pushboolean(L, ret);
  return 1;
}
static int lua_rlUnloadFramebuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadFramebuffer(id);
  return 0;
}
static int lua_rlLoadShaderCode(lua_State *L) {
  const char * vsCode = luaL_checkstring(L, 1);
  const char * fsCode = luaL_checkstring(L, 2);
  unsigned int ret = rlLoadShaderCode(vsCode, fsCode);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlCompileShader(lua_State *L) {
  const char * shaderCode = luaL_checkstring(L, 1);
  int type = luaL_checkinteger(L, 2);
  unsigned int ret = rlCompileShader(shaderCode, type);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlLoadShaderProgram(lua_State *L) {
  unsigned int vShaderId = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int fShaderId = (unsigned int)luaL_checkinteger(L, 2);
  unsigned int ret = rlLoadShaderProgram(vShaderId, fShaderId);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlUnloadShaderProgram(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadShaderProgram(id);
  return 0;
}
static int lua_rlGetLocationUniform(lua_State *L) {
  unsigned int shaderId = (unsigned int)luaL_checkinteger(L, 1);
  const char * uniformName = luaL_checkstring(L, 2);
  int ret = rlGetLocationUniform(shaderId, uniformName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlGetLocationAttrib(lua_State *L) {
  unsigned int shaderId = (unsigned int)luaL_checkinteger(L, 1);
  const char * attribName = luaL_checkstring(L, 2);
  int ret = rlGetLocationAttrib(shaderId, attribName);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlSetUniform(lua_State *L) {
  int locIndex = luaL_checkinteger(L, 1);
  const void * value = lua_touserdata(L, 2);
  int uniformType = luaL_checkinteger(L, 3);
  int count = luaL_checkinteger(L, 4);
  rlSetUniform(locIndex, value, uniformType, count);
  return 0;
}
static int lua_rlSetUniformMatrix(lua_State *L) {
  int locIndex = luaL_checkinteger(L, 1);
  Matrix mat = lua_checkMatrix(L, 2);
  rlSetUniformMatrix(locIndex, mat);
  return 0;
}
static int lua_rlSetUniformSampler(lua_State *L) {
  int locIndex = luaL_checkinteger(L, 1);
  unsigned int textureId = (unsigned int)luaL_checkinteger(L, 2);
  rlSetUniformSampler(locIndex, textureId);
  return 0;
}
static int lua_rlSetShader(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  int * locs = (int *)lua_touserdata(L, 2);
  rlSetShader(id, locs);
  return 0;
}
static int lua_rlLoadComputeShaderProgram(lua_State *L) {
  unsigned int shaderId = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int ret = rlLoadComputeShaderProgram(shaderId);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlComputeShaderDispatch(lua_State *L) {
  unsigned int groupX = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int groupY = (unsigned int)luaL_checkinteger(L, 2);
  unsigned int groupZ = (unsigned int)luaL_checkinteger(L, 3);
  rlComputeShaderDispatch(groupX, groupY, groupZ);
  return 0;
}
static int lua_rlLoadShaderBuffer(lua_State *L) {
  unsigned int size = (unsigned int)luaL_checkinteger(L, 1);
  const void * data = lua_touserdata(L, 2);
  int usageHint = luaL_checkinteger(L, 3);
  unsigned int ret = rlLoadShaderBuffer(size, data, usageHint);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlUnloadShaderBuffer(lua_State *L) {
  unsigned int ssboId = (unsigned int)luaL_checkinteger(L, 1);
  rlUnloadShaderBuffer(ssboId);
  return 0;
}
static int lua_rlUpdateShaderBuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  const void * data = lua_touserdata(L, 2);
  unsigned int dataSize = (unsigned int)luaL_checkinteger(L, 3);
  unsigned int offset = (unsigned int)luaL_checkinteger(L, 4);
  rlUpdateShaderBuffer(id, data, dataSize, offset);
  return 0;
}
static int lua_rlBindShaderBuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int index = (unsigned int)luaL_checkinteger(L, 2);
  rlBindShaderBuffer(id, index);
  return 0;
}
static int lua_rlReadShaderBuffer(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  void * dest = lua_touserdata(L, 2);
  unsigned int count = (unsigned int)luaL_checkinteger(L, 3);
  unsigned int offset = (unsigned int)luaL_checkinteger(L, 4);
  rlReadShaderBuffer(id, dest, count, offset);
  return 0;
}
static int lua_rlCopyShaderBuffer(lua_State *L) {
  unsigned int destId = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int srcId = (unsigned int)luaL_checkinteger(L, 2);
  unsigned int destOffset = (unsigned int)luaL_checkinteger(L, 3);
  unsigned int srcOffset = (unsigned int)luaL_checkinteger(L, 4);
  unsigned int count = (unsigned int)luaL_checkinteger(L, 5);
  rlCopyShaderBuffer(destId, srcId, destOffset, srcOffset, count);
  return 0;
}
static int lua_rlGetShaderBufferSize(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int ret = rlGetShaderBufferSize(id);
  lua_pushinteger(L, ret);
  return 1;
}
static int lua_rlBindImageTexture(lua_State *L) {
  unsigned int id = (unsigned int)luaL_checkinteger(L, 1);
  unsigned int index = (unsigned int)luaL_checkinteger(L, 2);
  int format = luaL_checkinteger(L, 3);
  bool readonly = lua_toboolean(L, 4);
  rlBindImageTexture(id, index, format, readonly);
  return 0;
}
static int lua_rlGetMatrixModelview(lua_State *L) {
  Matrix ret = rlGetMatrixModelview();
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_rlGetMatrixProjection(lua_State *L) {
  Matrix ret = rlGetMatrixProjection();
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_rlGetMatrixTransform(lua_State *L) {
  Matrix ret = rlGetMatrixTransform();
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_rlGetMatrixProjectionStereo(lua_State *L) {
  int eye = luaL_checkinteger(L, 1);
  Matrix ret = rlGetMatrixProjectionStereo(eye);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_rlGetMatrixViewOffsetStereo(lua_State *L) {
  int eye = luaL_checkinteger(L, 1);
  Matrix ret = rlGetMatrixViewOffsetStereo(eye);
  lua_pushMatrix(L, &ret);
  return 1;
}
static int lua_rlSetMatrixProjection(lua_State *L) {
  Matrix proj = lua_checkMatrix(L, 1);
  rlSetMatrixProjection(proj);
  return 0;
}
static int lua_rlSetMatrixModelview(lua_State *L) {
  Matrix view = lua_checkMatrix(L, 1);
  rlSetMatrixModelview(view);
  return 0;
}
static int lua_rlSetMatrixProjectionStereo(lua_State *L) {
  Matrix right = lua_checkMatrix(L, 1);
  Matrix left = lua_checkMatrix(L, 2);
  rlSetMatrixProjectionStereo(right, left);
  return 0;
}
static int lua_rlSetMatrixViewOffsetStereo(lua_State *L) {
  Matrix right = lua_checkMatrix(L, 1);
  Matrix left = lua_checkMatrix(L, 2);
  rlSetMatrixViewOffsetStereo(right, left);
  return 0;
}
static int lua_rlLoadDrawCube(lua_State *L) {
  rlLoadDrawCube();
  return 0;
}
static int lua_rlLoadDrawQuad(lua_State *L) {
  rlLoadDrawQuad();
  return 0;
}
static void initialize_raylua(lua_State *L) {
  luaL_Reg funcs[] = {
    {"InitWindow", lua_InitWindow},
    {"CloseWindow", lua_CloseWindow},
    {"WindowShouldClose", lua_WindowShouldClose},
    {"IsWindowReady", lua_IsWindowReady},
    {"IsWindowFullscreen", lua_IsWindowFullscreen},
    {"IsWindowHidden", lua_IsWindowHidden},
    {"IsWindowMinimized", lua_IsWindowMinimized},
    {"IsWindowMaximized", lua_IsWindowMaximized},
    {"IsWindowFocused", lua_IsWindowFocused},
    {"IsWindowResized", lua_IsWindowResized},
    {"IsWindowState", lua_IsWindowState},
    {"SetWindowState", lua_SetWindowState},
    {"ClearWindowState", lua_ClearWindowState},
    {"ToggleFullscreen", lua_ToggleFullscreen},
    {"ToggleBorderlessWindowed", lua_ToggleBorderlessWindowed},
    {"MaximizeWindow", lua_MaximizeWindow},
    {"MinimizeWindow", lua_MinimizeWindow},
    {"RestoreWindow", lua_RestoreWindow},
    {"SetWindowIcon", lua_SetWindowIcon},
    {"SetWindowIcons", lua_SetWindowIcons},
    {"SetWindowTitle", lua_SetWindowTitle},
    {"SetWindowPosition", lua_SetWindowPosition},
    {"SetWindowMonitor", lua_SetWindowMonitor},
    {"SetWindowMinSize", lua_SetWindowMinSize},
    {"SetWindowMaxSize", lua_SetWindowMaxSize},
    {"SetWindowSize", lua_SetWindowSize},
    {"SetWindowOpacity", lua_SetWindowOpacity},
    {"SetWindowFocused", lua_SetWindowFocused},
    {"GetWindowHandle", lua_GetWindowHandle},
    {"GetScreenWidth", lua_GetScreenWidth},
    {"GetScreenHeight", lua_GetScreenHeight},
    {"GetRenderWidth", lua_GetRenderWidth},
    {"GetRenderHeight", lua_GetRenderHeight},
    {"GetMonitorCount", lua_GetMonitorCount},
    {"GetCurrentMonitor", lua_GetCurrentMonitor},
    {"GetMonitorPosition", lua_GetMonitorPosition},
    {"GetMonitorWidth", lua_GetMonitorWidth},
    {"GetMonitorHeight", lua_GetMonitorHeight},
    {"GetMonitorPhysicalWidth", lua_GetMonitorPhysicalWidth},
    {"GetMonitorPhysicalHeight", lua_GetMonitorPhysicalHeight},
    {"GetMonitorRefreshRate", lua_GetMonitorRefreshRate},
    {"GetWindowPosition", lua_GetWindowPosition},
    {"GetWindowScaleDPI", lua_GetWindowScaleDPI},
    {"GetMonitorName", lua_GetMonitorName},
    {"SetClipboardText", lua_SetClipboardText},
    {"GetClipboardText", lua_GetClipboardText},
    {"EnableEventWaiting", lua_EnableEventWaiting},
    {"DisableEventWaiting", lua_DisableEventWaiting},
    {"ShowCursor", lua_ShowCursor},
    {"HideCursor", lua_HideCursor},
    {"IsCursorHidden", lua_IsCursorHidden},
    {"EnableCursor", lua_EnableCursor},
    {"DisableCursor", lua_DisableCursor},
    {"IsCursorOnScreen", lua_IsCursorOnScreen},
    {"ClearBackground", lua_ClearBackground},
    {"BeginDrawing", lua_BeginDrawing},
    {"EndDrawing", lua_EndDrawing},
    {"BeginMode2D", lua_BeginMode2D},
    {"EndMode2D", lua_EndMode2D},
    {"BeginMode3D", lua_BeginMode3D},
    {"EndMode3D", lua_EndMode3D},
    {"BeginTextureMode", lua_BeginTextureMode},
    {"EndTextureMode", lua_EndTextureMode},
    {"BeginShaderMode", lua_BeginShaderMode},
    {"EndShaderMode", lua_EndShaderMode},
    {"BeginBlendMode", lua_BeginBlendMode},
    {"EndBlendMode", lua_EndBlendMode},
    {"BeginScissorMode", lua_BeginScissorMode},
    {"EndScissorMode", lua_EndScissorMode},
    {"BeginVrStereoMode", lua_BeginVrStereoMode},
    {"EndVrStereoMode", lua_EndVrStereoMode},
    {"LoadVrStereoConfig", lua_LoadVrStereoConfig},
    {"UnloadVrStereoConfig", lua_UnloadVrStereoConfig},
    {"LoadShader", lua_LoadShader},
    {"LoadShaderFromMemory", lua_LoadShaderFromMemory},
    {"IsShaderReady", lua_IsShaderReady},
    {"GetShaderLocation", lua_GetShaderLocation},
    {"GetShaderLocationAttrib", lua_GetShaderLocationAttrib},
    {"SetShaderValue", lua_SetShaderValue},
    {"SetShaderValueV", lua_SetShaderValueV},
    {"SetShaderValueMatrix", lua_SetShaderValueMatrix},
    {"SetShaderValueTexture", lua_SetShaderValueTexture},
    {"UnloadShader", lua_UnloadShader},
    {"GetMouseRay", lua_GetMouseRay},
    {"GetCameraMatrix", lua_GetCameraMatrix},
    {"GetCameraMatrix2D", lua_GetCameraMatrix2D},
    {"GetWorldToScreen", lua_GetWorldToScreen},
    {"GetScreenToWorld2D", lua_GetScreenToWorld2D},
    {"GetWorldToScreenEx", lua_GetWorldToScreenEx},
    {"GetWorldToScreen2D", lua_GetWorldToScreen2D},
    {"SetTargetFPS", lua_SetTargetFPS},
    {"GetFrameTime", lua_GetFrameTime},
    {"GetTime", lua_GetTime},
    {"GetFPS", lua_GetFPS},
    {"SwapScreenBuffer", lua_SwapScreenBuffer},
    {"PollInputEvents", lua_PollInputEvents},
    {"WaitTime", lua_WaitTime},
    {"SetRandomSeed", lua_SetRandomSeed},
    {"GetRandomValue", lua_GetRandomValue},
    {"LoadRandomSequence", lua_LoadRandomSequence},
    {"UnloadRandomSequence", lua_UnloadRandomSequence},
    {"TakeScreenshot", lua_TakeScreenshot},
    {"SetConfigFlags", lua_SetConfigFlags},
    {"OpenURL", lua_OpenURL},
    {"TraceLog", lua_TraceLog},
    {"SetTraceLogLevel", lua_SetTraceLogLevel},
    {"MemAlloc", lua_MemAlloc},
    {"MemRealloc", lua_MemRealloc},
    {"MemFree", lua_MemFree},
    {"SetTraceLogCallback", lua_SetTraceLogCallback},
    {"SetLoadFileDataCallback", lua_SetLoadFileDataCallback},
    {"SetSaveFileDataCallback", lua_SetSaveFileDataCallback},
    {"SetLoadFileTextCallback", lua_SetLoadFileTextCallback},
    {"SetSaveFileTextCallback", lua_SetSaveFileTextCallback},
    {"LoadFileData", lua_LoadFileData},
    {"UnloadFileData", lua_UnloadFileData},
    {"SaveFileData", lua_SaveFileData},
    {"ExportDataAsCode", lua_ExportDataAsCode},
    {"LoadFileText", lua_LoadFileText},
    {"UnloadFileText", lua_UnloadFileText},
    {"SaveFileText", lua_SaveFileText},
    {"FileExists", lua_FileExists},
    {"DirectoryExists", lua_DirectoryExists},
    {"IsFileExtension", lua_IsFileExtension},
    {"GetFileLength", lua_GetFileLength},
    {"GetFileExtension", lua_GetFileExtension},
    {"GetFileName", lua_GetFileName},
    {"GetFileNameWithoutExt", lua_GetFileNameWithoutExt},
    {"GetDirectoryPath", lua_GetDirectoryPath},
    {"GetPrevDirectoryPath", lua_GetPrevDirectoryPath},
    {"GetWorkingDirectory", lua_GetWorkingDirectory},
    {"GetApplicationDirectory", lua_GetApplicationDirectory},
    {"ChangeDirectory", lua_ChangeDirectory},
    {"IsPathFile", lua_IsPathFile},
    {"LoadDirectoryFiles", lua_LoadDirectoryFiles},
    {"LoadDirectoryFilesEx", lua_LoadDirectoryFilesEx},
    {"UnloadDirectoryFiles", lua_UnloadDirectoryFiles},
    {"IsFileDropped", lua_IsFileDropped},
    {"LoadDroppedFiles", lua_LoadDroppedFiles},
    {"UnloadDroppedFiles", lua_UnloadDroppedFiles},
    {"GetFileModTime", lua_GetFileModTime},
    {"CompressData", lua_CompressData},
    {"DecompressData", lua_DecompressData},
    {"EncodeDataBase64", lua_EncodeDataBase64},
    {"DecodeDataBase64", lua_DecodeDataBase64},
    {"LoadAutomationEventList", lua_LoadAutomationEventList},
    {"UnloadAutomationEventList", lua_UnloadAutomationEventList},
    {"ExportAutomationEventList", lua_ExportAutomationEventList},
    {"SetAutomationEventList", lua_SetAutomationEventList},
    {"SetAutomationEventBaseFrame", lua_SetAutomationEventBaseFrame},
    {"StartAutomationEventRecording", lua_StartAutomationEventRecording},
    {"StopAutomationEventRecording", lua_StopAutomationEventRecording},
    {"PlayAutomationEvent", lua_PlayAutomationEvent},
    {"IsKeyPressed", lua_IsKeyPressed},
    {"IsKeyPressedRepeat", lua_IsKeyPressedRepeat},
    {"IsKeyDown", lua_IsKeyDown},
    {"IsKeyReleased", lua_IsKeyReleased},
    {"IsKeyUp", lua_IsKeyUp},
    {"GetKeyPressed", lua_GetKeyPressed},
    {"GetCharPressed", lua_GetCharPressed},
    {"SetExitKey", lua_SetExitKey},
    {"IsGamepadAvailable", lua_IsGamepadAvailable},
    {"GetGamepadName", lua_GetGamepadName},
    {"IsGamepadButtonPressed", lua_IsGamepadButtonPressed},
    {"IsGamepadButtonDown", lua_IsGamepadButtonDown},
    {"IsGamepadButtonReleased", lua_IsGamepadButtonReleased},
    {"IsGamepadButtonUp", lua_IsGamepadButtonUp},
    {"GetGamepadButtonPressed", lua_GetGamepadButtonPressed},
    {"GetGamepadAxisCount", lua_GetGamepadAxisCount},
    {"GetGamepadAxisMovement", lua_GetGamepadAxisMovement},
    {"SetGamepadMappings", lua_SetGamepadMappings},
    {"IsMouseButtonPressed", lua_IsMouseButtonPressed},
    {"IsMouseButtonDown", lua_IsMouseButtonDown},
    {"IsMouseButtonReleased", lua_IsMouseButtonReleased},
    {"IsMouseButtonUp", lua_IsMouseButtonUp},
    {"GetMouseX", lua_GetMouseX},
    {"GetMouseY", lua_GetMouseY},
    {"GetMousePosition", lua_GetMousePosition},
    {"GetMouseDelta", lua_GetMouseDelta},
    {"SetMousePosition", lua_SetMousePosition},
    {"SetMouseOffset", lua_SetMouseOffset},
    {"SetMouseScale", lua_SetMouseScale},
    {"GetMouseWheelMove", lua_GetMouseWheelMove},
    {"GetMouseWheelMoveV", lua_GetMouseWheelMoveV},
    {"SetMouseCursor", lua_SetMouseCursor},
    {"GetTouchX", lua_GetTouchX},
    {"GetTouchY", lua_GetTouchY},
    {"GetTouchPosition", lua_GetTouchPosition},
    {"GetTouchPointId", lua_GetTouchPointId},
    {"GetTouchPointCount", lua_GetTouchPointCount},
    {"SetGesturesEnabled", lua_SetGesturesEnabled},
    {"IsGestureDetected", lua_IsGestureDetected},
    {"GetGestureDetected", lua_GetGestureDetected},
    {"GetGestureHoldDuration", lua_GetGestureHoldDuration},
    {"GetGestureDragVector", lua_GetGestureDragVector},
    {"GetGestureDragAngle", lua_GetGestureDragAngle},
    {"GetGesturePinchVector", lua_GetGesturePinchVector},
    {"GetGesturePinchAngle", lua_GetGesturePinchAngle},
    {"UpdateCamera", lua_UpdateCamera},
    {"UpdateCameraPro", lua_UpdateCameraPro},
    {"SetShapesTexture", lua_SetShapesTexture},
    {"DrawPixel", lua_DrawPixel},
    {"DrawPixelV", lua_DrawPixelV},
    {"DrawLine", lua_DrawLine},
    {"DrawLineV", lua_DrawLineV},
    {"DrawLineEx", lua_DrawLineEx},
    {"DrawLineStrip", lua_DrawLineStrip},
    {"DrawLineBezier", lua_DrawLineBezier},
    {"DrawCircle", lua_DrawCircle},
    {"DrawCircleSector", lua_DrawCircleSector},
    {"DrawCircleSectorLines", lua_DrawCircleSectorLines},
    {"DrawCircleGradient", lua_DrawCircleGradient},
    {"DrawCircleV", lua_DrawCircleV},
    {"DrawCircleLines", lua_DrawCircleLines},
    {"DrawCircleLinesV", lua_DrawCircleLinesV},
    {"DrawEllipse", lua_DrawEllipse},
    {"DrawEllipseLines", lua_DrawEllipseLines},
    {"DrawRing", lua_DrawRing},
    {"DrawRingLines", lua_DrawRingLines},
    {"DrawRectangle", lua_DrawRectangle},
    {"DrawRectangleV", lua_DrawRectangleV},
    {"DrawRectangleRec", lua_DrawRectangleRec},
    {"DrawRectanglePro", lua_DrawRectanglePro},
    {"DrawRectangleGradientV", lua_DrawRectangleGradientV},
    {"DrawRectangleGradientH", lua_DrawRectangleGradientH},
    {"DrawRectangleGradientEx", lua_DrawRectangleGradientEx},
    {"DrawRectangleLines", lua_DrawRectangleLines},
    {"DrawRectangleLinesEx", lua_DrawRectangleLinesEx},
    {"DrawRectangleRounded", lua_DrawRectangleRounded},
    {"DrawRectangleRoundedLines", lua_DrawRectangleRoundedLines},
    {"DrawTriangle", lua_DrawTriangle},
    {"DrawTriangleLines", lua_DrawTriangleLines},
    {"DrawTriangleFan", lua_DrawTriangleFan},
    {"DrawTriangleStrip", lua_DrawTriangleStrip},
    {"DrawPoly", lua_DrawPoly},
    {"DrawPolyLines", lua_DrawPolyLines},
    {"DrawPolyLinesEx", lua_DrawPolyLinesEx},
    {"DrawSplineLinear", lua_DrawSplineLinear},
    {"DrawSplineBasis", lua_DrawSplineBasis},
    {"DrawSplineCatmullRom", lua_DrawSplineCatmullRom},
    {"DrawSplineBezierQuadratic", lua_DrawSplineBezierQuadratic},
    {"DrawSplineBezierCubic", lua_DrawSplineBezierCubic},
    {"DrawSplineSegmentLinear", lua_DrawSplineSegmentLinear},
    {"DrawSplineSegmentBasis", lua_DrawSplineSegmentBasis},
    {"DrawSplineSegmentCatmullRom", lua_DrawSplineSegmentCatmullRom},
    {"DrawSplineSegmentBezierQuadratic", lua_DrawSplineSegmentBezierQuadratic},
    {"DrawSplineSegmentBezierCubic", lua_DrawSplineSegmentBezierCubic},
    {"GetSplinePointLinear", lua_GetSplinePointLinear},
    {"GetSplinePointBasis", lua_GetSplinePointBasis},
    {"GetSplinePointCatmullRom", lua_GetSplinePointCatmullRom},
    {"GetSplinePointBezierQuad", lua_GetSplinePointBezierQuad},
    {"GetSplinePointBezierCubic", lua_GetSplinePointBezierCubic},
    {"CheckCollisionRecs", lua_CheckCollisionRecs},
    {"CheckCollisionCircles", lua_CheckCollisionCircles},
    {"CheckCollisionCircleRec", lua_CheckCollisionCircleRec},
    {"CheckCollisionPointRec", lua_CheckCollisionPointRec},
    {"CheckCollisionPointCircle", lua_CheckCollisionPointCircle},
    {"CheckCollisionPointTriangle", lua_CheckCollisionPointTriangle},
    {"CheckCollisionPointPoly", lua_CheckCollisionPointPoly},
    {"CheckCollisionLines", lua_CheckCollisionLines},
    {"CheckCollisionPointLine", lua_CheckCollisionPointLine},
    {"GetCollisionRec", lua_GetCollisionRec},
    {"LoadImage", lua_LoadImage},
    {"LoadImageRaw", lua_LoadImageRaw},
    {"LoadImageSvg", lua_LoadImageSvg},
    {"LoadImageAnim", lua_LoadImageAnim},
    {"LoadImageFromMemory", lua_LoadImageFromMemory},
    {"LoadImageFromTexture", lua_LoadImageFromTexture},
    {"LoadImageFromScreen", lua_LoadImageFromScreen},
    {"IsImageReady", lua_IsImageReady},
    {"UnloadImage", lua_UnloadImage},
    {"ExportImage", lua_ExportImage},
    {"ExportImageToMemory", lua_ExportImageToMemory},
    {"ExportImageAsCode", lua_ExportImageAsCode},
    {"GenImageColor", lua_GenImageColor},
    {"GenImageGradientLinear", lua_GenImageGradientLinear},
    {"GenImageGradientRadial", lua_GenImageGradientRadial},
    {"GenImageGradientSquare", lua_GenImageGradientSquare},
    {"GenImageChecked", lua_GenImageChecked},
    {"GenImageWhiteNoise", lua_GenImageWhiteNoise},
    {"GenImagePerlinNoise", lua_GenImagePerlinNoise},
    {"GenImageCellular", lua_GenImageCellular},
    {"GenImageText", lua_GenImageText},
    {"ImageCopy", lua_ImageCopy},
    {"ImageFromImage", lua_ImageFromImage},
    {"ImageText", lua_ImageText},
    {"ImageTextEx", lua_ImageTextEx},
    {"ImageFormat", lua_ImageFormat},
    {"ImageToPOT", lua_ImageToPOT},
    {"ImageCrop", lua_ImageCrop},
    {"ImageAlphaCrop", lua_ImageAlphaCrop},
    {"ImageAlphaClear", lua_ImageAlphaClear},
    {"ImageAlphaMask", lua_ImageAlphaMask},
    {"ImageAlphaPremultiply", lua_ImageAlphaPremultiply},
    {"ImageBlurGaussian", lua_ImageBlurGaussian},
    {"ImageResize", lua_ImageResize},
    {"ImageResizeNN", lua_ImageResizeNN},
    {"ImageResizeCanvas", lua_ImageResizeCanvas},
    {"ImageMipmaps", lua_ImageMipmaps},
    {"ImageDither", lua_ImageDither},
    {"ImageFlipVertical", lua_ImageFlipVertical},
    {"ImageFlipHorizontal", lua_ImageFlipHorizontal},
    {"ImageRotate", lua_ImageRotate},
    {"ImageRotateCW", lua_ImageRotateCW},
    {"ImageRotateCCW", lua_ImageRotateCCW},
    {"ImageColorTint", lua_ImageColorTint},
    {"ImageColorInvert", lua_ImageColorInvert},
    {"ImageColorGrayscale", lua_ImageColorGrayscale},
    {"ImageColorContrast", lua_ImageColorContrast},
    {"ImageColorBrightness", lua_ImageColorBrightness},
    {"ImageColorReplace", lua_ImageColorReplace},
    {"LoadImageColors", lua_LoadImageColors},
    {"LoadImagePalette", lua_LoadImagePalette},
    {"UnloadImageColors", lua_UnloadImageColors},
    {"UnloadImagePalette", lua_UnloadImagePalette},
    {"GetImageAlphaBorder", lua_GetImageAlphaBorder},
    {"GetImageColor", lua_GetImageColor},
    {"ImageClearBackground", lua_ImageClearBackground},
    {"ImageDrawPixel", lua_ImageDrawPixel},
    {"ImageDrawPixelV", lua_ImageDrawPixelV},
    {"ImageDrawLine", lua_ImageDrawLine},
    {"ImageDrawLineV", lua_ImageDrawLineV},
    {"ImageDrawCircle", lua_ImageDrawCircle},
    {"ImageDrawCircleV", lua_ImageDrawCircleV},
    {"ImageDrawCircleLines", lua_ImageDrawCircleLines},
    {"ImageDrawCircleLinesV", lua_ImageDrawCircleLinesV},
    {"ImageDrawRectangle", lua_ImageDrawRectangle},
    {"ImageDrawRectangleV", lua_ImageDrawRectangleV},
    {"ImageDrawRectangleRec", lua_ImageDrawRectangleRec},
    {"ImageDrawRectangleLines", lua_ImageDrawRectangleLines},
    {"ImageDraw", lua_ImageDraw},
    {"ImageDrawText", lua_ImageDrawText},
    {"ImageDrawTextEx", lua_ImageDrawTextEx},
    {"LoadTexture", lua_LoadTexture},
    {"LoadTextureFromImage", lua_LoadTextureFromImage},
    {"LoadTextureCubemap", lua_LoadTextureCubemap},
    {"LoadRenderTexture", lua_LoadRenderTexture},
    {"IsTextureReady", lua_IsTextureReady},
    {"UnloadTexture", lua_UnloadTexture},
    {"IsRenderTextureReady", lua_IsRenderTextureReady},
    {"UnloadRenderTexture", lua_UnloadRenderTexture},
    {"UpdateTexture", lua_UpdateTexture},
    {"UpdateTextureRec", lua_UpdateTextureRec},
    {"GenTextureMipmaps", lua_GenTextureMipmaps},
    {"SetTextureFilter", lua_SetTextureFilter},
    {"SetTextureWrap", lua_SetTextureWrap},
    {"DrawTexture", lua_DrawTexture},
    {"DrawTextureV", lua_DrawTextureV},
    {"DrawTextureEx", lua_DrawTextureEx},
    {"DrawTextureRec", lua_DrawTextureRec},
    {"DrawTexturePro", lua_DrawTexturePro},
    {"DrawTextureNPatch", lua_DrawTextureNPatch},
    {"Fade", lua_Fade},
    {"ColorToInt", lua_ColorToInt},
    {"ColorNormalize", lua_ColorNormalize},
    {"ColorFromNormalized", lua_ColorFromNormalized},
    {"ColorToHSV", lua_ColorToHSV},
    {"ColorFromHSV", lua_ColorFromHSV},
    {"ColorTint", lua_ColorTint},
    {"ColorBrightness", lua_ColorBrightness},
    {"ColorContrast", lua_ColorContrast},
    {"ColorAlpha", lua_ColorAlpha},
    {"ColorAlphaBlend", lua_ColorAlphaBlend},
    {"GetColor", lua_GetColor},
    {"GetPixelColor", lua_GetPixelColor},
    {"SetPixelColor", lua_SetPixelColor},
    {"GetPixelDataSize", lua_GetPixelDataSize},
    {"GetFontDefault", lua_GetFontDefault},
    {"LoadFont", lua_LoadFont},
    {"LoadFontEx", lua_LoadFontEx},
    {"LoadFontFromImage", lua_LoadFontFromImage},
    {"LoadFontFromMemory", lua_LoadFontFromMemory},
    {"IsFontReady", lua_IsFontReady},
    {"LoadFontData", lua_LoadFontData},
    {"GenImageFontAtlas", lua_GenImageFontAtlas},
    {"UnloadFontData", lua_UnloadFontData},
    {"UnloadFont", lua_UnloadFont},
    {"ExportFontAsCode", lua_ExportFontAsCode},
    {"DrawFPS", lua_DrawFPS},
    {"DrawText", lua_DrawText},
    {"DrawTextEx", lua_DrawTextEx},
    {"DrawTextPro", lua_DrawTextPro},
    {"DrawTextCodepoint", lua_DrawTextCodepoint},
    {"DrawTextCodepoints", lua_DrawTextCodepoints},
    {"SetTextLineSpacing", lua_SetTextLineSpacing},
    {"MeasureText", lua_MeasureText},
    {"MeasureTextEx", lua_MeasureTextEx},
    {"GetGlyphIndex", lua_GetGlyphIndex},
    {"GetGlyphInfo", lua_GetGlyphInfo},
    {"GetGlyphAtlasRec", lua_GetGlyphAtlasRec},
    {"LoadUTF8", lua_LoadUTF8},
    {"UnloadUTF8", lua_UnloadUTF8},
    {"LoadCodepoints", lua_LoadCodepoints},
    {"UnloadCodepoints", lua_UnloadCodepoints},
    {"GetCodepointCount", lua_GetCodepointCount},
    {"GetCodepoint", lua_GetCodepoint},
    {"GetCodepointNext", lua_GetCodepointNext},
    {"GetCodepointPrevious", lua_GetCodepointPrevious},
    {"CodepointToUTF8", lua_CodepointToUTF8},
    {"TextCopy", lua_TextCopy},
    {"TextIsEqual", lua_TextIsEqual},
    {"TextLength", lua_TextLength},
    {"TextFormat", lua_TextFormat},
    {"TextSubtext", lua_TextSubtext},
    {"TextReplace", lua_TextReplace},
    {"TextInsert", lua_TextInsert},
    {"TextJoin", lua_TextJoin},
    {"TextSplit", lua_TextSplit},
    {"TextAppend", lua_TextAppend},
    {"TextFindIndex", lua_TextFindIndex},
    {"TextToUpper", lua_TextToUpper},
    {"TextToLower", lua_TextToLower},
    {"TextToPascal", lua_TextToPascal},
    {"TextToInteger", lua_TextToInteger},
    {"DrawLine3D", lua_DrawLine3D},
    {"DrawPoint3D", lua_DrawPoint3D},
    {"DrawCircle3D", lua_DrawCircle3D},
    {"DrawTriangle3D", lua_DrawTriangle3D},
    {"DrawTriangleStrip3D", lua_DrawTriangleStrip3D},
    {"DrawCube", lua_DrawCube},
    {"DrawCubeV", lua_DrawCubeV},
    {"DrawCubeWires", lua_DrawCubeWires},
    {"DrawCubeWiresV", lua_DrawCubeWiresV},
    {"DrawSphere", lua_DrawSphere},
    {"DrawSphereEx", lua_DrawSphereEx},
    {"DrawSphereWires", lua_DrawSphereWires},
    {"DrawCylinder", lua_DrawCylinder},
    {"DrawCylinderEx", lua_DrawCylinderEx},
    {"DrawCylinderWires", lua_DrawCylinderWires},
    {"DrawCylinderWiresEx", lua_DrawCylinderWiresEx},
    {"DrawCapsule", lua_DrawCapsule},
    {"DrawCapsuleWires", lua_DrawCapsuleWires},
    {"DrawPlane", lua_DrawPlane},
    {"DrawRay", lua_DrawRay},
    {"DrawGrid", lua_DrawGrid},
    {"LoadModel", lua_LoadModel},
    {"LoadModelFromMesh", lua_LoadModelFromMesh},
    {"IsModelReady", lua_IsModelReady},
    {"UnloadModel", lua_UnloadModel},
    {"GetModelBoundingBox", lua_GetModelBoundingBox},
    {"DrawModel", lua_DrawModel},
    {"DrawModelEx", lua_DrawModelEx},
    {"DrawModelWires", lua_DrawModelWires},
    {"DrawModelWiresEx", lua_DrawModelWiresEx},
    {"DrawBoundingBox", lua_DrawBoundingBox},
    {"DrawBillboard", lua_DrawBillboard},
    {"DrawBillboardRec", lua_DrawBillboardRec},
    {"DrawBillboardPro", lua_DrawBillboardPro},
    {"UploadMesh", lua_UploadMesh},
    {"UpdateMeshBuffer", lua_UpdateMeshBuffer},
    {"UnloadMesh", lua_UnloadMesh},
    {"DrawMesh", lua_DrawMesh},
    {"DrawMeshInstanced", lua_DrawMeshInstanced},
    {"ExportMesh", lua_ExportMesh},
    {"GetMeshBoundingBox", lua_GetMeshBoundingBox},
    {"GenMeshTangents", lua_GenMeshTangents},
    {"GenMeshPoly", lua_GenMeshPoly},
    {"GenMeshPlane", lua_GenMeshPlane},
    {"GenMeshCube", lua_GenMeshCube},
    {"GenMeshSphere", lua_GenMeshSphere},
    {"GenMeshHemiSphere", lua_GenMeshHemiSphere},
    {"GenMeshCylinder", lua_GenMeshCylinder},
    {"GenMeshCone", lua_GenMeshCone},
    {"GenMeshTorus", lua_GenMeshTorus},
    {"GenMeshKnot", lua_GenMeshKnot},
    {"GenMeshHeightmap", lua_GenMeshHeightmap},
    {"GenMeshCubicmap", lua_GenMeshCubicmap},
    {"LoadMaterials", lua_LoadMaterials},
    {"LoadMaterialDefault", lua_LoadMaterialDefault},
    {"IsMaterialReady", lua_IsMaterialReady},
    {"UnloadMaterial", lua_UnloadMaterial},
    {"SetMaterialTexture", lua_SetMaterialTexture},
    {"SetModelMeshMaterial", lua_SetModelMeshMaterial},
    {"LoadModelAnimations", lua_LoadModelAnimations},
    {"UpdateModelAnimation", lua_UpdateModelAnimation},
    {"UnloadModelAnimation", lua_UnloadModelAnimation},
    {"UnloadModelAnimations", lua_UnloadModelAnimations},
    {"IsModelAnimationValid", lua_IsModelAnimationValid},
    {"CheckCollisionSpheres", lua_CheckCollisionSpheres},
    {"CheckCollisionBoxes", lua_CheckCollisionBoxes},
    {"CheckCollisionBoxSphere", lua_CheckCollisionBoxSphere},
    {"GetRayCollisionSphere", lua_GetRayCollisionSphere},
    {"GetRayCollisionBox", lua_GetRayCollisionBox},
    {"GetRayCollisionMesh", lua_GetRayCollisionMesh},
    {"GetRayCollisionTriangle", lua_GetRayCollisionTriangle},
    {"GetRayCollisionQuad", lua_GetRayCollisionQuad},
    {"InitAudioDevice", lua_InitAudioDevice},
    {"CloseAudioDevice", lua_CloseAudioDevice},
    {"IsAudioDeviceReady", lua_IsAudioDeviceReady},
    {"SetMasterVolume", lua_SetMasterVolume},
    {"GetMasterVolume", lua_GetMasterVolume},
    {"LoadWave", lua_LoadWave},
    {"LoadWaveFromMemory", lua_LoadWaveFromMemory},
    {"IsWaveReady", lua_IsWaveReady},
    {"LoadSound", lua_LoadSound},
    {"LoadSoundFromWave", lua_LoadSoundFromWave},
    {"LoadSoundAlias", lua_LoadSoundAlias},
    {"IsSoundReady", lua_IsSoundReady},
    {"UpdateSound", lua_UpdateSound},
    {"UnloadWave", lua_UnloadWave},
    {"UnloadSound", lua_UnloadSound},
    {"UnloadSoundAlias", lua_UnloadSoundAlias},
    {"ExportWave", lua_ExportWave},
    {"ExportWaveAsCode", lua_ExportWaveAsCode},
    {"PlaySound", lua_PlaySound},
    {"StopSound", lua_StopSound},
    {"PauseSound", lua_PauseSound},
    {"ResumeSound", lua_ResumeSound},
    {"IsSoundPlaying", lua_IsSoundPlaying},
    {"SetSoundVolume", lua_SetSoundVolume},
    {"SetSoundPitch", lua_SetSoundPitch},
    {"SetSoundPan", lua_SetSoundPan},
    {"WaveCopy", lua_WaveCopy},
    {"WaveCrop", lua_WaveCrop},
    {"WaveFormat", lua_WaveFormat},
    {"LoadWaveSamples", lua_LoadWaveSamples},
    {"UnloadWaveSamples", lua_UnloadWaveSamples},
    {"LoadMusicStream", lua_LoadMusicStream},
    {"LoadMusicStreamFromMemory", lua_LoadMusicStreamFromMemory},
    {"IsMusicReady", lua_IsMusicReady},
    {"UnloadMusicStream", lua_UnloadMusicStream},
    {"PlayMusicStream", lua_PlayMusicStream},
    {"IsMusicStreamPlaying", lua_IsMusicStreamPlaying},
    {"UpdateMusicStream", lua_UpdateMusicStream},
    {"StopMusicStream", lua_StopMusicStream},
    {"PauseMusicStream", lua_PauseMusicStream},
    {"ResumeMusicStream", lua_ResumeMusicStream},
    {"SeekMusicStream", lua_SeekMusicStream},
    {"SetMusicVolume", lua_SetMusicVolume},
    {"SetMusicPitch", lua_SetMusicPitch},
    {"SetMusicPan", lua_SetMusicPan},
    {"GetMusicTimeLength", lua_GetMusicTimeLength},
    {"GetMusicTimePlayed", lua_GetMusicTimePlayed},
    {"LoadAudioStream", lua_LoadAudioStream},
    {"IsAudioStreamReady", lua_IsAudioStreamReady},
    {"UnloadAudioStream", lua_UnloadAudioStream},
    {"UpdateAudioStream", lua_UpdateAudioStream},
    {"IsAudioStreamProcessed", lua_IsAudioStreamProcessed},
    {"PlayAudioStream", lua_PlayAudioStream},
    {"PauseAudioStream", lua_PauseAudioStream},
    {"ResumeAudioStream", lua_ResumeAudioStream},
    {"IsAudioStreamPlaying", lua_IsAudioStreamPlaying},
    {"StopAudioStream", lua_StopAudioStream},
    {"SetAudioStreamVolume", lua_SetAudioStreamVolume},
    {"SetAudioStreamPitch", lua_SetAudioStreamPitch},
    {"SetAudioStreamPan", lua_SetAudioStreamPan},
    {"SetAudioStreamBufferSizeDefault", lua_SetAudioStreamBufferSizeDefault},
    {"SetAudioStreamCallback", lua_SetAudioStreamCallback},
    {"AttachAudioStreamProcessor", lua_AttachAudioStreamProcessor},
    {"DetachAudioStreamProcessor", lua_DetachAudioStreamProcessor},
    {"AttachAudioMixedProcessor", lua_AttachAudioMixedProcessor},
    {"DetachAudioMixedProcessor", lua_DetachAudioMixedProcessor},
    {"Clamp", lua_Clamp},
    {"Lerp", lua_Lerp},
    {"Normalize", lua_Normalize},
    {"Remap", lua_Remap},
    {"Wrap", lua_Wrap},
    {"FloatEquals", lua_FloatEquals},
    {"Vector2Zero", lua_Vector2Zero},
    {"Vector2One", lua_Vector2One},
    {"Vector2Add", lua_Vector2Add},
    {"Vector2AddValue", lua_Vector2AddValue},
    {"Vector2Subtract", lua_Vector2Subtract},
    {"Vector2SubtractValue", lua_Vector2SubtractValue},
    {"Vector2Length", lua_Vector2Length},
    {"Vector2LengthSqr", lua_Vector2LengthSqr},
    {"Vector2DotProduct", lua_Vector2DotProduct},
    {"Vector2Distance", lua_Vector2Distance},
    {"Vector2DistanceSqr", lua_Vector2DistanceSqr},
    {"Vector2Angle", lua_Vector2Angle},
    {"Vector2LineAngle", lua_Vector2LineAngle},
    {"Vector2Scale", lua_Vector2Scale},
    {"Vector2Multiply", lua_Vector2Multiply},
    {"Vector2Negate", lua_Vector2Negate},
    {"Vector2Divide", lua_Vector2Divide},
    {"Vector2Normalize", lua_Vector2Normalize},
    {"Vector2Transform", lua_Vector2Transform},
    {"Vector2Lerp", lua_Vector2Lerp},
    {"Vector2Reflect", lua_Vector2Reflect},
    {"Vector2Rotate", lua_Vector2Rotate},
    {"Vector2MoveTowards", lua_Vector2MoveTowards},
    {"Vector2Invert", lua_Vector2Invert},
    {"Vector2Clamp", lua_Vector2Clamp},
    {"Vector2ClampValue", lua_Vector2ClampValue},
    {"Vector2Equals", lua_Vector2Equals},
    {"Vector3Zero", lua_Vector3Zero},
    {"Vector3One", lua_Vector3One},
    {"Vector3Add", lua_Vector3Add},
    {"Vector3AddValue", lua_Vector3AddValue},
    {"Vector3Subtract", lua_Vector3Subtract},
    {"Vector3SubtractValue", lua_Vector3SubtractValue},
    {"Vector3Scale", lua_Vector3Scale},
    {"Vector3Multiply", lua_Vector3Multiply},
    {"Vector3CrossProduct", lua_Vector3CrossProduct},
    {"Vector3Perpendicular", lua_Vector3Perpendicular},
    {"Vector3Length", lua_Vector3Length},
    {"Vector3LengthSqr", lua_Vector3LengthSqr},
    {"Vector3DotProduct", lua_Vector3DotProduct},
    {"Vector3Distance", lua_Vector3Distance},
    {"Vector3DistanceSqr", lua_Vector3DistanceSqr},
    {"Vector3Angle", lua_Vector3Angle},
    {"Vector3Negate", lua_Vector3Negate},
    {"Vector3Divide", lua_Vector3Divide},
    {"Vector3Normalize", lua_Vector3Normalize},
    {"Vector3Project", lua_Vector3Project},
    {"Vector3Reject", lua_Vector3Reject},
    {"Vector3OrthoNormalize", lua_Vector3OrthoNormalize},
    {"Vector3Transform", lua_Vector3Transform},
    {"Vector3RotateByQuaternion", lua_Vector3RotateByQuaternion},
    {"Vector3RotateByAxisAngle", lua_Vector3RotateByAxisAngle},
    {"Vector3Lerp", lua_Vector3Lerp},
    {"Vector3Reflect", lua_Vector3Reflect},
    {"Vector3Min", lua_Vector3Min},
    {"Vector3Max", lua_Vector3Max},
    {"Vector3Barycenter", lua_Vector3Barycenter},
    {"Vector3Unproject", lua_Vector3Unproject},
    {"Vector3ToFloatV", lua_Vector3ToFloatV},
    {"Vector3Invert", lua_Vector3Invert},
    {"Vector3Clamp", lua_Vector3Clamp},
    {"Vector3ClampValue", lua_Vector3ClampValue},
    {"Vector3Equals", lua_Vector3Equals},
    {"Vector3Refract", lua_Vector3Refract},
    {"MatrixDeterminant", lua_MatrixDeterminant},
    {"MatrixTrace", lua_MatrixTrace},
    {"MatrixTranspose", lua_MatrixTranspose},
    {"MatrixInvert", lua_MatrixInvert},
    {"MatrixIdentity", lua_MatrixIdentity},
    {"MatrixAdd", lua_MatrixAdd},
    {"MatrixSubtract", lua_MatrixSubtract},
    {"MatrixMultiply", lua_MatrixMultiply},
    {"MatrixTranslate", lua_MatrixTranslate},
    {"MatrixRotate", lua_MatrixRotate},
    {"MatrixRotateX", lua_MatrixRotateX},
    {"MatrixRotateY", lua_MatrixRotateY},
    {"MatrixRotateZ", lua_MatrixRotateZ},
    {"MatrixRotateXYZ", lua_MatrixRotateXYZ},
    {"MatrixRotateZYX", lua_MatrixRotateZYX},
    {"MatrixScale", lua_MatrixScale},
    {"MatrixFrustum", lua_MatrixFrustum},
    {"MatrixPerspective", lua_MatrixPerspective},
    {"MatrixOrtho", lua_MatrixOrtho},
    {"MatrixLookAt", lua_MatrixLookAt},
    {"MatrixToFloatV", lua_MatrixToFloatV},
    {"QuaternionAdd", lua_QuaternionAdd},
    {"QuaternionAddValue", lua_QuaternionAddValue},
    {"QuaternionSubtract", lua_QuaternionSubtract},
    {"QuaternionSubtractValue", lua_QuaternionSubtractValue},
    {"QuaternionIdentity", lua_QuaternionIdentity},
    {"QuaternionLength", lua_QuaternionLength},
    {"QuaternionNormalize", lua_QuaternionNormalize},
    {"QuaternionInvert", lua_QuaternionInvert},
    {"QuaternionMultiply", lua_QuaternionMultiply},
    {"QuaternionScale", lua_QuaternionScale},
    {"QuaternionDivide", lua_QuaternionDivide},
    {"QuaternionLerp", lua_QuaternionLerp},
    {"QuaternionNlerp", lua_QuaternionNlerp},
    {"QuaternionSlerp", lua_QuaternionSlerp},
    {"QuaternionFromVector3ToVector3", lua_QuaternionFromVector3ToVector3},
    {"QuaternionFromMatrix", lua_QuaternionFromMatrix},
    {"QuaternionToMatrix", lua_QuaternionToMatrix},
    {"QuaternionFromAxisAngle", lua_QuaternionFromAxisAngle},
    {"QuaternionToAxisAngle", lua_QuaternionToAxisAngle},
    {"QuaternionFromEuler", lua_QuaternionFromEuler},
    {"QuaternionToEuler", lua_QuaternionToEuler},
    {"QuaternionTransform", lua_QuaternionTransform},
    {"QuaternionEquals", lua_QuaternionEquals},
    {"rlMatrixMode", lua_rlMatrixMode},
    {"rlPushMatrix", lua_rlPushMatrix},
    {"rlPopMatrix", lua_rlPopMatrix},
    {"rlLoadIdentity", lua_rlLoadIdentity},
    {"rlTranslatef", lua_rlTranslatef},
    {"rlRotatef", lua_rlRotatef},
    {"rlScalef", lua_rlScalef},
    {"rlMultMatrixf", lua_rlMultMatrixf},
    {"rlFrustum", lua_rlFrustum},
    {"rlOrtho", lua_rlOrtho},
    {"rlViewport", lua_rlViewport},
    {"rlBegin", lua_rlBegin},
    {"rlEnd", lua_rlEnd},
    {"rlVertex2i", lua_rlVertex2i},
    {"rlVertex2f", lua_rlVertex2f},
    {"rlVertex3f", lua_rlVertex3f},
    {"rlTexCoord2f", lua_rlTexCoord2f},
    {"rlNormal3f", lua_rlNormal3f},
    {"rlColor4ub", lua_rlColor4ub},
    {"rlColor3f", lua_rlColor3f},
    {"rlColor4f", lua_rlColor4f},
    {"rlEnableVertexArray", lua_rlEnableVertexArray},
    {"rlDisableVertexArray", lua_rlDisableVertexArray},
    {"rlEnableVertexBuffer", lua_rlEnableVertexBuffer},
    {"rlDisableVertexBuffer", lua_rlDisableVertexBuffer},
    {"rlEnableVertexBufferElement", lua_rlEnableVertexBufferElement},
    {"rlDisableVertexBufferElement", lua_rlDisableVertexBufferElement},
    {"rlEnableVertexAttribute", lua_rlEnableVertexAttribute},
    {"rlDisableVertexAttribute", lua_rlDisableVertexAttribute},
    {"rlActiveTextureSlot", lua_rlActiveTextureSlot},
    {"rlEnableTexture", lua_rlEnableTexture},
    {"rlDisableTexture", lua_rlDisableTexture},
    {"rlEnableTextureCubemap", lua_rlEnableTextureCubemap},
    {"rlDisableTextureCubemap", lua_rlDisableTextureCubemap},
    {"rlTextureParameters", lua_rlTextureParameters},
    {"rlCubemapParameters", lua_rlCubemapParameters},
    {"rlEnableShader", lua_rlEnableShader},
    {"rlDisableShader", lua_rlDisableShader},
    {"rlEnableFramebuffer", lua_rlEnableFramebuffer},
    {"rlDisableFramebuffer", lua_rlDisableFramebuffer},
    {"rlActiveDrawBuffers", lua_rlActiveDrawBuffers},
    {"rlBlitFramebuffer", lua_rlBlitFramebuffer},
    {"rlEnableColorBlend", lua_rlEnableColorBlend},
    {"rlDisableColorBlend", lua_rlDisableColorBlend},
    {"rlEnableDepthTest", lua_rlEnableDepthTest},
    {"rlDisableDepthTest", lua_rlDisableDepthTest},
    {"rlEnableDepthMask", lua_rlEnableDepthMask},
    {"rlDisableDepthMask", lua_rlDisableDepthMask},
    {"rlEnableBackfaceCulling", lua_rlEnableBackfaceCulling},
    {"rlDisableBackfaceCulling", lua_rlDisableBackfaceCulling},
    {"rlSetCullFace", lua_rlSetCullFace},
    {"rlEnableScissorTest", lua_rlEnableScissorTest},
    {"rlDisableScissorTest", lua_rlDisableScissorTest},
    {"rlScissor", lua_rlScissor},
    {"rlEnableWireMode", lua_rlEnableWireMode},
    {"rlEnablePointMode", lua_rlEnablePointMode},
    {"rlDisableWireMode", lua_rlDisableWireMode},
    {"rlSetLineWidth", lua_rlSetLineWidth},
    {"rlGetLineWidth", lua_rlGetLineWidth},
    {"rlEnableSmoothLines", lua_rlEnableSmoothLines},
    {"rlDisableSmoothLines", lua_rlDisableSmoothLines},
    {"rlEnableStereoRender", lua_rlEnableStereoRender},
    {"rlDisableStereoRender", lua_rlDisableStereoRender},
    {"rlIsStereoRenderEnabled", lua_rlIsStereoRenderEnabled},
    {"rlClearColor", lua_rlClearColor},
    {"rlClearScreenBuffers", lua_rlClearScreenBuffers},
    {"rlCheckErrors", lua_rlCheckErrors},
    {"rlSetBlendMode", lua_rlSetBlendMode},
    {"rlSetBlendFactors", lua_rlSetBlendFactors},
    {"rlSetBlendFactorsSeparate", lua_rlSetBlendFactorsSeparate},
    {"rlglInit", lua_rlglInit},
    {"rlglClose", lua_rlglClose},
    {"rlLoadExtensions", lua_rlLoadExtensions},
    {"rlGetVersion", lua_rlGetVersion},
    {"rlSetFramebufferWidth", lua_rlSetFramebufferWidth},
    {"rlGetFramebufferWidth", lua_rlGetFramebufferWidth},
    {"rlSetFramebufferHeight", lua_rlSetFramebufferHeight},
    {"rlGetFramebufferHeight", lua_rlGetFramebufferHeight},
    {"rlGetTextureIdDefault", lua_rlGetTextureIdDefault},
    {"rlGetShaderIdDefault", lua_rlGetShaderIdDefault},
    {"rlGetShaderLocsDefault", lua_rlGetShaderLocsDefault},
    {"rlLoadRenderBatch", lua_rlLoadRenderBatch},
    {"rlUnloadRenderBatch", lua_rlUnloadRenderBatch},
    {"rlDrawRenderBatch", lua_rlDrawRenderBatch},
    {"rlSetRenderBatchActive", lua_rlSetRenderBatchActive},
    {"rlDrawRenderBatchActive", lua_rlDrawRenderBatchActive},
    {"rlCheckRenderBatchLimit", lua_rlCheckRenderBatchLimit},
    {"rlSetTexture", lua_rlSetTexture},
    {"rlLoadVertexArray", lua_rlLoadVertexArray},
    {"rlLoadVertexBuffer", lua_rlLoadVertexBuffer},
    {"rlLoadVertexBufferElement", lua_rlLoadVertexBufferElement},
    {"rlUpdateVertexBuffer", lua_rlUpdateVertexBuffer},
    {"rlUpdateVertexBufferElements", lua_rlUpdateVertexBufferElements},
    {"rlUnloadVertexArray", lua_rlUnloadVertexArray},
    {"rlUnloadVertexBuffer", lua_rlUnloadVertexBuffer},
    {"rlSetVertexAttribute", lua_rlSetVertexAttribute},
    {"rlSetVertexAttributeDivisor", lua_rlSetVertexAttributeDivisor},
    {"rlSetVertexAttributeDefault", lua_rlSetVertexAttributeDefault},
    {"rlDrawVertexArray", lua_rlDrawVertexArray},
    {"rlDrawVertexArrayElements", lua_rlDrawVertexArrayElements},
    {"rlDrawVertexArrayInstanced", lua_rlDrawVertexArrayInstanced},
    {"rlDrawVertexArrayElementsInstanced", lua_rlDrawVertexArrayElementsInstanced},
    {"rlLoadTexture", lua_rlLoadTexture},
    {"rlLoadTextureDepth", lua_rlLoadTextureDepth},
    {"rlLoadTextureCubemap", lua_rlLoadTextureCubemap},
    {"rlUpdateTexture", lua_rlUpdateTexture},
    {"rlGetGlTextureFormats", lua_rlGetGlTextureFormats},
    {"rlGetPixelFormatName", lua_rlGetPixelFormatName},
    {"rlUnloadTexture", lua_rlUnloadTexture},
    {"rlGenTextureMipmaps", lua_rlGenTextureMipmaps},
    {"rlReadTexturePixels", lua_rlReadTexturePixels},
    {"rlReadScreenPixels", lua_rlReadScreenPixels},
    {"rlLoadFramebuffer", lua_rlLoadFramebuffer},
    {"rlFramebufferAttach", lua_rlFramebufferAttach},
    {"rlFramebufferComplete", lua_rlFramebufferComplete},
    {"rlUnloadFramebuffer", lua_rlUnloadFramebuffer},
    {"rlLoadShaderCode", lua_rlLoadShaderCode},
    {"rlCompileShader", lua_rlCompileShader},
    {"rlLoadShaderProgram", lua_rlLoadShaderProgram},
    {"rlUnloadShaderProgram", lua_rlUnloadShaderProgram},
    {"rlGetLocationUniform", lua_rlGetLocationUniform},
    {"rlGetLocationAttrib", lua_rlGetLocationAttrib},
    {"rlSetUniform", lua_rlSetUniform},
    {"rlSetUniformMatrix", lua_rlSetUniformMatrix},
    {"rlSetUniformSampler", lua_rlSetUniformSampler},
    {"rlSetShader", lua_rlSetShader},
    {"rlLoadComputeShaderProgram", lua_rlLoadComputeShaderProgram},
    {"rlComputeShaderDispatch", lua_rlComputeShaderDispatch},
    {"rlLoadShaderBuffer", lua_rlLoadShaderBuffer},
    {"rlUnloadShaderBuffer", lua_rlUnloadShaderBuffer},
    {"rlUpdateShaderBuffer", lua_rlUpdateShaderBuffer},
    {"rlBindShaderBuffer", lua_rlBindShaderBuffer},
    {"rlReadShaderBuffer", lua_rlReadShaderBuffer},
    {"rlCopyShaderBuffer", lua_rlCopyShaderBuffer},
    {"rlGetShaderBufferSize", lua_rlGetShaderBufferSize},
    {"rlBindImageTexture", lua_rlBindImageTexture},
    {"rlGetMatrixModelview", lua_rlGetMatrixModelview},
    {"rlGetMatrixProjection", lua_rlGetMatrixProjection},
    {"rlGetMatrixTransform", lua_rlGetMatrixTransform},
    {"rlGetMatrixProjectionStereo", lua_rlGetMatrixProjectionStereo},
    {"rlGetMatrixViewOffsetStereo", lua_rlGetMatrixViewOffsetStereo},
    {"rlSetMatrixProjection", lua_rlSetMatrixProjection},
    {"rlSetMatrixModelview", lua_rlSetMatrixModelview},
    {"rlSetMatrixProjectionStereo", lua_rlSetMatrixProjectionStereo},
    {"rlSetMatrixViewOffsetStereo", lua_rlSetMatrixViewOffsetStereo},
    {"rlLoadDrawCube", lua_rlLoadDrawCube},
    {"rlLoadDrawQuad", lua_rlLoadDrawQuad},
    {NULL, NULL}
  };
  lua_pushinteger(L, 5);
  lua_setglobal(L, "RAYLIB_VERSION_MAJOR");
  lua_pushinteger(L, 0);
  lua_setglobal(L, "RAYLIB_VERSION_MINOR");
  lua_pushinteger(L, 0);
  lua_setglobal(L, "RAYLIB_VERSION_PATCH");
  lua_pushstring(L, "5.0");
  lua_setglobal(L, "RAYLIB_VERSION");
  lua_pushnumber(L, 3.1415926535898);
  lua_setglobal(L, "PI");
  lua_pushColor(L, &(Color){200, 200, 200, 255});
  lua_setglobal(L, "LIGHTGRAY");
  lua_pushColor(L, &(Color){130, 130, 130, 255});
  lua_setglobal(L, "GRAY");
  lua_pushColor(L, &(Color){80, 80, 80, 255});
  lua_setglobal(L, "DARKGRAY");
  lua_pushColor(L, &(Color){253, 249, 0, 255});
  lua_setglobal(L, "YELLOW");
  lua_pushColor(L, &(Color){255, 203, 0, 255});
  lua_setglobal(L, "GOLD");
  lua_pushColor(L, &(Color){255, 161, 0, 255});
  lua_setglobal(L, "ORANGE");
  lua_pushColor(L, &(Color){255, 109, 194, 255});
  lua_setglobal(L, "PINK");
  lua_pushColor(L, &(Color){230, 41, 55, 255});
  lua_setglobal(L, "RED");
  lua_pushColor(L, &(Color){190, 33, 55, 255});
  lua_setglobal(L, "MAROON");
  lua_pushColor(L, &(Color){0, 228, 48, 255});
  lua_setglobal(L, "GREEN");
  lua_pushColor(L, &(Color){0, 158, 47, 255});
  lua_setglobal(L, "LIME");
  lua_pushColor(L, &(Color){0, 117, 44, 255});
  lua_setglobal(L, "DARKGREEN");
  lua_pushColor(L, &(Color){102, 191, 255, 255});
  lua_setglobal(L, "SKYBLUE");
  lua_pushColor(L, &(Color){0, 121, 241, 255});
  lua_setglobal(L, "BLUE");
  lua_pushColor(L, &(Color){0, 82, 172, 255});
  lua_setglobal(L, "DARKBLUE");
  lua_pushColor(L, &(Color){200, 122, 255, 255});
  lua_setglobal(L, "PURPLE");
  lua_pushColor(L, &(Color){135, 60, 190, 255});
  lua_setglobal(L, "VIOLET");
  lua_pushColor(L, &(Color){112, 31, 126, 255});
  lua_setglobal(L, "DARKPURPLE");
  lua_pushColor(L, &(Color){211, 176, 131, 255});
  lua_setglobal(L, "BEIGE");
  lua_pushColor(L, &(Color){127, 106, 79, 255});
  lua_setglobal(L, "BROWN");
  lua_pushColor(L, &(Color){76, 63, 47, 255});
  lua_setglobal(L, "DARKBROWN");
  lua_pushColor(L, &(Color){255, 255, 255, 255});
  lua_setglobal(L, "WHITE");
  lua_pushColor(L, &(Color){0, 0, 0, 255});
  lua_setglobal(L, "BLACK");
  lua_pushColor(L, &(Color){0, 0, 0, 0});
  lua_setglobal(L, "BLANK");
  lua_pushColor(L, &(Color){255, 0, 255, 255});
  lua_setglobal(L, "MAGENTA");
  lua_pushColor(L, &(Color){245, 245, 245, 255});
  lua_setglobal(L, "RAYWHITE");
  LuaStartEnum(L);
  LuaSetEnum(L, "FLAG_VSYNC_HINT", 64);
  LuaSetEnum(L, "FLAG_FULLSCREEN_MODE", 2);
  LuaSetEnum(L, "FLAG_WINDOW_RESIZABLE", 4);
  LuaSetEnum(L, "FLAG_WINDOW_UNDECORATED", 8);
  LuaSetEnum(L, "FLAG_WINDOW_HIDDEN", 128);
  LuaSetEnum(L, "FLAG_WINDOW_MINIMIZED", 512);
  LuaSetEnum(L, "FLAG_WINDOW_MAXIMIZED", 1024);
  LuaSetEnum(L, "FLAG_WINDOW_UNFOCUSED", 2048);
  LuaSetEnum(L, "FLAG_WINDOW_TOPMOST", 4096);
  LuaSetEnum(L, "FLAG_WINDOW_ALWAYS_RUN", 256);
  LuaSetEnum(L, "FLAG_WINDOW_TRANSPARENT", 16);
  LuaSetEnum(L, "FLAG_WINDOW_HIGHDPI", 8192);
  LuaSetEnum(L, "FLAG_WINDOW_MOUSE_PASSTHROUGH", 16384);
  LuaSetEnum(L, "FLAG_BORDERLESS_WINDOWED_MODE", 32768);
  LuaSetEnum(L, "FLAG_MSAA_4X_HINT", 32);
  LuaSetEnum(L, "FLAG_INTERLACED_HINT", 65536);
  LuaEndEnum(L, "ConfigFlags");
  LuaStartEnum(L);
  LuaSetEnum(L, "LOG_ALL", 0);
  LuaSetEnum(L, "LOG_TRACE", 1);
  LuaSetEnum(L, "LOG_DEBUG", 2);
  LuaSetEnum(L, "LOG_INFO", 3);
  LuaSetEnum(L, "LOG_WARNING", 4);
  LuaSetEnum(L, "LOG_ERROR", 5);
  LuaSetEnum(L, "LOG_FATAL", 6);
  LuaSetEnum(L, "LOG_NONE", 7);
  LuaEndEnum(L, "TraceLogLevel");
  LuaStartEnum(L);
  LuaSetEnum(L, "KEY_NULL", 0);
  LuaSetEnum(L, "KEY_APOSTROPHE", 39);
  LuaSetEnum(L, "KEY_COMMA", 44);
  LuaSetEnum(L, "KEY_MINUS", 45);
  LuaSetEnum(L, "KEY_PERIOD", 46);
  LuaSetEnum(L, "KEY_SLASH", 47);
  LuaSetEnum(L, "KEY_ZERO", 48);
  LuaSetEnum(L, "KEY_ONE", 49);
  LuaSetEnum(L, "KEY_TWO", 50);
  LuaSetEnum(L, "KEY_THREE", 51);
  LuaSetEnum(L, "KEY_FOUR", 52);
  LuaSetEnum(L, "KEY_FIVE", 53);
  LuaSetEnum(L, "KEY_SIX", 54);
  LuaSetEnum(L, "KEY_SEVEN", 55);
  LuaSetEnum(L, "KEY_EIGHT", 56);
  LuaSetEnum(L, "KEY_NINE", 57);
  LuaSetEnum(L, "KEY_SEMICOLON", 59);
  LuaSetEnum(L, "KEY_EQUAL", 61);
  LuaSetEnum(L, "KEY_A", 65);
  LuaSetEnum(L, "KEY_B", 66);
  LuaSetEnum(L, "KEY_C", 67);
  LuaSetEnum(L, "KEY_D", 68);
  LuaSetEnum(L, "KEY_E", 69);
  LuaSetEnum(L, "KEY_F", 70);
  LuaSetEnum(L, "KEY_G", 71);
  LuaSetEnum(L, "KEY_H", 72);
  LuaSetEnum(L, "KEY_I", 73);
  LuaSetEnum(L, "KEY_J", 74);
  LuaSetEnum(L, "KEY_K", 75);
  LuaSetEnum(L, "KEY_L", 76);
  LuaSetEnum(L, "KEY_M", 77);
  LuaSetEnum(L, "KEY_N", 78);
  LuaSetEnum(L, "KEY_O", 79);
  LuaSetEnum(L, "KEY_P", 80);
  LuaSetEnum(L, "KEY_Q", 81);
  LuaSetEnum(L, "KEY_R", 82);
  LuaSetEnum(L, "KEY_S", 83);
  LuaSetEnum(L, "KEY_T", 84);
  LuaSetEnum(L, "KEY_U", 85);
  LuaSetEnum(L, "KEY_V", 86);
  LuaSetEnum(L, "KEY_W", 87);
  LuaSetEnum(L, "KEY_X", 88);
  LuaSetEnum(L, "KEY_Y", 89);
  LuaSetEnum(L, "KEY_Z", 90);
  LuaSetEnum(L, "KEY_LEFT_BRACKET", 91);
  LuaSetEnum(L, "KEY_BACKSLASH", 92);
  LuaSetEnum(L, "KEY_RIGHT_BRACKET", 93);
  LuaSetEnum(L, "KEY_GRAVE", 96);
  LuaSetEnum(L, "KEY_SPACE", 32);
  LuaSetEnum(L, "KEY_ESCAPE", 256);
  LuaSetEnum(L, "KEY_ENTER", 257);
  LuaSetEnum(L, "KEY_TAB", 258);
  LuaSetEnum(L, "KEY_BACKSPACE", 259);
  LuaSetEnum(L, "KEY_INSERT", 260);
  LuaSetEnum(L, "KEY_DELETE", 261);
  LuaSetEnum(L, "KEY_RIGHT", 262);
  LuaSetEnum(L, "KEY_LEFT", 263);
  LuaSetEnum(L, "KEY_DOWN", 264);
  LuaSetEnum(L, "KEY_UP", 265);
  LuaSetEnum(L, "KEY_PAGE_UP", 266);
  LuaSetEnum(L, "KEY_PAGE_DOWN", 267);
  LuaSetEnum(L, "KEY_HOME", 268);
  LuaSetEnum(L, "KEY_END", 269);
  LuaSetEnum(L, "KEY_CAPS_LOCK", 280);
  LuaSetEnum(L, "KEY_SCROLL_LOCK", 281);
  LuaSetEnum(L, "KEY_NUM_LOCK", 282);
  LuaSetEnum(L, "KEY_PRINT_SCREEN", 283);
  LuaSetEnum(L, "KEY_PAUSE", 284);
  LuaSetEnum(L, "KEY_F1", 290);
  LuaSetEnum(L, "KEY_F2", 291);
  LuaSetEnum(L, "KEY_F3", 292);
  LuaSetEnum(L, "KEY_F4", 293);
  LuaSetEnum(L, "KEY_F5", 294);
  LuaSetEnum(L, "KEY_F6", 295);
  LuaSetEnum(L, "KEY_F7", 296);
  LuaSetEnum(L, "KEY_F8", 297);
  LuaSetEnum(L, "KEY_F9", 298);
  LuaSetEnum(L, "KEY_F10", 299);
  LuaSetEnum(L, "KEY_F11", 300);
  LuaSetEnum(L, "KEY_F12", 301);
  LuaSetEnum(L, "KEY_LEFT_SHIFT", 340);
  LuaSetEnum(L, "KEY_LEFT_CONTROL", 341);
  LuaSetEnum(L, "KEY_LEFT_ALT", 342);
  LuaSetEnum(L, "KEY_LEFT_SUPER", 343);
  LuaSetEnum(L, "KEY_RIGHT_SHIFT", 344);
  LuaSetEnum(L, "KEY_RIGHT_CONTROL", 345);
  LuaSetEnum(L, "KEY_RIGHT_ALT", 346);
  LuaSetEnum(L, "KEY_RIGHT_SUPER", 347);
  LuaSetEnum(L, "KEY_KB_MENU", 348);
  LuaSetEnum(L, "KEY_KP_0", 320);
  LuaSetEnum(L, "KEY_KP_1", 321);
  LuaSetEnum(L, "KEY_KP_2", 322);
  LuaSetEnum(L, "KEY_KP_3", 323);
  LuaSetEnum(L, "KEY_KP_4", 324);
  LuaSetEnum(L, "KEY_KP_5", 325);
  LuaSetEnum(L, "KEY_KP_6", 326);
  LuaSetEnum(L, "KEY_KP_7", 327);
  LuaSetEnum(L, "KEY_KP_8", 328);
  LuaSetEnum(L, "KEY_KP_9", 329);
  LuaSetEnum(L, "KEY_KP_DECIMAL", 330);
  LuaSetEnum(L, "KEY_KP_DIVIDE", 331);
  LuaSetEnum(L, "KEY_KP_MULTIPLY", 332);
  LuaSetEnum(L, "KEY_KP_SUBTRACT", 333);
  LuaSetEnum(L, "KEY_KP_ADD", 334);
  LuaSetEnum(L, "KEY_KP_ENTER", 335);
  LuaSetEnum(L, "KEY_KP_EQUAL", 336);
  LuaSetEnum(L, "KEY_BACK", 4);
  LuaSetEnum(L, "KEY_MENU", 82);
  LuaSetEnum(L, "KEY_VOLUME_UP", 24);
  LuaSetEnum(L, "KEY_VOLUME_DOWN", 25);
  LuaEndEnum(L, "KeyboardKey");
  LuaStartEnum(L);
  LuaSetEnum(L, "MOUSE_BUTTON_LEFT", 0);
  LuaSetEnum(L, "MOUSE_BUTTON_RIGHT", 1);
  LuaSetEnum(L, "MOUSE_BUTTON_MIDDLE", 2);
  LuaSetEnum(L, "MOUSE_BUTTON_SIDE", 3);
  LuaSetEnum(L, "MOUSE_BUTTON_EXTRA", 4);
  LuaSetEnum(L, "MOUSE_BUTTON_FORWARD", 5);
  LuaSetEnum(L, "MOUSE_BUTTON_BACK", 6);
  LuaEndEnum(L, "MouseButton");
  LuaStartEnum(L);
  LuaSetEnum(L, "MOUSE_CURSOR_DEFAULT", 0);
  LuaSetEnum(L, "MOUSE_CURSOR_ARROW", 1);
  LuaSetEnum(L, "MOUSE_CURSOR_IBEAM", 2);
  LuaSetEnum(L, "MOUSE_CURSOR_CROSSHAIR", 3);
  LuaSetEnum(L, "MOUSE_CURSOR_POINTING_HAND", 4);
  LuaSetEnum(L, "MOUSE_CURSOR_RESIZE_EW", 5);
  LuaSetEnum(L, "MOUSE_CURSOR_RESIZE_NS", 6);
  LuaSetEnum(L, "MOUSE_CURSOR_RESIZE_NWSE", 7);
  LuaSetEnum(L, "MOUSE_CURSOR_RESIZE_NESW", 8);
  LuaSetEnum(L, "MOUSE_CURSOR_RESIZE_ALL", 9);
  LuaSetEnum(L, "MOUSE_CURSOR_NOT_ALLOWED", 10);
  LuaEndEnum(L, "MouseCursor");
  LuaStartEnum(L);
  LuaSetEnum(L, "GAMEPAD_BUTTON_UNKNOWN", 0);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_FACE_UP", 1);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_FACE_RIGHT", 2);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_FACE_DOWN", 3);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_FACE_LEFT", 4);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_FACE_UP", 5);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_FACE_RIGHT", 6);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_FACE_DOWN", 7);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_FACE_LEFT", 8);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_TRIGGER_1", 9);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_TRIGGER_2", 10);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_TRIGGER_1", 11);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_TRIGGER_2", 12);
  LuaSetEnum(L, "GAMEPAD_BUTTON_MIDDLE_LEFT", 13);
  LuaSetEnum(L, "GAMEPAD_BUTTON_MIDDLE", 14);
  LuaSetEnum(L, "GAMEPAD_BUTTON_MIDDLE_RIGHT", 15);
  LuaSetEnum(L, "GAMEPAD_BUTTON_LEFT_THUMB", 16);
  LuaSetEnum(L, "GAMEPAD_BUTTON_RIGHT_THUMB", 17);
  LuaEndEnum(L, "GamepadButton");
  LuaStartEnum(L);
  LuaSetEnum(L, "GAMEPAD_AXIS_LEFT_X", 0);
  LuaSetEnum(L, "GAMEPAD_AXIS_LEFT_Y", 1);
  LuaSetEnum(L, "GAMEPAD_AXIS_RIGHT_X", 2);
  LuaSetEnum(L, "GAMEPAD_AXIS_RIGHT_Y", 3);
  LuaSetEnum(L, "GAMEPAD_AXIS_LEFT_TRIGGER", 4);
  LuaSetEnum(L, "GAMEPAD_AXIS_RIGHT_TRIGGER", 5);
  LuaEndEnum(L, "GamepadAxis");
  LuaStartEnum(L);
  LuaSetEnum(L, "MATERIAL_MAP_ALBEDO", 0);
  LuaSetEnum(L, "MATERIAL_MAP_METALNESS", 1);
  LuaSetEnum(L, "MATERIAL_MAP_NORMAL", 2);
  LuaSetEnum(L, "MATERIAL_MAP_ROUGHNESS", 3);
  LuaSetEnum(L, "MATERIAL_MAP_OCCLUSION", 4);
  LuaSetEnum(L, "MATERIAL_MAP_EMISSION", 5);
  LuaSetEnum(L, "MATERIAL_MAP_HEIGHT", 6);
  LuaSetEnum(L, "MATERIAL_MAP_CUBEMAP", 7);
  LuaSetEnum(L, "MATERIAL_MAP_IRRADIANCE", 8);
  LuaSetEnum(L, "MATERIAL_MAP_PREFILTER", 9);
  LuaSetEnum(L, "MATERIAL_MAP_BRDF", 10);
  LuaEndEnum(L, "MaterialMapIndex");
  LuaStartEnum(L);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_POSITION", 0);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_TEXCOORD01", 1);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_TEXCOORD02", 2);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_NORMAL", 3);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_TANGENT", 4);
  LuaSetEnum(L, "SHADER_LOC_VERTEX_COLOR", 5);
  LuaSetEnum(L, "SHADER_LOC_MATRIX_MVP", 6);
  LuaSetEnum(L, "SHADER_LOC_MATRIX_VIEW", 7);
  LuaSetEnum(L, "SHADER_LOC_MATRIX_PROJECTION", 8);
  LuaSetEnum(L, "SHADER_LOC_MATRIX_MODEL", 9);
  LuaSetEnum(L, "SHADER_LOC_MATRIX_NORMAL", 10);
  LuaSetEnum(L, "SHADER_LOC_VECTOR_VIEW", 11);
  LuaSetEnum(L, "SHADER_LOC_COLOR_DIFFUSE", 12);
  LuaSetEnum(L, "SHADER_LOC_COLOR_SPECULAR", 13);
  LuaSetEnum(L, "SHADER_LOC_COLOR_AMBIENT", 14);
  LuaSetEnum(L, "SHADER_LOC_MAP_ALBEDO", 15);
  LuaSetEnum(L, "SHADER_LOC_MAP_METALNESS", 16);
  LuaSetEnum(L, "SHADER_LOC_MAP_NORMAL", 17);
  LuaSetEnum(L, "SHADER_LOC_MAP_ROUGHNESS", 18);
  LuaSetEnum(L, "SHADER_LOC_MAP_OCCLUSION", 19);
  LuaSetEnum(L, "SHADER_LOC_MAP_EMISSION", 20);
  LuaSetEnum(L, "SHADER_LOC_MAP_HEIGHT", 21);
  LuaSetEnum(L, "SHADER_LOC_MAP_CUBEMAP", 22);
  LuaSetEnum(L, "SHADER_LOC_MAP_IRRADIANCE", 23);
  LuaSetEnum(L, "SHADER_LOC_MAP_PREFILTER", 24);
  LuaSetEnum(L, "SHADER_LOC_MAP_BRDF", 25);
  LuaEndEnum(L, "ShaderLocationIndex");
  LuaStartEnum(L);
  LuaSetEnum(L, "SHADER_UNIFORM_FLOAT", 0);
  LuaSetEnum(L, "SHADER_UNIFORM_VEC2", 1);
  LuaSetEnum(L, "SHADER_UNIFORM_VEC3", 2);
  LuaSetEnum(L, "SHADER_UNIFORM_VEC4", 3);
  LuaSetEnum(L, "SHADER_UNIFORM_INT", 4);
  LuaSetEnum(L, "SHADER_UNIFORM_IVEC2", 5);
  LuaSetEnum(L, "SHADER_UNIFORM_IVEC3", 6);
  LuaSetEnum(L, "SHADER_UNIFORM_IVEC4", 7);
  LuaSetEnum(L, "SHADER_UNIFORM_SAMPLER2D", 8);
  LuaEndEnum(L, "ShaderUniformDataType");
  LuaStartEnum(L);
  LuaSetEnum(L, "SHADER_ATTRIB_FLOAT", 0);
  LuaSetEnum(L, "SHADER_ATTRIB_VEC2", 1);
  LuaSetEnum(L, "SHADER_ATTRIB_VEC3", 2);
  LuaSetEnum(L, "SHADER_ATTRIB_VEC4", 3);
  LuaEndEnum(L, "ShaderAttributeDataType");
  LuaStartEnum(L);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_GRAYSCALE", 1);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA", 2);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R5G6B5", 3);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R8G8B8", 4);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R5G5B5A1", 5);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R4G4B4A4", 6);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R8G8B8A8", 7);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R32", 8);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R32G32B32", 9);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R32G32B32A32", 10);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R16", 11);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R16G16B16", 12);
  LuaSetEnum(L, "PIXELFORMAT_UNCOMPRESSED_R16G16B16A16", 13);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_DXT1_RGB", 14);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_DXT1_RGBA", 15);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_DXT3_RGBA", 16);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_DXT5_RGBA", 17);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_ETC1_RGB", 18);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_ETC2_RGB", 19);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA", 20);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_PVRT_RGB", 21);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_PVRT_RGBA", 22);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA", 23);
  LuaSetEnum(L, "PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA", 24);
  LuaEndEnum(L, "PixelFormat");
  LuaStartEnum(L);
  LuaSetEnum(L, "TEXTURE_FILTER_POINT", 0);
  LuaSetEnum(L, "TEXTURE_FILTER_BILINEAR", 1);
  LuaSetEnum(L, "TEXTURE_FILTER_TRILINEAR", 2);
  LuaSetEnum(L, "TEXTURE_FILTER_ANISOTROPIC_4X", 3);
  LuaSetEnum(L, "TEXTURE_FILTER_ANISOTROPIC_8X", 4);
  LuaSetEnum(L, "TEXTURE_FILTER_ANISOTROPIC_16X", 5);
  LuaEndEnum(L, "TextureFilter");
  LuaStartEnum(L);
  LuaSetEnum(L, "TEXTURE_WRAP_REPEAT", 0);
  LuaSetEnum(L, "TEXTURE_WRAP_CLAMP", 1);
  LuaSetEnum(L, "TEXTURE_WRAP_MIRROR_REPEAT", 2);
  LuaSetEnum(L, "TEXTURE_WRAP_MIRROR_CLAMP", 3);
  LuaEndEnum(L, "TextureWrap");
  LuaStartEnum(L);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_AUTO_DETECT", 0);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_LINE_VERTICAL", 1);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_LINE_HORIZONTAL", 2);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR", 3);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE", 4);
  LuaSetEnum(L, "CUBEMAP_LAYOUT_PANORAMA", 5);
  LuaEndEnum(L, "CubemapLayout");
  LuaStartEnum(L);
  LuaSetEnum(L, "FONT_DEFAULT", 0);
  LuaSetEnum(L, "FONT_BITMAP", 1);
  LuaSetEnum(L, "FONT_SDF", 2);
  LuaEndEnum(L, "FontType");
  LuaStartEnum(L);
  LuaSetEnum(L, "BLEND_ALPHA", 0);
  LuaSetEnum(L, "BLEND_ADDITIVE", 1);
  LuaSetEnum(L, "BLEND_MULTIPLIED", 2);
  LuaSetEnum(L, "BLEND_ADD_COLORS", 3);
  LuaSetEnum(L, "BLEND_SUBTRACT_COLORS", 4);
  LuaSetEnum(L, "BLEND_ALPHA_PREMULTIPLY", 5);
  LuaSetEnum(L, "BLEND_CUSTOM", 6);
  LuaSetEnum(L, "BLEND_CUSTOM_SEPARATE", 7);
  LuaEndEnum(L, "BlendMode");
  LuaStartEnum(L);
  LuaSetEnum(L, "GESTURE_NONE", 0);
  LuaSetEnum(L, "GESTURE_TAP", 1);
  LuaSetEnum(L, "GESTURE_DOUBLETAP", 2);
  LuaSetEnum(L, "GESTURE_HOLD", 4);
  LuaSetEnum(L, "GESTURE_DRAG", 8);
  LuaSetEnum(L, "GESTURE_SWIPE_RIGHT", 16);
  LuaSetEnum(L, "GESTURE_SWIPE_LEFT", 32);
  LuaSetEnum(L, "GESTURE_SWIPE_UP", 64);
  LuaSetEnum(L, "GESTURE_SWIPE_DOWN", 128);
  LuaSetEnum(L, "GESTURE_PINCH_IN", 256);
  LuaSetEnum(L, "GESTURE_PINCH_OUT", 512);
  LuaEndEnum(L, "Gesture");
  LuaStartEnum(L);
  LuaSetEnum(L, "CAMERA_CUSTOM", 0);
  LuaSetEnum(L, "CAMERA_FREE", 1);
  LuaSetEnum(L, "CAMERA_ORBITAL", 2);
  LuaSetEnum(L, "CAMERA_FIRST_PERSON", 3);
  LuaSetEnum(L, "CAMERA_THIRD_PERSON", 4);
  LuaEndEnum(L, "CameraMode");
  LuaStartEnum(L);
  LuaSetEnum(L, "CAMERA_PERSPECTIVE", 0);
  LuaSetEnum(L, "CAMERA_ORTHOGRAPHIC", 1);
  LuaEndEnum(L, "CameraProjection");
  LuaStartEnum(L);
  LuaSetEnum(L, "NPATCH_NINE_PATCH", 0);
  LuaSetEnum(L, "NPATCH_THREE_PATCH_VERTICAL", 1);
  LuaSetEnum(L, "NPATCH_THREE_PATCH_HORIZONTAL", 2);
  LuaEndEnum(L, "NPatchLayout");
  lua_pushnumber(L, 3.1415926535898);
  lua_setglobal(L, "PI");
  lua_pushnumber(L, 1e-06);
  lua_setglobal(L, "EPSILON");
  lua_pushstring(L, "4.5");
  lua_setglobal(L, "RLGL_VERSION");
  lua_pushinteger(L, 8192);
  lua_setglobal(L, "RL_DEFAULT_BATCH_BUFFER_ELEMENTS");
  lua_pushinteger(L, 1);
  lua_setglobal(L, "RL_DEFAULT_BATCH_BUFFERS");
  lua_pushinteger(L, 256);
  lua_setglobal(L, "RL_DEFAULT_BATCH_DRAWCALLS");
  lua_pushinteger(L, 4);
  lua_setglobal(L, "RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS");
  lua_pushinteger(L, 32);
  lua_setglobal(L, "RL_MAX_MATRIX_STACK_SIZE");
  lua_pushinteger(L, 32);
  lua_setglobal(L, "RL_MAX_SHADER_LOCATIONS");
  lua_pushinteger(L, 10242);
  lua_setglobal(L, "RL_TEXTURE_WRAP_S");
  lua_pushinteger(L, 10243);
  lua_setglobal(L, "RL_TEXTURE_WRAP_T");
  lua_pushinteger(L, 10240);
  lua_setglobal(L, "RL_TEXTURE_MAG_FILTER");
  lua_pushinteger(L, 10241);
  lua_setglobal(L, "RL_TEXTURE_MIN_FILTER");
  lua_pushinteger(L, 9728);
  lua_setglobal(L, "RL_TEXTURE_FILTER_NEAREST");
  lua_pushinteger(L, 9729);
  lua_setglobal(L, "RL_TEXTURE_FILTER_LINEAR");
  lua_pushinteger(L, 9984);
  lua_setglobal(L, "RL_TEXTURE_FILTER_MIP_NEAREST");
  lua_pushinteger(L, 9986);
  lua_setglobal(L, "RL_TEXTURE_FILTER_NEAREST_MIP_LINEAR");
  lua_pushinteger(L, 9985);
  lua_setglobal(L, "RL_TEXTURE_FILTER_LINEAR_MIP_NEAREST");
  lua_pushinteger(L, 9987);
  lua_setglobal(L, "RL_TEXTURE_FILTER_MIP_LINEAR");
  lua_pushinteger(L, 12288);
  lua_setglobal(L, "RL_TEXTURE_FILTER_ANISOTROPIC");
  lua_pushinteger(L, 16384);
  lua_setglobal(L, "RL_TEXTURE_MIPMAP_BIAS_RATIO");
  lua_pushinteger(L, 10497);
  lua_setglobal(L, "RL_TEXTURE_WRAP_REPEAT");
  lua_pushinteger(L, 33071);
  lua_setglobal(L, "RL_TEXTURE_WRAP_CLAMP");
  lua_pushinteger(L, 33648);
  lua_setglobal(L, "RL_TEXTURE_WRAP_MIRROR_REPEAT");
  lua_pushinteger(L, 34626);
  lua_setglobal(L, "RL_TEXTURE_WRAP_MIRROR_CLAMP");
  lua_pushinteger(L, 5888);
  lua_setglobal(L, "RL_MODELVIEW");
  lua_pushinteger(L, 5889);
  lua_setglobal(L, "RL_PROJECTION");
  lua_pushinteger(L, 5890);
  lua_setglobal(L, "RL_TEXTURE");
  lua_pushinteger(L, 1);
  lua_setglobal(L, "RL_LINES");
  lua_pushinteger(L, 4);
  lua_setglobal(L, "RL_TRIANGLES");
  lua_pushinteger(L, 7);
  lua_setglobal(L, "RL_QUADS");
  lua_pushinteger(L, 5121);
  lua_setglobal(L, "RL_UNSIGNED_BYTE");
  lua_pushinteger(L, 5126);
  lua_setglobal(L, "RL_FLOAT");
  lua_pushinteger(L, 35040);
  lua_setglobal(L, "RL_STREAM_DRAW");
  lua_pushinteger(L, 35041);
  lua_setglobal(L, "RL_STREAM_READ");
  lua_pushinteger(L, 35042);
  lua_setglobal(L, "RL_STREAM_COPY");
  lua_pushinteger(L, 35044);
  lua_setglobal(L, "RL_STATIC_DRAW");
  lua_pushinteger(L, 35045);
  lua_setglobal(L, "RL_STATIC_READ");
  lua_pushinteger(L, 35046);
  lua_setglobal(L, "RL_STATIC_COPY");
  lua_pushinteger(L, 35048);
  lua_setglobal(L, "RL_DYNAMIC_DRAW");
  lua_pushinteger(L, 35049);
  lua_setglobal(L, "RL_DYNAMIC_READ");
  lua_pushinteger(L, 35050);
  lua_setglobal(L, "RL_DYNAMIC_COPY");
  lua_pushinteger(L, 35632);
  lua_setglobal(L, "RL_FRAGMENT_SHADER");
  lua_pushinteger(L, 35633);
  lua_setglobal(L, "RL_VERTEX_SHADER");
  lua_pushinteger(L, 37305);
  lua_setglobal(L, "RL_COMPUTE_SHADER");
  lua_pushinteger(L, 0);
  lua_setglobal(L, "RL_ZERO");
  lua_pushinteger(L, 1);
  lua_setglobal(L, "RL_ONE");
  lua_pushinteger(L, 768);
  lua_setglobal(L, "RL_SRC_COLOR");
  lua_pushinteger(L, 769);
  lua_setglobal(L, "RL_ONE_MINUS_SRC_COLOR");
  lua_pushinteger(L, 770);
  lua_setglobal(L, "RL_SRC_ALPHA");
  lua_pushinteger(L, 771);
  lua_setglobal(L, "RL_ONE_MINUS_SRC_ALPHA");
  lua_pushinteger(L, 772);
  lua_setglobal(L, "RL_DST_ALPHA");
  lua_pushinteger(L, 773);
  lua_setglobal(L, "RL_ONE_MINUS_DST_ALPHA");
  lua_pushinteger(L, 774);
  lua_setglobal(L, "RL_DST_COLOR");
  lua_pushinteger(L, 775);
  lua_setglobal(L, "RL_ONE_MINUS_DST_COLOR");
  lua_pushinteger(L, 776);
  lua_setglobal(L, "RL_SRC_ALPHA_SATURATE");
  lua_pushinteger(L, 32769);
  lua_setglobal(L, "RL_CONSTANT_COLOR");
  lua_pushinteger(L, 32770);
  lua_setglobal(L, "RL_ONE_MINUS_CONSTANT_COLOR");
  lua_pushinteger(L, 32771);
  lua_setglobal(L, "RL_CONSTANT_ALPHA");
  lua_pushinteger(L, 32772);
  lua_setglobal(L, "RL_ONE_MINUS_CONSTANT_ALPHA");
  lua_pushinteger(L, 32774);
  lua_setglobal(L, "RL_FUNC_ADD");
  lua_pushinteger(L, 32775);
  lua_setglobal(L, "RL_MIN");
  lua_pushinteger(L, 32776);
  lua_setglobal(L, "RL_MAX");
  lua_pushinteger(L, 32778);
  lua_setglobal(L, "RL_FUNC_SUBTRACT");
  lua_pushinteger(L, 32779);
  lua_setglobal(L, "RL_FUNC_REVERSE_SUBTRACT");
  lua_pushinteger(L, 32777);
  lua_setglobal(L, "RL_BLEND_EQUATION");
  lua_pushinteger(L, 32777);
  lua_setglobal(L, "RL_BLEND_EQUATION_RGB");
  lua_pushinteger(L, 34877);
  lua_setglobal(L, "RL_BLEND_EQUATION_ALPHA");
  lua_pushinteger(L, 32968);
  lua_setglobal(L, "RL_BLEND_DST_RGB");
  lua_pushinteger(L, 32969);
  lua_setglobal(L, "RL_BLEND_SRC_RGB");
  lua_pushinteger(L, 32970);
  lua_setglobal(L, "RL_BLEND_DST_ALPHA");
  lua_pushinteger(L, 32971);
  lua_setglobal(L, "RL_BLEND_SRC_ALPHA");
  lua_pushinteger(L, 32773);
  lua_setglobal(L, "RL_BLEND_COLOR");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_OPENGL_11", 1);
  LuaSetEnum(L, "RL_OPENGL_21", 2);
  LuaSetEnum(L, "RL_OPENGL_33", 3);
  LuaSetEnum(L, "RL_OPENGL_43", 4);
  LuaSetEnum(L, "RL_OPENGL_ES_20", 5);
  LuaSetEnum(L, "RL_OPENGL_ES_30", 6);
  LuaEndEnum(L, "rlGlVersion");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_LOG_ALL", 0);
  LuaSetEnum(L, "RL_LOG_TRACE", 1);
  LuaSetEnum(L, "RL_LOG_DEBUG", 2);
  LuaSetEnum(L, "RL_LOG_INFO", 3);
  LuaSetEnum(L, "RL_LOG_WARNING", 4);
  LuaSetEnum(L, "RL_LOG_ERROR", 5);
  LuaSetEnum(L, "RL_LOG_FATAL", 6);
  LuaSetEnum(L, "RL_LOG_NONE", 7);
  LuaEndEnum(L, "rlTraceLogLevel");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE", 1);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA", 2);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5", 3);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8", 4);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1", 5);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4", 6);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8", 7);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R32", 8);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32", 9);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32", 10);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R16", 11);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16", 12);
  LuaSetEnum(L, "RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16", 13);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_DXT1_RGB", 14);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_DXT1_RGBA", 15);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_DXT3_RGBA", 16);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_DXT5_RGBA", 17);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_ETC1_RGB", 18);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_ETC2_RGB", 19);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA", 20);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_PVRT_RGB", 21);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_PVRT_RGBA", 22);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA", 23);
  LuaSetEnum(L, "RL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA", 24);
  LuaEndEnum(L, "rlPixelFormat");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_POINT", 0);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_BILINEAR", 1);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_TRILINEAR", 2);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_ANISOTROPIC_4X", 3);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_ANISOTROPIC_8X", 4);
  LuaSetEnum(L, "RL_TEXTURE_FILTER_ANISOTROPIC_16X", 5);
  LuaEndEnum(L, "rlTextureFilter");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_BLEND_ALPHA", 0);
  LuaSetEnum(L, "RL_BLEND_ADDITIVE", 1);
  LuaSetEnum(L, "RL_BLEND_MULTIPLIED", 2);
  LuaSetEnum(L, "RL_BLEND_ADD_COLORS", 3);
  LuaSetEnum(L, "RL_BLEND_SUBTRACT_COLORS", 4);
  LuaSetEnum(L, "RL_BLEND_ALPHA_PREMULTIPLY", 5);
  LuaSetEnum(L, "RL_BLEND_CUSTOM", 6);
  LuaSetEnum(L, "RL_BLEND_CUSTOM_SEPARATE", 7);
  LuaEndEnum(L, "rlBlendMode");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_POSITION", 0);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_TEXCOORD01", 1);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_TEXCOORD02", 2);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_NORMAL", 3);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_TANGENT", 4);
  LuaSetEnum(L, "RL_SHADER_LOC_VERTEX_COLOR", 5);
  LuaSetEnum(L, "RL_SHADER_LOC_MATRIX_MVP", 6);
  LuaSetEnum(L, "RL_SHADER_LOC_MATRIX_VIEW", 7);
  LuaSetEnum(L, "RL_SHADER_LOC_MATRIX_PROJECTION", 8);
  LuaSetEnum(L, "RL_SHADER_LOC_MATRIX_MODEL", 9);
  LuaSetEnum(L, "RL_SHADER_LOC_MATRIX_NORMAL", 10);
  LuaSetEnum(L, "RL_SHADER_LOC_VECTOR_VIEW", 11);
  LuaSetEnum(L, "RL_SHADER_LOC_COLOR_DIFFUSE", 12);
  LuaSetEnum(L, "RL_SHADER_LOC_COLOR_SPECULAR", 13);
  LuaSetEnum(L, "RL_SHADER_LOC_COLOR_AMBIENT", 14);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_ALBEDO", 15);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_METALNESS", 16);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_NORMAL", 17);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_ROUGHNESS", 18);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_OCCLUSION", 19);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_EMISSION", 20);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_HEIGHT", 21);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_CUBEMAP", 22);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_IRRADIANCE", 23);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_PREFILTER", 24);
  LuaSetEnum(L, "RL_SHADER_LOC_MAP_BRDF", 25);
  LuaEndEnum(L, "rlShaderLocationIndex");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_FLOAT", 0);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_VEC2", 1);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_VEC3", 2);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_VEC4", 3);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_INT", 4);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_IVEC2", 5);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_IVEC3", 6);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_IVEC4", 7);
  LuaSetEnum(L, "RL_SHADER_UNIFORM_SAMPLER2D", 8);
  LuaEndEnum(L, "rlShaderUniformDataType");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_SHADER_ATTRIB_FLOAT", 0);
  LuaSetEnum(L, "RL_SHADER_ATTRIB_VEC2", 1);
  LuaSetEnum(L, "RL_SHADER_ATTRIB_VEC3", 2);
  LuaSetEnum(L, "RL_SHADER_ATTRIB_VEC4", 3);
  LuaEndEnum(L, "rlShaderAttributeDataType");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL0", 0);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL1", 1);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL2", 2);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL3", 3);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL4", 4);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL5", 5);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL6", 6);
  LuaSetEnum(L, "RL_ATTACHMENT_COLOR_CHANNEL7", 7);
  LuaSetEnum(L, "RL_ATTACHMENT_DEPTH", 100);
  LuaSetEnum(L, "RL_ATTACHMENT_STENCIL", 200);
  LuaEndEnum(L, "rlFramebufferAttachType");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_POSITIVE_X", 0);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_NEGATIVE_X", 1);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_POSITIVE_Y", 2);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_NEGATIVE_Y", 3);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_POSITIVE_Z", 4);
  LuaSetEnum(L, "RL_ATTACHMENT_CUBEMAP_NEGATIVE_Z", 5);
  LuaSetEnum(L, "RL_ATTACHMENT_TEXTURE2D", 100);
  LuaSetEnum(L, "RL_ATTACHMENT_RENDERBUFFER", 200);
  LuaEndEnum(L, "rlFramebufferAttachTextureType");
  LuaStartEnum(L);
  LuaSetEnum(L, "RL_CULL_FACE_FRONT", 0);
  LuaSetEnum(L, "RL_CULL_FACE_BACK", 1);
  LuaEndEnum(L, "rlCullMode");
  luaL_newlib(L, funcs);
  lua_setglobal(L, "rl");
}


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