#ifndef __BOARDEDITOR_H__
#define __BOARDEDITOR_H__

#include <vector>

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include <QPoint>
#include <QPushButton>
#include <QWidget>

#include "ForwardDeclarations.hpp"

#include "AbstractEditor.hpp"
#include "Modifier.hpp"
#include "ModifierPattern.hpp"
#include "RenderedBoard.hpp"
#include "RenderedTile.hpp"
#include "Settings.hpp"
#include "VirtualBoard.hpp"

#include "BoardType.hpp"
#include "Direction.hpp"
#include "ModifierType.hpp"

namespace wf
{
    class BoardEditor : public AbstractEditor
    {
        Q_OBJECT
        
        public:
            BoardEditor(Settings* a_settings, QWidget* a_parent = nullptr);
            ~BoardEditor();
        
        private slots:
            void loadBoard();
            void highlightTiles(const QPoint& a_position);
            void unhighlightTiles(const QPoint& a_position);
        
        private:
            void prepare() override;
            void save() override;

            void mouseMoveEvent(QMouseEvent* a_event);
            void createModifierPalette();
            void createStandardModifiers();
            void addCustomModifier(Modifier a_modifier);
            void createMirroringGroup();

            QGroupBox group{"Board"};
            QGridLayout layout;

            QGroupBox modifier_palette_group{"Modifier Palette"};
            QGridLayout modifier_palette_layout;

            QGroupBox mirroring_group{"Mirroring"};
            QGridLayout mirroring_layout;
            QCheckBox mirror_vertical{"Vertical"};
            QCheckBox mirror_horisontal{"Horisontal"};

            QPushButton reset_board_button{"Reset Board"};

            QWidget vertical_spacer;

            RenderedBoard board;
            RenderedTile selection;
            RenderedBoard modifier_palette;
            
            std::vector<Modifier> standard_modifiers;
            std::vector<Modifier> custom_modifiers;
    };
}

#endif // __BOARDEDITOR_H__