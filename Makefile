all:
	/usr/lib/emscripten/emcc --std=c++11 --bind -s EXTRA_EXPORTED_RUNTIME_METHODS='["Pointer_stringify"]' -s WASM=0 foo.cpp bindings.cpp -o foo.js