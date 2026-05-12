# TerminalAttack

TerminalAttack CLI-based game written in C using the `ncurses` library. 

I wanted to get more used to writting C so I made this.

## Controls

Navigate your ship using either the WASD keys or the Arrow keys:

| Action | Key(s) |
| :--- | :--- |
| **Move Up** | `W` / `↑` |
| **Move Down** | `S` / `↓` |
| **Move Left** | `A` / `←` |
| **Move Right** | `D` / `→` |
| **Shoot** | `Space` |

## Requirements

To build and run TerminalAttack, you need:
- A C compiler (e.g., `gcc`)
- The `ncurses` development library (e.g., `libncurses5-dev` on Ubuntu/Debian)

## Installation & Running

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd terminalAttack
   ```

2. **Build the game:**
   Use the provided `makefile`:
   ```bash
   make
   ```

3. **Run the game:**
   ```bash
   ./terminalattack
   ```
