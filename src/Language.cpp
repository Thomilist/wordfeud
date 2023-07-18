#include "Language.hpp"
namespace wf
{
    Language::Language()
    { }
    
    Language::Language(QString a_language)
        : language_name(a_language)
    {
        loadLanguage(a_language);
    }
    
    Language::~Language()
    { }
    
    QString Language::getPath(QString a_language)
    {
        return QString() % "resources/dictionaries/" % a_language % "/";
    }
    
    QString Language::getWordListPath(QString a_language)
    {
        return QString() % "resources/dictionaries/" % a_language % "/" % a_language % "-words.txt";
    }
    
    QString Language::getBiasedWordListPath(QString a_language)
    {
        return QString() % "resources/dictionaries/" % a_language % "/" % a_language % "-biased-words.txt";
    }
    
    QString Language::getLetterListPath(QString a_language)
    {
        return QString() % "resources/dictionaries/" % a_language % "/" % a_language % "-letters.csv";
    }
    
    void Language::setLetterList(std::vector<LetterData>* a_letter_list)
    {
        letter_list.clear();
        letter_list = *a_letter_list;
        return;
    }
    
    bool operator<(Language& a_first, Language& a_second)
    {
        return a_first.getName() < a_second.getName();
    }
    
    std::vector<LetterData>* Language::getLetterList()
    {
        return &letter_list;
    }
    
    const std::unordered_set<QString>* Language::getWordList() const
    {
        return &word_list;
    }
    
    bool Language::isInWordList(QString a_word)
    {
        return word_list.contains(a_word);
    }
    
    bool Language::isInBiasedWordList(QString a_word)
    {
        return biased_word_list.contains(a_word);
    }
    
    bool Language::hasBiasedWords()
    {
        return !biased_word_list.empty();
    }
    
    const QString& Language::getName() const
    {
        return language_name;
    }
    
    void Language::setName(QString a_name)
    {
        language_name = a_name;
        return;
    }
    
    void Language::loadLanguage(QString a_language)
    {
        QString letters_path{getLetterListPath(a_language)};
        QString words_path{getWordListPath(a_language)};
        QString biased_words_path{getBiasedWordListPath(a_language)};

        if (!QFile::exists(words_path) || !QFile::exists(letters_path))
        {
            return;
        }

        loadLettersFromFile(letters_path);
        loadWordListFromFilePlain(words_path);

        if (QFile::exists(biased_words_path))
        {
            loadWordListFromFilePlain(biased_words_path, true);
        }

        return;
    }
    
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
    
    void Language::loadWordListFromFilePlain(QString a_file_path, bool a_load_biased_words)
    {
        QFile word_list_file{a_file_path};

        if (word_list_file.open(QIODevice::ReadOnly))
        {
            if (a_load_biased_words)
            {
                biased_word_list.clear();
            }
            else
            {
                word_list.clear();
            }
            
            QTextStream words{&word_list_file};

            while (!words.atEnd())
            {
                QString word = word_list_file.readLine().trimmed().toUpper();
                word_list.insert(word);

                if (a_load_biased_words)
                {
                    biased_word_list.insert(word);
                }
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
    
    void Language::exportWordList(bool a_export_biased_words)
    {
        if (a_export_biased_words)
        {
            exportWordList("resources/export/" + language_name + "_biased_words_export.txt", true);
        }
        else
        {
            exportWordList("resources/export/" + language_name + "_words_export.txt");
        }
        
        return;
    }
    
    void Language::exportWordList(QString a_file_path, bool a_export_biased_words)
    {
        QFile export_file{a_file_path};
        QFileInfo export_file_info{export_file};
        QDir export_directory{export_file_info.absoluteDir()};

        if (!export_directory.exists())
        {
            QDir().mkpath(export_directory.absolutePath());
        }

        if (export_file.open(QIODevice::WriteOnly))
        {
            QTextStream word_export{&export_file};
            std::unordered_set<QString>& list = a_export_biased_words ? biased_word_list : word_list;

            for (auto word : list)
            {
                word_export << word << "\n";
            }

            export_file.close();
        }
        
        return;
    }
    
    void Language::exportLetterList()
    {
        exportLetterList("resources/export/" + language_name + "_letters_export.txt");
        return;
    }
    
    void Language::exportLetterList(QString a_file_path)
    {
        QFile export_file{a_file_path};
        QFileInfo export_file_info{export_file};
        QDir export_directory{export_file_info.absoluteDir()};

        if (!export_directory.exists())
        {
            QDir().mkpath(export_directory.absolutePath());
        }

        if (export_file.open(QIODevice::WriteOnly))
        {
            QTextStream letter_export{&export_file};

            for (auto letter : letter_list)
            {
                letter_export << letter.letter << "," << letter.count << "," << letter.points << "\n";
            }

            export_file.close();
        }

        return;
    }
    
    void Language::addLetter(LetterData a_letter_data)
    {
        letter_list.push_back(a_letter_data);
        return;
    }
    
    bool Language::removeLetter(size_t a_index)
    {
        if (a_index >= letter_list.size())
        {
            return false;
        }

        letter_list.erase(letter_list.begin() + a_index);
        return true;
    }
    
    void Language::removeAllLetters()
    {
        letter_list.clear();
        return;
    }
}