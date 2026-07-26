"""Minimal beam demo driven from Python.

Mirrors the structure of the C++ basic_game example: build a scene, register it
with the App's SceneManager, then hand control to the engine's game loop. The
scene subclasses beam.Scene and overrides update/draw, so the C++ loop calls
back into Python each frame.

Run with:  python examples/python_demo/main.py
"""

import beam


class CounterScene(beam.Scene):
    def __init__(self):
        super().__init__("counter")
        self.frames = 0
        self.label = beam.Text("Hello from Python", 20, 100, 20)
        self.add(self.label)

    def on_enter(self, manager):
        manager.set_background_color(beam.Color(30, 30, 40, 255))

    def update(self, delta_time, manager):
        self.frames += 1
        self.label.set_text(f"frame {self.frames}")
        # Close after a short while so the demo exits on its own.
        if self.frames >= 120 or manager.is_key_pressed(256):  # 256 = ESC
            manager.close()

    def draw(self, manager):
        # Draw the children registered via add().
        beam.Scene.draw(self, manager)


def main():
    app = beam.App("beam python demo", 640, 360, 60)
    manager = app.get_manager()

    scene_manager = app.get_scene_manager()
    scene = CounterScene()
    scene_manager.add(scene)
    scene_manager.switch_to_scene("counter", manager)

    app.init()
    app.run()

    print(f"ran {scene.frames} frames")


if __name__ == "__main__":
    main()
