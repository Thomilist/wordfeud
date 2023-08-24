#ifndef __FORWARDDECLARATIONS_H__
#define __FORWARDDECLARATIONS_H__

namespace wf
{
    class AboutDialog;
    class AbstractEditor;
    class AbstractEditorLauncher;
    class BoardEditor;
    class BoardEditorLauncher;
    class ButtonPanel;
    class DictionaryEditor;
    class DictionaryEditorLauncher;
    class DictionaryLookupDialog;
    class Game;
    class Header;
    class Language;
    class Letter;
    class LetterEditorWidget;
    class LetterPool;
    class LetterTable;
    class LetterTableModel;
    class Modifier;
    class ModifierPattern;
    class Player;
    class PlayerAI;
    class PlayerAIWorker;
    class PlayerSettings;
    class ProposalInfo;
    class RecordContainer;
    class RecordDialog;
    class RecordSortFilterProxyModel;
    class RecordTableModel;
    class RenderedBoard;
    class RenderedTile;
    class Settings;
    class SettingsDialog;
    class StartupProgressDialog;
    class UpdateDialog;
    class Version;
    class VirtualBoard;
    class VirtualTile;
    class Word;
    class Wordfeud;

    struct LetterData;
    struct QCharHash;
    struct Score;

    enum class BoardType;
    enum class Direction;
    enum class EditorMode;
    enum class GameState;
    enum class LetterStatus;
    enum class LetterType;
    enum class ModifierType;
    enum class PlayerType;
    enum class PlayType;
    enum class TileInteractMode;
    enum class UpdateStatus;

    enum RecordColumn : unsigned int;
    enum TileNeighbour : unsigned int;
}

#endif // __FORWARDDECLARATIONS_H__