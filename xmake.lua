add_rules("mode.debug", "mode.release")

add_requires(
    "glfw",
    "glad",
    "glm",
    "tinyobjloader",
    "stb",
    "miniaudio")

target("brightengine")
    set_kind("binary")
    add_files(
        "src/*.cpp")

    add_packages(
        "glfw",
        "glad",
        "glm",
        "tinyobjloader",
        "stb",
        "miniaudio")