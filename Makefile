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

bindings: builddir
	lua ray.lua $(BUILD_DST)/raylua.c

raylib_library: builddir
	cd raylib/src && $(MAKE) PLATFORM=PLATFORM_DESKTOP RAYLIB_LIBTYPE=SHARED

raylib: raylib_library
	mv raylib/src/*.dylib $(BUILD_DST)

minilua: builddir
	echo $(BUILD_DST)
	$(CC) -shared -fpic minilua.c -o $(BUILD_DST)/libminilua.dylib

raylua_library: bindings raylib minilua
	$(CC) -shared -fpic \
          -Ideps \
          -Iminilua \
		  -Iraylib/src \
          $(BUILD_DST)/raylua.c \
          -DGRAPHICS_API_OPENGL_33 \
          -L$(BUILD_DST) -lminilua -lraylib -o $(BUILD_DST)/libraylua.dylib

raylua: bindings raylua_library
	$(CC) -rpath $(BUILD_DST) \
		  -Iraylib/src \
		  -Iminilua \
		  $(BUILD_DST)/raylua.c \
          -DRAYLUA_RUNNER -DGRAPHICS_API_OPENGL_33 \
          -L$(BUILD_DST) -lminilua -lraylib \
          -o $(BUILD_DST)/raylua

test: raylua
	./build/raylua test.lua

all: raylua  

clean:
	rm -rf $(BUILD_DST)
