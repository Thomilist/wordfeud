#include "BoardEditor.hpp"


namespace wf
{
    BoardEditor::BoardEditor(Settings* a_settings, QWidget* a_parent)
        : AbstractEditor(
            "Board",
            a_settings,
            a_parent)
        , board(
            BoardType::EditableBoard,
            settings,
            &selection,
            nullptr)
        , selection(
            settings,
            &selection,
            nullptr,
            BoardType::EditorSelection,
            this,
            true)
        , modifier_palette(
            BoardType::Source,
            settings,
            &selection,
            nullptr)
    {
        createModifierPalette();
        createMirroringGroup();

        connect(&reset_board_button, &QPushButton::clicked, this, &BoardEditor::loadBoard);

        vertical_spacer.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

        int layout_row = 0;

        layout.addWidget(&modifier_palette_group, layout_row++, 0);
        layout.addWidget(&mirroring_group, layout_row++, 0);
        layout.addWidget(&reset_board_button, layout_row++, 0);
        layout.addWidget(&vertical_spacer, layout_row++, 0);

        layout.addWidget(&board, 0, 1, layout_row, 1);

        group.setLayout(&layout);
        getBodyLayout()->addWidget(&group, 0, 0);

        setMouseTracking(true);
        selection.raise();
    }
    
    BoardEditor::~BoardEditor()
    { }
    
    void BoardEditor::loadBoard()
    {
        board.reset();

        switch (mode)
        {
            case EditorMode::CreateNew:
            {
                
                break;
            }
            case EditorMode::OpenCopy:
            case EditorMode::EditExisting:
            {
                ModifierPattern original_pattern = ModifierPattern{original_item};
                board.placeModifiers(&original_pattern);
                break;
            }
            default:
            {
                break;
            }
        }

        board.repaint();

        return;
    }
    
    void BoardEditor::prepare()
    {
        setExistingItems(settings->getAvailableModifierPatternsAsStrings());
        updateHeaderText();
        updateNameSuggestion();
        loadBoard();
        selection.reset();
    }
    
    void BoardEditor::save()
    {
        ModifierPattern pattern;
        pattern.loadFromBoard(&board);
        pattern.exportToJSON(ModifierPattern::getPath(current_item));
        return;
    }
    
    void BoardEditor::mouseMoveEvent(QMouseEvent* a_event)
    {
        selection.move(a_event->pos() + QPoint{1, 1});
        return;
    }
    
    void BoardEditor::createModifierPalette()
    {
        createStandardModifiers();

        modifier_palette_layout.addWidget(&modifier_palette, 0, 0);
        modifier_palette_group.setLayout(&modifier_palette_layout);

        return;
    }
    
    void BoardEditor::createStandardModifiers()
    {
        standard_modifiers.clear();
        
        // Start
        standard_modifiers.push_back(Modifier(ModifierType::Start, "", 1, QColor{100, 100, 100}));

        // Double letter
        standard_modifiers.push_back(Modifier(ModifierType::Letter, "DL", 2, QColor{64, 192, 16}));
        
        // Triple letter
        standard_modifiers.push_back(Modifier(ModifierType::Letter, "TL", 3, QColor{24, 48, 255}));

        // Double word
        standard_modifiers.push_back(Modifier(ModifierType::Word, "DW", 2, QColor{192, 128, 32}));

        // Triple word
        standard_modifiers.push_back(Modifier(ModifierType::Word, "TW", 3, QColor{192, 32, 32}));

        for (int column = 0; column < modifier_palette.getGridDimensions().width(); ++column)
        {
            RenderedTile* tile = modifier_palette.getTileAtPosition(column, 0);
            tile->setModifier(&standard_modifiers[column]);
        }

        return;
    }
    
    void BoardEditor::addCustomModifier(Modifier /*a_modifier*/)
    {
        
    }
    
    void BoardEditor::createMirroringGroup()
    {
        mirroring_layout.addWidget(&mirror_vertical, 0, 0);
        mirroring_layout.addWidget(&mirror_horisontal, 1, 0);

        mirroring_group.setLayout(&mirroring_layout);

        return;
    }
}