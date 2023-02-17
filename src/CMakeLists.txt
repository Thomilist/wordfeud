add_library(Letter Letter.cpp)
target_link_libraries(
    Letter
    Qt5::Widgets
)

add_library(Modifier Modifier.cpp)
target_link_libraries(
    Modifier
    Qt5::Widgets
)

add_library(Tile Tile.cpp)
target_link_libraries(
    Tile
    Qt5::Widgets
    Letter
    Modifier
)

add_library(Board Board.cpp)
target_link_libraries(
    Board
    Qt5::Widgets
    Tile
)

add_library(Settings Settings.cpp)
target_link_libraries(
    Settings
    Qt5::Widgets
    Language
)

add_library(Language Language.cpp)
target_link_libraries(
    Language
    Qt5::Widgets
    Letter
)

add_library(LetterPool LetterPool.cpp)
target_link_libraries(
    LetterPool
    Qt5::Widgets
    Letter
    Language
)

add_library(Player Player.cpp)
target_link_libraries(
    Player
    Qt5::Widgets
    Board
    LetterPool
    Letter
)

add_library(Game Game.cpp)
target_link_libraries(
    Game
    Qt5::Widgets
    Settings
    Letter
    Modifier
    Board
    Language
    LetterPool
    Player
    ButtonPanel
    Header
    Word
    ProposalInfo
    ModifierPattern
    PlayerAI
)

add_library(ButtonPanel ButtonPanel.cpp)
target_link_libraries(
    ButtonPanel
    Qt5::Widgets
)

add_library(Header Header.cpp)
target_link_libraries(
    Header
    Qt5::Widgets
    Player
)

add_library(Word Word.cpp)
target_link_libraries(
    Word
    Qt5::Widgets
    Tile
    Letter
    Modifier
)

add_library(ProposalInfo ProposalInfo.cpp)
target_link_libraries(
    ProposalInfo
    Qt5::Widgets
)

add_library(Wordfeud Wordfeud.cpp)
target_link_libraries(
    Wordfeud
    Qt5::Widgets
    Settings
    Game
    SettingsDialog
)

add_library(SettingsDialog SettingsDialog.cpp)
target_link_libraries(
    SettingsDialog
    Qt5::Widgets
)

add_library(ModifierPattern ModifierPattern.cpp)
target_link_libraries(
    ModifierPattern
    Qt5::Widgets
)

add_library(PlayerAI PlayerAI.cpp)
target_link_libraries(
    PlayerAI
    Qt5::Widgets
    Player
)