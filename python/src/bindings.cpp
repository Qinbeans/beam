// Python bindings for the beam engine, using nanobind.
//
// Scope: beam's own API (Node hierarchy, scenes, game objects, App, Manager,
// WebSocketClient, logger). raylib itself is intentionally not bound beyond
// what beam's public headers need: a handful of raylib value types
// (Vector2, Color, Rectangle, Padding), bound below purely as plumbing, and
// the four asset resource types (Image, Texture2D, Font, Sound) that
// Manager's asset registry stores - see the AssetHandle note below.

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/trampoline.h>

#include "beam/core/app.h"
#include "beam/core/event.h"
#include "beam/core/logger.h"
#include "beam/core/manager.h"
#include "beam/core/node.h"
#include "beam/core/websocket_client.h"
#include "beam/objects/button.h"
#include "beam/objects/frame.h"
#include "beam/objects/game_object.h"
#include "beam/objects/input.h"
#include "beam/objects/sprite.h"
#include "beam/objects/text.h"
#include "beam/objects/tilemap.h"
#include "beam/scene/scene.h"
#include "beam/scene/scene_manager.h"

namespace nb = nanobind;
using namespace beam;

namespace {

// Trampoline for Node: lets Python subclasses override init/update/draw and
// have C++ (App's game loop, parent nodes, ...) dispatch into them.
struct PyNode : Node {
  NB_TRAMPOLINE(Node, 3);

  void init(SharedManager manager) override { NB_OVERRIDE(init, manager); }
  void update(float deltaTime, SharedManager manager) override {
    NB_OVERRIDE(update, deltaTime, manager);
  }
  void draw(SharedManager manager) override { NB_OVERRIDE(draw, manager); }
};

// Trampoline for Scene: on top of Node's overridables, exposes onEnter/
// onExit under their Pythonic names since that's the primary scene
// lifecycle extension point.
struct PyScene : Scene {
  NB_TRAMPOLINE(Scene, 5);

  void init(SharedManager manager) override { NB_OVERRIDE(init, manager); }
  void update(float deltaTime, SharedManager manager) override {
    NB_OVERRIDE(update, deltaTime, manager);
  }
  void draw(SharedManager manager) override { NB_OVERRIDE(draw, manager); }
  void onEnter(SharedManager manager) override {
    NB_OVERRIDE_NAME("on_enter", onEnter, manager);
  }
  void onExit(SharedManager manager) override {
    NB_OVERRIDE_NAME("on_exit", onExit, manager);
  }
};

// raylib's Image/Texture2D/Font/Sound are plain structs with no destructor;
// beam owns them via ImageAsset/TextureAsset/FontAsset/SoundAsset (asset.h),
// which call the matching Unload* when the Manager drops or replaces the
// asset. Manager::setAsset<T> copies the struct by value, but that copy is
// shallow - e.g. Image::data is a raw pointer, Font owns heap-allocated
// glyph arrays - so once a handle's resource has been handed to the
// Manager, this wrapper must stop owning it, or both sides would Unload*
// the same underlying handle. `owned` tracks that, and each handle is
// move-only so a resource has exactly one owner at a time. get_*_asset
// below returns handles with owned=false: a non-owning view, valid only as
// long as the Manager keeps that asset around - the same borrow contract
// Manager::getAsset<T>'s C++ callers (button.cpp, sprite.cpp) already rely
// on.
template <typename T, void (*Unload)(T)> struct AssetHandle {
  T data{};
  bool owned = false;

  AssetHandle() = default;
  explicit AssetHandle(T value, bool owned_ = true) : data(value), owned(owned_) {}

  AssetHandle(const AssetHandle &) = delete;
  AssetHandle &operator=(const AssetHandle &) = delete;

  AssetHandle(AssetHandle &&other) noexcept : data(other.data), owned(other.owned) {
    other.owned = false;
  }
  AssetHandle &operator=(AssetHandle &&other) noexcept {
    if (this != &other) {
      release();
      data = other.data;
      owned = other.owned;
      other.owned = false;
    }
    return *this;
  }

  ~AssetHandle() { release(); }

  // Called once `data` has been copied into a Manager asset, so this
  // handle no longer owns the underlying resource.
  void disown() { owned = false; }

private:
  void release() {
    if (owned)
      Unload(data);
    owned = false;
  }
};

using ImageHandle = AssetHandle<Image, UnloadImage>;
using TextureHandle = AssetHandle<Texture2D, UnloadTexture>;
using FontHandle = AssetHandle<Font, UnloadFont>;
using SoundHandle = AssetHandle<Sound, UnloadSound>;

} // namespace

NB_MODULE(_beam, m) {
  m.doc() = "Python bindings for the beam game engine";

  // -- raylib value types used throughout beam's public API -----------------

  // Vector2/Color/Rectangle/Padding are raylib aggregates with no
  // user-declared constructor, so `nb::init<Args...>()` would need C++20's
  // parenthesized-aggregate-init to call them as Vector2(x, y). This module
  // also has to stay compatible with websocketpp (via WebSocketClient,
  // bound below), which is incompatible with strict C++20, so these use
  // brace-init in a placement-new __init__ instead, which only needs C++11.
  nb::class_<Vector2>(m, "Vector2")
      .def(
          "__init__",
          [](Vector2 *self, float x, float y) { new (self) Vector2{x, y}; },
          nb::arg("x") = 0.0f, nb::arg("y") = 0.0f)
      .def_rw("x", &Vector2::x)
      .def_rw("y", &Vector2::y)
      .def("__repr__", [](const Vector2 &v) {
        return "Vector2(x=" + std::to_string(v.x) + ", y=" + std::to_string(v.y) + ")";
      });

  nb::class_<Color>(m, "Color")
      .def(
          "__init__",
          [](Color *self, unsigned char r, unsigned char g, unsigned char b,
             unsigned char a) { new (self) Color{r, g, b, a}; },
          nb::arg("r") = 0, nb::arg("g") = 0, nb::arg("b") = 0, nb::arg("a") = 255)
      .def_rw("r", &Color::r)
      .def_rw("g", &Color::g)
      .def_rw("b", &Color::b)
      .def_rw("a", &Color::a)
      .def("__repr__", [](const Color &c) {
        return "Color(r=" + std::to_string(c.r) + ", g=" + std::to_string(c.g) +
               ", b=" + std::to_string(c.b) + ", a=" + std::to_string(c.a) + ")";
      });

  nb::class_<Rectangle>(m, "Rectangle")
      .def(
          "__init__",
          [](Rectangle *self, float x, float y, float width, float height) {
            new (self) Rectangle{x, y, width, height};
          },
          nb::arg("x") = 0.0f, nb::arg("y") = 0.0f, nb::arg("width") = 0.0f,
          nb::arg("height") = 0.0f)
      .def_rw("x", &Rectangle::x)
      .def_rw("y", &Rectangle::y)
      .def_rw("width", &Rectangle::width)
      .def_rw("height", &Rectangle::height)
      .def("__repr__", [](const Rectangle &r) {
        return "Rectangle(x=" + std::to_string(r.x) + ", y=" + std::to_string(r.y) +
               ", width=" + std::to_string(r.width) +
               ", height=" + std::to_string(r.height) + ")";
      });

  nb::class_<Padding>(m, "Padding")
      .def(
          "__init__",
          [](Padding *self, int top, int left, int bottom, int right) {
            new (self) Padding{top, left, bottom, right};
          },
          nb::arg("top") = 0, nb::arg("left") = 0, nb::arg("bottom") = 0,
          nb::arg("right") = 0)
      .def_rw("top", &Padding::top)
      .def_rw("left", &Padding::left)
      .def_rw("bottom", &Padding::bottom)
      .def_rw("right", &Padding::right)
      .def("__repr__", [](const Padding &p) {
        return "Padding(top=" + std::to_string(p.top) + ", left=" + std::to_string(p.left) +
               ", bottom=" + std::to_string(p.bottom) +
               ", right=" + std::to_string(p.right) + ")";
      });

  // -- asset resource types ------------------------------------------------
  //
  // These wrap AssetHandle (see above); Python only ever gets one via
  // load_image/load_texture/load_font/load_sound or Manager's asset
  // getters below, never by constructing one directly.

  nb::class_<ImageHandle>(m, "Image")
      .def_prop_ro("width", [](const ImageHandle &h) { return h.data.width; })
      .def_prop_ro("height", [](const ImageHandle &h) { return h.data.height; })
      .def("__repr__", [](const ImageHandle &h) {
        return "Image(width=" + std::to_string(h.data.width) +
               ", height=" + std::to_string(h.data.height) + ")";
      });

  nb::class_<TextureHandle>(m, "Texture")
      .def_prop_ro("width", [](const TextureHandle &h) { return h.data.width; })
      .def_prop_ro("height", [](const TextureHandle &h) { return h.data.height; })
      .def("__repr__", [](const TextureHandle &h) {
        return "Texture(width=" + std::to_string(h.data.width) +
               ", height=" + std::to_string(h.data.height) + ")";
      });

  nb::class_<FontHandle>(m, "Font")
      .def_prop_ro("base_size", [](const FontHandle &h) { return h.data.baseSize; })
      .def_prop_ro("glyph_count", [](const FontHandle &h) { return h.data.glyphCount; })
      .def("__repr__", [](const FontHandle &h) {
        return "Font(base_size=" + std::to_string(h.data.baseSize) +
               ", glyph_count=" + std::to_string(h.data.glyphCount) + ")";
      });

  nb::class_<SoundHandle>(m, "Sound")
      .def_prop_ro("frame_count", [](const SoundHandle &h) { return h.data.frameCount; })
      .def("__repr__", [](const SoundHandle &h) {
        return "Sound(frame_count=" + std::to_string(h.data.frameCount) + ")";
      });

  m.def(
      "load_image", [](const std::string &path) { return ImageHandle(LoadImage(path.c_str())); },
      nb::arg("path"));
  m.def(
      "load_texture",
      [](const std::string &path) { return TextureHandle(LoadTexture(path.c_str())); },
      nb::arg("path"));
  m.def(
      "load_font", [](const std::string &path) { return FontHandle(LoadFont(path.c_str())); },
      nb::arg("path"));
  m.def(
      "load_sound", [](const std::string &path) { return SoundHandle(LoadSound(path.c_str())); },
      nb::arg("path"));
  m.def(
      "load_texture_from_image",
      [](const ImageHandle &image) { return TextureHandle(LoadTextureFromImage(image.data)); },
      nb::arg("image"));

  // -- core -------------------------------------------------------------

  nb::class_<Manager>(m, "Manager")
      .def("has_asset", &Manager::hasAsset, nb::arg("name"))
      .def(
          "set_asset",
          [](Manager &self, const std::string &name, ImageHandle &image) {
            self.setAsset<Image>(name, image.data);
            image.disown();
          },
          nb::arg("name"), nb::arg("value"))
      .def(
          "set_asset",
          [](Manager &self, const std::string &name, TextureHandle &texture) {
            self.setAsset<Texture2D>(name, texture.data);
            texture.disown();
          },
          nb::arg("name"), nb::arg("value"))
      .def(
          "set_asset",
          [](Manager &self, const std::string &name, FontHandle &font) {
            self.setAsset<Font>(name, font.data);
            font.disown();
          },
          nb::arg("name"), nb::arg("value"))
      .def(
          "set_asset",
          [](Manager &self, const std::string &name, SoundHandle &sound) {
            self.setAsset<Sound>(name, sound.data);
            sound.disown();
          },
          nb::arg("name"), nb::arg("value"))
      .def(
          "get_image_asset",
          [](Manager &self, const std::string &name) {
            return ImageHandle(self.getAsset<Image>(name), /*owned_=*/false);
          },
          nb::arg("name"))
      .def(
          "get_texture_asset",
          [](Manager &self, const std::string &name) {
            return TextureHandle(self.getAsset<Texture2D>(name), /*owned_=*/false);
          },
          nb::arg("name"))
      .def(
          "get_font_asset",
          [](Manager &self, const std::string &name) {
            return FontHandle(self.getAsset<Font>(name), /*owned_=*/false);
          },
          nb::arg("name"))
      .def(
          "get_sound_asset",
          [](Manager &self, const std::string &name) {
            return SoundHandle(self.getAsset<Sound>(name), /*owned_=*/false);
          },
          nb::arg("name"))
      .def("close", &Manager::close)
      .def("closed", &Manager::closed)
      .def("set_background_color", &Manager::setBackgroundColor, nb::arg("color"))
      .def("get_background_color", &Manager::getBackgroundColor)
      .def("get_key_pressed", &Manager::getKeyPressed)
      .def("get_char_pressed", &Manager::getCharPressed)
      .def("get_mouse_x", &Manager::getMouseX)
      .def("get_mouse_y", &Manager::getMouseY)
      .def("is_mouse_button_down", &Manager::isMouseButtonDown, nb::arg("button"))
      .def("is_mouse_button_pressed", &Manager::isMouseButtonPressed, nb::arg("button"))
      .def("is_mouse_button_released", &Manager::isMouseButtonReleased, nb::arg("button"))
      .def("is_key_down", &Manager::isKeyDown, nb::arg("key"))
      .def("is_key_pressed", &Manager::isKeyPressed, nb::arg("key"))
      .def("is_key_released", &Manager::isKeyReleased, nb::arg("key"));

  nb::class_<Node, PyNode>(m, "Node")
      .def(nb::init<const std::string &>(), nb::arg("name") = "Node")
      .def("init", &Node::init, nb::arg("manager"))
      .def("update", &Node::update, nb::arg("delta_time"), nb::arg("manager"))
      .def("draw", &Node::draw, nb::arg("manager"))
      .def("set_position", &Node::setPosition, nb::arg("x"), nb::arg("y"))
      .def("set_active", &Node::setActive, nb::arg("active"))
      .def("set_parent", &Node::setParent, nb::arg("parent"))
      .def("get_position", &Node::getPosition)
      .def("is_active", &Node::isActive)
      .def("get_name", &Node::getName)
      .def("get_parent",
           [](Node &self) -> std::shared_ptr<Node> { return self.getParent().lock(); });

  // Note on `add`: the container node is taken as a shared_ptr rather than a
  // reference because beam's operator<< calls shared_from_this() to hand the
  // child a parent link. Converting the Python object to a shared_ptr is what
  // gives it a control block; nanobind reuses an existing one when the object
  // is already owned in C++ (e.g. App's root Event), so no double ownership.
  nb::class_<Event, Node>(m, "Event")
      .def(nb::init<const std::string &>(), nb::arg("name") = "Event")
      .def(
          "add",
          [](std::shared_ptr<Event> self, std::shared_ptr<Node> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def(
          "__lshift__",
          [](std::shared_ptr<Event> self, std::shared_ptr<Node> child) {
            *self << child;
            return self;
          },
          nb::arg("child"));

  nb::class_<WebSocketClient>(m, "WebSocketClient")
      .def(nb::init<>())
      .def("connect", &WebSocketClient::connect, nb::arg("uri"),
           nb::call_guard<nb::gil_scoped_release>())
      .def("disconnect", &WebSocketClient::disconnect,
           nb::call_guard<nb::gil_scoped_release>())
      .def("send_message", &WebSocketClient::send_message, nb::arg("message"))
      .def("set_message_handler", &WebSocketClient::set_message_handler, nb::arg("handler"))
      .def("set_open_handler", &WebSocketClient::set_open_handler, nb::arg("handler"))
      .def("set_close_handler", &WebSocketClient::set_close_handler, nb::arg("handler"))
      .def("is_connected", &WebSocketClient::is_connected);

  // -- scene --------------------------------------------------------------

  nb::class_<Scene, Node, PyScene>(m, "Scene")
      .def(nb::init<const std::string &>(), nb::arg("name") = "Scene")
      .def(
          "add",
          [](std::shared_ptr<Scene> self, std::shared_ptr<Node> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def(
          "__lshift__",
          [](std::shared_ptr<Scene> self, std::shared_ptr<Node> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def("on_enter", &Scene::onEnter, nb::arg("manager"))
      .def("on_exit", &Scene::onExit, nb::arg("manager"));

  nb::class_<SceneManager, Node>(m, "SceneManager")
      .def(nb::init<const std::string &>(), nb::arg("name") = "SceneManager")
      .def(
          "add",
          [](std::shared_ptr<SceneManager> self, std::shared_ptr<Scene> child) {
            *self << child;
            return self;
          },
          nb::arg("scene"))
      .def(
          "__lshift__",
          [](std::shared_ptr<SceneManager> self, std::shared_ptr<Scene> child) {
            *self << child;
            return self;
          },
          nb::arg("scene"))
      .def("switch_to_scene", &SceneManager::switchToScene, nb::arg("name"), nb::arg("manager"))
      .def("get_current_scene", &SceneManager::getCurrentScene)
      .def("get_scene", &SceneManager::getScene, nb::arg("name"));

  // -- objects --------------------------------------------------------------

  nb::class_<GameObject, Node>(m, "GameObject")
      .def(nb::init<const std::string &>(), nb::arg("name") = "GameObject");

  nb::class_<Text, GameObject>(m, "Text")
      .def(nb::init<const std::string &, float, float, const std::string &, float, float>(),
           nb::arg("text"), nb::arg("x"), nb::arg("y"), nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &Text::setText, nb::arg("text"))
      .def("set_color", &Text::setColor, nb::arg("color"))
      .def("set_font_name", &Text::setFontName, nb::arg("font_name"))
      .def("set_font_size", &Text::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &Text::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &Text::getText)
      .def("get_color", &Text::getColor)
      .def("get_font_size", &Text::getFontSize);

  nb::class_<Button, GameObject>(m, "Button")
      .def(nb::init<const std::string &, float, float, Vector2, Vector2, Padding, Color, Color,
                     Color, Color, const std::string &>(),
           nb::arg("text"), nb::arg("font_size"), nb::arg("font_spacing"), nb::arg("position"),
           nb::arg("size"), nb::arg("padding"), nb::arg("bg"), nb::arg("fg"),
           nb::arg("bg_hover"), nb::arg("fg_hover"), nb::arg("font_name") = "default")
      .def("set_text", &Button::setText, nb::arg("text"))
      .def("set_font_name", &Button::setFontName, nb::arg("font_name"))
      .def("set_font_size", &Button::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &Button::setFontSpacing, nb::arg("spacing"))
      .def("set_position", &Button::setPosition, nb::arg("position"))
      .def("set_size", &Button::setSize, nb::arg("size"))
      .def("set_padding", &Button::setPadding, nb::arg("padding"))
      .def("set_bg_color", &Button::setBgColor, nb::arg("color"))
      .def("set_fg_color", &Button::setFgColor, nb::arg("color"))
      .def("set_bg_hover_color", &Button::setBgHoverColor, nb::arg("color"))
      .def("set_fg_hover_color", &Button::setFgHoverColor, nb::arg("color"))
      .def("get_bounds", &Button::getBounds)
      .def("is_hovered", &Button::isHovered)
      .def("is_clicked", &Button::isClicked)
      .def("on_update", &Button::onUpdate, nb::arg("callback"))
      .def("on_hover", &Button::onHover, nb::arg("callback"))
      .def("on_click", &Button::onClick, nb::arg("callback"));

  nb::class_<Input, Button>(m, "Input")
      .def(nb::init<const std::string &, float, float, Vector2, Vector2, Padding, Color, Color,
                     Color, Color, float, const std::string &>(),
           nb::arg("placeholder"), nb::arg("font_size"), nb::arg("font_spacing"),
           nb::arg("position"), nb::arg("size"), nb::arg("padding"), nb::arg("bg"),
           nb::arg("fg"), nb::arg("bg_hover"), nb::arg("fg_hover"),
           nb::arg("cursor_blink_time") = 0.5f, nb::arg("font_name") = "default")
      .def("set_content", &Input::setContent, nb::arg("content"))
      .def("set_focused", &Input::setFocused, nb::arg("focused"))
      .def("get_content", &Input::getContent)
      .def("is_focused", &Input::isFocused)
      .def("on_focus", &Input::onFocus, nb::arg("callback"))
      .def("on_blur", &Input::onBlur, nb::arg("callback"));

  nb::class_<Sprite, GameObject>(m, "Sprite")
      .def(nb::init<SharedManager, const std::string &, Vector2, Rectangle, Color, Color>(),
           nb::arg("manager"), nb::arg("name"), nb::arg("position"), nb::arg("bound"),
           nb::arg("tint") = Color{255, 255, 255, 255}, nb::arg("bg") = Color{0, 0, 0, 0})
      .def("set_position", &Sprite::setPosition, nb::arg("position"))
      .def("set_bound", &Sprite::setBound, nb::arg("bound"))
      .def("set_tint", &Sprite::setTint, nb::arg("tint"))
      .def("set_background_color", &Sprite::setBackgroundColor, nb::arg("color"))
      .def("set_rotation", &Sprite::setRotation, nb::arg("rotation"))
      .def("set_scale", &Sprite::setScale, nb::arg("scale"))
      .def("get_position", &Sprite::getPosition)
      .def("get_bound", &Sprite::getBound)
      .def("get_tint", &Sprite::getTint)
      .def("get_background_color", &Sprite::getBackgroundColor)
      .def("get_rotation", &Sprite::getRotation)
      .def("get_scale", &Sprite::getScale)
      .def("on_update", &Sprite::onUpdate, nb::arg("callback"))
      .def("on_draw", &Sprite::onDraw, nb::arg("callback"));

  nb::class_<Frame, GameObject>(m, "Frame")
      .def(nb::init<const std::string &, Vector2, Rectangle, Color>(), nb::arg("name"),
           nb::arg("origin"), nb::arg("bound"), nb::arg("bg") = Color{0, 0, 0, 0})
      .def(
          "add",
          [](std::shared_ptr<Frame> self, std::shared_ptr<GameObject> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def("set_origin", &Frame::setOrigin, nb::arg("origin"))
      .def("set_bound", &Frame::setBound, nb::arg("bound"))
      .def("set_background_color", &Frame::setBackgroundColor, nb::arg("color"))
      .def("set_rotation", &Frame::setRotation, nb::arg("rotation"))
      .def("get_origin", &Frame::getOrigin)
      .def("get_bound", &Frame::getBound)
      .def("get_background_color", &Frame::getBackgroundColor)
      .def("get_rotation", &Frame::getRotation)
      .def("on_update", &Frame::onUpdate, nb::arg("callback"))
      .def("on_draw", &Frame::onDraw, nb::arg("callback"));

  nb::class_<TileMap, GameObject>(m, "TileMap")
      .def(nb::init<const std::string &, const std::vector<std::vector<int>> &, int, int,
                     Color>(),
           nb::arg("name"), nb::arg("tiles"), nb::arg("tile_width"), nb::arg("tile_height"),
           nb::arg("tint") = Color{255, 255, 255, 255})
      .def(nb::init<const std::string &, const std::string &, int, int, int, int, float,
                     float, TileRandomizerFunc, Color>(),
           nb::arg("name"), nb::arg("texture_path"), nb::arg("tile_width"),
           nb::arg("tile_height"), nb::arg("map_width"), nb::arg("map_height"),
           nb::arg("world_x"), nb::arg("world_y"), nb::arg("randomizer"),
           nb::arg("tint") = Color{255, 255, 255, 255})
      .def("generate_tiles", &TileMap::generateTiles)
      .def("load_texture", &TileMap::loadTexture, nb::arg("texture_path"))
      .def("set_randomizer_function", &TileMap::setRandomizerFunction, nb::arg("randomizer"))
      .def("get_tile_at", &TileMap::getTileAt, nb::arg("tile_x"), nb::arg("tile_y"))
      .def("set_tile_at", &TileMap::setTileAt, nb::arg("tile_x"), nb::arg("tile_y"),
           nb::arg("tile_index"))
      .def("get_world_position", &TileMap::getWorldPosition)
      .def("set_world_position", &TileMap::setWorldPosition, nb::arg("x"), nb::arg("y"))
      .def("get_map_size", &TileMap::getMapSize)
      .def("get_texture_tile_count", &TileMap::getTextureTileCount)
      .def("get_total_tile_count", &TileMap::getTotalTileCount)
      .def("regenerate_area", &TileMap::regenerateArea, nb::arg("start_x"), nb::arg("start_y"),
           nb::arg("width"), nb::arg("height"));

  // -- app --------------------------------------------------------------

  nb::class_<App>(m, "App")
      .def(nb::init<const std::string &, int, int, int, bool, bool>(), nb::arg("title"),
           nb::arg("width"), nb::arg("height"), nb::arg("fps"), nb::arg("fullscreen") = false,
           nb::arg("resizable") = false)
      // Recover App's own shared_ptr via shared_from_this() rather than
      // exposing a bare reference: this hands Python the real owning pointer,
      // so parent links created by add() stay alive for the App's lifetime.
      .def("get_event",
           [](App &self) {
             return std::static_pointer_cast<Event>(self.getEvent().shared_from_this());
           })
      .def("get_scene_manager",
           [](App &self) {
             return std::static_pointer_cast<SceneManager>(
                 self.getSceneManager().shared_from_this());
           })
      .def("init", &App::init)
      // Release the GIL for the game loop so background threads (e.g. the
      // WebSocketClient's ASIO thread) can call back into Python while it
      // runs; Node trampolines re-acquire the GIL for each Python override.
      .def("run", &App::run, nb::call_guard<nb::gil_scoped_release>())
      .def("get_manager", &App::getManager);

  // -- logger --------------------------------------------------------------

  m.def("log_info", [](const std::string &msg) { logger::info << msg; }, nb::arg("message"));
  m.def(
      "log_warning", [](const std::string &msg) { logger::warning << msg; }, nb::arg("message"));
  m.def("log_error", [](const std::string &msg) { logger::error << msg; }, nb::arg("message"));
  m.def("log_debug", [](const std::string &msg) { logger::debug << msg; }, nb::arg("message"));
}
