cmake_minimum_required (VERSION 3.12)

#
project(app C)

#源文件目录(创建变量)
set(src_path    ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(inc_path    ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(jxc_path    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/jxc)

include_directories(
    ${inc_path}
    ${jxc_path}/include
)

link_directories(
	${jxc_path}/lib
)


#扫描 src_path目录下的所有c文件，并存如SRC_FILES中
file(GLOB SRC_FILES "${src_path}/*.c")

add_executable(app ${SRC_FILES})

target_link_libraries(app jxc)
target_link_libraries(app pthread)

# find_package(Threads REQUIRED)
# target_link_libraries(app PRIVATE Threads::Threads)