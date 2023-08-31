#ifndef __VIRTUALBOARD_H__
#define __VIRTUALBOARD_H__

#include <algorithm>
#include <deque>
#include <set>
#include <vector>

#include <QPoint>
#include <QSize>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "Modifier.hpp"
#include "ModifierPattern.hpp"
#include "Settings.hpp"
#include "VirtualTile.hpp"
#include "Word.hpp"

#include "Direction.hpp"
#include "TileNeighbours.hpp"

namespace wf
{
    class VirtualBoard
    {
        public:
            VirtualBoard(Settings* a_settings);
            VirtualBoard(Settings* a_settings, VirtualBoard* a_board);
            ~VirtualBoard();

            void setWithBoard(VirtualBoard* a_board);
            void placeModifiers(ModifierPattern* a_modifier_pattern, bool a_shuffle = false);
            void importProposedLetters(std::vector<VirtualTile*> a_tiles);
            std::vector<VirtualTile*> getProposedLetters();
            virtual VirtualTile* getTileAtPosition(int a_column, int a_row);
            virtual VirtualTile* getTileAtPosition(const QPoint& a_position);
            QSize getGridDimensions() const;
            int getGridDimensionInDirection(Direction a_direction);
            int getTileCount() const;
            int getLetterCount();
            void evaluateProposedPlay(bool a_force = false, bool a_skip_placement_check=false, bool a_exit_early = false, bool a_use_bias = false);
            bool isPlacementValid();
            bool areProposedWordsValid();
            bool isProposedPlayValid();
            int getProposedLetterCount();
            std::vector<Word> getProposedWords();
            std::vector<Word> getInvalidProposedWords();
            int getProposedPlayPoints(bool a_skip_validation = false);
            int getProposedPlayPointsBiased(bool a_skip_validation = false);
            Word getMostRelevantWord();
            void lockProposedPlay();
            void lockProposedLetters();
            void lockRecentlyLockedLetters();
            void proposeLetter(VirtualTile* a_tile);
            void proposeLetter(QPoint a_position);
            void unproposeLetter(VirtualTile* a_tile);
            void unproposeLetter(QPoint a_position);
            void placeLetter(QPoint a_position, Letter* a_letter);
            [[nodiscard]] Letter* removeLetter(QPoint a_position);
            std::vector<Letter*> clearProposed();
            void discardProposed();
            void invalidateEvaluation();
            const QPoint mirrorTilePosition(const QPoint& a_position, Direction a_direction);
            bool hasStartModifier();
        
        protected:
            void assignNeighbours();
            
            std::vector<std::vector<VirtualTile>> board;
            QSize grid_dimensions;

        private:
            bool isPlacementConnectedToStart(VirtualTile* a_tile);
            bool isPlacementLinear();
            bool isPlacementInOneCollumn();
            bool isPlacementInOneRow();
            bool isPlacementContiguous();
            void updateProposedLines();
            void findProposedWords(bool a_exit_early);
            Word findWordWithLetter(VirtualTile* a_tile, Direction a_direction);
            void findInvalidProposedWords();
            void calculateProposedPoints(bool a_use_bias);

            Settings* settings;
            std::vector<Modifier> modifiers;
            std::vector<VirtualTile*> proposed_letters;
            std::vector<Letter*> locked_letters;
            std::vector<VirtualTile*> checked_tiles;
            std::vector<Word> proposed_words;
            std::vector<Word> invalid_words;
            bool proposed_play_evaluated = false;
            bool proposed_placement_valid = false;
            bool proposed_words_valid = false;
            int proposed_play_points = 0;
            int proposed_play_points_biased = 0;
            std::set<int> proposed_columns;
            std::set<int> proposed_rows;
    };
}

#endif // __VIRTUALBOARD_H__