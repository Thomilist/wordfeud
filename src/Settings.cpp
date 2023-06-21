#include "Settings.hpp"


namespace wf
{
    Settings::Settings(QMainWindow* a_main_window)
        : main_window(a_main_window)
        , languages{
            Language{LanguageName::Danish},
            Language{LanguageName::English}}
    {
        int monospace_font_family_id = QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Regular.ttf");
        QString monospace_font_family = QFontDatabase::applicationFontFamilies(monospace_font_family_id).at(0);
        monospace_font.setFamily(monospace_font_family);
        monospace_font.setStyleHint(QFont::TypeWriter);

        load();
    }
    
    Settings::~Settings()
    {
        save();
    }
    
    void Settings::save()
    {
        // Save main window dimensions and position
        setValue("window/geometry", main_window->saveGeometry());

        // Save dictionary language
        setValue("general/dictionary", getLanguage()->asString());

        // Save modifier distribution
        setValue("general/modifiers", getModifierPattern()->getDistributionAsText());

        // Save letter colouring
        setValue("general/letter_colouring", getLetterColouring());

        // Save left player
        setValue("player_left/name", getLeftPlayer()->getName());
        setValue("player_left/type", getLeftPlayer()->getTypeAsString());
        setValue("player_left/random_name", getLeftPlayer()->usesRandomName());
        setValue("player_left/ai_difficulty", getLeftPlayer()->getAIDifficulty());

        // Save right player
        setValue("player_right/name", getRightPlayer()->getName());
        setValue("player_right/type", getRightPlayer()->getTypeAsString());
        setValue("player_right/random_name", getRightPlayer()->usesRandomName());
        setValue("player_right/ai_difficulty", getRightPlayer()->getAIDifficulty());

        // Save minimum AI turn time
        setValue("ai/minimum_turn_time", getMinimumAITurnTime());

        return;
    }
    
    void Settings::load()
    {
        // Load random names
        loadRandomNames("resources/names/");
        
        // Load main window dimensions and position
        const QByteArray geometry = value("window/geometry", QByteArray()).toByteArray();
        
        if (!geometry.isEmpty())
        {
            main_window->restoreGeometry(geometry);
        }

        // Load dictionary language
        QString dictionary = value("general/dictionary", "English").toString();
        setLanguage(dictionary);

        // Load modifier distribution
        QString modifier_distribution = value("general/modifiers", "Default").toString();
        getModifierPattern()->setDistribution(modifier_distribution);

        // Load letter colouring
        setLetterColouring(value("general/letter_colouring", "Default").toString());

        // Load left player
        QString left_player_name = value("player_left/name", "Player 1").toString();
        QString left_player_type = value("player_left/type", "Human").toString();
        bool left_player_uses_random_name = value("player_left/random_name", "false").toBool();
        int left_player_ai_difficulty = value("player_left/ai_difficulty", "5").toInt();
        getLeftPlayer()->setName(left_player_name);
        getLeftPlayer()->setTypeWithString(left_player_type);
        getLeftPlayer()->setRandomNameUse(left_player_uses_random_name);
        getLeftPlayer()->setRandomNames(random_names);
        getLeftPlayer()->setAIDifficulty(left_player_ai_difficulty);

        // Load right player
        QString right_player_name = value("player_right/name", "Player 2").toString();
        QString right_player_type = value("player_right/type", "AI").toString();
        bool right_player_uses_random_name = value("player_right/random_name", "true").toBool();
        int right_player_ai_difficulty = value("player_right/ai_difficulty", "5").toInt();
        getRightPlayer()->setName(right_player_name);
        getRightPlayer()->setTypeWithString(right_player_type);
        getRightPlayer()->setRandomNameUse(right_player_uses_random_name);
        getRightPlayer()->setRandomNames(random_names);
        getRightPlayer()->setAIDifficulty(right_player_ai_difficulty);

        // Load minimum AI turn time
        setMinimumAITurnTime(value("ai/minimum_turn_time", "0").toInt());

        return;
    }
    
    void Settings::setGridDimensions(int a_rows, int a_collumns)
    {
        board_dimensions.setHeight(a_rows);
        board_dimensions.setWidth(a_collumns);

        return;
    }
    
    void Settings::setTileSize(int size)
    {
        board_tile_size.setHeight(size);
        board_tile_size.setWidth(size);

        return;
    }
    
    void Settings::setLanguage(LanguageName a_language)
    {
        for (auto& language : languages)
        {
            if (language.asEnum() == a_language)
            {
                current_language = &language;
                return;
            }
        }

        return;
    }
    
    void Settings::setLanguage(QString a_language)
    {
        for (auto& language : languages)
        {
            if (language.asString() == a_language)
            {
                current_language = &language;
                return;
            }
        }

        return;
    }
    
    Language* Settings::getLanguage()
    {
        return current_language;
    }
    
    std::vector<Language>& Settings::getAvailableLanguages()
    {
        return languages;
    }
    
    const QSize& Settings::getBoardDimensions() const
    {
        return board_dimensions;
    }
    
    const QSize& Settings::getBoardTileSize() const
    {
        return board_tile_size;
    }
    
    const QSize& Settings::getHandDimensions() const
    {
        return hand_dimensions;
    }
    
    const QSize& Settings::getHandTileSize() const
    {
        return hand_tile_size;
    }
    
    const QSize& Settings::getSelectionTileSize() const
    {
        return selection_tile_size;
    }
    
    QFont Settings::getMonospaceFont() const
    {
        return monospace_font;
    }
    
    ModifierPattern* Settings::getModifierPattern()
    {
        return &modifier_pattern;
    }
    
    PlayerSettings* Settings::getLeftPlayer()
    {
        return &left_player;
    }
    
    PlayerSettings* Settings::getRightPlayer()
    {
        return &right_player;
    }
    
    PlayerSettings* Settings::getPlayer(int a_index)
    {
        switch (a_index)
        {
            case 0:
            {
                return getLeftPlayer();
            }
            case 1:
            {
                return getRightPlayer();
            }
            default:
            {
                return nullptr;
            }
        }
    }
    
    void Settings::loadRandomNames(QString a_directory)
    {
        QDir directory{a_directory};
        QDirIterator name_directory{directory};
        QString file_path;
        QString name;
        
        while (name_directory.hasNext())
        {
            file_path = name_directory.next();
            QFile name_file{file_path};

            if (name_file.open(QIODevice::ReadOnly))
            {
                QTextStream names{&name_file};

                while (!names.atEnd())
                {
                    name = names.readLine();
                    random_names.insert(name);
                }

                name_file.close();
            }
        }

        return;
    }
    
    int Settings::getMinimumAITurnTime() const
    {
        return minimum_ai_turn_time;
    }
    
    void Settings::setMinimumAITurnTime(int a_time)
    {
        minimum_ai_turn_time = a_time;
        return;
    }
    
    const QString Settings::getLetterColouring() const
    {
        return letter_colouring;
    }
    
    void Settings::setLetterColouring(QString a_colour)
    {
        letter_colouring = a_colour;
        return;
    }
    
    int Settings::getMinimumAIDifficulty() const
    {
        return minimum_ai_difficulty;
    }
    
    int Settings::getMaximumAIDifficulty() const
    {
        return maximum_ai_difficulty;
    }
}