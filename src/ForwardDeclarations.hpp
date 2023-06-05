#ifndef __FORWARDDECLARATIONS_H__
#define __FORWARDDECLARATIONS_H__

namespace wf
{
    class RenderedBoard;
    class ButtonPanel;
    class Game;
    class Header;
    class Language;
    class Letter;
    class LetterPool;
    class Modifier;
    class Player;
    class PlayerAI;
    class Settings;
    class SettingsDialog;
    class RenderedTile;
    class VirtualBoard;
    class VirtualTile;
    class Word;
    class Wordfeud;

    struct LetterData;
    struct QCharHash;

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

    enum TileNeighbour : unsigned int;
}

#endif // __FORWARDDECLARATIONS_H__