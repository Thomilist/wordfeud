#ifndef __PLAYERAI_H__
#define __PLAYERAI_H__

#include <array>
#include <set>
#include <vector>

#include <QChar>
#include <QColor>
#include <QFuture>
#include <QPoint>
#include <QString>
#include <QtConcurrent>
#include <QThread>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Player.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "Direction.hpp"
#include "LetterType.hpp"
#include "TileNeighbours.hpp"

namespace wf
{
    struct QCharHash
    {
        size_t operator()(const QChar& a_char) const
        {
            return qHash(a_char);
        }
    };

    struct Sandbox
    {
        VirtualBoard board;
        VirtualBoard best_play;
        std::vector<QChar> best_play_wildcard_letters;
        std::vector<Letter*> available_letters;
        int available_letter_count = 0;
        int touch_count = 0;
    };
    
    class PlayerAI : public Player
    {
        Q_OBJECT
        
        public:
            PlayerAI(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                RenderedTile* a_selection,
                RenderedBoard* a_board);
            ~PlayerAI();

        public slots:
            void playIfTurn();
            void cancelTurn();
        
        signals:
            void playComplete();
            void passTurn();
            void swapAllTiles();
            void letterPlaced();

        private:
            void executeTurn();
            void endTurn();
            void startOfTurnSetup();
            void fetchAvailableLetters(Sandbox* a_sandbox);
            void initialiseSandboxes();
            void findBestPlay();
            void findPlayInLine(Sandbox* a_sandbox, Direction a_direction, int a_index);
            void findPlayInHorisontalLine(Sandbox* a_sandbox, int a_row);
            void findPlayInVerticalLine(Sandbox* a_sandbox, int a_collumn);
            void findPlayRecursively(Sandbox* a_sandbox, int a_collumn, int a_row, Direction a_direction);
            void tryLetterAndRecurse(Sandbox* a_sandbox, Letter*& a_letter, VirtualTile* a_tile, int a_collumn, int a_row, Direction a_direction);
            void recurse(Sandbox* a_sandbox, int a_collumn, int a_row, Direction a_direction);
            bool indexOutOfBounds(Sandbox* a_sandbox, int a_collumn, int a_row);
            void updateBestPlay(Sandbox* a_sandbox);
            void updateBestPlay(Sandbox* a_sandbox_with_best, Sandbox* a_sandbox_with_new);
            void executeBestPlay();
            void setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<VirtualTile*> a_proposed_tiles);
            void setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<QChar> a_proposed_letters);
            void updateRelevantLines();
            void initialiseBoardEvaluation(VirtualBoard* a_board);
            void evaluateBoard(VirtualBoard* a_board);
            int evaluateTile(VirtualTile* a_tile);

            bool cancelled = false;
            RenderedBoard* live_board;
            Sandbox best_sandbox;
            std::vector<Sandbox> sandboxes;
            std::set<int> relevant_collumns;
            std::set<int> relevant_rows;
            std::vector<std::vector<int>> board_evaluation;
    };
}

#endif // __PLAYERAI_H__