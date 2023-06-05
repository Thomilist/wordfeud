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

#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"

#include "BoardType.hpp"
#include "TileNeighbours.hpp"
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
                QWidget* a_parent = nullptr);
            ~RenderedBoard();

            RenderedTile* getTileAtPosition(int a_collumn, int a_row);
            BoardType getType() const;
            void setDimmedAndDisabled(bool a_state);
            void setTileInteractMode(TileInteractMode a_mode);
            void reset();

        private:
            void createEmptyGrid();
            void updateBoardSize();
            void assignNeighbours();

            Settings* settings;
            QGridLayout grid;
            QSize tile_size;
            QSize board_size;
            RenderedTile* selection;
            BoardType type;
            bool dimmed_and_disabled = false;

    };
}
#endif // __RENDEREDBOARD_H__