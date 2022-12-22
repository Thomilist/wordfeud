#include "Language.hpp"
namespace wf
{
    Language::Language()
    {
        
    }
    
    Language::~Language()
    { }
    
    void Language::loadLettersFromFile(QString a_file_path)
    {
        QFile letter_file{a_file_path};

        if (letter_file.open(QIODevice::ReadOnly))
        {
            letter_list.clear();
            
            QTextStream letters{&letter_file};

            while (!letters.atEnd())
            {
                QString letter_data_raw = letter_file.readLine();
                QStringList letter_data_split = letter_data_raw.split(',');

                if (letter_data_split.size() == 3)
                {
                    LetterData letter_data = 
                    {
                        letter_data_split.at(0),
                        letter_data_split.at(1).toInt(),
                        letter_data_split.at(2).toInt()
                    };

                    letter_list.push_back(letter_data);
                }
            }

            letter_file.close();
        }
    }
    
    void Language::loadWordListFromFile(QString a_file_path)
    {
        QFile word_list_file{a_file_path};

        if (word_list_file.open(QIODevice::ReadOnly))
        {
            word_list.clear();
            
            QTextStream words{&word_list_file};

            while (!words.atEnd())
            {
                QString word = word_list_file.readLine();
                word_list.insert(word.trimmed());
            }

            word_list_file.close();
        }
    }
    
    const std::vector<LetterData>& Language::getLetterList() const
    {
        return letter_list;
    }
}