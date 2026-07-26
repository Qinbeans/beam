#!/usr/bin/env python3
"""Bump the project version in CMakeLists.txt and pyproject.toml in lockstep.

CMakeLists.txt's `project(Beam VERSION X.Y.Z ...)` is treated as the source
of truth; pyproject.toml's `version = "X.Y.Z"` is kept in sync with it.
Prints the new version (e.g. "1.2.0") to stdout.
"""

import argparse
import pathlib
import re

ROOT = pathlib.Path(__file__).resolve().parent.parent
CMAKE_FILE = ROOT / "CMakeLists.txt"
PYPROJECT_FILE = ROOT / "pyproject.toml"

CMAKE_VERSION_RE = re.compile(r"(project\(Beam VERSION )(\d+)\.(\d+)\.(\d+)(\s)")
PYPROJECT_VERSION_RE = re.compile(r'(?m)(^version = ")(\d+)\.(\d+)\.(\d+)(")')


def read_current_version() -> tuple[int, int, int]:
    """Read the current X.Y.Z version out of CMakeLists.txt."""
    match = CMAKE_VERSION_RE.search(CMAKE_FILE.read_text())
    if not match:
        raise SystemExit("could not find a version in CMakeLists.txt")
    return int(match.group(2)), int(match.group(3)), int(match.group(4))


def bump(version: tuple[int, int, int], kind: str) -> tuple[int, int, int]:
    """Return `version` bumped at the major, minor, or patch component."""
    major, minor, patch = version
    if kind == "major":
        return major + 1, 0, 0
    if kind == "minor":
        return major, minor + 1, 0
    if kind == "patch":
        return major, minor, patch + 1
    raise SystemExit(f"unknown bump kind: {kind!r}")


def write_version(new_version: tuple[int, int, int]) -> None:
    """Write `new_version` into CMakeLists.txt and pyproject.toml."""
    new_str = "{}.{}.{}".format(*new_version)

    cmake_text, n = CMAKE_VERSION_RE.subn(
        rf"\g<1>{new_str}\g<5>", CMAKE_FILE.read_text()
    )
    if n != 1:
        raise SystemExit("expected exactly one version to update in CMakeLists.txt")
    CMAKE_FILE.write_text(cmake_text)

    pyproject_text, n = PYPROJECT_VERSION_RE.subn(
        rf"\g<1>{new_str}\g<5>", PYPROJECT_FILE.read_text()
    )
    if n != 1:
        raise SystemExit("expected exactly one version to update in pyproject.toml")
    PYPROJECT_FILE.write_text(pyproject_text)


def main() -> None:
    """Bump the version according to CLI args and print the new version."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("bump", choices=["major", "minor", "patch"])
    args = parser.parse_args()

    new_version = bump(read_current_version(), args.bump)
    write_version(new_version)
    print("{}.{}.{}".format(*new_version))


if __name__ == "__main__":
    main()
