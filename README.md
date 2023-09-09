Another raylib lua bingind. Generate depend on raylib parser.

## QuickStart
use [[raylib-parser][https://github.com/raysan5/raylib/tree/master/parser]] generate api file. 
`raylib_parser -f LUA -o {project_path}/raylib_api.lua`

generate lua api
`lua generate_clua.lua`

build & run example
`clang main.c -llua -lraylib -o raylua`
`./raylua`
