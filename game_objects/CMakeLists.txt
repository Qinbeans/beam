file(GLOB_RECURSE GAME_OBJECTS_SRC *.cpp)

add_library(game_objects OBJECT ${GAME_OBJECTS_SRC})

set_property(TARGET game_objects PROPERTY
    OBJECT_OUTPUTS ${GAME_OBJECTS_DIR}/*.o
    OBJECT_DEPENDS ${GAME_OBJECTS_SRC}
)
