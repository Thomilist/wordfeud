#ifndef __VIRTUALBOARD_H__
#define __VIRTUALBOARD_H__

#include <algorithm>
#include <unordered_set>
#include <vector>

#include <QSize>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Settings.hpp"
#include "VirtualTile.hpp"
#include "Word.hpp"

#include "Direction.hpp"

namespace wf
{
    class VirtualBoard
    {
        public:
            VirtualBoard(Settings* a_settings);
            VirtualBoard(
                Settings* a_settings,
                VirtualBoard* a_board
            );
            ~VirtualBoard();

            void setWithBoard(VirtualBoard* a_board);
            virtual VirtualTile* getTileAtPosition(int a_collumn, int a_row);
            QSize getGridDimensions() const;
            int getTileCount() const;
            int getLetterCount();
            void evaluateProposedPlay();
            bool isPlacementValid();
            bool areProposedWordsValid();
            bool isProposedPlayValid();
            int getProposedLetterCount();
            std::vector<Word> getProposedWords();
            std::vector<Word*> getInvalidProposedWords();
            int getProposedPlayPoints();
            Word getMostRelevantWord();
            void lockProposedPlay();
            void lockProposedLetters();
            void lockRecentlyLockedLetters();
            void proposeLetter(VirtualTile* a_tile);
            void unproposeLetter(VirtualTile* a_tile);
            std::vector<Letter*> clearProposed();
        
        protected:
            std::vector<std::vector<VirtualTile>> board;
            QSize grid_dimensions;

        private:
            bool isPlacementConnectedToStart(VirtualTile* a_tile);
            bool isPlacementLinear();
            bool isPlacementInOneCollumn();
            bool isPlacementInOneRow();
            void findProposedWords();
            void findProposedWordInLine(int a_line, int a_max_index, Direction a_direction);
            void findInvalidProposedWords();
            void calculateProposedPoints();

            Settings* settings;
            std::vector<VirtualTile*> proposed_letters;
            std::vector<Letter*> locked_letters;
            std::vector<VirtualTile*> checked_tiles;
            std::vector<Word> proposed_words;
            std::vector<Word*> invalid_words;
            bool proposed_play_evaluated = false;
            bool proposed_placement_valid = false;
            bool proposed_words_valid = false;
            int proposed_play_points = 0;
    };
}

#endif // __VIRTUALBOARD_H__