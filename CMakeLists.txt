cmake_minimum_required(VERSION 3.5.1)

# プロジェクト名
set(project_name renju_game_position)
project(${project_name} CXX)

# Build Type(Release or Debug)
set(CMAKE_BUILD_TYPE Release)

# ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

# コンパイルオプション
add_definitions("-Wall -std=c++14")

# インクルードパス
include_directories(${CMAKE_SOURCE_DIR}/realcore/src/include)
include_directories($ENV{BOOST_DIR}/include)

# ライブラリパス
link_directories($ENV{BOOST_DIR}/lib)

# ソースファイル
add_executable(
    ${project_name}
    ./realcore/src/CSVReader.cc
    ./realcore/src/Move.cc
    ./realcore/src/MoveList.cc
    ./realcore/src/BitSearch.cc
    ./realcore/src/BitBoard.cc
    ./realcore/src/Board.cc
    ./realcore/src/OpenState.cc
    ./realcore/src/BoardOpenState.cc
    ./realcore/src/LineNeighborhood.cc
    ./src/renju_game_position.cc
)

# ライブラリ
target_link_libraries(${project_name} boost_program_options)

if(APPLE OR WIN32)
  target_link_libraries(${project_name} boost_system-mt)
  target_link_libraries(${project_name} boost_thread-mt)
else()
  target_link_libraries(${project_name} boost_system)
  target_link_libraries(${project_name} boost_thread)
endif()
