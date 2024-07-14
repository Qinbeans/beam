set(EVENT_SRC event.cpp)
set(EVENT_OBJ ${OBJECTS_DIR}/event.o)

add_library(event OBJECT ${EVENT_SRC})

set_property(TARGET event PROPERTY
    OBJECT_OUTPUTS ${EVENT_OBJ}
    OBJECT_DEPENDS ${EVENT_SRC}
)
