set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable (lab1
	"${SRC_DIR}/shader_program.cpp"
	"${SRC_DIR}/utils.cpp"
)

target_include_directories(common PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(common PRIVATE ${LINK_DIRS})
target_link_libraries(common ${ALL_LIBS})
