add_library(AboutDialog AboutDialog.cpp)
target_link_libraries(
    AboutDialog
    Qt::Widgets
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
    Qt::Widgets
    Settings
)

add_library(AbstractEditorLauncher AbstractEditorLauncher.cpp)
target_link_libraries(
    AbstractEditorLauncher
    Qt::Widgets
    Settings
)

add_library(BoardEditor BoardEditor.cpp)
target_link_libraries(
    BoardEditor
    Qt::Widgets
    AbstractEditor
    RenderedBoard
    RenderedTile
    Settings
)

add_library(BoardEditorLauncher BoardEditorLauncher.cpp)
target_link_libraries(
    BoardEditorLauncher
    Qt::Widgets
    AbstractEditorLauncher
)

add_library(ButtonPanel ButtonPanel.cpp)
target_link_libraries(
    ButtonPanel
    Qt::Widgets
    Settings
)

add_library(DictionaryEditor DictionaryEditor.cpp)
target_link_libraries(
    DictionaryEditor
    Qt::Widgets
    AbstractEditor
    LetterEditorWidget
    LetterTable
    LetterTableModel
    Settings
)

add_library(DictionaryEditorLauncher DictionaryEditorLauncher.cpp)
target_link_libraries(
    DictionaryEditorLauncher
    Qt::Widgets
    AbstractEditorLauncher
    Language
    Settings
)

add_library(DictionaryLookupDialog DictionaryLookupDialog.cpp)
target_link_libraries(
    DictionaryLookupDialog
    Qt::Widgets
    Settings
)

add_library(Game Game.cpp)
target_link_libraries(
    Game
    Qt::Widgets
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
    Qt::Widgets
    Player
    Settings
)

add_library(Language Language.cpp)
target_link_libraries(
    Language
    Qt::Widgets
    Letter
)

add_library(Letter Letter.cpp)
target_link_libraries(
    Letter
    Qt::Widgets
)

add_library(LetterEditorWidget LetterEditorWidget.cpp)
target_link_libraries(
    LetterEditorWidget
    Qt::Widgets
    Letter
    RenderedTile
)

add_library(LetterPool LetterPool.cpp)
target_link_libraries(
    LetterPool
    Qt::Widgets
    Language
    Letter
)

add_library(LetterTable LetterTable.cpp)
target_link_libraries(
    LetterTable
    Qt::Widgets
)

add_library(LetterTableModel LetterTableModel.cpp)
target_link_libraries(
    LetterTableModel
    Qt::Widgets
    Language
    Letter
)

add_library(Modifier Modifier.cpp)
target_link_libraries(
    Modifier
    Qt::Widgets
    nlohmann_json::nlohmann_json
)

add_library(ModifierPattern ModifierPattern.cpp)
target_link_libraries(
    ModifierPattern
    Qt::Widgets
    nlohmann_json::nlohmann_json
    Modifier
    VirtualBoard
    VirtualTile
)

add_library(Player Player.cpp)
target_link_libraries(
    Player
    Qt::Widgets
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
    Qt::Widgets
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
    Qt::Widgets
    Letter
    LetterPool
    Settings
    VirtualBoard
    VirtualTile
)

add_library(PlayerSettings PlayerSettings.cpp)
target_link_libraries(
    PlayerSettings
    Qt::Widgets
)

add_library(ProposalInfo ProposalInfo.cpp)
target_link_libraries(
    ProposalInfo
    Qt::Widgets
    Settings
)

add_library(RecordContainer RecordContainer.cpp)
target_link_libraries(
    RecordContainer
    Qt::Widgets
    Score
)

add_library(RecordDialog RecordDialog.cpp)
target_link_libraries(
    RecordDialog
    Qt::Widgets
    RecordSortFilterProxyModel
)

add_library(RecordSortFilterProxyModel RecordSortFilterProxyModel.cpp)
target_link_libraries(
    RecordSortFilterProxyModel
    Qt::Widgets
    Score
)

add_library(RecordTableModel RecordTableModel.cpp)
target_link_libraries(
    RecordTableModel
    Qt::Widgets
    RecordContainer
    Score
)

add_library(RenderedBoard RenderedBoard.cpp)
target_link_libraries(
    RenderedBoard
    Qt::Widgets
    LetterPool
    Modifier
    RenderedTile
    Settings
    VirtualBoard
)

add_library(RenderedTile RenderedTile.cpp)
target_link_libraries(
    RenderedTile
    Qt::Widgets
    Letter
    LetterPool
    Modifier
    Settings
    VirtualTile
)

add_library(Score Score.cpp)
target_link_libraries(
    Score
    Qt::Widgets
)

add_library(Settings Settings.cpp)
target_link_libraries(
    Settings
    Qt::Widgets
    Language
    ModifierPattern
    PlayerSettings
    StartupProgressDialog
)

add_library(SettingsDialog SettingsDialog.cpp)
target_link_libraries(
    SettingsDialog
    Qt::Widgets
    Settings
)

add_library(StartupProgressDialog StartupProgressDialog.cpp)
target_link_libraries(
    StartupProgressDialog
    Qt::Widgets
)

add_library(UpdateDialog UpdateDialog.cpp)
target_link_libraries(
    UpdateDialog
    Qt::Widgets
    Version
)

add_library(Version Version.cpp)
target_link_libraries(
    Version
    Qt::Widgets
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

add_library(VirtualBoard VirtualBoard.cpp)
target_link_libraries(
    VirtualBoard
    Qt::Widgets
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
    Qt::Widgets
    Letter
    Modifier
    VirtualTile
)

add_library(Wordfeud Wordfeud.cpp)
target_link_libraries(
    Wordfeud
    Qt::Widgets
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