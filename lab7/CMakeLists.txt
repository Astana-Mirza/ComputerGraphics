﻿set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable (lab7
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/controls.cpp"
	"${SRC_DIR}/callbacks.cpp"
	"${SRC_DIR}/shape.cpp"
	"${SRC_DIR}/light.cpp"
)

target_include_directories(lab7 PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(lab7 PRIVATE ${LINK_DIRS})
target_link_libraries(lab7 ${ALL_LIBS})
