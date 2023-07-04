#ifndef __FORWARDDECLARATIONS_H__
#define __FORWARDDECLARATIONS_H__

namespace wf
{
    class AboutDialog;
    class ButtonPanel;
    class Game;
    class Header;
    class Language;
    class Letter;
    class LetterPool;
    class Modifier;
    class ModifierPattern;
    class Player;
    class PlayerAI;
    class PlayerAIWorker;
    class PlayerSettings;
    class ProposalInfo;
    class RecordDialog;
    class RecordTracker;
    class RenderedBoard;
    class RenderedTile;
    class Settings;
    class SettingsDialog;
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
    enum class GameState;
    enum class LanguageName;
    enum class LetterStatus;
    enum class LetterType;
    enum class ModifierDistribution;
    enum class ModifierType;
    enum class PlayerType;
    enum class PlayType;
    enum class TileInteractMode;
    enum class UpdateStatus;

    enum TileNeighbour : unsigned int;
}

#endif // __FORWARDDECLARATIONS_H__