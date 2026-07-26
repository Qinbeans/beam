"""Minimal beam demo driven from Python.

Mirrors the structure of the C++ basic_game example: build a scene, register it
with the App's SceneManager, then hand control to the engine's game loop. The
scene subclasses beam.Scene and overrides update/draw, so the C++ loop calls
back into Python each frame.

Run with:  python examples/python_demo/main.py
"""

import beam

MAX_FRAMES = 120
ESC_KEY = 256


class CounterScene(beam.Scene):
    """Scene that counts frames and closes the app after MAX_FRAMES."""

    def __init__(self) -> None:
        """Build the scene, its frame-counter label, and an options panel."""
        super().__init__("counter")
        self.frames = 0
        self.label = beam.Text("Hello from Python", 20, 100)

        # Frame << CheckBox, then Scene << Frame: validates that operator<<
        # (bound as __lshift__ in Python) works for nesting game objects, not
        # just the .add() alias.
        self.checkbox = beam.CheckBox("Enable", beam.Vector2(20, 140), beam.Vector2(20, 20))
        self.panel = beam.Frame(
            "options", beam.Vector2(0, 0), beam.Rectangle(10, 120, 160, 60), beam.Color(30, 30, 40, 200)
        )
        self.panel << self.checkbox

        self << self.label << self.panel

    def on_enter(self, manager: beam.Manager) -> None:
        """Set the background color when the scene becomes active."""
        manager.set_background_color(beam.Color(30, 30, 40, 255))

    def update(self, _delta_time: float, manager: beam.Manager) -> None:
        """Advance the frame counter and close after MAX_FRAMES or ESC."""
        self.frames += 1
        self.label.set_text(f"frame {self.frames}")
        ch = manager.get_char_pressed()  # Clear the char buffer so ESC doesn't get stuck in it.
        print(f"char pressed: {ch}")
        # Close after a short while so the demo exits on its own.
        if self.frames >= MAX_FRAMES or manager.is_key_pressed(ESC_KEY):
            manager.close()

    def draw(self, manager: beam.Manager) -> None:
        """Draw the children registered via <<."""
        beam.Scene.draw(self, manager)


def main() -> None:
    """Run the counter scene until it closes itself."""
    app = beam.App("beam python demo", 640, 360, 60)
    manager = app.get_manager()

    scene_manager = app.get_scene_manager()
    scene = CounterScene()
    scene_manager << scene
    scene_manager.switch_to_scene("counter", manager)
    app.init()
    app.run()

    print(f"ran {scene.frames} frames")


if __name__ == "__main__":
    main()
