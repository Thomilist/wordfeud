#include "Language.hpp"
namespace wf
{
    Language::Language(LanguageName a_language)
        : language(a_language)
    {
        loadInternalLanguage(a_language);
    }
    
    Language::Language(QString a_language)
        : language_string(a_language)
    {
        loadExternalLanguage(a_language);
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
                        letter_data_split.at(0).size() == 0 ? QString{} : letter_data_split.at(0),
                        letter_data_split.at(1).toInt(),
                        letter_data_split.at(2).toInt()
                    };

                    letter_list.push_back(letter_data);
                }
            }

            letter_file.close();
        }

        return;
    }
    
    void Language::loadWordListFromFilePlain(QString a_file_path)
    {
        QFile word_list_file{a_file_path};

        if (word_list_file.open(QIODevice::ReadOnly))
        {
            word_list.clear();
            
            QTextStream words{&word_list_file};

            while (!words.atEnd())
            {
                QString word = word_list_file.readLine();
                word_list.insert(word.trimmed().toUpper());
            }

            word_list_file.close();
        }

        return;
    }
    
    void Language::loadWordListFromFileCOR(QString a_file_path)
    {
        QFile word_list_file{a_file_path};

        if (word_list_file.open(QIODevice::ReadOnly))
        {
            word_list.clear();
            
            QTextStream words{&word_list_file};

            while (!words.atEnd())
            {
                QString line = word_list_file.readLine();
                QStringList line_split = line.split('\t');
                QString word = line_split.at(4);

                // Words containing whitespace are not included (each part should appear on their own)
                if (word.contains("\\s"))
                {
                    continue;
                }

                word_list.insert(word.trimmed().toUpper());
            }

            word_list_file.close();
        }

        return;
    }
    
    void Language::exportWordList()
    {
        QFile export_file{language_string + "_word_list_export.txt"};

        if (export_file.open(QIODevice::Append))
        {
            QTextStream word_export{&export_file};

            for (auto word : word_list)
            {
                word_export << "\"" << word << "\",\n";
            }

            export_file.close();
        }
    }
    
    const std::vector<LetterData>& Language::getLetterList() const
    {
        return letter_list;
    }
    
    const std::unordered_set<QString>& Language::getWordList() const
    {
        return word_list;
    }
    
    bool Language::isInWordList(QString a_word)
    {
        return word_list.contains(a_word);
    }
    
    const QString& Language::asString() const
    {
        return language_string;
    }
    
    const LanguageName& Language::asEnum() const
    {
        return language;
    }
    
    void Language::loadInternalLanguage(LanguageName a_language)
    {
        switch (a_language)
        {
            case LanguageName::Danish:
            {
                language_string = "Danish";
                word_list.reserve(530916);
                loadWordListFromFileCOR(":/dictionaries/internal/Danish/Danish-words.txt");
                loadLettersFromFile(":/dictionaries/internal/Danish/Danish-letters.csv");
                break;
            }
            case LanguageName::English:
            {
                language_string = "English";
                word_list.reserve(370105);
                loadWordListFromFilePlain(":/dictionaries/internal/English/English-words.txt");
                loadLettersFromFile(":/dictionaries/internal/English/English-letters.csv");
                break;
            }
            case LanguageName::German:
            {
                language_string = "German";
                word_list.reserve(1908815);
                loadWordListFromFilePlain(":/dictionaries/internal/German/German-words.txt");
                loadLettersFromFile(":/dictionaries/internal/German/German-letters.csv");
                break;
            }
            default:
            {
                break;
            }
        }

        return;
    }
    
    void Language::loadExternalLanguage(QString a_language)
    {
        QString words_path{QString() % "./resources/dictionaries/external/" % a_language % "/" % a_language % "-words.txt"};
        QString letters_path{QString() % "./resources/dictionaries/external/" % a_language % "/" % a_language % "-letters.csv"};

        if (!QFile::exists(words_path) || !QFile::exists(letters_path))
        {
            language = LanguageName::Invalid;
            return;
        }

        language = LanguageName::External;
        loadWordListFromFilePlain(words_path);
        loadLettersFromFile(letters_path);
        return;
    }
}