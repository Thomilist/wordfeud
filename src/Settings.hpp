#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QMainWindow>
#include <QSettings>
#include <QSize>
#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QTextStream>

#include "ForwardDeclarations.hpp"

#include "Language.hpp"
#include "ModifierPattern.hpp"
#include "PlayerSettings.hpp"
#include "StartupProgressDialog.hpp"

#include "PlayerType.hpp"

namespace wf
{
    class Settings : public QSettings
    {
        Q_OBJECT
        
        public:
            Settings(QMainWindow* a_main_window, StartupProgressDialog* a_startup_progress_dialog);
            ~Settings();

            void save();
            void load();
            void newSessionApply();
            void newGameApply();
            void nextTurnApply();
            void immediateApply();

            void setLanguage(const QString& a_language);
            Language* getCurrentLanguage();
            Language* getTempLanguage();
            Language* getLanguage(const QString& a_language);
            std::vector<Language>& getLoadedLanguages();
            const std::set<QString> getLoadedLanguagesAsStrings() const;
            const std::set<QString> getAvailableLanguagesAsStrings();
            const std::set<QString> getLanguagesToLoadAsStrings();
            void setLanguagesToLoad(const std::set<QString>& a_language_names);
            void detectLanguages();
            void createDefaultLanguages();
            void loadLanguages();

            void setModifierPattern(const QString& a_pattern);
            ModifierPattern* getCurrentModifierPattern();
            ModifierPattern* getTempModifierPattern();
            ModifierPattern* getModifierPattern(const QString& a_pattern);
            const std::vector<ModifierPattern>& getLoadedModifierPatterns();
            const std::set<QString> getLoadedModifierPatternsAsStrings() const;
            const std::set<QString> getAvailableModifierPatternsAsStrings();
            void detectModifierPatterns();
            void createDefaultModifierPatterns();
            void loadModifierPatterns();
            bool getModifierShuffling() const;
            bool getTempModifierShuffling() const;
            void setModifierShuffling(bool a_enabled);

            void setGridDimensions(int a_rows, int a_collumns);
            void setTileSize(int a_size);
            const QSize& getBoardDimensions() const;
            const QSize& getBoardTileSize() const;
            const QSize& getHandDimensions() const;
            const QSize& getHandTileSize() const;
            const QSize& getSelectionTileSize() const;
            const QSize& getEditorSelectionTileSize() const;
            const QSize& getDisplayTileSize() const;
            const QSize& getSourceDimensions() const;
            QFont getMonospaceFont() const;
            PlayerSettings* getLeftPlayer();
            PlayerSettings* getRightPlayer();
            PlayerSettings* getPlayer(int a_index);
            void loadRandomNames();
            int getMinimumAITurnTime() const;
            int getTempMinimumAITurnTime() const;
            void setMinimumAITurnTime(int a_time);
            int getAILetterPlacingDelay() const;
            int getTempAILetterPlacingDelay() const;
            void setAILetterPlacingDelay(int a_time);
            const QString getLetterColouring() const;
            const QString getTempLetterColouring() const;
            void setLetterColouring(QString a_colour);
            int getMinimumAIDifficulty() const;
            int getMaximumAIDifficulty() const;
            void setAutoRestartDelay(int a_time);
            int getAutoRestartDelay() const;
            int getTempAutoRestartDelay() const;
            void enableAutoRestart(bool a_enabled);
            bool isAutoRestartEnabled() const;
            int getBiasStrength() const;
            int getTempBiasStrength() const;
            void setBiasStrength(int a_strength);
        
        signals:
            void languagesCounted(int a_count);
            void updateLoadingText(QString a_text);
            void incrementLoadingProgress();

        private:
            QMainWindow* main_window;
            bool first_time = true;
            QStringList default_languages{"Danish", "English"};
            std::vector<Language> loaded_languages;
            std::set<QString> loaded_languages_names;
            std::set<QString> available_languages_names;
            QStringList default_modifier_patterns{"Scrabble", "Wordfeud"};
            std::vector<ModifierPattern> loaded_modifier_patterns;
            std::set<QString> loaded_modifier_patterns_names;
            std::set<QString> available_modifier_patterns_names;
            QSize board_dimensions{15,15};
            QSize board_tile_size{42,42};
            QSize hand_dimensions{7,1};
            QSize hand_tile_size{90,90};
            QSize selection_tile_size{64,64};
            QSize editor_selection_tile_size{30,30};
            QSize display_tile_size{200,200};
            QSize source_dimensions{5,5};
            QFont monospace_font;
            std::set<QString> random_names;
            const int minimum_ai_difficulty = 1;
            const int maximum_ai_difficulty = 10;
            bool auto_restart_enabled;

            // User-facing settings
            Language* current_language = nullptr;
            ModifierPattern* current_modifier_pattern = nullptr;
            bool shuffle_modifiers;
            QString letter_colouring;

            PlayerSettings left_player;
            PlayerSettings right_player;
            
            int minimum_ai_turn_time;
            int ai_letter_placing_delay;
            int auto_restart_delay;
            int bias_strength;

            std::set<QString> languages_to_load;

            // Temporary user-facing settings
            Language* current_language_temp = nullptr;
            ModifierPattern* current_modifier_pattern_temp = nullptr;
            bool shuffle_modifiers_temp;
            QString letter_colouring_temp;

            int minimum_ai_turn_time_temp;
            int ai_letter_placing_delay_temp;
            int auto_restart_delay_temp;
            int bias_strength_temp;
    };
}
#endif // __SETTINGS_H__