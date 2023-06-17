add_library(ButtonPanel ButtonPanel.cpp)
target_link_libraries(
    ButtonPanel
    Qt5::Widgets
    Settings
)

add_library(Game Game.cpp)
target_link_libraries(
    Game
    Qt5::Widgets
    ButtonPanel
    Header
    Language
    Letter
    LetterPool
    Modifier
    ModifierPattern
    Player
    ProposalInfo
    RecordTracker
    RenderedBoard
    RenderedTile
    Settings
    Word
)

add_library(Header Header.cpp)
target_link_libraries(
    Header
    Qt5::Widgets
    Player
    Settings
)

add_library(Language Language.cpp)
target_link_libraries(
    Language
    Qt5::Widgets
    Letter
)

add_library(Letter Letter.cpp)
target_link_libraries(
    Letter
    Qt5::Widgets
)

add_library(LetterPool LetterPool.cpp)
target_link_libraries(
    LetterPool
    Qt5::Widgets
    Language
    Letter
)

add_library(Modifier Modifier.cpp)
target_link_libraries(
    Modifier
    Qt5::Widgets
)

add_library(ModifierPattern ModifierPattern.cpp)
target_link_libraries(
    ModifierPattern
    Qt5::Widgets
    Modifier
)

add_library(Player Player.cpp)
target_link_libraries(
    Player
    Qt5::Widgets
    Letter
    LetterPool
    PlayerAI
    RenderedBoard
    RenderedTile
    Settings
)

add_library(PlayerAI PlayerAI.cpp)
target_link_libraries(
    PlayerAI
    Qt5::Widgets
    Qt5::Concurrent
    Player
    RenderedTile
    Settings
    VirtualBoard
    VirtualTile
)

add_library(PlayerSettings PlayerSettings.cpp)
target_link_libraries(
    PlayerSettings
    Qt5::Widgets
)

add_library(ProposalInfo ProposalInfo.cpp)
target_link_libraries(
    ProposalInfo
    Qt5::Widgets
    Settings
)

add_library(RecordDialog RecordDialog.cpp)
target_link_libraries(
    RecordDialog
    Qt5::Widgets
)

add_library(RecordTracker RecordTracker.cpp)
target_link_libraries(
    RecordTracker
    Qt5::Widgets
)

add_library(RenderedTile RenderedTile.cpp)
target_link_libraries(
    RenderedTile
    Qt5::Widgets
    Letter
    Modifier
    Settings
    VirtualTile
)

add_library(RenderedBoard RenderedBoard.cpp)
target_link_libraries(
    RenderedBoard
    Qt5::Widgets
    RenderedTile
    Settings
    VirtualBoard
)

add_library(Settings Settings.cpp)
target_link_libraries(
    Settings
    Qt5::Widgets
    Language
    ModifierPattern
    PlayerSettings
)

add_library(SettingsDialog SettingsDialog.cpp)
target_link_libraries(
    SettingsDialog
    Qt5::Widgets
    Settings
)

add_library(VirtualBoard VirtualBoard.cpp)
target_link_libraries(
    VirtualBoard
    Qt5::Widgets
    Letter
    Settings
    VirtualTile
    Word
)

add_library(VirtualTile VirtualTile.cpp)
target_link_libraries(
    VirtualTile
    Letter
    Modifier
)

add_library(Word Word.cpp)
target_link_libraries(
    Word
    Qt5::Widgets
    Letter
    Modifier
    VirtualTile
)

add_library(Wordfeud Wordfeud.cpp)
target_link_libraries(
    Wordfeud
    Qt5::Widgets
    Game
    RecordDialog
    Settings
    SettingsDialog
)