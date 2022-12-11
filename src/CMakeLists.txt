add_library(Letter Letter.cpp)

add_library(Modifier Modifier.cpp)

add_library(Tile Tile.cpp)
target_link_libraries(Tile Qt5::Widgets)

add_library(Board Board.cpp)
target_link_libraries(Board Qt5::Widgets Tile)

add_library(Settings Settings.cpp)
target_link_libraries(Settings Qt5::Widgets)