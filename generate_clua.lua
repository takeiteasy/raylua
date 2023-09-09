
local unknownType = {}
function addUnknownType(typeName, funcName)
  unknownType[typeName] = unknownType[typeName] or {}
  table.insert(unknownType[typeName], funcName)
end

local raylibApi = require("raylib_api")
print("function count ", #raylibApi.functions)

local f,e = io.open("./rayliblua.h", "w")
if not f then
  print("file open error", e)
  return
end
io.output(f)
io.write("#ifndef RAYLIB_LUA_API\n#define RAYLIB_LUA_API\n")
io.write("#include \"lua.h\"\n")
io.write("#include \"lauxlib.h\"\n")
io.write("#include \"lualib.h\"\n\n")
io.write("#include \"raylib.h\"\n\n")

local typeEncoder = {
  ["int"] = {get = "luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)"},
  ["int *"] = {get = "(int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["const int *"] = {get = "(const int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["unsigned int"] = {get = "(unsigned int)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)"},
  ["unsigned int *"] = {get = "(unsigned int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["void *"] = {get = "lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, %s)"},
  ["const void *"] = {get = "lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, %s)"},
  ["long"] = {get = "(long)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)"},
  ["char"] = {get = "(char)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)"},
  ["char *"] = {get = "(char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)"},
  ["char **"] = {get = "(char **)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["const char *"] = {get = "luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)"},
  ["const char **"] = {get = "(const char **)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["unsigned char"] = {get = "(unsigned char)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)"},
  ["unsigned char *"] = {get = "(unsigned char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, (const char *)%s)"},
  ["const unsigned char *"] = {get = "(const unsigned char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)"},
  ["unsigned short *"] = {get = "(unsigned short *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["float"] = {get = "(float)luaL_checknumber(L, %d)", set = "lua_pushnumber(L, %s)"},
  ["float *"] = {get = "(float *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"},
  ["double"] = {get = "(double)luaL_checknumber(L, %d)", set = "lua_pushnumber(L, %s)"},
  ["bool"] = {get = "lua_toboolean(L, %d)", set = "lua_pushboolean(L, %s)"},
}

local structEncoder = {}
for i = 1, #raylibApi.structs do
  local structDesc = raylibApi.structs[i]
  structEncoder[structDesc.name] = {get = "lua_check" .. structDesc.name .. "(L, %d)", set = "lua_push" .. structDesc.name .. "(L, %s)"}
end
for i = 1, #raylibApi.aliases do
  local aliasDesc = raylibApi.aliases[i]
  structEncoder[aliasDesc.name] = structEncoder[aliasDesc.type]
end

local pointEncoder = {}
for name, _ in pairs(structEncoder) do
  local pointName = name .. " *"
  local ppName = name .. " **"
  local constPointName = "const " .. name .. " *"
  if string.match(name, "%*") then
    pointName = name .. "*"
    constPointName = "const " .. name .. "*"
    ppName = name .. "**"
  end
  pointEncoder[pointName] = {get = "(" .. pointName .. ")lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"};
  pointEncoder[constPointName] = {get = "(" .. constPointName .. ")lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"};
  pointEncoder[ppName] = {get = "(" .. ppName .. ")lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)"};
end

for i = 1, #raylibApi.structs do
  local structDesc = raylibApi.structs[i]
  local unknowParam = false
  for fieldi = 1, #structDesc.fields do
    local fieldDesc = structDesc.fields[fieldi]
    if not typeEncoder[fieldDesc.type] and not structEncoder[fieldDesc.type] and not pointEncoder[fieldDesc.type] then
      io.write("// unknow field type " .. fieldDesc.type .. "\n")
      unknowParam = true
    end
  end

  if unknowParam then
    io.write("void lua_push" .. structDesc.name .. "(lua_State *L, ".. structDesc.name .. "* val) {}\n")
    io.write(structDesc.name .. " lua_check" .. structDesc.name .. "(lua_State* L, int index) {" .. structDesc.name .. " ret; return ret;}\n\n")
  else
    io.write("void lua_push" .. structDesc.name .. "(lua_State *L, ".. structDesc.name .. "* val) {\n")
    io.write("  lua_newtable(L);\n")
    for fieldi = 1, #structDesc.fields do
      local fieldDesc = structDesc.fields[fieldi]
      if typeEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  " .. string.format(typeEncoder[fieldDesc.type].set, "val->" ..fieldDesc.name) .. ";\n")
        io.write("  lua_settable(L, -3);\n")
      elseif structEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  " .. string.format(structEncoder[fieldDesc.type].set, "&(val->" .. fieldDesc.name .. ")") .. ";\n")
        io.write("  lua_settable(L, -3);\n")
      elseif pointEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  " .. string.format(pointEncoder[fieldDesc.type].set, "val->" .. fieldDesc.name) .. ";\n")
        io.write("  lua_settable(L, -3);\n")
      else
        io.write("// unknown field type " .. fieldDesc.type .. ";\n")
        addUnknownType("sf " .. fieldDesc.type, structDesc.name)
      end
    end
    io.write("}\n")

    io.write(structDesc.name .. " lua_check" .. structDesc.name .. "(lua_State* L, int index) {\n")
    io.write("  " .. structDesc.name .. " ret;\n")
    for fieldi = 1, #structDesc.fields do
      local fieldDesc = structDesc.fields[fieldi]
      if typeEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  lua_gettable(L, index);\n")
        io.write("  ret." .. fieldDesc.name .. " = " .. string.format(typeEncoder[fieldDesc.type].get, -1) .. ";\n")
      elseif structEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  lua_gettable(L, index);\n")
        io.write("  ret." .. fieldDesc.name .. " = " .. string.format(structEncoder[fieldDesc.type].get, -1) .. ";\n")
      elseif pointEncoder[fieldDesc.type] then
        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
        io.write("  lua_gettable(L, index);\n")
        io.write("  ret." .. fieldDesc.name .. " = " .. string.format(pointEncoder[fieldDesc.type].get, -1) .. ";\n")
      else
        io.write("// unknown field type " .. fieldDesc.type .. ";\n")
        addUnknownType("sf " .. fieldDesc.type, structDesc.name);
      end
    end
    io.write("  return ret;\n")
    io.write("}\n\n")
  end
end

local funcLib = {}
for i = 1, #raylibApi.functions do
  local funcDesc = raylibApi.functions[i]
  local unknowParam = false
  for parami = 1, funcDesc.params and #funcDesc.params or 0 do
    local paramDesc = funcDesc.params[parami]
    if not typeEncoder[paramDesc.type] and not structEncoder[paramDesc.type] and not pointEncoder[paramDesc.type] then
      io.write("// unknow param type " .. paramDesc.type .. "\n")
      unknowParam = true
    end
  end

  table.insert(funcLib, funcDesc.name)
  if unknowParam then
    io.write("int lua_"..funcDesc.name.."(lua_State *L) {return 0;}\n\n")
  else
    io.write("int lua_"..funcDesc.name.."(lua_State *L) {\n")
    -- get param
    if funcDesc.params then
      for parami = 1, #funcDesc.params do
        local paramDesc = funcDesc.params[parami]
        if typeEncoder[paramDesc.type] then
          io.write("  " .. paramDesc.type .. " " .. paramDesc.name .. " = " .. string.format(typeEncoder[paramDesc.type].get, parami) .. ";\n")
        elseif structEncoder[paramDesc.type] then
          io.write("  " .. paramDesc.type .. " " .. paramDesc.name .. " = " .. string.format(structEncoder[paramDesc.type].get, parami) .. ";\n")
        elseif pointEncoder[paramDesc.type] then
          io.write("  " .. paramDesc.type .. " " .. paramDesc.name .. " = " .. string.format(pointEncoder[paramDesc.type].get, parami) .. ";\n")
        elseif paramDesc.type == "..." then
          io.write("  const char *" .. paramDesc.name .. " = luaL_checkstring(L, " .. parami .. ");\n")
        else
          io.write("// unknown param type " .. paramDesc.type .. ";\n");
          addUnknownType("p " .. paramDesc.type, funcDesc.name)
        end
      end
    end

    -- call c func
    if funcDesc.returnType == "void" then
      io.write("  ")
    else
      io.write("  " .. funcDesc.returnType .. " ret = ")
    end

    io.write(funcDesc.name .. "(")
    -- set func param
    if funcDesc.params then
      for parami = 1, #funcDesc.params do
        local paramDesc = funcDesc.params[parami]
        io.write(paramDesc.name)
        if parami ~= #funcDesc.params then
          io.write(", ")
        end
      end
    end
    io.write(");\n")

    if funcDesc.returnType == "void" then
      io.write("  return 0;\n")
    else
      if typeEncoder[funcDesc.returnType] then
        io.write("  " .. string.format(typeEncoder[funcDesc.returnType].set, "ret") .. ";\n")
      elseif structEncoder[funcDesc.returnType] then
        io.write("  " .. string.format(structEncoder[funcDesc.returnType].set, "&ret") .. ";\n")
      elseif pointEncoder[funcDesc.returnType] then
        io.write("  " .. string.format(pointEncoder[funcDesc.returnType].set, "ret") .. ";\n")
      else
        io.write("// unknown return type " .. funcDesc.returnType .. ";\n")
        addUnknownType("r " .. funcDesc.returnType, funcDesc.name)
      end
      io.write("  return 1;\n")
    end


    io.write("}\n")
  end
end

io.write("void initRLLib(lua_State *L) {\n")
io.write("  luaL_Reg funcs[] = {\n")
for i = 1, #funcLib do
  io.write("    {\"" .. funcLib[i] .. "\", lua_" .. funcLib[i] .. "},\n")
end
io.write("  };\n")
io.write("  luaL_newlib(L, funcs);\n")
io.write("  lua_setglobal(L, \"rl\");\n")
io.write("}\n")

io.write("#endif")

io.flush(f)
io.close(f)
