# Beam

Beam is a small C++17 game engine built on top of [raylib](https://www.raylib.com/), with a
`Node` → `Scene`/`GameObject` hierarchy, an `App`/`Manager` game loop, and optional
[nanobind](https://github.com/wjakob/nanobind)-based Python bindings.

## Features

- **Node hierarchy** — `Node`, `Event`, `Scene`, and `SceneManager` for composing and updating a
  tree of game objects.
- **Game objects** — `Text`, `Button`, `Input`, `Sprite`, `Frame`, and `TileMap` built on a common
  `GameObject` base.
- **App loop** — `App`/`Manager` wrap raylib's window, input, and asset handling.
- **WebSockets** — `WebSocketClient` (via websocketpp/ASIO) for talking to a server from the game
  loop. See [WEBSOCKET_INTEGRATION.md](WEBSOCKET_INTEGRATION.md).
- **Python bindings** — the same API exposed as the `beam` package, generated from
  [python/src/bindings.cpp](python/src/bindings.cpp).

## Building the C++ library and examples

Requires CMake 3.15+ and a C++17 compiler. raylib, ASIO, and websocketpp are fetched
automatically via `FetchContent` if not already installed.

```sh
./init.sh          # configures into ./build
cmake --build build
```

Example binaries are written under `build/examples/<name>/`. Build options (set with
`-D<OPTION>=ON|OFF` at configure time):

| Option                        | Default | Description                              |
|-------------------------------|---------|-------------------------------------------|
| `BEAM_BUILD_EXAMPLES`         | `ON`    | Build the executables under `examples/`.   |
| `BEAM_BUILD_PYTHON_BINDINGS`  | `OFF`   | Build the `_beam` nanobind extension module (requires the `nanobind` Python package). |

## Python bindings

The Python package lives in [python/beam/](python/beam/) and wraps the compiled `_beam` extension
module. Build and install it with [scikit-build-core](https://scikit-build-core.readthedocs.io/):

```sh
uv sync   # or: pip install -e .
```

```python
import beam

app = beam.App("my game", 640, 360, 60)
...
```

See [examples/python_demo/main.py](examples/python_demo/main.py) for a complete example.

## Repository layout

```
include/beam/    Public C++ headers (core, objects, scene)
src/             C++ implementation
python/          nanobind bindings (python/src) and the beam package (python/beam)
examples/        basic_game, websocket_example, and python_demo
scripts/         maintenance scripts (e.g. bump_version.py)
```

## Versioning

`CMakeLists.txt`'s `project(Beam VERSION X.Y.Z ...)` is the source of truth for the project
version; `pyproject.toml`'s `version` is kept in sync with it. Bump both with:

```sh
scripts/bump_version.py {major,minor,patch}
```
