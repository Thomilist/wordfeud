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
        , type(a_board_type)
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

        if (type == BoardType::Board)
        {
            letter->setStatus(LetterStatus::Proposed);
            emit proposeLetter(this);
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

        current_letter->setStatus(LetterStatus::Free);

        if (follows_mouse)
        {
            hide();
        }

        emit letterAddedRemoved();

        return current_letter;
    }
    
    Letter* Tile::getLetter()
    {
        return letter;
    }
    
    void Tile::setModifier(Modifier* a_modifier)
    {
        modifier = a_modifier;
        return;
    }
    
    Modifier* Tile::getModifier()
    {
        return modifier;
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
                    painter.setBrush(QColor{255, 255, 192, alpha});
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

            painter.setFont(QFont{"Monospace", size().height() / 5});
            painter.drawText(points_alignment, Qt::AlignRight | Qt::AlignVCenter, letter->getPointsAsText());
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

        return;
    }
    
    void Tile::mousePressEvent(QMouseEvent*)
    {
        if (follows_mouse)
        {
            return;
        }

        if (    letter != nullptr
            && (letter->getStatus() == LetterStatus::Locked 
            ||  letter->getStatus() == LetterStatus::LockedRecently))
        {
            return;
        }
        
        if (selection->getLetter() == nullptr && letter != nullptr)
        {
            if (letter->getStatus() == LetterStatus::Proposed)
            {
                emit unproposeLetter(this);
            }

            selection->placeLetter(removeLetter());
        }
        else if (selection->getLetter() != nullptr && letter == nullptr)
        {
            placeLetter(selection->removeLetter());
        }
        else
        {
            return;
        }

        repaint();
        return;
    }
}