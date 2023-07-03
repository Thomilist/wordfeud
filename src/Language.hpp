#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <QFile>
#include <QHash>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "ForwardDeclarations.hpp"

#include "Letter.hpp"

#include "LanguageName.hpp"

namespace wf
{
    class Language
    {
        public:
            Language(LanguageName a_language);
            ~Language();

            const std::vector<LetterData>& getLetterList() const;
            const std::unordered_set<QString>& getWordList() const;
            bool isInWordList(QString a_word);
            const QString& asString() const;
            const LanguageName& asEnum() const;

        private:
            void loadLanguage(LanguageName a_language);
            void loadLettersFromFile(QString a_file_path);
            void loadWordListFromFilePlain(QString a_file_path);
            void loadWordListFromFileCOR(QString a_file_path);
            void exportWordList();
            std::vector<LetterData> letter_list;
            std::unordered_map<QString, QString> letter_transformations;
            std::unordered_set<QString> word_list;
            QString language_string;
            LanguageName language;
    };
}

#endif // __LANGUAGE_H__