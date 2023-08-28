#ifndef __RENDEREDBOARD_H__
#define __RENDEREDBOARD_H__

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLayoutItem>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QColor>

#include "ForwardDeclarations.hpp"

#include "LetterPool.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"

#include "BoardType.hpp"
#include "TileInteractMode.hpp"

namespace wf
{
    class RenderedBoard : public QWidget, public VirtualBoard
    {
        public:
            RenderedBoard(
                BoardType a_board_type,
                Settings* a_settings,
                RenderedTile* a_selection,
                LetterPool* a_letter_pool,
                QWidget* a_parent = nullptr);
            ~RenderedBoard();

            RenderedTile* getTileAtPosition(int a_column, int a_row);
            RenderedTile* getTileAtPosition(QPoint a_position);
            BoardType getType() const;
            void setDimmedAndDisabled(bool a_state);
            void setTileInteractMode(TileInteractMode a_mode);
            void reset();

        private:
            void createEmptyGrid();
            void updateBoardSize();

            Settings* settings;
            RenderedTile* selection;
            LetterPool* letter_pool;

            QGridLayout grid;
            QSize tile_size;
            QSize board_size;
            BoardType type;
            bool dimmed_and_disabled = false;

    };
}
#endif // __RENDEREDBOARD_H__