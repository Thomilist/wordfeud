#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QIODevice>
#include <QLocale>
#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QTextStream>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"

namespace wf
{
    class Language
    {
        public:
            Language();
            Language(const QString& a_language);
            ~Language();
            friend bool operator<(Language& a_first, Language& a_second);

            static const QString getPath(const QString& a_language);
            static const QString getWordListPath(const QString& a_language);
            static const QString getBiasedWordListPath(const QString& a_language);
            static const QString getLetterListPath(const QString& a_language);

            const QString getPath() const;
            const QString getWordListPath() const;
            const QString getBiasedWordListPath() const;
            const QString getLetterListPath() const;
            bool isInternal() const;
            void setInternal(bool a_state);
            void setLetterList(std::vector<LetterData>* a_letter_list);
            std::vector<LetterData>* getLetterList();
            const std::unordered_set<QString>* getWordList() const;
            bool isInWordList(QString a_word);
            bool isInBiasedWordList(QString a_word);
            bool hasBiasedWords();
            const QString& getName() const;
            void setName(const QString& a_name);
            void loadLanguage(const QString& a_language);
            void loadLettersFromFile(const QString& a_file_path);
            void loadWordListFromFilePlain(const QString& a_file_path, bool a_load_biased_words = false);
            void loadWordListFromFileCOR(const QString& a_file_path);
            void exportWordList(bool a_export_biased_words = false);
            void exportWordList(const QString& a_file_path, bool a_export_biased_words = false);
            void exportLetterList();
            void exportLetterList(const QString& a_file_path);
            void addLetter(LetterData a_letter_data);
            bool removeLetter(size_t a_index);
            void removeAllLetters();

        private:
            QString language_name;
            bool internal = false;
            std::vector<LetterData> letter_list;
            std::unordered_set<QString> word_list;
            std::unordered_set<QString> biased_word_list;
            QString letter_list_path;
            QString word_list_path;
            QString biased_words_path;
            QLocale system_locale;
    };
}

#endif // __LANGUAGE_H__