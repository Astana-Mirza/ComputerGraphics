set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ui")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(common
	"${SRC_DIR}/shader_program.cpp"
	"${SRC_DIR}/utils.cpp"
	"${UI_DIR}/gl_window.cpp"
	"${UI_DIR}/main_window.cpp"
)

target_include_directories(common PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(common PRIVATE ${LINK_DIRS})
target_link_libraries(common ${ALL_LIBS})
