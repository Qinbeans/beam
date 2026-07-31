"""Type stubs for the compiled beam._beam nanobind extension module.

This module is built from src/bindings.cpp via CMake/scikit-build and has
no Python source of its own, so pyright cannot infer its types from the
extension binary. These stubs mirror the bindings by hand and must be kept
in sync with python/src/bindings.cpp.
"""

from collections.abc import Buffer, Callable
from typing import overload

# -- raylib value types -----------------------------------------------------

class Vector2:
    """A 2D vector with `x` and `y` components."""
    x: float
    y: float

    def __init__(self, x: float = 0.0, y: float = 0.0) -> None:
        """Construct a new `Vector2`."""
        ...

class Color:
    """An RGBA color with 8-bit channels."""
    r: int
    g: int
    b: int
    a: int

    def __init__(self, r: int = 0, g: int = 0, b: int = 0, a: int = 255) -> None:
        """Construct a new `Color`."""
        ...

class Rectangle:
    """An axis-aligned rectangle defined by position and size."""
    x: float
    y: float
    width: float
    height: float

    def __init__(
        self,
        x: float = 0.0,
        y: float = 0.0,
        width: float = 0.0,
        height: float = 0.0,
    ) -> None:
        """Construct a new `Rectangle`."""
        ...

class Padding:
    """Edge padding/margins for a UI element."""
    top: int
    left: int
    bottom: int
    right: int

    def __init__(
        self, top: int = 0, left: int = 0, bottom: int = 0, right: int = 0
    ) -> None:
        """Construct a new `Padding`."""
        ...

# -- 3D value types -----------------------------------------------------

class Vector3:
    """A 3D vector with `x`, `y`, and `z` components."""
    x: float
    y: float
    z: float

    def __init__(self, x: float = 0.0, y: float = 0.0, z: float = 0.0) -> None:
        """Construct a new `Vector3`."""
        ...

class BoundingBox:
    """An axis-aligned 3D bounding box defined by `min` and `max` corners."""
    min: Vector3
    max: Vector3

    def __init__(self, min: Vector3, max: Vector3) -> None:
        """Construct a new `BoundingBox`."""
        ...

class Ray:
    """A 3D ray defined by an origin `position` and a `direction`."""
    position: Vector3
    direction: Vector3

    def __init__(self, position: Vector3, direction: Vector3) -> None:
        """Construct a new `Ray`."""
        ...

class RayCollision:
    """The result of a ray-object collision test."""
    hit: bool
    distance: float
    point: Vector3
    normal: Vector3

class Matrix:
    """A 4x4 transformation matrix."""
    @property
    def values(self) -> list[float]:
        """Return the matrix's 16 elements in row-major order."""
        ...

# -- asset resource types -------------------------------------------------
#
# Only obtained via load_image/load_texture/load_font/load_sound or
# Manager's asset getters below - there is no public constructor.

class Image:
    """CPU-side pixel data loaded via `load_image` or a `Manager` asset getter."""
    @property
    def width(self) -> int:
        """Return the width, in pixels."""
        ...
    @property
    def height(self) -> int:
        """Return the height, in pixels."""
        ...

class Texture:
    """A GPU-resident texture loaded via `load_texture`/`load_texture_from_image` or a
    `Manager` asset getter."""
    @property
    def width(self) -> int:
        """Return the width, in pixels."""
        ...
    @property
    def height(self) -> int:
        """Return the height, in pixels."""
        ...

class Font:
    """A loaded font resource used for text rendering."""
    @property
    def base_size(self) -> int:
        """Return the font's base glyph size, in pixels."""
        ...
    @property
    def glyph_count(self) -> int:
        """Return the number of glyphs available in the font."""
        ...

class Sound:
    """A short sound effect resource loaded via `load_sound`."""
    @property
    def frame_count(self) -> int:
        """Return the total number of audio frames."""
        ...

class Music:
    """A streamed music resource loaded via `load_music`."""
    @property
    def frame_count(self) -> int:
        """Return the total number of audio frames."""
        ...
    @property
    def looping(self) -> bool:
        """Return whether the track is set to loop."""
        ...

class Mesh:
    """Raw vertex/index geometry data, e.g. produced by a `gen_mesh_*` function."""
    @property
    def vertex_count(self) -> int:
        """Return the number of vertices in the mesh."""
        ...
    @property
    def triangle_count(self) -> int:
        """Return the number of triangles in the mesh."""
        ...

class Material:
    """A model material resource."""

class Model:
    """A 3D model resource loaded via `load_model`, made up of meshes and materials."""
    @property
    def mesh_count(self) -> int:
        """Return the number of meshes in the model."""
        ...
    @property
    def material_count(self) -> int:
        """Return the number of materials in the model."""
        ...
    @property
    def bone_count(self) -> int:
        """Return the number of bones in the model's skeleton."""
        ...

def load_image(path: str) -> Image:
    """Load an image from `path` into memory (not GPU-uploaded)."""
    ...
def load_texture(path: str) -> Texture:
    """Load a texture from `path`, uploading it to the GPU."""
    ...
def load_font(path: str) -> Font:
    """Load a font from `path`."""
    ...
def load_sound(path: str) -> Sound:
    """Load a sound effect from `path`."""
    ...
def load_music(path: str) -> Music:
    """Load a streamed music track from `path`."""
    ...
def load_model(path: str) -> Model:
    """Load a 3D model from `path`."""
    ...
def load_material_default() -> Material:
    """Return raylib's default material."""
    ...
def gen_mesh_cube(width: float, height: float, length: float) -> Mesh:
    """Generate a cuboid mesh with the given dimensions."""
    ...
def gen_mesh_sphere(radius: float, rings: int = 16, slices: int = 16) -> Mesh:
    """Generate a sphere mesh with the given radius, rings, and slices."""
    ...
def gen_mesh_cylinder(radius: float, height: float, slices: int = 16) -> Mesh:
    """Generate a cylinder mesh with the given radius, height, and slices."""
    ...
def gen_mesh_cone(radius: float, height: float, slices: int = 16) -> Mesh:
    """Generate a cone mesh with the given radius, height, and slices."""
    ...
def gen_mesh_torus(
    radius: float, size: float, rad_seg: int = 16, sides: int = 16
) -> Mesh:
    """Generate a torus mesh with the given radius, tube size, and segment counts."""
    ...
def gen_mesh_plane(
    width: float, length: float, res_x: int = 1, res_z: int = 1
) -> Mesh:
    """Generate a subdivided flat plane mesh."""
    ...
def upload_mesh(
    vertices: Buffer,
    indices: Buffer | None = None,
    normals: Buffer | None = None,
    texcoords: Buffer | None = None,
    colors: Buffer | None = None,
    dynamic: bool = False,
) -> Mesh:
    """Upload geometry you generated yourself to the GPU and return it as a
    `Mesh`, ready to hand to `Mesh3D`. This is the counterpart to the
    `gen_mesh_*` primitives, for shapes that have no primitive form - a voxel
    chunk mesher being the usual case.

    Every array is a 1-D, C-contiguous CPU buffer rather than a list: numpy
    arrays, `array.array`, `memoryview` and `bytes` all work, and the data is
    copied straight out without a per-element Python conversion.

    `vertices` holds 3 floats per vertex, `normals` 3, `texcoords` 2, and
    `colors` 4 bytes (RGBA); each optional attribute must cover every vertex.
    `indices` holds 3 unsigned shorts per triangle, which caps an indexed mesh
    at 65536 vertices - split larger geometry across several meshes. Without
    `indices`, vertices are drawn as consecutive triples.

    Pass `dynamic=True` if the buffers will be rewritten later. Requires an
    initialized window, since the upload needs a GL context.

    Raises:
        ValueError: if an array's length disagrees with the vertex count, or an
            index points past the last vertex.
    """
    ...
def load_texture_from_image(image: Image) -> Texture:
    """Upload `image` to the GPU and return the resulting texture."""
    ...
def measure_text_ex(
    text: str, font_size: float, spacing: float = 1.0, font: Font | None = None
) -> Vector2:
    """Measure the pixel size of `text` rendered with `font` at `font_size`."""
    ...
def load_gui_style(path: str) -> None:
    """Load a raygui style file from `path` and apply it globally."""
    ...

# -- core ---------------------------------------------------------------

class Manager:
    """Provides access to the asset cache, window/input state, and other engine
    services; passed to nodes during init/update/draw."""
    def has_asset(self, name: str) -> bool:
        """Return whether an asset named `name` exists in the cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Image) -> None:
        """Store `value` (a Image) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Texture) -> None:
        """Store `value` (a Texture) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Font) -> None:
        """Store `value` (a Font) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Sound) -> None:
        """Store `value` (a Sound) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Music) -> None:
        """Store `value` (a Music) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Mesh) -> None:
        """Store `value` (a Mesh) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Material) -> None:
        """Store `value` (a Material) under `name` in the asset cache."""
        ...
    @overload
    def set_asset(self, name: str, value: Model) -> None:
        """Store `value` (a Model) under `name` in the asset cache."""
        ...
    def get_image_asset(self, name: str) -> Image:
        """Return the image asset."""
        ...
    def get_texture_asset(self, name: str) -> Texture:
        """Return the texture asset."""
        ...
    def get_font_asset(self, name: str) -> Font:
        """Return the font asset."""
        ...
    def get_sound_asset(self, name: str) -> Sound:
        """Return the sound asset."""
        ...
    def get_music_asset(self, name: str) -> Music:
        """Return the music asset."""
        ...
    def get_mesh_asset(self, name: str) -> Mesh:
        """Return the mesh asset."""
        ...
    def get_material_asset(self, name: str) -> Material:
        """Return the material asset."""
        ...
    def get_model_asset(self, name: str) -> Model:
        """Return the model asset."""
        ...
    def close(self) -> None:
        """Close the window and shut down the manager, releasing GPU resources."""
        ...
    def closed(self) -> bool:
        """Return whether the window has been closed."""
        ...
    def set_background_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def get_background_color(self) -> Color:
        """Return the background color."""
        ...
    def get_screen_width(self) -> int:
        """Return the render/screen width in pixels."""
        ...
    def get_screen_height(self) -> int:
        """Return the render/screen height in pixels."""
        ...
    def get_window_width(self) -> int:
        """Return the OS window width in pixels."""
        ...
    def get_window_height(self) -> int:
        """Return the OS window height in pixels."""
        ...
    def get_fps(self) -> int:
        """Return the current frames-per-second."""
        ...
    def is_window_resized(self) -> bool:
        """Return whether the window was resized this frame."""
        ...
    def get_key_pressed(self) -> int:
        """Return the key code of the last key pressed this frame, or 0 if none."""
        ...
    def get_char_pressed(self) -> str:
        """Return the character typed this frame, or an empty string if none."""
        ...
    def get_mouse_x(self) -> int:
        """Return the mouse cursor's X position in window coordinates."""
        ...
    def get_mouse_y(self) -> int:
        """Return the mouse cursor's Y position in window coordinates."""
        ...
    def is_mouse_button_down(self, button: int) -> bool:
        """Return whether `button` is currently held down."""
        ...
    def is_mouse_button_pressed(self, button: int) -> bool:
        """Return whether `button` was pressed this frame."""
        ...
    def is_mouse_button_released(self, button: int) -> bool:
        """Return whether `button` was released this frame."""
        ...
    def is_key_down(self, key: int) -> bool:
        """Return whether `key` is currently held down."""
        ...
    def is_key_pressed(self, key: int) -> bool:
        """Return whether `key` was pressed this frame."""
        ...
    def is_key_released(self, key: int) -> bool:
        """Return whether `key` was released this frame."""
        ...

class Node:
    """Base class for everything in the scene graph, providing lifecycle hooks and
    parent/child relationships."""
    def __init__(self, name: str = "Node") -> None:
        """Construct a new `Node`."""
        ...
    def init(self, manager: Manager) -> None:
        """Called once before the first update; override to perform setup using
        `manager`."""
        ...
    def update(self, delta_time: float, manager: Manager) -> None:
        """Called every frame with the elapsed `delta_time`; override to update state.
        Called every frame with the elapsed `delta_time`; override to update state."""
        ...
    def draw(self, manager: Manager) -> None:
        """Called every frame after `update`; override to render using `manager`."""
        ...
    def set_position(self, x: float, y: float) -> None:
        """Set the position."""
        ...
    def set_active(self, active: bool) -> None:
        """Set whether this node and its subtree are active (updated and drawn)."""
        ...
    def set_parent(self, parent: Node) -> None:
        """Reparent this node under `parent`.

        The parent is held weakly, so something else must already own it - add
        it to its `Scene`, `Frame` or `Camera3D` first. Passing a node nothing
        else holds raises `RuntimeError` rather than silently leaving this node
        unparented.
        """
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def is_active(self) -> bool:
        """Return True if active, otherwise False."""
        ...
    def get_name(self) -> str:
        """Return this node's name."""
        ...
    def get_parent(self) -> Node | None:
        """Return the parent node, or None if this is a root node."""
        ...

class Event(Node):
    """A `Node` that groups child event handlers/listeners under a common parent."""
    def __init__(self, name: str = "Event") -> None:
        """Construct a new `Event`."""
        ...
    def add(self, child: Node) -> Event:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: Node) -> Event:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...

class WebSocketClient:
    """A client connection to a WebSocket server with async message/open/close
    callbacks."""
    def __init__(self) -> None:
        """Construct a new `WebSocketClient`."""
        ...
    def connect(self, uri: str) -> bool:
        """Connect to the WebSocket server at `uri`. Return True on success."""
        ...
    def disconnect(self) -> None:
        """Close the connection."""
        ...
    def send_message(self, message: str) -> None:
        """Send `message` as a text frame to the server."""
        ...
    def set_message_handler(self, handler: Callable[[str], None]) -> None:
        """Set the callback invoked with each received text message."""
        ...
    def set_open_handler(self, handler: Callable[[], None]) -> None:
        """Set the callback invoked when the connection is established."""
        ...
    def set_close_handler(self, handler: Callable[[], None]) -> None:
        """Set the callback invoked when the connection is closed."""
        ...
    def is_connected(self) -> bool:
        """Return True if connected, otherwise False."""
        ...

# -- scene --------------------------------------------------------------

class Scene(Node):
    """A `Node` representing a distinct screen or state of the application, managed by a
    `SceneManager`."""
    def __init__(self, name: str = "Scene") -> None:
        """Construct a new `Scene`."""
        ...
    def add(self, child: Node) -> Scene:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: Node) -> Scene:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def on_enter(self, manager: Manager) -> None:
        """Called by the `SceneManager` when this scene becomes active."""
        ...
    def on_exit(self, manager: Manager) -> None:
        """Called by the `SceneManager` when this scene is deactivated."""
        ...

class SceneManager(Node):
    """A `Node` that owns a collection of `Scene`s and switches between them."""
    def __init__(self, name: str = "SceneManager") -> None:
        """Construct a new `SceneManager`."""
        ...
    def add(self, scene: Scene) -> SceneManager:
        """Register `scene` under its name and return self for chaining."""
        ...
    def __lshift__(self, scene: Scene) -> SceneManager:
        """Operator alias for `add`; append `scene` and return self for chaining."""
        ...
    def switch_to_scene(self, name: str, manager: Manager) -> None:
        """Switch the active scene to the one registered under `name`."""
        ...
    def get_current_scene(self) -> Scene | None:
        """Return the currently active scene, or None if none is active."""
        ...
    def get_scene(self, name: str) -> Scene | None:
        """Return the registered scene named `name`, or None if not found."""
        ...

# -- objects --------------------------------------------------------------

class GameObject(Node):
    """Base class for renderable/interactive nodes placed within a scene."""
    def __init__(self, name: str = "GameObject") -> None:
        """Construct a new `GameObject`."""
        ...

class Text(GameObject):
    """A `GameObject` that draws a line of styled text."""
    def __init__(
        self,
        text: str,
        x: float,
        y: float,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `Text`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_font_size(self) -> float:
        """Return the font size."""
        ...

class Button(GameObject):
    """A clickable `GameObject` with background/foreground colors and hover state."""
    def __init__(
        self,
        text: str,
        font_size: float,
        font_spacing: float,
        position: Vector2,
        size: Vector2,
        padding: Padding,
        bg: Color,
        fg: Color,
        bg_hover: Color,
        fg_hover: Color,
        font_name: str = "default",
    ) -> None:
        """Construct a new `Button`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_padding(self, padding: Padding) -> None:
        """Set the padding."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_bg_hover_color(self, color: Color) -> None:
        """Set the background hover color."""
        ...
    def set_fg_hover_color(self, color: Color) -> None:
        """Set the foreground hover color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def is_hovered(self) -> bool:
        """Return True if hovered, otherwise False."""
        ...
    def is_clicked(self) -> bool:
        """Return True if clicked, otherwise False."""
        ...
    def on_update(
        self, callback: Callable[[float, Button, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_hover(self, callback: Callable[[Button, Manager], None]) -> None:
        """Register a callback invoked when it is hovered."""
        ...
    def on_click(self, callback: Callable[[Button, Manager], None]) -> None:
        """Register a callback invoked when it is clicked."""
        ...

class Input(Button):
    """A `Button` variant that accepts and displays typed text input."""
    def __init__(
        self,
        placeholder: str,
        font_size: float,
        font_spacing: float,
        position: Vector2,
        size: Vector2,
        padding: Padding,
        bg: Color,
        fg: Color,
        bg_hover: Color,
        fg_hover: Color,
        cursor_blink_time: float = 0.5,
        font_name: str = "default",
    ) -> None:
        """Construct a new `Input`."""
        ...
    def set_content(self, content: str) -> None:
        """Set the content text."""
        ...
    def set_focused(self, focused: bool) -> None:
        """Set the focused."""
        ...
    def get_content(self) -> str:
        """Return the content text."""
        ...
    def is_focused(self) -> bool:
        """Return True if focused, otherwise False."""
        ...
    def on_focus(self, callback: Callable[[Input, Manager], None]) -> None:
        """Register a callback invoked when it gains focus."""
        ...
    def on_blur(self, callback: Callable[[Input, Manager], None]) -> None:
        """Register a callback invoked when it loses focus."""
        ...

class CheckBox(GameObject):
    """A `GameObject` presenting a labeled boolean checkbox control."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        checked: bool = False,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `CheckBox`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_checked(self, checked: bool) -> None:
        """Set the checked."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def is_checked(self) -> bool:
        """Return True if checked, otherwise False."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[CheckBox, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class Toggle(GameObject):
    """A `GameObject` presenting a labeled on/off toggle control."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        active: bool = False,
        bg: Color = ...,
        fg: Color = ...,
        bg_active: Color = ...,
        fg_active: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `Toggle`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_active(self, active: bool) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_bg_active_color(self, color: Color) -> None:
        """Set the background active color."""
        ...
    def set_fg_active_color(self, color: Color) -> None:
        """Set the foreground active color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def is_active(self) -> bool:
        """Return True if active, otherwise False."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bg_active_color(self) -> Color:
        """Return the background active color."""
        ...
    def get_fg_active_color(self) -> Color:
        """Return the foreground active color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[Toggle, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class Slider(GameObject):
    """A `GameObject` presenting a draggable value slider."""
    def __init__(
        self,
        position: Vector2,
        size: Vector2,
        value: float = 0.0,
        min_value: float = 0.0,
        max_value: float = 1.0,
        bg: Color = ...,
        fg: Color = ...,
        text_left: str = "",
        text_right: str = "",
    ) -> None:
        """Construct a new `Slider`."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: float) -> None:
        """Set the value."""
        ...
    def set_range(self, min_value: float, max_value: float) -> None:
        """Set the allowed minimum and maximum values."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_text_left(self, text: str) -> None:
        """Set the text left."""
        ...
    def set_text_right(self, text: str) -> None:
        """Set the text right."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> float:
        """Return the value."""
        ...
    def get_min_value(self) -> float:
        """Return the minimum value."""
        ...
    def get_max_value(self) -> float:
        """Return the maximum value."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[Slider, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ProgressBar(GameObject):
    """A `GameObject` that visually represents progress toward a value range."""
    def __init__(
        self,
        position: Vector2,
        size: Vector2,
        value: float = 0.0,
        min_value: float = 0.0,
        max_value: float = 1.0,
        bg: Color = ...,
        fg: Color = ...,
        text_left: str = "",
        text_right: str = "",
    ) -> None:
        """Construct a new `ProgressBar`."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: float) -> None:
        """Set the value."""
        ...
    def set_range(self, min_value: float, max_value: float) -> None:
        """Set the allowed minimum and maximum values."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_text_left(self, text: str) -> None:
        """Set the text left."""
        ...
    def set_text_right(self, text: str) -> None:
        """Set the text right."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> float:
        """Return the value."""
        ...
    def get_min_value(self) -> float:
        """Return the minimum value."""
        ...
    def get_max_value(self) -> float:
        """Return the maximum value."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...

class GroupBox(GameObject):
    """A `GameObject` that draws a labeled bordered group container."""
    def __init__(
        self, text: str, position: Vector2, size: Vector2, color: Color = ...
    ) -> None:
        """Construct a new `GroupBox`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...

class Line(GameObject):
    """A `GameObject` that draws a horizontal divider line with optional text."""
    def __init__(
        self, position: Vector2, size: Vector2, text: str = "", color: Color = ...
    ) -> None:
        """Construct a new `Line`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...

class DummyRec(GameObject):
    """A `GameObject` representing a simple clickable placeholder rectangle."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `DummyRec`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_click(self, callback: Callable[[DummyRec, Manager], None]) -> None:
        """Register a callback invoked when it is clicked."""
        ...

class StatusBar(GameObject):
    """A `GameObject` that draws a single-row status/info bar."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `StatusBar`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_click(self, callback: Callable[[StatusBar, Manager], None]) -> None:
        """Register a callback invoked when it is clicked."""
        ...

class Grid(GameObject):
    """A `GameObject` that draws a subdivided grid, useful for editors/level tools."""
    def __init__(
        self,
        position: Vector2,
        size: Vector2,
        spacing: float = 16.0,
        subdivisions: int = 4,
        color: Color = ...,
    ) -> None:
        """Construct a new `Grid`."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_spacing(self, spacing: float) -> None:
        """Set the spacing."""
        ...
    def set_subdivisions(self, subdivisions: int) -> None:
        """Set the subdivisions."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_spacing(self) -> float:
        """Return the spacing."""
        ...
    def get_subdivisions(self) -> int:
        """Return the subdivisions."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_mouse_cell(self) -> Vector2:
        """Return the grid cell currently under the mouse cursor."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...

class LabelButton(GameObject):
    """A `GameObject` presenting clickable text with no background."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `LabelButton`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_click(self, callback: Callable[[LabelButton, Manager], None]) -> None:
        """Register a callback invoked when it is clicked."""
        ...

class ToggleGroup(GameObject):
    """A `GameObject` presenting a set of mutually exclusive toggle buttons."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = 0,
        bg: Color = ...,
        fg: Color = ...,
        bg_active: Color = ...,
        fg_active: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `ToggleGroup`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_bg_active_color(self, color: Color) -> None:
        """Set the background active color."""
        ...
    def set_fg_active_color(self, color: Color) -> None:
        """Set the foreground active color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bg_active_color(self) -> Color:
        """Return the background active color."""
        ...
    def get_fg_active_color(self) -> Color:
        """Return the foreground active color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ToggleGroup, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ToggleSlider(GameObject):
    """A `GameObject` presenting a slider-style control for choosing among a set of
    items."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = 0,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `ToggleSlider`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ToggleSlider, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ComboBox(GameObject):
    """A `GameObject` presenting a combo box for selecting one of several items."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = 0,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `ComboBox`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ComboBox, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class SliderBar(GameObject):
    """A `GameObject` presenting a slider bar control for a numeric value range."""
    def __init__(
        self,
        position: Vector2,
        size: Vector2,
        value: float = 0.0,
        min_value: float = 0.0,
        max_value: float = 1.0,
        bg: Color = ...,
        fg: Color = ...,
        text_left: str = "",
        text_right: str = "",
    ) -> None:
        """Construct a new `SliderBar`."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: float) -> None:
        """Set the value."""
        ...
    def set_range(self, min_value: float, max_value: float) -> None:
        """Set the allowed minimum and maximum values."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_text_left(self, text: str) -> None:
        """Set the text left."""
        ...
    def set_text_right(self, text: str) -> None:
        """Set the text right."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> float:
        """Return the value."""
        ...
    def get_min_value(self) -> float:
        """Return the minimum value."""
        ...
    def get_max_value(self) -> float:
        """Return the maximum value."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[SliderBar, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class DropdownBox(GameObject):
    """A `GameObject` presenting a collapsible dropdown list of items."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = 0,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `DropdownBox`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_open(self, open: bool) -> None:
        """Set whether the dropdown list is expanded (open)."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def is_open(self) -> bool:
        """Return True if open, otherwise False."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[DropdownBox, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class Spinner(GameObject):
    """A `GameObject` presenting a numeric spinner control with increment/decrement
    buttons."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        value: int = 0,
        min_value: int = 0,
        max_value: int = 100,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `Spinner`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: int) -> None:
        """Set the value."""
        ...
    def set_range(self, min_value: int, max_value: int) -> None:
        """Set the allowed minimum and maximum values."""
        ...
    def set_edit_mode(self, edit_mode: bool) -> None:
        """Set whether the control is in editable (typing) mode."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> int:
        """Return the value."""
        ...
    def get_min_value(self) -> int:
        """Return the minimum value."""
        ...
    def get_max_value(self) -> int:
        """Return the maximum value."""
        ...
    def is_edit_mode(self) -> bool:
        """Return whether the control is in editable (typing) mode."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[Spinner, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ValueBox(GameObject):
    """A `GameObject` presenting an editable integer value box."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        value: int = 0,
        min_value: int = 0,
        max_value: int = 100,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `ValueBox`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: int) -> None:
        """Set the value."""
        ...
    def set_range(self, min_value: int, max_value: int) -> None:
        """Set the allowed minimum and maximum values."""
        ...
    def set_edit_mode(self, edit_mode: bool) -> None:
        """Set whether the control is in editable (typing) mode."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> int:
        """Return the value."""
        ...
    def get_min_value(self) -> int:
        """Return the minimum value."""
        ...
    def get_max_value(self) -> int:
        """Return the maximum value."""
        ...
    def is_edit_mode(self) -> bool:
        """Return whether the control is in editable (typing) mode."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ValueBox, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ValueBoxFloat(GameObject):
    """A `GameObject` presenting an editable floating-point value box."""
    def __init__(
        self,
        text: str,
        position: Vector2,
        size: Vector2,
        value: float = 0.0,
        bg: Color = ...,
        fg: Color = ...,
        font_name: str = "default",
        font_size: float = 20.0,
        font_spacing: float = 1.0,
    ) -> None:
        """Construct a new `ValueBoxFloat`."""
        ...
    def set_text(self, text: str) -> None:
        """Set the text."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_value(self, value: float) -> None:
        """Set the value."""
        ...
    def set_edit_mode(self, edit_mode: bool) -> None:
        """Set whether the control is in editable (typing) mode."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_font_name(self, font_name: str) -> None:
        """Set the font name."""
        ...
    def set_font_size(self, size: float) -> None:
        """Set the font size."""
        ...
    def set_font_spacing(self, spacing: float) -> None:
        """Set the font spacing."""
        ...
    def get_text(self) -> str:
        """Return the text."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_value(self) -> float:
        """Return the value."""
        ...
    def is_edit_mode(self) -> bool:
        """Return whether the control is in editable (typing) mode."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ValueBoxFloat, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class WindowBox(GameObject):
    """A `GameObject` container that draws a titled window frame around its children."""
    def __init__(
        self,
        title: str,
        position: Vector2,
        size: Vector2,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `WindowBox`."""
        ...
    def add(self, child: GameObject) -> WindowBox:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: GameObject) -> WindowBox:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def set_title(self, title: str) -> None:
        """Set the title."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_title(self) -> str:
        """Return the title."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_close(self, callback: Callable[[WindowBox, Manager], None]) -> None:
        """Register a callback invoked when it is closed."""
        ...

class ScrollPanel(GameObject):
    """A `GameObject` container that clips and scrolls its content within a viewport."""
    def __init__(
        self,
        position: Vector2,
        size: Vector2,
        content: Rectangle,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `ScrollPanel`."""
        ...
    def add(self, child: GameObject) -> ScrollPanel:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: GameObject) -> ScrollPanel:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_content(self, content: Rectangle) -> None:
        """Set the content text."""
        ...
    def set_scroll(self, scroll: Vector2) -> None:
        """Set the scroll."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_content(self) -> Rectangle:
        """Return the content text."""
        ...
    def get_scroll(self) -> Vector2:
        """Return the scroll."""
        ...
    def get_view(self) -> Rectangle:
        """Return the view."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...

class ListView(GameObject):
    """A `GameObject` presenting a scrollable, selectable list of items."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = -1,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `ListView`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_scroll_index(self, scroll_index: int) -> None:
        """Set the scroll index."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_scroll_index(self) -> int:
        """Return the scroll index."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_focus_index(self) -> int:
        """Return the focus index."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ListView, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class TabBar(GameObject):
    """A `GameObject` presenting a horizontally scrollable row of selectable tabs."""
    def __init__(
        self,
        items: list[str],
        position: Vector2,
        size: Vector2,
        active: int = 0,
        bg: Color = ...,
        fg: Color = ...,
        bg_active: Color = ...,
        fg_active: Color = ...,
    ) -> None:
        """Construct a new `TabBar`."""
        ...
    def set_items(self, items: list[str]) -> None:
        """Set the items."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_hscroll(self, hscroll: int) -> None:
        """Set the horizontal scroll."""
        ...
    def set_active(self, active: int) -> None:
        """Set the active."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def set_bg_active_color(self, color: Color) -> None:
        """Set the background active color."""
        ...
    def set_fg_active_color(self, color: Color) -> None:
        """Set the foreground active color."""
        ...
    def get_items(self) -> list[str]:
        """Return the items."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_hscroll(self) -> int:
        """Return the horizontal scroll."""
        ...
    def get_active(self) -> int:
        """Return the active."""
        ...
    def get_focus_index(self) -> int:
        """Return the focus index."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bg_active_color(self) -> Color:
        """Return the background active color."""
        ...
    def get_fg_active_color(self) -> Color:
        """Return the foreground active color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[TabBar, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class MessageBox(GameObject):
    """A `GameObject` presenting a modal message dialog with a title, message, and
    buttons."""
    def __init__(
        self,
        title: str,
        message: str,
        buttons: list[str],
        position: Vector2,
        size: Vector2,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `MessageBox`."""
        ...
    def set_title(self, title: str) -> None:
        """Set the title."""
        ...
    def set_message(self, message: str) -> None:
        """Set the message."""
        ...
    def set_buttons(self, buttons: list[str]) -> None:
        """Set the buttons."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_title(self) -> str:
        """Return the title."""
        ...
    def get_message(self) -> str:
        """Return the message."""
        ...
    def get_buttons(self) -> list[str]:
        """Return the buttons."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_last_button(self) -> int:
        """Return the index of the last button pressed, or -1 if none."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_button(
        self, callback: Callable[[MessageBox, Manager, int], None]
    ) -> None:
        """Register a callback invoked when one of its buttons is pressed."""
        ...

class TextInputBox(GameObject):
    """A `GameObject` presenting a modal dialog with a text entry field and buttons."""
    def __init__(
        self,
        title: str,
        message: str,
        buttons: list[str],
        position: Vector2,
        size: Vector2,
        bg: Color = ...,
        fg: Color = ...,
    ) -> None:
        """Construct a new `TextInputBox`."""
        ...
    def set_title(self, title: str) -> None:
        """Set the title."""
        ...
    def set_message(self, message: str) -> None:
        """Set the message."""
        ...
    def set_content(self, content: str) -> None:
        """Set the content text."""
        ...
    def set_buttons(self, buttons: list[str]) -> None:
        """Set the buttons."""
        ...
    def set_secret_view(self, secret_view: bool) -> None:
        """Set whether typed text is masked (e.g. for password entry)."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def set_bg_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_fg_color(self, color: Color) -> None:
        """Set the foreground color."""
        ...
    def get_title(self) -> str:
        """Return the title."""
        ...
    def get_message(self) -> str:
        """Return the message."""
        ...
    def get_content(self) -> str:
        """Return the content text."""
        ...
    def get_buttons(self) -> list[str]:
        """Return the buttons."""
        ...
    def is_secret_view(self) -> bool:
        """Return whether typed text is masked (e.g. for password entry)."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_last_button(self) -> int:
        """Return the index of the last button pressed, or -1 if none."""
        ...
    def get_bg_color(self) -> Color:
        """Return the background color."""
        ...
    def get_fg_color(self) -> Color:
        """Return the foreground color."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_button(
        self, callback: Callable[[TextInputBox, Manager, int], None]
    ) -> None:
        """Register a callback invoked when one of its buttons is pressed."""
        ...

class ColorPicker(GameObject):
    """A `GameObject` presenting a full color picker (hue/saturation/value + alpha)."""
    def __init__(
        self, position: Vector2, size: Vector2, color: Color = ...
    ) -> None:
        """Construct a new `ColorPicker`."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ColorPicker, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ColorPanel(GameObject):
    """A `GameObject` presenting a saturation/value color panel for a fixed hue."""
    def __init__(
        self, position: Vector2, size: Vector2, color: Color = ...
    ) -> None:
        """Construct a new `ColorPanel`."""
        ...
    def set_color(self, color: Color) -> None:
        """Set the color."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def get_color(self) -> Color:
        """Return the color."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ColorPanel, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ColorBarAlpha(GameObject):
    """A `GameObject` presenting a draggable alpha channel bar."""
    def __init__(
        self, position: Vector2, size: Vector2, alpha: float = 1.0
    ) -> None:
        """Construct a new `ColorBarAlpha`."""
        ...
    def set_alpha(self, alpha: float) -> None:
        """Set the alpha."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def get_alpha(self) -> float:
        """Return the alpha."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(
        self, callback: Callable[[ColorBarAlpha, Manager], None]
    ) -> None:
        """Register a callback invoked when its value changes."""
        ...

class ColorBarHue(GameObject):
    """A `GameObject` presenting a draggable hue bar."""
    def __init__(self, position: Vector2, size: Vector2, hue: float = 0.0) -> None:
        """Construct a new `ColorBarHue`."""
        ...
    def set_hue(self, hue: float) -> None:
        """Set the hue."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_size(self, size: Vector2) -> None:
        """Set the size."""
        ...
    def get_hue(self) -> float:
        """Return the hue."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_size(self) -> Vector2:
        """Return the size."""
        ...
    def get_bounds(self) -> Rectangle:
        """Return the bounding rectangle used for layout and input hit-testing."""
        ...
    def on_change(self, callback: Callable[[ColorBarHue, Manager], None]) -> None:
        """Register a callback invoked when its value changes."""
        ...

class Sprite(GameObject):
    """A `GameObject` that draws a region of a named texture asset."""
    def __init__(
        self,
        manager: Manager,
        name: str,
        position: Vector2,
        bound: Rectangle,
        tint: Color = ...,
        bg: Color = ...,
    ) -> None:
        """Construct a new `Sprite`."""
        ...
    def set_position(self, position: Vector2) -> None:
        """Set the position."""
        ...
    def set_bound(self, bound: Rectangle) -> None:
        """Set the bound."""
        ...
    def set_tint(self, tint: Color) -> None:
        """Set the tint."""
        ...
    def set_background_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_rotation(self, rotation: float) -> None:
        """Set the rotation."""
        ...
    def set_scale(self, scale: float) -> None:
        """Set the scale."""
        ...
    def get_position(self) -> Vector2:
        """Return the position."""
        ...
    def get_bound(self) -> Rectangle:
        """Return the bound."""
        ...
    def get_tint(self) -> Color:
        """Return the tint."""
        ...
    def get_background_color(self) -> Color:
        """Return the background color."""
        ...
    def get_rotation(self) -> float:
        """Return the rotation."""
        ...
    def get_scale(self) -> float:
        """Return the scale."""
        ...
    def on_update(
        self, callback: Callable[[float, Sprite, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Sprite, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

class Frame(GameObject):
    """A `GameObject` container that clips and positions its children within a
    rectangular region."""
    def __init__(
        self,
        name: str,
        origin: Vector2,
        bound: Rectangle,
        bg: Color = ...,
    ) -> None:
        """Construct a new `Frame`."""
        ...
    def add(self, child: GameObject) -> Frame:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: GameObject) -> Frame:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def set_origin(self, origin: Vector2) -> None:
        """Set the origin."""
        ...
    def set_bound(self, bound: Rectangle) -> None:
        """Set the bound."""
        ...
    def set_background_color(self, color: Color) -> None:
        """Set the background color."""
        ...
    def set_rotation(self, rotation: float) -> None:
        """Set the rotation."""
        ...
    def get_origin(self) -> Vector2:
        """Return the origin."""
        ...
    def get_bound(self) -> Rectangle:
        """Return the bound."""
        ...
    def get_background_color(self) -> Color:
        """Return the background color."""
        ...
    def get_rotation(self) -> float:
        """Return the rotation."""
        ...
    def on_update(
        self, callback: Callable[[float, Frame, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Frame, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

class TileMap(GameObject):
    """A `GameObject` that renders a grid of tiles from a tileset texture."""
    @overload
    def __init__(
        self,
        name: str,
        tiles: list[list[int]],
        tile_width: int,
        tile_height: int,
        tint: Color = ...,
    ) -> None:
        """Construct a new `TileMap`."""
        ...
    @overload
    def __init__(
        self,
        name: str,
        texture_path: str,
        tile_width: int,
        tile_height: int,
        map_width: int,
        map_height: int,
        world_x: float,
        world_y: float,
        randomizer: Callable[[int, int, float, float], int],
        tint: Color = ...,
    ) -> None:
        """Construct a new `TileMap`."""
        ...
    def generate_tiles(self) -> None:
        """(Re)generate the tile layout, e.g. after changing the randomizer function."""
        ...
    def load_texture(self, texture_path: str) -> bool:
        """Load the tileset texture from `texture_path`. Return True on success."""
        ...
    def set_randomizer_function(
        self, randomizer: Callable[[int, int, float, float], int]
    ) -> None:
        """Set the callback used to procedurally choose a tile index for each cell."""
        ...
    def get_tile_at(self, tile_x: int, tile_y: int) -> int:
        """Return the tile index at cell (`tile_x`, `tile_y`)."""
        ...
    def set_tile_at(self, tile_x: int, tile_y: int, tile_index: int) -> None:
        """Set the tile index at cell (`tile_x`, `tile_y`) to `tile_index`."""
        ...
    def get_world_position(self) -> Vector2:
        """Return the map's world-space origin."""
        ...
    def set_world_position(self, x: float, y: float) -> None:
        """Set the map's world-space origin to (`x`, `y`)."""
        ...
    def get_map_size(self) -> Vector2:
        """Return the map size in tiles as a `Vector2`."""
        ...
    def get_texture_tile_count(self) -> Vector2:
        """Return the number of tile columns/rows available in the tileset texture."""
        ...
    def get_total_tile_count(self) -> int:
        """Return the total number of tiles in the map."""
        ...
    def regenerate_area(
        self, start_x: int, start_y: int, width: int, height: int
    ) -> None:
        """Regenerate the tiles within the rectangular area starting at (`start_x`,
        `start_y`) with the given `width`/`height`."""
        ...

# -- 3D objects -----------------------------------------------------------

class Object3D(GameObject):
    """Base class for 3D scene objects with position, rotation, scale, and tint."""
    def __init__(
        self,
        name: str,
        position: Vector3,
        rotation_axis: Vector3 = ...,
        rotation_angle: float = 0.0,
        scale: Vector3 = ...,
        tint: Color = ...,
    ) -> None:
        """Construct a new `Object3D`."""
        ...
    def set_position(self, position: Vector3) -> None:
        """Set the position."""
        ...
    def set_rotation_axis(self, rotation_axis: Vector3) -> None:
        """Set the rotation axis."""
        ...
    def set_rotation_angle(self, rotation_angle: float) -> None:
        """Set the rotation angle."""
        ...
    def set_scale(self, scale: Vector3) -> None:
        """Set the scale."""
        ...
    def set_pivot(self, pivot: Vector3) -> None:
        """Set the local-space point that rotation and scale happen around, and
        that `set_position` places. Defaults to the origin.

        This is what makes jointed animation work without a skeleton: give a
        limb a pivot at its joint, parent it to the torso, and `set_euler`
        swings it about that joint the way a bone would.
        """
        ...
    def set_euler(self, pitch: float, yaw: float, roll: float) -> None:
        """Set the rotation as Euler angles in degrees - `pitch` around X, `yaw`
        around Y, `roll` around Z. Stored as the equivalent axis/angle, so
        `get_rotation_axis` and `get_rotation_angle` stay meaningful.
        """
        ...
    def set_tint(self, tint: Color) -> None:
        """Set the tint."""
        ...
    def get_position(self) -> Vector3:
        """Return the position, relative to the parent `Object3D` if there is
        one (see `get_world_matrix`)."""
        ...
    def get_rotation_axis(self) -> Vector3:
        """Return the rotation axis."""
        ...
    def get_rotation_angle(self) -> float:
        """Return the rotation angle."""
        ...
    def get_scale(self) -> Vector3:
        """Return the scale."""
        ...
    def get_pivot(self) -> Vector3:
        """Return the local-space point rotation and scale happen around."""
        ...
    def get_euler(self) -> Vector3:
        """Return the rotation as Euler angles in degrees (see `set_euler`)."""
        ...
    def get_tint(self) -> Color:
        """Return the tint."""
        ...
    def get_local_matrix(self) -> Matrix:
        """Return this object's own transform - pivot, then scale, then
        rotation, then position - without any parent's contribution."""
        ...
    def get_world_matrix(self) -> Matrix:
        """Return `get_local_matrix` composed with the local matrix of every
        `Object3D` above this one in the node tree. This is the matrix the
        object actually draws with.

        Parenting composes transforms but does not draw: `Node` keeps a parent
        pointer and no child list, so only container nodes (`Camera3D`,
        `Scene`, ...) render their children. Build a limb hierarchy by adding
        every part to the `Camera3D` and calling `set_parent` to say how the
        parts move together.
        """
        ...
    def get_world_position(self) -> Vector3:
        """Return the translation of `get_world_matrix` - where this object
        ends up once its parents are taken into account."""
        ...
    def get_bounding_box(self) -> BoundingBox:
        """Return the axis-aligned world-space box enclosing this object under
        `get_world_matrix`, so it accounts for rotation and any parent."""
        ...
    def check_ray_collision(self, ray: Ray) -> RayCollision:
        """Test `ray` against this object and return the collision result."""
        ...
    def collides_with(self, other: Object3D) -> bool:
        """Return whether this object's bounding box intersects `other`'s."""
        ...

class Cube3D(Object3D):
    """An `Object3D` that draws a solid or wireframe cuboid."""
    def __init__(
        self,
        name: str,
        position: Vector3,
        size: Vector3,
        tint: Color = ...,
        wireframe: bool = False,
        wire_color: Color = ...,
    ) -> None:
        """Construct a new `Cube3D`."""
        ...
    def set_size(self, size: Vector3) -> None:
        """Set the size."""
        ...
    def set_wireframe(self, wireframe: bool) -> None:
        """Set the wireframe."""
        ...
    def set_wire_color(self, wire_color: Color) -> None:
        """Set the wire color."""
        ...
    def get_size(self) -> Vector3:
        """Return the size."""
        ...
    def is_wireframe(self) -> bool:
        """Return True if wireframe, otherwise False."""
        ...
    def get_wire_color(self) -> Color:
        """Return the wire color."""
        ...
    def on_update(
        self, callback: Callable[[float, Cube3D, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Cube3D, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

class Mesh3D(Object3D):
    """An `Object3D` that draws an arbitrary `Mesh`."""
    def __init__(
        self,
        manager: Manager,
        name: str,
        cache_key: str,
        mesh: Mesh,
        position: Vector3,
        rotation_axis: Vector3 = ...,
        rotation_angle: float = 0.0,
        scale: Vector3 = ...,
        tint: Color = ...,
        wireframe: bool = False,
    ) -> None:
        """Construct a new `Mesh3D`."""
        ...
    def set_mesh(self, manager: Manager, cache_key: str, mesh: Mesh) -> None:
        """Replace this object's geometry, unloading whatever it drew before.

        The constructor keeps a cached mesh when its key is already taken, so
        several objects can share one upload. Rebuilding needs the opposite:
        this always replaces the asset under `cache_key` and unloads the mesh it
        displaced, so anything else still drawing that key would be left
        pointing at freed buffers.

        This is what makes editable geometry possible -- a voxel chunk that is
        dug into has to be remeshed.
        """
        ...
    def set_wireframe(self, wireframe: bool) -> None:
        """Set the wireframe."""
        ...
    def is_wireframe(self) -> bool:
        """Return True if wireframe, otherwise False."""
        ...
    def on_update(
        self, callback: Callable[[float, Mesh3D, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Mesh3D, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

class Model3D(Object3D):
    """An `Object3D` that draws a loaded `Model`, with optional skeletal animation
    playback."""
    def __init__(
        self,
        manager: Manager,
        name: str,
        model_path: str,
        position: Vector3,
        rotation_axis: Vector3 = ...,
        rotation_angle: float = 0.0,
        scale: Vector3 = ...,
        tint: Color = ...,
        wireframe: bool = False,
    ) -> None:
        """Construct a new `Model3D`."""
        ...
    def set_wireframe(self, wireframe: bool) -> None:
        """Set the wireframe."""
        ...
    def is_wireframe(self) -> bool:
        """Return True if wireframe, otherwise False."""
        ...
    def load_animations(self, manager: Manager, animation_path: str) -> None:
        """Load animation clips from `animation_path` for this model."""
        ...
    def get_animation_names(self) -> list[str]:
        """Return the names of all loaded animation clips."""
        ...
    def play_animation(
        self, name: str, loop: bool = True, speed: float = 1.0
    ) -> None:
        """Start playing the animation clip `name`."""
        ...
    def crossfade_to(
        self, name: str, duration: float, loop: bool = True, speed: float = 1.0
    ) -> None:
        """Smoothly blend from the current animation into `name` over `duration`
        seconds."""
        ...
    def pause_animation(self) -> None:
        """Pause the currently playing animation."""
        ...
    def resume_animation(self) -> None:
        """Resume a paused animation."""
        ...
    def get_current_animation(self) -> str:
        """Return the name of the currently playing animation."""
        ...
    def is_crossfading(self) -> bool:
        """Return True if crossfading, otherwise False."""
        ...
    def get_blend_factor(self) -> float:
        """Return the current crossfade blend factor, from 0.0 (old) to 1.0 (new)."""
        ...
    def set_animation_fps(self, fps: float) -> None:
        """Set the playback rate, in animation frames per second."""
        ...
    def get_bone_count(self) -> int:
        """Return the number of bones in the model's skeleton."""
        ...
    def get_bone_name(self, index: int) -> str:
        """Return the name of the bone at `index`."""
        ...
    def get_bone_parent(self, index: int) -> int:
        """Return the parent bone index for the bone at `index`, or -1 if it has none.
        Return the parent bone index for the bone at `index`, or -1 if it has none."""
        ...
    def on_update(
        self, callback: Callable[[float, Model3D, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Model3D, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

# raylib's CameraProjection values, for Camera3D's `projection` argument.
# Orthographic is the usual choice for an isometric-looking scene.
CAMERA_PERSPECTIVE: int
CAMERA_ORTHOGRAPHIC: int

# raylib's CameraMode values, for Camera3D's `mode` argument. CAMERA_CUSTOM
# leaves the camera entirely under the game's control; the others let raylib
# drive it from input each frame.
CAMERA_CUSTOM: int
CAMERA_FREE: int
CAMERA_ORBITAL: int
CAMERA_FIRST_PERSON: int
CAMERA_THIRD_PERSON: int

class Camera3D(GameObject):
    """A `GameObject` that defines the 3D viewpoint used to render its children."""
    def __init__(
        self,
        name: str,
        position: Vector3,
        target: Vector3,
        up: Vector3 = ...,
        fovy: float = 45.0,
        projection: int = ...,
        mode: int = ...,
    ) -> None:
        """Construct a new `Camera3D`."""
        ...
    def add(self, child: GameObject) -> Camera3D:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: GameObject) -> Camera3D:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def set_position(self, position: Vector3) -> None:
        """Set the position."""
        ...
    def set_target(self, target: Vector3) -> None:
        """Set the target."""
        ...
    def set_up(self, up: Vector3) -> None:
        """Set the up."""
        ...
    def set_fovy(self, fovy: float) -> None:
        """Set the fovy."""
        ...
    def set_projection(self, projection: int) -> None:
        """Set the projection."""
        ...
    def set_mode(self, mode: int) -> None:
        """Set the mode."""
        ...
    def get_position(self) -> Vector3:
        """Return the position."""
        ...
    def get_target(self) -> Vector3:
        """Return the target."""
        ...
    def get_up(self) -> Vector3:
        """Return the up."""
        ...
    def get_fovy(self) -> float:
        """Return the fovy."""
        ...
    def get_projection(self) -> int:
        """Return the projection."""
        ...
    def get_mode(self) -> int:
        """Return the mode."""
        ...
    def get_view_matrix(self) -> Matrix:
        """Return the camera's current view matrix."""
        ...
    def world_to_screen(self, point: Vector3) -> Vector2:
        """Project a world-space `point` to 2D screen coordinates."""
        ...
    def screen_to_world_ray(self, point: Vector2) -> Ray:
        """Unproject a screen-space `point` into a world-space `Ray`."""
        ...
    def on_update(
        self, callback: Callable[[float, Camera3D, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Camera3D, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

class Camera2D(GameObject):
    """A `GameObject` that defines the 2D viewpoint (offset/target/rotation/zoom) used
    to render its children."""
    def __init__(
        self,
        name: str,
        offset: Vector2,
        target: Vector2,
        rotation: float = 0.0,
        zoom: float = 1.0,
    ) -> None:
        """Construct a new `Camera2D`."""
        ...
    def add(self, child: GameObject) -> Camera2D:
        """Add `child` as a child node and return self for chaining."""
        ...
    def __lshift__(self, child: GameObject) -> Camera2D:
        """Operator alias for `add`; append `child` and return self for chaining."""
        ...
    def set_offset(self, offset: Vector2) -> None:
        """Set the offset."""
        ...
    def set_target(self, target: Vector2) -> None:
        """Set the target."""
        ...
    def set_rotation(self, rotation: float) -> None:
        """Set the rotation."""
        ...
    def set_zoom(self, zoom: float) -> None:
        """Set the zoom."""
        ...
    def get_offset(self) -> Vector2:
        """Return the offset."""
        ...
    def get_target(self) -> Vector2:
        """Return the target."""
        ...
    def get_rotation(self) -> float:
        """Return the rotation."""
        ...
    def get_zoom(self) -> float:
        """Return the zoom."""
        ...
    def get_view_matrix(self) -> Matrix:
        """Return the camera's current view matrix."""
        ...
    def world_to_screen(self, point: Vector2) -> Vector2:
        """Transform a world-space `point` to screen coordinates using this camera."""
        ...
    def screen_to_world(self, point: Vector2) -> Vector2:
        """Transform a screen-space `point` to world coordinates using this camera."""
        ...
    def on_update(
        self, callback: Callable[[float, Camera2D, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...
    def on_draw(self, callback: Callable[[Camera2D, Manager], None]) -> None:
        """Register a callback invoked after the object's default drawing."""
        ...

# -- audio ------------------------------------------------------------

class AudioSource(GameObject):
    """A `GameObject` that plays a one-shot or loopable sound effect."""
    def __init__(
        self,
        manager: Manager,
        name: str,
        sound_path: str,
        volume: float = 1.0,
        pitch: float = 1.0,
        pan: float = 0.5,
    ) -> None:
        """Construct a new `AudioSource`."""
        ...
    def play(self) -> None:
        """Start playback from the beginning."""
        ...
    def stop(self) -> None:
        """Stop playback and reset to the beginning."""
        ...
    def pause(self) -> None:
        """Pause playback."""
        ...
    def resume(self) -> None:
        """Resume playback from where it was paused."""
        ...
    def is_playing(self) -> bool:
        """Return True if playing, otherwise False."""
        ...
    def set_volume(self, volume: float) -> None:
        """Set the volume."""
        ...
    def set_pitch(self, pitch: float) -> None:
        """Set the pitch."""
        ...
    def set_pan(self, pan: float) -> None:
        """Set the pan."""
        ...
    def get_volume(self) -> float:
        """Return the volume."""
        ...
    def get_pitch(self) -> float:
        """Return the pitch."""
        ...
    def get_pan(self) -> float:
        """Return the pan."""
        ...
    def on_update(
        self, callback: Callable[[float, AudioSource, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...

class MusicPlayer(GameObject):
    """A `GameObject` that streams and plays a music track."""
    def __init__(
        self,
        manager: Manager,
        name: str,
        music_path: str,
        looping: bool = True,
        volume: float = 1.0,
        pitch: float = 1.0,
        pan: float = 0.5,
    ) -> None:
        """Construct a new `MusicPlayer`."""
        ...
    def play(self) -> None:
        """Start playback from the beginning."""
        ...
    def stop(self) -> None:
        """Stop playback and reset to the beginning."""
        ...
    def pause(self) -> None:
        """Pause playback."""
        ...
    def resume(self) -> None:
        """Resume playback from where it was paused."""
        ...
    def is_playing(self) -> bool:
        """Return True if playing, otherwise False."""
        ...
    def set_looping(self, looping: bool) -> None:
        """Set whether the track restarts automatically when it ends."""
        ...
    def is_looping(self) -> bool:
        """Return True if looping, otherwise False."""
        ...
    def set_volume(self, volume: float) -> None:
        """Set the volume."""
        ...
    def set_pitch(self, pitch: float) -> None:
        """Set the pitch."""
        ...
    def set_pan(self, pan: float) -> None:
        """Set the pan."""
        ...
    def get_volume(self) -> float:
        """Return the volume."""
        ...
    def get_pitch(self) -> float:
        """Return the pitch."""
        ...
    def get_pan(self) -> float:
        """Return the pan."""
        ...
    def get_time_played(self) -> float:
        """Return the elapsed playback time, in seconds."""
        ...
    def get_time_length(self) -> float:
        """Return the total track length, in seconds."""
        ...
    def on_update(
        self, callback: Callable[[float, MusicPlayer, Manager], None]
    ) -> None:
        """Register a callback invoked every frame with the elapsed delta time."""
        ...

# -- app --------------------------------------------------------------

class App:
    """The application entry point; owns the window, main loop, event bus, and scene
    manager."""
    def __init__(
        self,
        title: str,
        width: int,
        height: int,
        fps: int,
        fullscreen: bool = False,
        resizable: bool = False,
    ) -> None:
        """Construct a new `App`."""
        ...
    def get_event(self) -> Event:
        """Return the root `Event` node used for global event handling."""
        ...
    def get_scene_manager(self) -> SceneManager:
        """Return the application's `SceneManager`."""
        ...
    def init(self) -> None:
        """Initialize the window and engine subsystems; call before `run`."""
        ...
    def run(self) -> None:
        """Run the main loop until the window is closed."""
        ...
    def get_manager(self) -> Manager:
        """Return the `Manager` instance used to interact with engine services."""
        ...

# -- logger --------------------------------------------------------------

def log_info(message: str) -> None:
    """Log an informational message."""
    ...
def log_warning(message: str) -> None:
    """Log a warning message."""
    ...
def log_error(message: str) -> None:
    """Log an error message."""
    ...
def log_debug(message: str) -> None:
    """Log a debug message."""
    ...
