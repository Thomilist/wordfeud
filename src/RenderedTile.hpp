#ifndef __RENDEREDTILE_H__
#define __RENDEREDTILE_H__

#include <array>

#include <QColor>
#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QWheelEvent>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"
#include "LetterPool.hpp"
#include "Modifier.hpp"
#include "Settings.hpp"
#include "VirtualTile.hpp"

#include "BoardType.hpp"
#include "LetterStatus.hpp"
#include "ModifierType.hpp"
#include "TileInteractMode.hpp"
#include "TileNeighbours.hpp"

namespace wf
{
    class RenderedTile : public QWidget, public VirtualTile
    {
        Q_OBJECT
        
        public:
            RenderedTile(
                Settings* a_settings,
                RenderedTile* a_selection,
                LetterPool* a_letter_pool,
                BoardType a_board_type,
                QWidget* a_parent = nullptr,
                bool a_follows_mouse = false);
            ~RenderedTile();

            void placeLetter(Letter* a_letter);
            [[nodiscard]] Letter* removeLetter();
            BoardType getBoardType() const;
            void setDimmed(bool a_state);
            void setInteractMode(TileInteractMode a_mode);
            void setSwapMarking(bool a_state);
            bool getSwapMarking();
            void reset();
        
        signals:
            void proposeLetter(RenderedTile* a_tile);
            void unproposeLetter(RenderedTile* a_tile);
            void letterAddedRemoved();
            void wildcardPlacedOnBoard(RenderedTile* a_tile);
            void markForSwap(RenderedTile* a_tile);
            void unmarkForSwap(RenderedTile* a_tile);
            void wildcardScrolled();

        private:
            void paintEvent(QPaintEvent* a_event);
            void mousePressEvent(QMouseEvent* a_event);
            void wheelEvent (QWheelEvent* a_event);
            
            Settings* settings;
            RenderedTile* selection;
            LetterPool* letter_pool;

            BoardType board_type;
            QSize tile_size;
            bool follows_mouse;
            bool dimmed = false;
            TileInteractMode interact_mode = TileInteractMode::Move;
            bool swap_marking = false;
            
            // Dimensions, percentage of tile size
            int margin = 5; 
            int radius = 20;
    };
}

#endif // __RENDEREDTILE_H__