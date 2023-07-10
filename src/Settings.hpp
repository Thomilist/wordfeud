#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

#include <QDebug>

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
#include <QTextStream>

#include "ForwardDeclarations.hpp"

#include "Language.hpp"
#include "ModifierPattern.hpp"
#include "PlayerSettings.hpp"

#include "PlayerType.hpp"

namespace wf
{
    class Settings : public QSettings
    {
        public:
            Settings(QMainWindow* a_main_window);
            ~Settings();

            void save();
            void load();
            void newGameApply();
            void nextTurnApply();
            void immediateApply();
            void setGridDimensions(int a_rows, int a_collumns);
            void setTileSize(int a_size);
            void setLanguage(QString a_language);
            Language* getCurrentLanguage();
            Language* getTempLanguage();
            Language* getLanguage(QString a_language);
            std::vector<Language>& getLoadedLanguages();
            std::set<QString> getAvailableLanguages();
            void detectLanguages();
            void createDefaultLanguagesIfNoneFound();
            void loadLanguages();
            const QSize& getBoardDimensions() const;
            const QSize& getBoardTileSize() const;
            const QSize& getHandDimensions() const;
            const QSize& getHandTileSize() const;
            const QSize& getSelectionTileSize() const;
            QFont getMonospaceFont() const;
            ModifierPattern* getModifierPattern();
            const QString getTempModifierPattern() const;
            void setModifierPattern(QString a_pattern);
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

        private:
            QMainWindow* main_window;
            std::vector<Language> languages;
            std::set<QString> language_names;
            QSize board_dimensions{15,15};
            QSize board_tile_size{42,42};
            QSize hand_dimensions{7,1};
            QSize hand_tile_size{90,90};
            QSize selection_tile_size{64,64};
            QFont monospace_font;
            std::set<QString> random_names;
            const int minimum_ai_difficulty = 1;
            const int maximum_ai_difficulty = 10;
            bool auto_restart_enabled;

            // User-facing settings
            Language* current_language = nullptr;
            ModifierPattern modifier_pattern;
            int minimum_ai_turn_time;
            int ai_letter_placing_delay;
            QString letter_colouring;
            int auto_restart_delay;

            PlayerSettings left_player;
            PlayerSettings right_player;

            // Temporary user-facing settings
            Language* current_language_temp;
            QString modifier_pattern_temp;
            int minimum_ai_turn_time_temp;
            int ai_letter_placing_delay_temp;
            QString letter_colouring_temp;
            int auto_restart_delay_temp;
    };
}
#endif // __SETTINGS_H__