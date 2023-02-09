#include "Tile.hpp"

namespace wf
{
    Tile::Tile(
        const QSize& a_size,
        Tile* a_selection,
        BoardType a_board_type,
        QWidget* a_parent,
        bool a_follows_mouse)
        : QWidget(a_parent)
        , selection(a_selection)
        , follows_mouse(a_follows_mouse)
        , board_type(a_board_type)
    {
        setMouseTracking(true);
        resize(a_size);
        setMaximumSize(a_size);
        setMinimumSize(a_size);
        repaint();
    }
    
    Tile::~Tile()
    { }
    
    void Tile::placeLetter(Letter* a_letter)
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

                if (letter->getType() == LetterType::Wildcard)
                {
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
            case BoardType::None:
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
    
    Letter* Tile::removeLetter()
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
    
    Letter* Tile::getLetter() const
    {
        return letter;
    }
    
    void Tile::setModifier(Modifier* a_modifier)
    {
        modifier = a_modifier;
        return;
    }
    
    Modifier* Tile::getModifier() const
    {
        return modifier;
    }
    
    std::array<Tile*, 4> Tile::getNeighbours() const
    {
        return neighbours;
    }
    
    Tile* Tile::getNeighbour(TileNeighbour a_index) const
    {
        return neighbours[a_index];
    }
    
    void Tile::setNeighbour(Tile* a_tile, TileNeighbour a_index)
    {
        neighbours[a_index] = a_tile;
    }
    
    void Tile::setGridPosition(int a_collumn, int a_row)
    {
        grid_position = QPoint{a_collumn, a_row};
        return;
    }
    
    QPoint Tile::getGridPosition() const
    {
        return grid_position;
    }
    
    BoardType Tile::getBoardType() const
    {
        return board_type;
    }
    
    void Tile::setDimmed(bool a_state)
    {
        dimmed = a_state;
        return;
    }
    
    void Tile::setInteractMode(TileInteractMode a_mode)
    {
        interact_mode = a_mode;
        return;
    }
    
    void Tile::setSwapMarking(bool a_state)
    {
        swap_marking = a_state;
        return;
    }
    
    void Tile::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

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
            QFont modifier_font{"Monospace", size().height() / 3};
            modifier_font.setBold(true);

            painter.setPen(QColor{255, 255, 255});
            painter.setFont(modifier_font);
            painter.drawText(tile_shape, Qt::AlignCenter, modifier->getText());
        }

        if (letter != nullptr)
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
            switch (letter->getStatus())
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
                size().width() * 2 / 10,
                size().height() * 2 / 10,
                size().width() * 45 / 100,
                size().height() * 6 / 10
            };
            QRect points_alignment{
                size().width() * 1 / 10,
                size().height() * 1 / 10,
                size().width() * 8 / 10,
                size().height() * 3 / 10
            };

            // Draw text in black
            painter.setPen(QColor{0, 0, 0});
            painter.setFont(QFont{"Monospace", size().height() / 2});
            painter.drawText(letter_alignment, Qt::AlignCenter, letter->getText());

            if (letter->getPoints() != 0)
            {
                painter.setFont(QFont{"Monospace", size().height() / 5});
                painter.drawText(points_alignment, Qt::AlignRight | Qt::AlignVCenter, letter->getPointsAsText());
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

        if (letter == nullptr && (modifier == nullptr || modifier->getType() == ModifierType::None))
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
    
    void Tile::mousePressEvent(QMouseEvent*)
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
}