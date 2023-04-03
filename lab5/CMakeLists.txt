set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable (lab5
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/controls.cpp"
)

target_include_directories(lab5 PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(lab5 PRIVATE ${LINK_DIRS})
target_link_libraries(lab5 ${ALL_LIBS})
