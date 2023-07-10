#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <QDebug>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QIODevice>
#include <QString>
#include <QStringBuilder>
#include <QStringList>
#include <QTextStream>

#include <vector>
#include <unordered_map>
#include <unordered_set>

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

            const std::vector<LetterData>& getLetterList() const;
            const std::unordered_set<QString>& getWordList() const;
            bool isInWordList(QString a_word);
            const QString& getName() const;
            void setName(QString a_name);
            void loadLanguage(QString a_language);
            void loadLettersFromFile(QString a_file_path);
            void loadWordListFromFilePlain(QString a_file_path);
            void loadWordListFromFileCOR(QString a_file_path);
            void exportWordList();
            void exportWordList(QString a_file_path);
            void exportLetterList();
            void exportLetterList(QString a_file_path);

        private:
            std::vector<LetterData> letter_list;
            std::unordered_map<QString, QString> letter_transformations;
            std::unordered_set<QString> word_list;
            QString language_name;
    };
}

#endif // __LANGUAGE_H__