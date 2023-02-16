set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable (lab1_2
	"${SRC_DIR}/main.cpp"
	"${SRC_DIR}/controls.cpp"
)

target_include_directories(lab1_2 PRIVATE ${INCLUDE_DIRS} ${INCLUDE_DIR})
target_link_directories(lab1_2 PRIVATE ${LINK_DIRS})
target_link_libraries(lab1_2 ${ALL_LIBS})
