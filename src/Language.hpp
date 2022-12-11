#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include <QString>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace wf
{
    struct LetterData
    {
        QString letter;
        int count;
        int points;
    };
    
    class Language
    {
        public:
            Language();
            ~Language();

            void loadLettersFromFile(QString a_file_path);
            void loadWordListFromFile(QString a_file_path);
            const std::vector<LetterData>& getLetterList();

        private:
            std::vector<LetterData> letter_list;
            std::unordered_map<QString, QString> letter_transformations;
            std::unordered_set<QString> word_list;
    };
}
#endif // __LANGUAGE_H__