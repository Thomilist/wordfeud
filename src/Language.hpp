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
            Language(QString a_language);
            ~Language();
            friend bool operator<(Language& a_first, Language& a_second);

            static QString getPath(QString a_language);
            static QString getWordListPath(QString a_language);
            static QString getBiasedWordListPath(QString a_language);
            static QString getLetterListPath(QString a_language);

            void setLetterList(std::vector<LetterData>* a_letter_list);
            std::vector<LetterData>* getLetterList();
            const std::unordered_set<QString>* getWordList() const;
            bool isInWordList(QString a_word);
            bool isInBiasedWordList(QString a_word);
            bool hasBiasedWords();
            const QString& getName() const;
            void setName(QString a_name);
            void loadLanguage(QString a_language);
            void loadLettersFromFile(QString a_file_path);
            void loadWordListFromFilePlain(QString a_file_path, bool a_load_biased_words = false);
            void loadWordListFromFileCOR(QString a_file_path);
            void exportWordList(bool a_export_biased_words = false);
            void exportWordList(QString a_file_path, bool a_export_biased_words = false);
            void exportLetterList();
            void exportLetterList(QString a_file_path);
            void addLetter(LetterData a_letter_data);
            bool removeLetter(size_t a_index);
            void removeAllLetters();

        private:
            std::vector<LetterData> letter_list;
            std::unordered_set<QString> word_list;
            std::unordered_set<QString> biased_word_list;
            QString language_name;
    };
}

#endif // __LANGUAGE_H__