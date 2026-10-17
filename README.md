# Computer Graphics

Coursework for the Computer Graphics lecture (WS 2025/26, AIN5).
Each exercise lives in its own folder.

## Progress

| Übung | Topic | Status |
| ----- | ----- | ------ |
| 1 | Bresenham (lines, circle) | Done |
| 2 | – | Not started |
| 3 | – | Not started |
| 4 | – | Not started |

## Übung 1 – Bresenham

A small C++ / OpenGL (freeglut) program that draws into a low-resolution
60×60 pixel texture, which is scaled up to the window. It contains:

- `BresenhamLine` and `BresenhamLine2` – line drawing
- `BresenhamCircle` – circle drawing
- `Point.h` / `Color.h` – small helper classes for coordinates and colors

### Requirements

- Windows with Visual Studio (MSVC, toolset v140 or newer)
- freeglut – `freeglut.h`, `freeglut.lib` and `freeglut.dll` are included in `Übung1/`

### Build and run

1. Open `Übung1/Uebung1.sln` in Visual Studio.
2. Build and run (`F5`).

`freeglut.dll` must be next to the built `.exe` (or on your `PATH`).

## Project structure

```
Übung1/
├── main.cpp          # window setup, texture handling, Bresenham algorithms
├── Point.h           # n-dimensional point class
├── Color.h           # color class
├── Uebung1.sln       # Visual Studio solution
└── freeglut*, glut.h # freeglut headers and libraries
```

## Credits

Code framework by Georg Umlauf.
