add_library(AboutDialog AboutDialog.cpp)
target_link_libraries(
    AboutDialog
    Qt5::Widgets
    Version
)
add_custom_target(
    AlwaysRebuildAboutDialog
    ALL
    COMMAND
    ${CMAKE_COMMAND} -E touch_nocreate ${CMAKE_SOURCE_DIR}/src/AboutDialog.cpp
)
add_dependencies(
    AboutDialog
    AlwaysRebuildAboutDialog
)

add_library(AbstractEditor AbstractEditor.cpp)
target_link_libraries(
    AbstractEditor
    Qt5::Widgets
    Settings
)

add_library(AbstractEditorLauncher AbstractEditorLauncher.cpp)
target_link_libraries(
    AbstractEditorLauncher
    Qt5::Widgets
    Settings
)

add_library(BoardEditor BoardEditor.cpp)
target_link_libraries(
    BoardEditor
    Qt5::Widgets
    AbstractEditor
    RenderedBoard
    RenderedTile
    Settings
)

add_library(BoardEditorLauncher BoardEditorLauncher.cpp)
target_link_libraries(
    BoardEditorLauncher
    Qt5::Widgets
    AbstractEditorLauncher
)

add_library(ButtonPanel ButtonPanel.cpp)
target_link_libraries(
    ButtonPanel
    Qt5::Widgets
    Settings
)

add_library(DictionaryEditor DictionaryEditor.cpp)
target_link_libraries(
    DictionaryEditor
    Qt5::Widgets
    AbstractEditor
    LetterEditorWidget
    LetterTable
    LetterTableModel
    Settings
)

add_library(DictionaryEditorLauncher DictionaryEditorLauncher.cpp)
target_link_libraries(
    DictionaryEditorLauncher
    Qt5::Widgets
    AbstractEditorLauncher
    Language
    Settings
)

add_library(DictionaryLookupDialog DictionaryLookupDialog.cpp)
target_link_libraries(
    DictionaryLookupDialog
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
    RecordTableModel
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

add_library(LetterEditorWidget LetterEditorWidget.cpp)
target_link_libraries(
    LetterEditorWidget
    Qt5::Widgets
    Letter
    RenderedTile
)

add_library(LetterPool LetterPool.cpp)
target_link_libraries(
    LetterPool
    Qt5::Widgets
    Language
    Letter
)

add_library(LetterTable LetterTable.cpp)
target_link_libraries(
    LetterTable
    Qt5::Widgets
)

add_library(LetterTableModel LetterTableModel.cpp)
target_link_libraries(
    LetterTableModel
    Qt5::Widgets
    Language
    Letter
)

add_library(Modifier Modifier.cpp)
target_link_libraries(
    Modifier
    Qt5::Widgets
    nlohmann_json::nlohmann_json
)

add_library(ModifierPattern ModifierPattern.cpp)
target_link_libraries(
    ModifierPattern
    Qt5::Widgets
    nlohmann_json::nlohmann_json
    Modifier
    VirtualBoard
    VirtualTile
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
    Player
    PlayerAIWorker
    PlayerSettings
    RenderedTile
    Settings
    VirtualBoard
    VirtualTile
)

add_library(PlayerAIWorker PlayerAIWorker.cpp)
target_link_libraries(
    PlayerAIWorker
    Qt5::Widgets
    Letter
    LetterPool
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

add_library(RecordContainer RecordContainer.cpp)
target_link_libraries(
    RecordContainer
    Qt5::Widgets
    Score
)

add_library(RecordDialog RecordDialog.cpp)
target_link_libraries(
    RecordDialog
    Qt5::Widgets
    RecordSortFilterProxyModel
)

add_library(RecordSortFilterProxyModel RecordSortFilterProxyModel.cpp)
target_link_libraries(
    RecordSortFilterProxyModel
    Qt5::Widgets
    Score
)

add_library(RecordTableModel RecordTableModel.cpp)
target_link_libraries(
    RecordTableModel
    Qt5::Widgets
    RecordContainer
    Score
)

add_library(RenderedTile RenderedTile.cpp)
target_link_libraries(
    RenderedTile
    Qt5::Widgets
    Letter
    LetterPool
    Modifier
    Settings
    VirtualTile
)

add_library(RenderedBoard RenderedBoard.cpp)
target_link_libraries(
    RenderedBoard
    Qt5::Widgets
    LetterPool
    RenderedTile
    Settings
    VirtualBoard
)

add_library(Score Score.cpp)
target_link_libraries(
    Score
    Qt5::Widgets
)

add_library(Settings Settings.cpp)
target_link_libraries(
    Settings
    Qt5::Widgets
    Language
    ModifierPattern
    PlayerSettings
    StartupProgressDialog
)

add_library(SettingsDialog SettingsDialog.cpp)
target_link_libraries(
    SettingsDialog
    Qt5::Widgets
    Settings
)

add_library(StartupProgressDialog StartupProgressDialog.cpp)
target_link_libraries(
    StartupProgressDialog
    Qt5::Widgets
)

add_library(UpdateDialog UpdateDialog.cpp)
target_link_libraries(
    UpdateDialog
    Qt5::Widgets
    Version
)

add_library(Version Version.cpp)
target_link_libraries(
    Version
    Qt5::Widgets
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

add_library(VirtualBoard VirtualBoard.cpp)
target_link_libraries(
    VirtualBoard
    Qt5::Widgets
    Letter
    Modifier
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
    AboutDialog
    BoardEditor
    BoardEditorLauncher
    DictionaryEditor
    DictionaryEditorLauncher
    DictionaryLookupDialog
    Game
    RecordDialog
    Settings
    SettingsDialog
    StartupProgressDialog
    UpdateDialog
)