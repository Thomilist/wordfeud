#include "Settings.hpp"


namespace wf
{
    Settings::Settings(QMainWindow* a_main_window, StartupProgressDialog* a_startup_progress_dialog)
        : main_window(a_main_window)
    {
        connect(this, &Settings::languagesCounted, a_startup_progress_dialog, &StartupProgressDialog::setLanguageCount);
        connect(this, &Settings::updateLoadingText, a_startup_progress_dialog, &StartupProgressDialog::setText);
        connect(this, &Settings::incrementLoadingProgress, a_startup_progress_dialog, &StartupProgressDialog::incrementProgress);
        
        int monospace_font_family_id = QFontDatabase::addApplicationFont(":/fonts/NotoSansMono-Regular.ttf");
        QString monospace_font_family = QFontDatabase::applicationFontFamilies(monospace_font_family_id).at(0);
        monospace_font.setFamily(monospace_font_family);
        monospace_font.setStyleHint(QFont::TypeWriter);

        load();
        std::sort(loaded_languages.begin(), loaded_languages.end());
        newSessionApply();
        emit incrementLoadingProgress();
        emit updateLoadingText("Initialising game...");
    }
    
    Settings::~Settings()
    {
        newSessionApply();
        save();
    }
    
    void Settings::save()
    {
        // Save first time state
        setValue("meta/first_time", first_time);
        
        // Save main window dimensions and position
        setValue("window/geometry", main_window->saveGeometry());

        // Save general settings
        if (current_language == nullptr)
        {
            setValue("common/dictionary", "Error: No dictionaries");
        }
        else
        {
            setValue("common/dictionary", getCurrentLanguage()->getName());
        }
        
        setValue("common/modifiers", getCurrentModifierPattern()->getName());
        setValue("common/shuffle_modifiers", getModifierShuffling());
        setValue("common/letter_colouring", getLetterColouring());

        // Save left player settings
        setValue("player_left/name", getLeftPlayer()->getName());
        setValue("player_left/type", getLeftPlayer()->getTypeAsString());
        setValue("player_left/random_name", getLeftPlayer()->usesRandomName());
        setValue("player_left/ai_difficulty", getLeftPlayer()->getAIDifficulty());

        // Save right player settings
        setValue("player_right/name", getRightPlayer()->getName());
        setValue("player_right/type", getRightPlayer()->getTypeAsString());
        setValue("player_right/random_name", getRightPlayer()->usesRandomName());
        setValue("player_right/ai_difficulty", getRightPlayer()->getAIDifficulty());

        // Save AI settings
        setValue("ai/minimum_turn_time", getMinimumAITurnTime());
        setValue("ai/letter_placing_delay", getAILetterPlacingDelay());
        setValue("ai/auto_restart_delay", getAutoRestartDelay());
        setValue("ai/auto_restart_enabled", isAutoRestartEnabled());
        setValue("ai/bias_strength", getBiasStrength());

        // Save list of languages to load
        beginGroup("startup");
        beginWriteArray("dictionaries");

        int index = 0;

        for (auto language : languages_to_load)
        {
            setArrayIndex(index++);
            setValue("dictionary", language);
        }

        endArray();
        endGroup();

        return;
    }
    
    void Settings::load()
    {
        // Check if this is the first launch
        first_time = value("meta/first_time", "true").toBool();
        
        // Load list of languages to load
        if (first_time)
        {
            for (QString language_name : default_languages)
            {
                languages_to_load.insert(language_name);
            }
        }
        else
        {
            beginGroup("startup");
            int size = beginReadArray("dictionaries");

            for (int index = 0; index < size; ++index)
            {
                setArrayIndex(index);
                languages_to_load.insert(value("dictionary").toString());
            }

            endArray();
            endGroup();
        }

        // Load languages
        detectLanguages();

        if (available_languages_names.empty())
        {
            createDefaultLanguages();
        }
        else
        {
            loadLanguages();
        }

        // Load modifier patterns
        detectModifierPatterns();

        if (available_modifier_patterns_names.empty())
        {
            createDefaultModifierPatterns();
        }
        else
        {
            loadModifierPatterns();
        }

        // Load random names
        loadRandomNames();

        // Load the rest of the settings
        emit updateLoadingText("Loading settings...");
        
        // Load main window dimensions and position
        const QByteArray geometry = value("window/geometry", QByteArray()).toByteArray();
        
        if (!geometry.isEmpty())
        {
            main_window->restoreGeometry(geometry);
        }

        // Load general settings
        setLanguage(value("common/dictionary", "English").toString());
        setModifierPattern(value("common/modifiers", "Wordfeud").toString());
        setModifierShuffling(value("common/shuffle_modifiers", "false").toBool());
        setLetterColouring(value("common/letter_colouring", "Default").toString());

        // Load left player settings
        getLeftPlayer()->setName(value("player_left/name", "Player 1").toString());
        getLeftPlayer()->setTypeWithString(value("player_left/type", "Human").toString());
        getLeftPlayer()->setRandomNameUse(value("player_left/random_name", "false").toBool());
        getLeftPlayer()->setRandomNames(random_names);
        getLeftPlayer()->setAIDifficulty(value("player_left/ai_difficulty", "3").toInt());

        // Load right player settings
        getRightPlayer()->setName(value("player_right/name", "Player 2").toString());
        getRightPlayer()->setTypeWithString(value("player_right/type", "AI").toString());
        getRightPlayer()->setRandomNameUse(value("player_right/random_name", "true").toBool());
        getRightPlayer()->setRandomNames(random_names);
        getRightPlayer()->setAIDifficulty(value("player_right/ai_difficulty", "3").toInt());

        // Load AI settings
        setMinimumAITurnTime(value("ai/minimum_turn_time", "2").toInt());
        setAILetterPlacingDelay(value("ai/letter_placing_delay", "100").toInt());
        setAutoRestartDelay(value("ai/auto_restart_delay", "31").toInt());
        enableAutoRestart(value("ai/auto_restart_enabled", "false").toBool());
        setBiasStrength(value("ai/bias_strength", "10").toInt());

        first_time = false;
        return;
    }
    
    void Settings::newSessionApply()
    {
        
        
        newGameApply();
        return;
    }
    
    void Settings::newGameApply()
    {
        current_language = current_language_temp;
        current_modifier_pattern = current_modifier_pattern_temp;
        shuffle_modifiers = shuffle_modifiers_temp;

        getLeftPlayer()->apply();
        getRightPlayer()->apply();

        bias_strength = bias_strength_temp;

        nextTurnApply();
        return;
    }
    
    void Settings::nextTurnApply()
    {
        minimum_ai_turn_time = minimum_ai_turn_time_temp;
        ai_letter_placing_delay = ai_letter_placing_delay_temp;
        auto_restart_delay = auto_restart_delay_temp;

        immediateApply();
        return;
    }
    
    void Settings::immediateApply()
    {
        letter_colouring = letter_colouring_temp;

        return;
    }
    
    void Settings::setLanguage(const QString& a_language)
    {
        for (auto& language : loaded_languages)
        {
            if (language.getName() == a_language)
            {
                current_language_temp = &language;
                return;
            }
        }

        if (!loaded_languages.empty())
        {
            current_language_temp = &loaded_languages.at(0);
        }

        return;
    }
    
    Language* Settings::getCurrentLanguage()
    {
        return current_language;
    }
    
    Language* Settings::getTempLanguage()
    {
        return current_language_temp;
    }
    
    Language* Settings::getLanguage(const QString& a_language)
    {
        for (auto& language : loaded_languages)
        {
            if (language.getName() == a_language)
            {
                return &language;
            }
        }

        return nullptr;
    }
    
    std::vector<Language>& Settings::getLoadedLanguages()
    {
        return loaded_languages;
    }
    
    const std::set<QString> Settings::getLoadedLanguagesAsStrings() const
    {
        return loaded_languages_names;
    }
    
    const std::set<QString> Settings::getAvailableLanguagesAsStrings()
    {
        detectLanguages();
        return available_languages_names;
    }
    
    const std::set<QString> Settings::getLanguagesToLoadAsStrings()
    {
        return languages_to_load;
    }
    
    void Settings::setLanguagesToLoad(const std::set<QString>& a_language_names)
    {
        languages_to_load = a_language_names;
        return;
    }
    
    void Settings::detectLanguages()
    {
        available_languages_names.clear();
        
        QDir directory{"resources/dictionaries/"};

        if (!directory.exists())
        {
            QDir().mkpath(directory.absolutePath());
        }

        QDirIterator dictionary_directory{directory};
        QString language_path;
        QString language_name;
        QString words_path;
        QString letters_path;

        while (dictionary_directory.hasNext())
        {
            language_path = dictionary_directory.next();
            QFileInfo file_info{language_path};

            if (!file_info.isDir())
            {
                continue;
            }

            language_name = file_info.baseName();

            words_path = QString() % language_path % "/" % language_name % "-words.txt";
            letters_path = QString() % language_path % "/" % language_name % "-letters.csv";

            if (!QFile::exists(words_path) || !QFile::exists(letters_path))
            {
                continue;
            }

            available_languages_names.insert(language_name);
        }

        return;
    }
    
    void Settings::createDefaultLanguages()
    {
        emit languagesCounted(default_languages.size());
        
        for (QString language_name : default_languages)
        {
            emit updateLoadingText(QString() % "Loading dictionaries... (" % language_name % ")");

            if (languages_to_load.contains(language_name))
            {
                Language language;
                language.loadLettersFromFile(":/dictionaries/" % language_name % "/" % language_name % "-letters.csv");
                language.loadWordListFromFilePlain(":/dictionaries/" % language_name % "/" % language_name % "-words.txt");
                language.setName(language_name);
                loaded_languages.push_back(language);
            }

            emit incrementLoadingProgress();
        }

        return;
    }
    
    void Settings::loadLanguages()
    {
        emit languagesCounted(available_languages_names.size());

        for (auto language_name : available_languages_names)
        {
            emit updateLoadingText(QString() % "Loading dictionaries... (" % language_name % ")");

            if (languages_to_load.contains(language_name))
            {
                loaded_languages.push_back(Language(language_name));
                loaded_languages_names.insert(language_name);
            }

            emit incrementLoadingProgress();
        }

        return;
    }
    
    void Settings::setModifierPattern(const QString& a_pattern)
    {
        for (auto& modifier_pattern : loaded_modifier_patterns)
        {
            if (modifier_pattern.getName() == a_pattern)
            {
                current_modifier_pattern_temp = &modifier_pattern;
                return;
            }
        }

        if (!loaded_modifier_patterns.empty())
        {
            current_modifier_pattern_temp = &loaded_modifier_patterns.at(0);
        }

        return;
    }
    
    ModifierPattern* Settings::getCurrentModifierPattern()
    {
        return current_modifier_pattern;
    }
    
    ModifierPattern* Settings::getTempModifierPattern()
    {
        return current_modifier_pattern_temp;
    }
    
    ModifierPattern* Settings::getModifierPattern(const QString& a_pattern)
    {
        for (auto& modifier_pattern : loaded_modifier_patterns)
        {
            if (modifier_pattern.getName() == a_pattern)
            {
                return &modifier_pattern;
            }
        }

        return nullptr;
    }
    
    const std::vector<ModifierPattern>& Settings::getLoadedModifierPatterns()
    {
        return loaded_modifier_patterns;
    }
    
    const std::set<QString> Settings::getLoadedModifierPatternsAsStrings() const
    {
        return loaded_modifier_patterns_names;
    }
    
    const std::set<QString> Settings::getAvailableModifierPatternsAsStrings()
    {
        detectModifierPatterns();
        return available_modifier_patterns_names;
    }
    
    void Settings::detectModifierPatterns()
    {
        available_modifier_patterns_names.clear();
        
        QDir directory{"resources/boards/"};

        if (!directory.exists())
        {
            QDir().mkpath(directory.absolutePath());
        }

        QDirIterator modifier_pattern_directory{directory};
        QString modifier_pattern_path;
        QString modifier_pattern_name;

        while (modifier_pattern_directory.hasNext())
        {
            modifier_pattern_path = modifier_pattern_directory.next();
            QFileInfo file_info{modifier_pattern_path};

            if (!file_info.isFile())
            {
                continue;
            }

            modifier_pattern_name = file_info.baseName();

            if (!QFile::exists(modifier_pattern_path))
            {
                continue;
            }

            available_modifier_patterns_names.insert(modifier_pattern_name);
        }

        return;
    }
    
    void Settings::createDefaultModifierPatterns()
    {
        emit updateLoadingText(QString() % "Loading boards...");
        
        for (QString modifier_pattern_name : default_modifier_patterns)
        {
            ModifierPattern modifier_pattern;
            modifier_pattern.loadFromJSON(":/boards/" % modifier_pattern_name % ".json");
            modifier_pattern.setName(modifier_pattern_name);
            loaded_modifier_patterns.push_back(modifier_pattern);
        }

        emit incrementLoadingProgress();

        return;
    }
    
    void Settings::loadModifierPatterns()
    {
        emit updateLoadingText(QString() % "Loading boards...");
        
        for (auto modifier_pattern_name : available_modifier_patterns_names)
        {
            loaded_modifier_patterns.push_back(ModifierPattern(modifier_pattern_name));
            loaded_modifier_patterns_names.insert(modifier_pattern_name);
        }

        emit incrementLoadingProgress();

        return;
    }
    
    bool Settings::getModifierShuffling() const
    {
        return shuffle_modifiers;
    }
    
    bool Settings::getTempModifierShuffling() const
    {
        return shuffle_modifiers_temp;
    }
    
    void Settings::setModifierShuffling(bool a_enabled)
    {
        shuffle_modifiers_temp = a_enabled;
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
    
    const QSize& Settings::getEditorSelectionTileSize() const
    {
        return editor_selection_tile_size;
    }
    
    const QSize& Settings::getDisplayTileSize() const
    {
        return display_tile_size;
    }
    
    const QSize& Settings::getSourceDimensions() const
    {
        return source_dimensions;
    }
    
    QFont Settings::getMonospaceFont() const
    {
        return monospace_font;
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
    
    void Settings::loadRandomNames()
    {
        emit updateLoadingText("Loading names...");
        
        QDir directory{"resources/names/"};
        QDirIterator name_directory{directory};
        QString file_path;
        QString name;
        
        while (name_directory.hasNext())
        {
            file_path = name_directory.next();
            QFile name_file{file_path};

            if (name_file.open(QIODevice::ReadOnly))
            {
                emit updateLoadingText(QString() % "Loading names... (" % name_file.fileName() % ")");
                
                QTextStream names{&name_file};

                while (!names.atEnd())
                {
                    name = names.readLine();
                    random_names.insert(name);
                }

                name_file.close();
            }
        }

        emit incrementLoadingProgress();

        return;
    }
    
    int Settings::getMinimumAITurnTime() const
    {
        return minimum_ai_turn_time;
    }
    
    int Settings::getTempMinimumAITurnTime() const
    {
        return minimum_ai_turn_time_temp;
    }
    
    void Settings::setMinimumAITurnTime(int a_time)
    {
        minimum_ai_turn_time_temp = a_time;
        return;
    }
    
    int Settings::getAILetterPlacingDelay() const
    {
        return ai_letter_placing_delay;
    }
    
    int Settings::getTempAILetterPlacingDelay() const
    {
        return ai_letter_placing_delay_temp;
    }
    
    void Settings::setAILetterPlacingDelay(int a_time)
    {
        ai_letter_placing_delay_temp = a_time;
        return;
    }
    
    const QString Settings::getLetterColouring() const
    {
        return letter_colouring;
    }
    
    const QString Settings::getTempLetterColouring() const
    {
        return letter_colouring_temp;
    }
    
    void Settings::setLetterColouring(QString a_colour)
    {
        letter_colouring_temp = a_colour;
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
    
    void Settings::setAutoRestartDelay(int a_time)
    {
        auto_restart_delay_temp = a_time;
        return;
    }
    
    int Settings::getAutoRestartDelay() const
    {
        return auto_restart_delay;
    }
    
    int Settings::getTempAutoRestartDelay() const
    {
        return auto_restart_delay_temp;
    }
    
    void Settings::enableAutoRestart(bool a_enabled)
    {
        auto_restart_enabled = a_enabled;
        return;
    }
    
    bool Settings::isAutoRestartEnabled() const
    {
        return auto_restart_enabled;
    }
    
    int Settings::getBiasStrength() const
    {
        return bias_strength;
    }
    
    int Settings::getTempBiasStrength() const
    {
        return bias_strength_temp;
    }
    
    void Settings::setBiasStrength(int a_strength)
    {
        bias_strength_temp = a_strength;
        return;
    }
}