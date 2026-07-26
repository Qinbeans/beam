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
#include "beam/objects/checkbox.h"
#include "beam/objects/color_bar_alpha.h"
#include "beam/objects/color_bar_hue.h"
#include "beam/objects/color_panel.h"
#include "beam/objects/color_picker.h"
#include "beam/objects/combo_box.h"
#include "beam/objects/dropdown_box.h"
#include "beam/objects/dummy_rec.h"
#include "beam/objects/frame.h"
#include "beam/objects/game_object.h"
#include "beam/objects/grid.h"
#include "beam/objects/group_box.h"
#include "beam/objects/input.h"
#include "beam/objects/label_button.h"
#include "beam/objects/line.h"
#include "beam/objects/list_view.h"
#include "beam/objects/message_box.h"
#include "beam/objects/progress_bar.h"
#include "beam/objects/scroll_panel.h"
#include "beam/objects/slider.h"
#include "beam/objects/slider_bar.h"
#include "beam/objects/spinner.h"
#include "beam/objects/sprite.h"
#include "beam/objects/status_bar.h"
#include "beam/objects/tab_bar.h"
#include "beam/objects/text.h"
#include "beam/objects/text_input_box.h"
#include "beam/objects/tilemap.h"
#include "beam/objects/toggle.h"
#include "beam/objects/toggle_group.h"
#include "beam/objects/toggle_slider.h"
#include "beam/objects/value_box.h"
#include "beam/objects/value_box_float.h"
#include "beam/objects/window_box.h"
#include "beam/scene/scene.h"
#include "beam/scene/scene_manager.h"
#include "raygui.h"

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
  m.def(
      "measure_text_ex",
      [](const std::string &text, float fontSize, float spacing, const FontHandle *font) {
        return MeasureTextEx(font ? font->data : GetFontDefault(), text.c_str(), fontSize,
                              spacing);
      },
      nb::arg("text"), nb::arg("font_size"), nb::arg("spacing") = 1.0f,
      nb::arg("font") = nullptr);
  m.def(
      "load_gui_style", [](const std::string &path) { GuiLoadStyle(path.c_str()); },
      nb::arg("path"));

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
      .def("get_screen_width", &Manager::getScreenWidth)
      .def("get_screen_height", &Manager::getScreenHeight)
      .def("get_window_width", &Manager::getWindowWidth)
      .def("get_window_height", &Manager::getWindowHeight)
      .def("get_fps", &Manager::getFPS)
      .def("is_window_resized", &Manager::isWindowResized)
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

  nb::class_<CheckBox, GameObject>(m, "CheckBox")
      .def(nb::init<const std::string &, Vector2, Vector2, bool, Color, Color,
                     const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("checked") = false, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &CheckBox::setText, nb::arg("text"))
      .def("set_position", &CheckBox::setPosition, nb::arg("position"))
      .def("set_size", &CheckBox::setSize, nb::arg("size"))
      .def("set_checked", &CheckBox::setChecked, nb::arg("checked"))
      .def("set_bg_color", &CheckBox::setBgColor, nb::arg("color"))
      .def("set_fg_color", &CheckBox::setFgColor, nb::arg("color"))
      .def("set_font_name", &CheckBox::setFontName, nb::arg("font_name"))
      .def("set_font_size", &CheckBox::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &CheckBox::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &CheckBox::getText)
      .def("get_position", &CheckBox::getPosition)
      .def("get_size", &CheckBox::getSize)
      .def("is_checked", &CheckBox::isChecked)
      .def("get_bg_color", &CheckBox::getBgColor)
      .def("get_fg_color", &CheckBox::getFgColor)
      .def("get_bounds", &CheckBox::getBounds)
      .def("on_change", &CheckBox::onChange, nb::arg("callback"));

  nb::class_<Toggle, GameObject>(m, "Toggle")
      .def(nb::init<const std::string &, Vector2, Vector2, bool, Color, Color,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = false, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255},
           nb::arg("bg_active") = Color{80, 80, 80, 255},
           nb::arg("fg_active") = Color{255, 255, 255, 255},
           nb::arg("font_name") = "default", nb::arg("font_size") = 20.0f,
           nb::arg("font_spacing") = 1.0f)
      .def("set_text", &Toggle::setText, nb::arg("text"))
      .def("set_position", &Toggle::setPosition, nb::arg("position"))
      .def("set_size", &Toggle::setSize, nb::arg("size"))
      .def("set_active", &Toggle::setActive, nb::arg("active"))
      .def("set_bg_color", &Toggle::setBgColor, nb::arg("color"))
      .def("set_fg_color", &Toggle::setFgColor, nb::arg("color"))
      .def("set_bg_active_color", &Toggle::setBgActiveColor, nb::arg("color"))
      .def("set_fg_active_color", &Toggle::setFgActiveColor, nb::arg("color"))
      .def("set_font_name", &Toggle::setFontName, nb::arg("font_name"))
      .def("set_font_size", &Toggle::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &Toggle::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &Toggle::getText)
      .def("get_position", &Toggle::getPosition)
      .def("get_size", &Toggle::getSize)
      .def("is_active", &Toggle::isActive)
      .def("get_bg_color", &Toggle::getBgColor)
      .def("get_fg_color", &Toggle::getFgColor)
      .def("get_bg_active_color", &Toggle::getBgActiveColor)
      .def("get_fg_active_color", &Toggle::getFgActiveColor)
      .def("get_bounds", &Toggle::getBounds)
      .def("on_change", &Toggle::onChange, nb::arg("callback"));

  nb::class_<Slider, GameObject>(m, "Slider")
      .def(nb::init<Vector2, Vector2, float, float, float, Color, Color,
                     const std::string &, const std::string &>(),
           nb::arg("position"), nb::arg("size"), nb::arg("value") = 0.0f,
           nb::arg("min_value") = 0.0f, nb::arg("max_value") = 1.0f,
           nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("text_left") = "",
           nb::arg("text_right") = "")
      .def("set_position", &Slider::setPosition, nb::arg("position"))
      .def("set_size", &Slider::setSize, nb::arg("size"))
      .def("set_value", &Slider::setValue, nb::arg("value"))
      .def("set_range", &Slider::setRange, nb::arg("min_value"), nb::arg("max_value"))
      .def("set_bg_color", &Slider::setBgColor, nb::arg("color"))
      .def("set_fg_color", &Slider::setFgColor, nb::arg("color"))
      .def("set_text_left", &Slider::setTextLeft, nb::arg("text"))
      .def("set_text_right", &Slider::setTextRight, nb::arg("text"))
      .def("get_position", &Slider::getPosition)
      .def("get_size", &Slider::getSize)
      .def("get_value", &Slider::getValue)
      .def("get_min_value", &Slider::getMinValue)
      .def("get_max_value", &Slider::getMaxValue)
      .def("get_bg_color", &Slider::getBgColor)
      .def("get_fg_color", &Slider::getFgColor)
      .def("get_bounds", &Slider::getBounds)
      .def("on_change", &Slider::onChange, nb::arg("callback"));

  nb::class_<ProgressBar, GameObject>(m, "ProgressBar")
      .def(nb::init<Vector2, Vector2, float, float, float, Color, Color,
                     const std::string &, const std::string &>(),
           nb::arg("position"), nb::arg("size"), nb::arg("value") = 0.0f,
           nb::arg("min_value") = 0.0f, nb::arg("max_value") = 1.0f,
           nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{0, 228, 48, 255}, nb::arg("text_left") = "",
           nb::arg("text_right") = "")
      .def("set_position", &ProgressBar::setPosition, nb::arg("position"))
      .def("set_size", &ProgressBar::setSize, nb::arg("size"))
      .def("set_value", &ProgressBar::setValue, nb::arg("value"))
      .def("set_range", &ProgressBar::setRange, nb::arg("min_value"), nb::arg("max_value"))
      .def("set_bg_color", &ProgressBar::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ProgressBar::setFgColor, nb::arg("color"))
      .def("set_text_left", &ProgressBar::setTextLeft, nb::arg("text"))
      .def("set_text_right", &ProgressBar::setTextRight, nb::arg("text"))
      .def("get_position", &ProgressBar::getPosition)
      .def("get_size", &ProgressBar::getSize)
      .def("get_value", &ProgressBar::getValue)
      .def("get_min_value", &ProgressBar::getMinValue)
      .def("get_max_value", &ProgressBar::getMaxValue)
      .def("get_bg_color", &ProgressBar::getBgColor)
      .def("get_fg_color", &ProgressBar::getFgColor)
      .def("get_bounds", &ProgressBar::getBounds);

  nb::class_<GroupBox, GameObject>(m, "GroupBox")
      .def(nb::init<const std::string &, Vector2, Vector2, Color>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("color") = Color{130, 130, 130, 255})
      .def("set_text", &GroupBox::setText, nb::arg("text"))
      .def("set_position", &GroupBox::setPosition, nb::arg("position"))
      .def("set_size", &GroupBox::setSize, nb::arg("size"))
      .def("set_color", &GroupBox::setColor, nb::arg("color"))
      .def("get_text", &GroupBox::getText)
      .def("get_position", &GroupBox::getPosition)
      .def("get_size", &GroupBox::getSize)
      .def("get_color", &GroupBox::getColor)
      .def("get_bounds", &GroupBox::getBounds);

  nb::class_<Line, GameObject>(m, "Line")
      .def(nb::init<Vector2, Vector2, const std::string &, Color>(),
           nb::arg("position"), nb::arg("size"), nb::arg("text") = "",
           nb::arg("color") = Color{130, 130, 130, 255})
      .def("set_text", &Line::setText, nb::arg("text"))
      .def("set_position", &Line::setPosition, nb::arg("position"))
      .def("set_size", &Line::setSize, nb::arg("size"))
      .def("set_color", &Line::setColor, nb::arg("color"))
      .def("get_text", &Line::getText)
      .def("get_position", &Line::getPosition)
      .def("get_size", &Line::getSize)
      .def("get_color", &Line::getColor)
      .def("get_bounds", &Line::getBounds);

  nb::class_<DummyRec, GameObject>(m, "DummyRec")
      .def(nb::init<const std::string &, Vector2, Vector2, Color, Color>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255})
      .def("set_text", &DummyRec::setText, nb::arg("text"))
      .def("set_position", &DummyRec::setPosition, nb::arg("position"))
      .def("set_size", &DummyRec::setSize, nb::arg("size"))
      .def("set_bg_color", &DummyRec::setBgColor, nb::arg("color"))
      .def("set_fg_color", &DummyRec::setFgColor, nb::arg("color"))
      .def("get_text", &DummyRec::getText)
      .def("get_position", &DummyRec::getPosition)
      .def("get_size", &DummyRec::getSize)
      .def("get_bg_color", &DummyRec::getBgColor)
      .def("get_fg_color", &DummyRec::getFgColor)
      .def("get_bounds", &DummyRec::getBounds)
      .def("on_click", &DummyRec::onClick, nb::arg("callback"));

  nb::class_<StatusBar, GameObject>(m, "StatusBar")
      .def(nb::init<const std::string &, Vector2, Vector2, Color, Color>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255})
      .def("set_text", &StatusBar::setText, nb::arg("text"))
      .def("set_position", &StatusBar::setPosition, nb::arg("position"))
      .def("set_size", &StatusBar::setSize, nb::arg("size"))
      .def("set_bg_color", &StatusBar::setBgColor, nb::arg("color"))
      .def("set_fg_color", &StatusBar::setFgColor, nb::arg("color"))
      .def("get_text", &StatusBar::getText)
      .def("get_position", &StatusBar::getPosition)
      .def("get_size", &StatusBar::getSize)
      .def("get_bg_color", &StatusBar::getBgColor)
      .def("get_fg_color", &StatusBar::getFgColor)
      .def("get_bounds", &StatusBar::getBounds)
      .def("on_click", &StatusBar::onClick, nb::arg("callback"));

  nb::class_<Grid, GameObject>(m, "Grid")
      .def(nb::init<Vector2, Vector2, float, int, Color>(), nb::arg("position"),
           nb::arg("size"), nb::arg("spacing") = 16.0f,
           nb::arg("subdivisions") = 4,
           nb::arg("color") = Color{200, 200, 200, 255})
      .def("set_position", &Grid::setPosition, nb::arg("position"))
      .def("set_size", &Grid::setSize, nb::arg("size"))
      .def("set_spacing", &Grid::setSpacing, nb::arg("spacing"))
      .def("set_subdivisions", &Grid::setSubdivisions, nb::arg("subdivisions"))
      .def("set_color", &Grid::setColor, nb::arg("color"))
      .def("get_position", &Grid::getPosition)
      .def("get_size", &Grid::getSize)
      .def("get_spacing", &Grid::getSpacing)
      .def("get_subdivisions", &Grid::getSubdivisions)
      .def("get_color", &Grid::getColor)
      .def("get_mouse_cell", &Grid::getMouseCell)
      .def("get_bounds", &Grid::getBounds);

  nb::class_<LabelButton, GameObject>(m, "LabelButton")
      .def(nb::init<const std::string &, Vector2, Vector2, Color,
                     const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &LabelButton::setText, nb::arg("text"))
      .def("set_position", &LabelButton::setPosition, nb::arg("position"))
      .def("set_size", &LabelButton::setSize, nb::arg("size"))
      .def("set_fg_color", &LabelButton::setFgColor, nb::arg("color"))
      .def("set_font_name", &LabelButton::setFontName, nb::arg("font_name"))
      .def("set_font_size", &LabelButton::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &LabelButton::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &LabelButton::getText)
      .def("get_position", &LabelButton::getPosition)
      .def("get_size", &LabelButton::getSize)
      .def("get_fg_color", &LabelButton::getFgColor)
      .def("get_bounds", &LabelButton::getBounds)
      .def("on_click", &LabelButton::onClick, nb::arg("callback"));

  nb::class_<ToggleGroup, GameObject>(m, "ToggleGroup")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int,
                     Color, Color, Color, Color, const std::string &, float,
                     float>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = 0, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255},
           nb::arg("bg_active") = Color{80, 80, 80, 255},
           nb::arg("fg_active") = Color{255, 255, 255, 255},
           nb::arg("font_name") = "default", nb::arg("font_size") = 20.0f,
           nb::arg("font_spacing") = 1.0f)
      .def("set_items", &ToggleGroup::setItems, nb::arg("items"))
      .def("set_position", &ToggleGroup::setPosition, nb::arg("position"))
      .def("set_size", &ToggleGroup::setSize, nb::arg("size"))
      .def("set_active", &ToggleGroup::setActive, nb::arg("active"))
      .def("set_bg_color", &ToggleGroup::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ToggleGroup::setFgColor, nb::arg("color"))
      .def("set_bg_active_color", &ToggleGroup::setBgActiveColor, nb::arg("color"))
      .def("set_fg_active_color", &ToggleGroup::setFgActiveColor, nb::arg("color"))
      .def("set_font_name", &ToggleGroup::setFontName, nb::arg("font_name"))
      .def("set_font_size", &ToggleGroup::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &ToggleGroup::setFontSpacing, nb::arg("spacing"))
      .def("get_items", &ToggleGroup::getItems)
      .def("get_position", &ToggleGroup::getPosition)
      .def("get_size", &ToggleGroup::getSize)
      .def("get_active", &ToggleGroup::getActive)
      .def("get_bg_color", &ToggleGroup::getBgColor)
      .def("get_fg_color", &ToggleGroup::getFgColor)
      .def("get_bg_active_color", &ToggleGroup::getBgActiveColor)
      .def("get_fg_active_color", &ToggleGroup::getFgActiveColor)
      .def("get_bounds", &ToggleGroup::getBounds)
      .def("on_change", &ToggleGroup::onChange, nb::arg("callback"));

  nb::class_<ToggleSlider, GameObject>(m, "ToggleSlider")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = 0, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_items", &ToggleSlider::setItems, nb::arg("items"))
      .def("set_position", &ToggleSlider::setPosition, nb::arg("position"))
      .def("set_size", &ToggleSlider::setSize, nb::arg("size"))
      .def("set_active", &ToggleSlider::setActive, nb::arg("active"))
      .def("set_bg_color", &ToggleSlider::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ToggleSlider::setFgColor, nb::arg("color"))
      .def("set_font_name", &ToggleSlider::setFontName, nb::arg("font_name"))
      .def("set_font_size", &ToggleSlider::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &ToggleSlider::setFontSpacing, nb::arg("spacing"))
      .def("get_items", &ToggleSlider::getItems)
      .def("get_position", &ToggleSlider::getPosition)
      .def("get_size", &ToggleSlider::getSize)
      .def("get_active", &ToggleSlider::getActive)
      .def("get_bg_color", &ToggleSlider::getBgColor)
      .def("get_fg_color", &ToggleSlider::getFgColor)
      .def("get_bounds", &ToggleSlider::getBounds)
      .def("on_change", &ToggleSlider::onChange, nb::arg("callback"));

  nb::class_<ComboBox, GameObject>(m, "ComboBox")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = 0, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_items", &ComboBox::setItems, nb::arg("items"))
      .def("set_position", &ComboBox::setPosition, nb::arg("position"))
      .def("set_size", &ComboBox::setSize, nb::arg("size"))
      .def("set_active", &ComboBox::setActive, nb::arg("active"))
      .def("set_bg_color", &ComboBox::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ComboBox::setFgColor, nb::arg("color"))
      .def("set_font_name", &ComboBox::setFontName, nb::arg("font_name"))
      .def("set_font_size", &ComboBox::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &ComboBox::setFontSpacing, nb::arg("spacing"))
      .def("get_items", &ComboBox::getItems)
      .def("get_position", &ComboBox::getPosition)
      .def("get_size", &ComboBox::getSize)
      .def("get_active", &ComboBox::getActive)
      .def("get_bg_color", &ComboBox::getBgColor)
      .def("get_fg_color", &ComboBox::getFgColor)
      .def("get_bounds", &ComboBox::getBounds)
      .def("on_change", &ComboBox::onChange, nb::arg("callback"));

  nb::class_<SliderBar, GameObject>(m, "SliderBar")
      .def(nb::init<Vector2, Vector2, float, float, float, Color, Color,
                     const std::string &, const std::string &>(),
           nb::arg("position"), nb::arg("size"), nb::arg("value") = 0.0f,
           nb::arg("min_value") = 0.0f, nb::arg("max_value") = 1.0f,
           nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("text_left") = "",
           nb::arg("text_right") = "")
      .def("set_position", &SliderBar::setPosition, nb::arg("position"))
      .def("set_size", &SliderBar::setSize, nb::arg("size"))
      .def("set_value", &SliderBar::setValue, nb::arg("value"))
      .def("set_range", &SliderBar::setRange, nb::arg("min_value"), nb::arg("max_value"))
      .def("set_bg_color", &SliderBar::setBgColor, nb::arg("color"))
      .def("set_fg_color", &SliderBar::setFgColor, nb::arg("color"))
      .def("set_text_left", &SliderBar::setTextLeft, nb::arg("text"))
      .def("set_text_right", &SliderBar::setTextRight, nb::arg("text"))
      .def("get_position", &SliderBar::getPosition)
      .def("get_size", &SliderBar::getSize)
      .def("get_value", &SliderBar::getValue)
      .def("get_min_value", &SliderBar::getMinValue)
      .def("get_max_value", &SliderBar::getMaxValue)
      .def("get_bg_color", &SliderBar::getBgColor)
      .def("get_fg_color", &SliderBar::getFgColor)
      .def("get_bounds", &SliderBar::getBounds)
      .def("on_change", &SliderBar::onChange, nb::arg("callback"));

  nb::class_<DropdownBox, GameObject>(m, "DropdownBox")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = 0, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_items", &DropdownBox::setItems, nb::arg("items"))
      .def("set_position", &DropdownBox::setPosition, nb::arg("position"))
      .def("set_size", &DropdownBox::setSize, nb::arg("size"))
      .def("set_active", &DropdownBox::setActive, nb::arg("active"))
      .def("set_open", &DropdownBox::setOpen, nb::arg("open"))
      .def("set_bg_color", &DropdownBox::setBgColor, nb::arg("color"))
      .def("set_fg_color", &DropdownBox::setFgColor, nb::arg("color"))
      .def("set_font_name", &DropdownBox::setFontName, nb::arg("font_name"))
      .def("set_font_size", &DropdownBox::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &DropdownBox::setFontSpacing, nb::arg("spacing"))
      .def("get_items", &DropdownBox::getItems)
      .def("get_position", &DropdownBox::getPosition)
      .def("get_size", &DropdownBox::getSize)
      .def("get_active", &DropdownBox::getActive)
      .def("is_open", &DropdownBox::isOpen)
      .def("get_bg_color", &DropdownBox::getBgColor)
      .def("get_fg_color", &DropdownBox::getFgColor)
      .def("get_bounds", &DropdownBox::getBounds)
      .def("on_change", &DropdownBox::onChange, nb::arg("callback"));

  nb::class_<Spinner, GameObject>(m, "Spinner")
      .def(nb::init<const std::string &, Vector2, Vector2, int, int, int,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("value") = 0, nb::arg("min_value") = 0,
           nb::arg("max_value") = 100, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &Spinner::setText, nb::arg("text"))
      .def("set_position", &Spinner::setPosition, nb::arg("position"))
      .def("set_size", &Spinner::setSize, nb::arg("size"))
      .def("set_value", &Spinner::setValue, nb::arg("value"))
      .def("set_range", &Spinner::setRange, nb::arg("min_value"), nb::arg("max_value"))
      .def("set_edit_mode", &Spinner::setEditMode, nb::arg("edit_mode"))
      .def("set_bg_color", &Spinner::setBgColor, nb::arg("color"))
      .def("set_fg_color", &Spinner::setFgColor, nb::arg("color"))
      .def("set_font_name", &Spinner::setFontName, nb::arg("font_name"))
      .def("set_font_size", &Spinner::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &Spinner::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &Spinner::getText)
      .def("get_position", &Spinner::getPosition)
      .def("get_size", &Spinner::getSize)
      .def("get_value", &Spinner::getValue)
      .def("get_min_value", &Spinner::getMinValue)
      .def("get_max_value", &Spinner::getMaxValue)
      .def("is_edit_mode", &Spinner::isEditMode)
      .def("get_bg_color", &Spinner::getBgColor)
      .def("get_fg_color", &Spinner::getFgColor)
      .def("get_bounds", &Spinner::getBounds)
      .def("on_change", &Spinner::onChange, nb::arg("callback"));

  nb::class_<ValueBox, GameObject>(m, "ValueBox")
      .def(nb::init<const std::string &, Vector2, Vector2, int, int, int,
                     Color, Color, const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("value") = 0, nb::arg("min_value") = 0,
           nb::arg("max_value") = 100, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &ValueBox::setText, nb::arg("text"))
      .def("set_position", &ValueBox::setPosition, nb::arg("position"))
      .def("set_size", &ValueBox::setSize, nb::arg("size"))
      .def("set_value", &ValueBox::setValue, nb::arg("value"))
      .def("set_range", &ValueBox::setRange, nb::arg("min_value"), nb::arg("max_value"))
      .def("set_edit_mode", &ValueBox::setEditMode, nb::arg("edit_mode"))
      .def("set_bg_color", &ValueBox::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ValueBox::setFgColor, nb::arg("color"))
      .def("set_font_name", &ValueBox::setFontName, nb::arg("font_name"))
      .def("set_font_size", &ValueBox::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &ValueBox::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &ValueBox::getText)
      .def("get_position", &ValueBox::getPosition)
      .def("get_size", &ValueBox::getSize)
      .def("get_value", &ValueBox::getValue)
      .def("get_min_value", &ValueBox::getMinValue)
      .def("get_max_value", &ValueBox::getMaxValue)
      .def("is_edit_mode", &ValueBox::isEditMode)
      .def("get_bg_color", &ValueBox::getBgColor)
      .def("get_fg_color", &ValueBox::getFgColor)
      .def("get_bounds", &ValueBox::getBounds)
      .def("on_change", &ValueBox::onChange, nb::arg("callback"));

  nb::class_<ValueBoxFloat, GameObject>(m, "ValueBoxFloat")
      .def(nb::init<const std::string &, Vector2, Vector2, float, Color, Color,
                     const std::string &, float, float>(),
           nb::arg("text"), nb::arg("position"), nb::arg("size"),
           nb::arg("value") = 0.0f, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255}, nb::arg("font_name") = "default",
           nb::arg("font_size") = 20.0f, nb::arg("font_spacing") = 1.0f)
      .def("set_text", &ValueBoxFloat::setText, nb::arg("text"))
      .def("set_position", &ValueBoxFloat::setPosition, nb::arg("position"))
      .def("set_size", &ValueBoxFloat::setSize, nb::arg("size"))
      .def("set_value", &ValueBoxFloat::setValue, nb::arg("value"))
      .def("set_edit_mode", &ValueBoxFloat::setEditMode, nb::arg("edit_mode"))
      .def("set_bg_color", &ValueBoxFloat::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ValueBoxFloat::setFgColor, nb::arg("color"))
      .def("set_font_name", &ValueBoxFloat::setFontName, nb::arg("font_name"))
      .def("set_font_size", &ValueBoxFloat::setFontSize, nb::arg("size"))
      .def("set_font_spacing", &ValueBoxFloat::setFontSpacing, nb::arg("spacing"))
      .def("get_text", &ValueBoxFloat::getText)
      .def("get_position", &ValueBoxFloat::getPosition)
      .def("get_size", &ValueBoxFloat::getSize)
      .def("get_value", &ValueBoxFloat::getValue)
      .def("is_edit_mode", &ValueBoxFloat::isEditMode)
      .def("get_bg_color", &ValueBoxFloat::getBgColor)
      .def("get_fg_color", &ValueBoxFloat::getFgColor)
      .def("get_bounds", &ValueBoxFloat::getBounds)
      .def("on_change", &ValueBoxFloat::onChange, nb::arg("callback"));

  nb::class_<WindowBox, GameObject>(m, "WindowBox")
      .def(nb::init<const std::string &, Vector2, Vector2>(), nb::arg("title"),
           nb::arg("position"), nb::arg("size"))
      .def(
          "add",
          [](std::shared_ptr<WindowBox> self, std::shared_ptr<GameObject> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def(
          "__lshift__",
          [](std::shared_ptr<WindowBox> self, std::shared_ptr<GameObject> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def("set_title", &WindowBox::setTitle, nb::arg("title"))
      .def("set_position", &WindowBox::setPosition, nb::arg("position"))
      .def("set_size", &WindowBox::setSize, nb::arg("size"))
      .def("get_title", &WindowBox::getTitle)
      .def("get_position", &WindowBox::getPosition)
      .def("get_size", &WindowBox::getSize)
      .def("get_bounds", &WindowBox::getBounds)
      .def("on_close", &WindowBox::onClose, nb::arg("callback"));

  nb::class_<ScrollPanel, GameObject>(m, "ScrollPanel")
      .def(nb::init<Vector2, Vector2, Rectangle>(), nb::arg("position"),
           nb::arg("size"), nb::arg("content"))
      .def(
          "add",
          [](std::shared_ptr<ScrollPanel> self, std::shared_ptr<GameObject> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def(
          "__lshift__",
          [](std::shared_ptr<ScrollPanel> self, std::shared_ptr<GameObject> child) {
            *self << child;
            return self;
          },
          nb::arg("child"))
      .def("set_position", &ScrollPanel::setPosition, nb::arg("position"))
      .def("set_size", &ScrollPanel::setSize, nb::arg("size"))
      .def("set_content", &ScrollPanel::setContent, nb::arg("content"))
      .def("set_scroll", &ScrollPanel::setScroll, nb::arg("scroll"))
      .def("get_position", &ScrollPanel::getPosition)
      .def("get_size", &ScrollPanel::getSize)
      .def("get_content", &ScrollPanel::getContent)
      .def("get_scroll", &ScrollPanel::getScroll)
      .def("get_view", &ScrollPanel::getView)
      .def("get_bounds", &ScrollPanel::getBounds);

  nb::class_<ListView, GameObject>(m, "ListView")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int,
                     Color, Color>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = -1, nb::arg("bg") = Color{200, 200, 200, 255},
           nb::arg("fg") = Color{80, 80, 80, 255})
      .def("set_items", &ListView::setItems, nb::arg("items"))
      .def("set_position", &ListView::setPosition, nb::arg("position"))
      .def("set_size", &ListView::setSize, nb::arg("size"))
      .def("set_scroll_index", &ListView::setScrollIndex, nb::arg("scroll_index"))
      .def("set_active", &ListView::setActive, nb::arg("active"))
      .def("set_bg_color", &ListView::setBgColor, nb::arg("color"))
      .def("set_fg_color", &ListView::setFgColor, nb::arg("color"))
      .def("get_items", &ListView::getItems)
      .def("get_position", &ListView::getPosition)
      .def("get_size", &ListView::getSize)
      .def("get_scroll_index", &ListView::getScrollIndex)
      .def("get_active", &ListView::getActive)
      .def("get_bg_color", &ListView::getBgColor)
      .def("get_fg_color", &ListView::getFgColor)
      .def("get_focus_index", &ListView::getFocusIndex)
      .def("get_bounds", &ListView::getBounds)
      .def("on_change", &ListView::onChange, nb::arg("callback"));

  nb::class_<TabBar, GameObject>(m, "TabBar")
      .def(nb::init<const std::vector<std::string> &, Vector2, Vector2, int>(),
           nb::arg("items"), nb::arg("position"), nb::arg("size"),
           nb::arg("active") = 0)
      .def("set_items", &TabBar::setItems, nb::arg("items"))
      .def("set_position", &TabBar::setPosition, nb::arg("position"))
      .def("set_size", &TabBar::setSize, nb::arg("size"))
      .def("set_hscroll", &TabBar::setHscroll, nb::arg("hscroll"))
      .def("set_active", &TabBar::setActive, nb::arg("active"))
      .def("get_items", &TabBar::getItems)
      .def("get_position", &TabBar::getPosition)
      .def("get_size", &TabBar::getSize)
      .def("get_hscroll", &TabBar::getHscroll)
      .def("get_active", &TabBar::getActive)
      .def("get_focus_index", &TabBar::getFocusIndex)
      .def("get_bounds", &TabBar::getBounds)
      .def("on_change", &TabBar::onChange, nb::arg("callback"));

  nb::class_<MessageBox, GameObject>(m, "MessageBox")
      .def(nb::init<const std::string &, const std::string &,
                     const std::vector<std::string> &, Vector2, Vector2>(),
           nb::arg("title"), nb::arg("message"), nb::arg("buttons"),
           nb::arg("position"), nb::arg("size"))
      .def("set_title", &MessageBox::setTitle, nb::arg("title"))
      .def("set_message", &MessageBox::setMessage, nb::arg("message"))
      .def("set_buttons", &MessageBox::setButtons, nb::arg("buttons"))
      .def("set_position", &MessageBox::setPosition, nb::arg("position"))
      .def("set_size", &MessageBox::setSize, nb::arg("size"))
      .def("get_title", &MessageBox::getTitle)
      .def("get_message", &MessageBox::getMessage)
      .def("get_buttons", &MessageBox::getButtons)
      .def("get_position", &MessageBox::getPosition)
      .def("get_size", &MessageBox::getSize)
      .def("get_last_button", &MessageBox::getLastButton)
      .def("get_bounds", &MessageBox::getBounds)
      .def("on_button", &MessageBox::onButton, nb::arg("callback"));

  nb::class_<TextInputBox, GameObject>(m, "TextInputBox")
      .def(nb::init<const std::string &, const std::string &,
                     const std::vector<std::string> &, Vector2, Vector2>(),
           nb::arg("title"), nb::arg("message"), nb::arg("buttons"),
           nb::arg("position"), nb::arg("size"))
      .def("set_title", &TextInputBox::setTitle, nb::arg("title"))
      .def("set_message", &TextInputBox::setMessage, nb::arg("message"))
      .def("set_content", &TextInputBox::setContent, nb::arg("content"))
      .def("set_buttons", &TextInputBox::setButtons, nb::arg("buttons"))
      .def("set_secret_view", &TextInputBox::setSecretView, nb::arg("secret_view"))
      .def("set_position", &TextInputBox::setPosition, nb::arg("position"))
      .def("set_size", &TextInputBox::setSize, nb::arg("size"))
      .def("get_title", &TextInputBox::getTitle)
      .def("get_message", &TextInputBox::getMessage)
      .def("get_content", &TextInputBox::getContent)
      .def("get_buttons", &TextInputBox::getButtons)
      .def("is_secret_view", &TextInputBox::isSecretView)
      .def("get_position", &TextInputBox::getPosition)
      .def("get_size", &TextInputBox::getSize)
      .def("get_last_button", &TextInputBox::getLastButton)
      .def("get_bounds", &TextInputBox::getBounds)
      .def("on_button", &TextInputBox::onButton, nb::arg("callback"));

  nb::class_<ColorPicker, GameObject>(m, "ColorPicker")
      .def(nb::init<Vector2, Vector2, Color>(), nb::arg("position"),
           nb::arg("size"), nb::arg("color") = Color{230, 41, 55, 255})
      .def("set_color", &ColorPicker::setColor, nb::arg("color"))
      .def("set_position", &ColorPicker::setPosition, nb::arg("position"))
      .def("set_size", &ColorPicker::setSize, nb::arg("size"))
      .def("get_color", &ColorPicker::getColor)
      .def("get_position", &ColorPicker::getPosition)
      .def("get_size", &ColorPicker::getSize)
      .def("get_bounds", &ColorPicker::getBounds)
      .def("on_change", &ColorPicker::onChange, nb::arg("callback"));

  nb::class_<ColorPanel, GameObject>(m, "ColorPanel")
      .def(nb::init<Vector2, Vector2, Color>(), nb::arg("position"),
           nb::arg("size"), nb::arg("color") = Color{230, 41, 55, 255})
      .def("set_color", &ColorPanel::setColor, nb::arg("color"))
      .def("set_position", &ColorPanel::setPosition, nb::arg("position"))
      .def("set_size", &ColorPanel::setSize, nb::arg("size"))
      .def("get_color", &ColorPanel::getColor)
      .def("get_position", &ColorPanel::getPosition)
      .def("get_size", &ColorPanel::getSize)
      .def("get_bounds", &ColorPanel::getBounds)
      .def("on_change", &ColorPanel::onChange, nb::arg("callback"));

  nb::class_<ColorBarAlpha, GameObject>(m, "ColorBarAlpha")
      .def(nb::init<Vector2, Vector2, float>(), nb::arg("position"),
           nb::arg("size"), nb::arg("alpha") = 1.0f)
      .def("set_alpha", &ColorBarAlpha::setAlpha, nb::arg("alpha"))
      .def("set_position", &ColorBarAlpha::setPosition, nb::arg("position"))
      .def("set_size", &ColorBarAlpha::setSize, nb::arg("size"))
      .def("get_alpha", &ColorBarAlpha::getAlpha)
      .def("get_position", &ColorBarAlpha::getPosition)
      .def("get_size", &ColorBarAlpha::getSize)
      .def("get_bounds", &ColorBarAlpha::getBounds)
      .def("on_change", &ColorBarAlpha::onChange, nb::arg("callback"));

  nb::class_<ColorBarHue, GameObject>(m, "ColorBarHue")
      .def(nb::init<Vector2, Vector2, float>(), nb::arg("position"),
           nb::arg("size"), nb::arg("hue") = 0.0f)
      .def("set_hue", &ColorBarHue::setHue, nb::arg("hue"))
      .def("set_position", &ColorBarHue::setPosition, nb::arg("position"))
      .def("set_size", &ColorBarHue::setSize, nb::arg("size"))
      .def("get_hue", &ColorBarHue::getHue)
      .def("get_position", &ColorBarHue::getPosition)
      .def("get_size", &ColorBarHue::getSize)
      .def("get_bounds", &ColorBarHue::getBounds)
      .def("on_change", &ColorBarHue::onChange, nb::arg("callback"));

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
      .def(
          "__lshift__",
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
