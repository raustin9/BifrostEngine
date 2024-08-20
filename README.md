# Bifrost Game Engine
The Bifrost Engine is a cross-platform game engine library alongside an editor that can be used to create 2D and 3D games.

## Building
On Linux/Mac use the included `go.sh` script and run `./go.sh Build-All` in order to compile/recompile the project.

On Windows use the included `go.ps` script and run `.\go.ps Build-All`.

Alternatively, you can use the actual build commands:
```bash
cmake -B build .
cmake --build build
```

## Clangd LSP
Use the `compile_commands.json` file that is output from CMake in the `build` directory in order to get proper 
completion and usage from the clangd language server.
