-- This is free and unencumbered software released into the public domain.

-- Anyone is free to copy, modify, publish, use, compile, sell, or
-- distribute this software, either in source code form or as a compiled
-- binary, for any purpose, commercial or non-commercial, and by any
-- means.

-- In jurisdictions that recognize copyright laws, the author or authors
-- of this software dedicate any and all copyright interest in the
-- software to the public domain. We make this dedication for the benefit
-- of the public at large and to the detriment of our heirs and
-- successors. We intend this dedication to be an overt act of
-- relinquishment in perpetuity of all present and future rights to this
-- software under copyright law.

-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
-- EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
-- MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
-- IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
-- OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
-- ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
-- OTHER DEALINGS IN THE SOFTWARE.

-- For more information, please refer to <https://unlicense.org>

local typeEncoder = {
    ["int"] = { get = "luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)" },
    ["int *"] = { get = "(int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["const int *"] = { get = "(const int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["unsigned int"] = { get = "(unsigned int)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)" },
    ["unsigned int *"] = { get = "(unsigned int *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["void *"] = { get = "lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, %s)" },
    ["const void *"] = { get = "lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, %s)" },
    ["long"] = { get = "(long)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)" },
    ["char"] = { get = "(char)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)" },
    ["char *"] = { get = "(char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)" },
    ["char **"] = { get = "(char **)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["const char *"] = { get = "luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)" },
    ["const char **"] = { get = "(const char **)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["unsigned char"] = { get = "(unsigned char)luaL_checkinteger(L, %d)", set = "lua_pushinteger(L, %s)" },
    ["unsigned char *"] = { get = "(unsigned char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, (const char *)%s)" },
    ["const unsigned char *"] = { get = "(const unsigned char *)luaL_checkstring(L, %d)", set = "lua_pushstring(L, %s)" },
    ["unsigned short *"] = { get = "(unsigned short *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["float"] = { get = "(float)luaL_checknumber(L, %d)", set = "lua_pushnumber(L, %s)" },
    ["float *"] = { get = "(float *)lua_touserdata(L, %d)", set = "lua_pushlightuserdata(L, (void *)%s)" },
    ["double"] = { get = "(double)luaL_checknumber(L, %d)", set = "lua_pushnumber(L, %s)" },
    ["bool"] = { get = "lua_toboolean(L, %d)", set = "lua_pushboolean(L, %s)" },
}

local unknownType = {}
local addUnknownType = function(typeName, funcName)
    unknownType[typeName] = unknownType[typeName] or {}
    table.insert(unknownType[typeName], funcName)
end
local structEncoder = {}
local funcLib = {}
local pointEncoder = {}

local defined_functions = {}
function isFuncDefined(str)
    return defined_functions[str] ~= nil
end
function addDefinedFunc(str)
  if not defined_functions[str] then
    defined_functions[str] = true
    table.insert(defined_functions, str)
  end
end

function generateStructDefinitions(api)
    for i = 1, #api.structs do
        local structDesc = api.structs[i]
        structEncoder[structDesc.name] = {
            get = "lua_check" .. structDesc.name .. "(L, %d)",
            set = "lua_push" ..
                structDesc.name .. "(L, %s)"
        }
    end
    for i = 1, #api.aliases do
        local aliasDesc = api.aliases[i]
        structEncoder[aliasDesc.name] = structEncoder[aliasDesc.type]
    end

    for name, _ in pairs(structEncoder) do
        local pointName = name .. " *"
        local ppName = name .. " **"
        local constPointName = "const " .. name .. " *"
        if string.match(name, "%*") then
            pointName = name .. "*"
            constPointName = "const " .. name .. "*"
            ppName = name .. "**"
        end
        pointEncoder[pointName] = {
            get = "(" .. pointName .. ")lua_touserdata(L, %d)",
            set =
            "lua_pushlightuserdata(L, (void *)%s)"
        };
        pointEncoder[constPointName] = {
            get = "(" .. constPointName .. ")lua_touserdata(L, %d)",
            set =
            "lua_pushlightuserdata(L, (void *)%s)"
        };
        pointEncoder[ppName] = {
            get = "(" .. ppName .. ")lua_touserdata(L, %d)",
            set =
            "lua_pushlightuserdata(L, (void *)%s)"
        };
    end

    for i = 1, #api.structs do
        local structDesc = api.structs[i]
        local unknowParam = false
        for fieldi = 1, #structDesc.fields do
            local fieldDesc = structDesc.fields[fieldi]
            if not typeEncoder[fieldDesc.type] and not structEncoder[fieldDesc.type] and not pointEncoder[fieldDesc.type] then
                io.write("// unknow field type " .. fieldDesc.type .. "\n")
                unknowParam = true
            end
        end

        if unknowParam then
            io.write("static void lua_push" .. structDesc.name .. "(lua_State *L, " .. structDesc.name .. "* val) {}\n")
            io.write("static " .. structDesc.name ..
                " lua_check" ..
                structDesc.name .. "(lua_State* L, int index) {" .. structDesc.name .. " ret; return ret;}\n\n")
        else
            if not isFuncDefined("lua_push" .. structDesc.name) then
                io.write("static void lua_push" ..
                structDesc.name .. "(lua_State *L, " .. structDesc.name .. "* val) {\n")
                io.write("  lua_newtable(L);\n")
                for fieldi = 1, #structDesc.fields do
                    local fieldDesc = structDesc.fields[fieldi]
                    if typeEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  " ..
                        string.format(typeEncoder[fieldDesc.type].set, "val->" .. fieldDesc.name) .. ";\n")
                        io.write("  lua_settable(L, -3);\n")
                    elseif structEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  " ..
                            string.format(structEncoder[fieldDesc.type].set, "&(val->" .. fieldDesc.name .. ")") .. ";\n")
                        io.write("  lua_settable(L, -3);\n")
                    elseif pointEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  " ..
                        string.format(pointEncoder[fieldDesc.type].set, "val->" .. fieldDesc.name) .. ";\n")
                        io.write("  lua_settable(L, -3);\n")
                    else
                        io.write("// unknown field type " .. fieldDesc.type .. ";\n")
                        addUnknownType("sf " .. fieldDesc.type, structDesc.name)
                    end
                end
                io.write("}\n")
                addDefinedFunc("lua_push" .. structDesc.name)
            end

            if not isFuncDefined("lua_check" .. structDesc.name) then
                io.write("static " ..
                structDesc.name .. " lua_check" .. structDesc.name .. "(lua_State* L, int index) {\n")
                io.write("  " .. structDesc.name .. " ret;\n")
                for fieldi = 1, #structDesc.fields do
                    local fieldDesc = structDesc.fields[fieldi]
                    if typeEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  lua_gettable(L, index);\n")
                        io.write("  ret." ..
                            fieldDesc.name .. " = " .. string.format(typeEncoder[fieldDesc.type].get, -1) .. ";\n")
                    elseif structEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  lua_gettable(L, index);\n")
                        io.write("  ret." ..
                            fieldDesc.name .. " = " .. string.format(structEncoder[fieldDesc.type].get, -1) .. ";\n")
                    elseif pointEncoder[fieldDesc.type] then
                        io.write("  lua_pushstring(L, \"" .. fieldDesc.name .. "\");\n")
                        io.write("  lua_gettable(L, index);\n")
                        io.write("  ret." ..
                            fieldDesc.name .. " = " .. string.format(pointEncoder[fieldDesc.type].get, -1) .. ";\n")
                    else
                        io.write("// unknown field type " .. fieldDesc.type .. ";\n")
                        addUnknownType("sf " .. fieldDesc.type, structDesc.name);
                    end
                end
                io.write("  return ret;\n")
                io.write("}\n")
                addDefinedFunc("lua_check" .. structDesc.name)
            end
            
            io.write("\n")
        end
    end
end

function generateFunctionDefinitions(api)
    for i = 1, #api.functions do
        local funcDesc = api.functions[i]
        local unknowParam = false
        for parami = 1, funcDesc.params and #funcDesc.params or 0 do
            local paramDesc = funcDesc.params[parami]
            if not typeEncoder[paramDesc.type] and not structEncoder[paramDesc.type] and not pointEncoder[paramDesc.type] then
                io.write("// unknow param type " .. paramDesc.type .. "\n")
                unknowParam = true
            end
        end

        -- I can't get these to work so fuck them
        if (funcDesc.name == "rlEnableStatePointer" or
            funcDesc.name == "rlDisableStatePointer") then
            goto skip
        end

        table.insert(funcLib, funcDesc.name)
        if unknowParam then
            io.write("static int lua_" .. funcDesc.name .. "(lua_State *L) {return 0;}\n\n")
        else
            io.write("static int lua_" .. funcDesc.name .. "(lua_State *L) {\n")
            -- get param
            if funcDesc.params then
                for parami = 1, #funcDesc.params do
                    local paramDesc = funcDesc.params[parami]
                    if typeEncoder[paramDesc.type] then
                        io.write("  " ..
                            paramDesc.type ..
                            " " ..
                            paramDesc.name .. " = " .. string.format(typeEncoder[paramDesc.type].get, parami) .. ";\n")
                    elseif structEncoder[paramDesc.type] then
                        io.write("  " ..
                            paramDesc.type ..
                            " " ..
                            paramDesc.name .. " = " .. string.format(structEncoder[paramDesc.type].get, parami) .. ";\n")
                    elseif pointEncoder[paramDesc.type] then
                        io.write("  " ..
                            paramDesc.type ..
                            " " ..
                            paramDesc.name .. " = " .. string.format(pointEncoder[paramDesc.type].get, parami) .. ";\n")
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
        
        ::skip::
    end
end

function parseColorString(str)
    local numbers = {}
    for num in str:gmatch("%d+") do
        numbers[#numbers + 1] = tonumber(num)
    end
    return table.unpack(numbers)
end

function generateEnumDefinitions(api)
    for k, v in pairs(api.defines) do
        if v['type'] == "FLOAT" then
            io.write("  lua_pushnumber(L, " .. v['value'] .. ");\n")
        elseif v['type'] == "INT" then
            io.write("  lua_pushinteger(L, " .. v['value'] .. ");\n")
        elseif v['type'] == "STRING" then
            io.write("  lua_pushstring(L, \"" .. v['value'] .. "\");\n")
        elseif v['type'] == "COLOR" then
            local r, g, b, a = parseColorString(v['value'])
            io.write(string.format("  lua_pushColor(L, &(Color){%d, %d, %d, %d});\n", r, g, b, a))
        else
            goto skip
        end
        io.write("  lua_setglobal(L, \"" .. v['name'] .. "\");\n")
        ::skip::
    end

    if #api.enums then
        for k, v in pairs(api.enums) do
            local name = v['name']
            io.write("  LuaStartEnum(L);\n")
            for kk, vv in pairs(v.values) do
                io.write(string.format("  LuaSetEnum(L, \"%s\", %s);\n", vv['name'], vv['value']))
            end
            io.write(string.format("  LuaEndEnum(L, \"%s\");\n", name))
        end
    end
end

local raylibApi = require 'raylib-api.raylib'
local raymathApi = require 'raylib-api.raymath'
local rlglApi = require 'raylib-api.rlgl'

local all = { raylibApi, raymathApi, rlglApi }

local f = nil
if #arg > 0 then
    local e = nil
    f, e = io.open(arg[1], "w")
    if not f then
        io.stderr:write(string.format("failed to open '%s' - %s", arg[1], e))
        return
    else
        io.output(f)
    end
end

function splitTemplate(path)
    local file = io.open(path, "r")
    if not file then
        return nil, "Could not open file"
    end
    local content = file:read("*all")
    file:close()
    local before, after = content:match("(.-)\n%$[^\n]*\n(.*)")
    if not before or not after then
        return nil, "No line starting with $ found"
    end
    return before, after
end

local header, footer = splitTemplate("raylua.template.c")

io.write(header)

for _, v in ipairs(all) do
    generateStructDefinitions(v)
    generateFunctionDefinitions(v)
end

io.write("static void initialize_raylua(lua_State *L) {\n")
io.write("  luaL_Reg funcs[] = {\n")
for i = 1, #funcLib do
    io.write("    {\"" .. funcLib[i] .. "\", lua_" .. funcLib[i] .. "},\n")
end
io.write("    {NULL, NULL}\n")
io.write("  };\n")
for _, v in ipairs(all) do
    generateEnumDefinitions(v)
end
io.write("  luaL_newlib(L, funcs);\n")
io.write("  lua_setglobal(L, \"rl\");\n")
io.write("}\n\n")

io.write(footer)

io.flush()
if #arg > 0 then
    io.close(f)
end
