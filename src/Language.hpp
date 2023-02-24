#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <QString>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>

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
            void loadLettersFromFile(QString a_file_path);
            void loadWordListFromFilePlain(QString a_file_path);
            void loadWordListFromFileCOR(QString a_file_path);
            std::vector<LetterData> letter_list;
            std::unordered_map<QString, QString> letter_transformations;
            std::unordered_set<QString> word_list;
            QString language_string;
            LanguageName language;
    };
}
#endif // __LANGUAGE_H__