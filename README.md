# Yache

Yet Another Chip-8 Emulator

This is currently a work in progress, with basic input and no sound support at the moment.

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
### Options

| Option        | Functionality                      |
|---------------|------------------------------------|
| -d            | Runs the game with the debugger     |
| -f [frequency]| Sets CPU frequency                  |
| -s            | Enables the shift quirk             |
| -l            | Enables the load store quirk        |

### Input

Exit the program with the ESC key.

The keymap is currently hard-coded with following values:

| CHIP Keyboard | Keymap |
|:-------------:|:------:|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| A | 4 |
| 4 | Q |
| 5 | W |
| 6 | E |
| B | R |
| 7 | A |
| 8 | S |
| 9 | D |
| C | F |
| D | Z |
| 0 | X |
| E | C |
| F | V |

## Debugger commands

| Command| Functionality
|--------|------------------------------------|
| `print`| Prints stack pointer, program counter and registers |
| `step` | Executes the next instruction      |
| `step n`| Executes the next `n` instruction |
| `continue`| The program continues executing without asking for the debug prompt|
| `exit` | Exits the emulator                 |