# Yache

Yet Another Chip-8 Emulator

This is currently a work in progress, no input and sound support at the moment.

Built with OpenGL.

## Build

Yache uses CMake, so it should quite simple to build.
The only dependencies are OpenGL and GLFW.

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

```bash
./yache [options...] [file]
```

For example using the PONG rom in the roms folder

```
./yache ../roms/PONG
```

Exit the program with the ESC key.

### Options

| Option | Functionality                      |
|--------|------------------------------------|
| -d     | Running the game with the debugger |

## Debugger commands

| Command| Functionality
|--------|------------------------------------|
| `print`| Prints stack pointer, program counter and registers |
| `step` | Executes the next instruction      |
| `step n`| Executes the next `n` instruction |
| `continue`| The program continues executing without asking for the debug prompt|
| `exit` | Exits the emulator                 |