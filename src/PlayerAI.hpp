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
#include "LetterPool.hpp"
#include "Player.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "Sandbox.hpp"

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
    
    class PlayerAI : public Player
    {
        Q_OBJECT
        
        public:
            PlayerAI(
                QString a_display_name,
                QColor a_color,
                Settings* a_settings,
                RenderedTile* a_selection,
                RenderedBoard* a_board,
                LetterPool* a_letter_pool);
            ~PlayerAI();

        public slots:
            void playIfTurn();
            void cancelTurn();
        
        signals:
            void playComplete();
            void letterPlaced();
            void startSwap();
            void letterMarkedForSwap();
            void swapComplete();
            void passTurn();

        private:
            void executeTurn();
            void endTurn();
            void startOfTurnSetup();
            void fetchAvailableLetters(Sandbox* a_sandbox);
            void initialiseSandboxes();
            void findBestPlay();
            void findPlayInLine(Sandbox* a_sandbox, Direction a_direction, int a_index);
            void findPlayInHorisontalLine(Sandbox* a_sandbox, int a_row);
            void findPlayInVerticalLine(Sandbox* a_sandbox, int a_column);
            void findPlayRecursively(Sandbox* a_sandbox, int a_column, int a_row, Direction a_direction);
            void tryLetterAndRecurse(Sandbox* a_sandbox, Letter*& a_letter, VirtualTile* a_tile, int a_column, int a_row, Direction a_direction);
            void recurse(Sandbox* a_sandbox, int a_column, int a_row, Direction a_direction);
            bool indexOutOfBounds(Sandbox* a_sandbox, int a_column, int a_row);
            void updateBestPlay(Sandbox* a_sandbox);
            void updateBestPlay(Sandbox* a_sandbox_with_best, Sandbox* a_sandbox_with_new);
            void executeBestPlay();
            void setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<VirtualTile*> a_proposed_tiles);
            void setBestPlayWildcardLetters(Sandbox* a_best_sandbox, std::vector<QChar> a_proposed_letters);
            void updateRelevantLines();
            void initialiseBoardEvaluation(VirtualBoard* a_board);
            void evaluateBoard(VirtualBoard* a_board);
            int evaluateTile(VirtualTile* a_tile);
            void swapAllLetters();

            bool cancelled = false;
            RenderedBoard* live_board;
            Sandbox best_sandbox;
            std::vector<Sandbox> sandboxes;
            std::set<int> relevant_collumns;
            std::set<int> relevant_rows;
            std::vector<std::vector<int>> board_evaluation;
            LetterPool* letter_pool;
            std::vector<QFuture<void>> worker_pool;
    };
}

#endif // __PLAYERAI_H__