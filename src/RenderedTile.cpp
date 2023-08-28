#include "RenderedTile.hpp"

// Include here to avoid circular dependency issues
#include "Player.hpp"

namespace wf
{
    RenderedTile::RenderedTile(
        Settings* a_settings,
        RenderedTile* a_selection,
        LetterPool* a_letter_pool,
        BoardType a_board_type,
        QWidget* a_parent,
        bool a_follows_mouse)
        : QWidget(a_parent)
        , settings(a_settings)
        , selection(a_selection)
        , letter_pool(a_letter_pool)
        , board_type(a_board_type)
        , follows_mouse(a_follows_mouse)
    {
        switch (board_type)
        {
            case BoardType::Board:
            {
                tile_size = settings->getBoardTileSize();
                break;
            }
            case BoardType::Hand:
            {
                tile_size = settings->getHandTileSize();
                break;
            }
            case BoardType::Selection:
            {
                tile_size = settings->getSelectionTileSize();
                break;
            }
            case BoardType::Display:
            {
                tile_size = settings->getDisplayTileSize();
                break;
            }
        }
        
        setMouseTracking(true);
        setMinimumSize(tile_size);
        setMaximumSize(tile_size);
        repaint();
    }
    
    RenderedTile::~RenderedTile()
    { }
    
    void RenderedTile::placeLetter(Letter* a_letter)
    {
        if (letter == nullptr)
        {
            letter = a_letter;
        }
        else
        {
            return;
        }

        switch (board_type)
        {
            case BoardType::Board:
            {
                letter->setStatus(LetterStatus::Proposed);
                emit proposeLetter(this);

                if (letter->getType() == LetterType::Wildcard && letter->getWildcardText().isEmpty())
                {
                    repaint();
                    emit wildcardPlacedOnBoard(this);
                }

                break;
            }
            case BoardType::Hand:
            {
                if (letter->getType() == LetterType::Wildcard)
                {
                    letter->setWildcardText("");
                }

                break;
            }
            case BoardType::Selection:
            {
                break;
            }
            case BoardType::Display:
            {
                break;
            }
        }

        if (follows_mouse)
        {
            show();
        }

        emit letterAddedRemoved();
        
        return;
    }
    
    Letter* RenderedTile::removeLetter()
    {
        if (    letter->getStatus() == LetterStatus::Locked 
            ||  letter->getStatus() == LetterStatus::LockedRecently)
        {
            return nullptr;
        }
        
        Letter* current_letter = letter;
        letter = nullptr;

        LetterStatus old_status = current_letter->getStatus();
        current_letter->setStatus(LetterStatus::Free);

        if (follows_mouse)
        {
            hide();
        }

        if (old_status == LetterStatus::Proposed)
        {
            emit unproposeLetter(this);
        }

        emit letterAddedRemoved();

        return current_letter;
    }
    
    BoardType RenderedTile::getBoardType() const
    {
        return board_type;
    }
    
    void RenderedTile::setDimmed(bool a_state)
    {
        dimmed = a_state;
        return;
    }
    
    void RenderedTile::setInteractMode(TileInteractMode a_mode)
    {
        interact_mode = a_mode;
        return;
    }
    
    void RenderedTile::setSwapMarking(bool a_state)
    {
        swap_marking = a_state;
        return;
    }
    
    bool RenderedTile::getSwapMarking()
    {
        return swap_marking;
    }
    
    void RenderedTile::reset()
    {
        modifier = nullptr;
        letter = nullptr;
        dimmed = false;
        interact_mode = TileInteractMode::Move;
        swap_marking = false;
        return;
    }
    
    void RenderedTile::paintEvent(QPaintEvent*)
    {
        Letter* current_letter = letter;
        
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        QFont monospace_font = settings->getMonospaceFont();

        // Draw solid black tile background
        if (!follows_mouse)
        {
            QRect background{QPoint{0,0}, size()};
            painter.fillRect(background, QColor{0, 0, 0});
        }

        // Slightly smaller rectangle used to draw the actual tile
        QRect tile_shape{
            QPoint{size().width() * margin / 100, size().height() * margin / 100},
            size() * (100 - 2 * margin) / 100
        };

        // Prepare to fill with no outline
        painter.setPen(Qt::PenStyle::NoPen);

        if (modifier != nullptr && modifier->getType() != ModifierType::None)
        {
            // Draw background color according to modifier type
            switch (modifier->getType())
            {
                case ModifierType::Start:
                {
                    painter.setBrush(QColor{100, 100, 100});
                    break;
                }
                case ModifierType::DoubleLetter:
                {
                    painter.setBrush(QColor{64, 192, 16});
                    break;
                }
                case ModifierType::DoubleWord:
                {
                    painter.setBrush(QColor{192, 128, 32});
                    break;
                }
                case ModifierType::TripleLetter:
                {
                    painter.setBrush(QColor{24, 48, 255});
                    break;
                }
                case ModifierType::TripleWord:
                {
                    painter.setBrush(QColor{192, 32, 32});
                    break;
                }
                default:
                {
                    return;
                }
            }

            painter.drawRoundedRect(tile_shape, radius, radius, Qt::RelativeSize);

            // Draw text in white
            monospace_font.setBold(true);
            monospace_font.setPointSize(size().height() / 3);

            painter.setPen(QColor{255, 255, 255});
            painter.setFont(monospace_font);
            painter.drawText(tile_shape, Qt::AlignCenter, modifier->getText());
        }

        if (current_letter != nullptr)
        {
            int alpha = 255;
            
            // Letters on modifiers have transparent backgrounds
            if (modifier != nullptr)
            {
                if (    modifier->getType() == ModifierType::DoubleLetter
                    ||  modifier->getType() == ModifierType::DoubleWord
                    ||  modifier->getType() == ModifierType::TripleLetter
                    ||  modifier->getType() == ModifierType::TripleWord)
                {
                    alpha = 200;
                }
            }
            
            // Draw letter background
            switch (current_letter->getStatus())
            {
                case LetterStatus::Proposed:
                {
                    painter.setBrush(QColor{192, 192, 192, alpha});
                    break;
                }
                case LetterStatus::LockedRecently:
                {
                    painter.setBrush(QColor{255, 255, 128, alpha});
                    break;
                }
                default:
                {
                    painter.setBrush(QColor{255, 255, 255, alpha});
                    break;
                }
            }
            
            painter.drawRoundedRect(tile_shape, radius, radius, Qt::RelativeSize);

            // Alignment boxes for letter text and points
            QRect letter_alignment{
                size().width() * 10 / 100,
                size().height() * 0 / 10,
                size().width() * 65 / 100,
                size().height() * 10 / 10
            };
            QRect points_alignment{
                size().width() * 1 / 10,
                size().height() * 1 / 10,
                size().width() * 8 / 10,
                size().height() * 3 / 10
            };

            // Draw letter text
            monospace_font.setBold(false);
            monospace_font.setPointSize(size().height() / (1 + current_letter->getText().length()));

            // Wildcard letters in hand should always be blank
            if (!(board_type == BoardType::Hand && current_letter->getType() == LetterType::Wildcard))
            {
                if (board_type != BoardType::Display && settings->getLetterColouring() == QString("Player colour"))
                {
                    painter.setPen(current_letter->getOwner()->getColor());
                }
                else
                {
                    painter.setPen(QColor{0, 0, 0});
                }
                
                painter.setFont(monospace_font);
                painter.drawText(letter_alignment, Qt::AlignCenter, current_letter->getText());

                if (current_letter->getPoints() != 0)
                {
                    monospace_font.setPointSize(size().height() / 5);
                    painter.setFont(monospace_font);
                    painter.drawText(points_alignment, Qt::AlignRight | Qt::AlignVCenter, current_letter->getPointsAsText());
                }
            }

            // Draw red cross over tiles marked for swapping
            if (swap_marking)
            {
                QPen red_cross_pen;
                red_cross_pen.setColor(QColor{255, 0, 0});
                red_cross_pen.setWidth(5);
                painter.setPen(red_cross_pen);
                painter.drawLine(tile_shape.topLeft(), tile_shape.bottomRight());
                painter.drawLine(tile_shape.bottomLeft(), tile_shape.topRight());
            }
        }
        else if (follows_mouse)
        {
            return;
        }

        if (current_letter == nullptr && (modifier == nullptr || modifier->getType() == ModifierType::None))
        {
            // Draw dark gray background
            painter.setBrush(QColor{50, 50, 50});
            painter.drawRoundedRect(tile_shape, radius, radius, Qt::RelativeSize);
        }

        if (dimmed)
        {
            QRect tile_area{QPoint{0,0}, size()};
            painter.fillRect(tile_area, QColor{0, 0, 0, 100});
        }

        return;
    }
    
    void RenderedTile::mousePressEvent(QMouseEvent*)
    {
        if (follows_mouse)
        {
            return;
        }

        switch (interact_mode)
        {
            case TileInteractMode::Move:
            {
                if (    letter != nullptr
                    && (letter->getStatus() == LetterStatus::Locked 
                    ||  letter->getStatus() == LetterStatus::LockedRecently))
                {
                    return;
                }
                
                if (selection->getLetter() == nullptr && letter != nullptr)
                {
                    selection->placeLetter(removeLetter());
                }
                else if (selection->getLetter() != nullptr && letter == nullptr)
                {
                    placeLetter(selection->removeLetter());
                }
                else if (selection->getLetter() != nullptr && letter != nullptr)
                {
                    Letter* letter_from_tile = removeLetter();
                    Letter* letter_from_selection = selection->removeLetter();

                    placeLetter(letter_from_selection);
                    selection->placeLetter(letter_from_tile);
                }
                else
                {
                    return;
                }

                break;
            }
            case TileInteractMode::Swap:
            {
                setSwapMarking(!swap_marking);

                if (swap_marking)
                {
                    emit markForSwap(this);
                }
                else
                {
                    emit unmarkForSwap(this);
                }

                break;
            }
        }

        repaint();
        return;
    }
    
    void RenderedTile::wheelEvent(QWheelEvent* a_event)
    {
        if (board_type == BoardType::Board
        &&  getLetter() != nullptr
        &&  getLetter()->getType() == LetterType::Wildcard
        &&  selection->getLetter() == nullptr)
        {
            int delta = a_event->angleDelta().y();
            Letter* letter = getLetter();
            QString new_letter;

            // Forward scroll, away from user
            if (delta > 0)
            {
                new_letter = letter_pool->getNonWildcardLetterBefore(letter->getWildcardText());
            }
            // Backwards scroll, towards user
            else if (delta < 0)
            {
                new_letter = letter_pool->getNonWildcardLetterAfter(letter->getWildcardText());
            }

            letter->setWildcardText(new_letter);
            emit wildcardScrolled();
            repaint();
        }

        return;
    }
}