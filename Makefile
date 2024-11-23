# This is free and unencumbered software released into the public domain.

# Anyone is free to copy, modify, publish, use, compile, sell, or
# distribute this software, either in source code form or as a compiled
# binary, for any purpose, commercial or non-commercial, and by any
# means.

# In jurisdictions that recognize copyright laws, the author or authors
# of this software dedicate any and all copyright interest in the
# software to the public domain. We make this dedication for the benefit
# of the public at large and to the detriment of our heirs and
# successors. We intend this dedication to be an overt act of
# relinquishment in perpetuity of all present and future rights to this
# software under copyright law.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

# For more information, please refer to <https://unlicense.org>

BUILD_DST=build

default: all

builddir:
	mkdir -p $(BUILD_DST)

minilua:
	wget -O https://raw.githubusercontent.com/edubart/minilua/refs/heads/main/minilua.h

raylib-api:
	git clone https://github.com/RobLoach/raylib-api.git

bindings: builddir raylib-api
	lua ray.lua $(BUILD_DST)/raylua.c

raylib:
	git clone https://github.com/raysan5/raylib.git; cd raylib; git checkout 5.5

raylib_library: builddir raylib
	cd raylib/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

raylib_build: raylib_library
	mv raylib/src/*.dylib $(BUILD_DST)

raylua_library: bindings raylib_build minilua
	$(CC) -shared -fpic $(BUILD_DST)/raylua.c minilua.c -I. -DGRAPHICS_API_OPENGL_33 -Iraylib/src -L$(BUILD_DST) -lraylib -o $(BUILD_DST)/libraylua.dylib

raylua: bindings raylua_library
	$(CC) -rpath $(BUILD_DST) $(BUILD_DST)/raylua.c -I. -DRAYLUA_RUNNER -DGRAPHICS_API_OPENGL_33 -Iraylib/src -L$(BUILD_DST) -lraylib -o $(BUILD_DST)/raylua

test: raylua
	./build/raylua test.lua

all: raylua

clean:
	rm -rf $(BUILD_DST)
