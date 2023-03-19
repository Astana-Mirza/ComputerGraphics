set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable (lab4
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/controls.cpp"
	"${SRC_DIR}/kb_knot_storage.cpp"
)

target_include_directories(lab4 PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(lab4 PRIVATE ${LINK_DIRS})
target_link_libraries(lab4 ${ALL_LIBS})
