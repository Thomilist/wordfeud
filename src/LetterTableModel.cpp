#include "LetterTableModel.hpp"


namespace wf
{
    LetterTableModel::LetterTableModel(Language* a_language, QObject* a_parent)
        : QAbstractTableModel(a_parent)
        , language(a_language)
    {
        
    }
    
    LetterTableModel::~LetterTableModel()
    { }
    
    int LetterTableModel::rowCount(const QModelIndex&) const 
    {
        return 3;
    }
    
    int LetterTableModel::columnCount(const QModelIndex&) const 
    {
        if (language == nullptr)
        {
            return 0;
        }
        else
        {
            return language->getLetterList()->size();
        }
    }
    
    bool LetterTableModel::insertColumns(int a_column, int a_count, const QModelIndex& a_parent)
    {
        beginInsertColumns(a_parent, a_column, a_column + a_count - 1);

        std::vector<LetterData>* letter_list = language->getLetterList();
        letter_list->insert(letter_list->begin() + a_column, a_count, LetterData());

        endInsertColumns();
        return true;
    }
    
    bool LetterTableModel::removeColumns(int a_column, int a_count, const QModelIndex& a_parent)
    {
        beginRemoveColumns(a_parent, a_column, a_column + a_count - 1);

        std::vector<LetterData>* letter_list = language->getLetterList();
        letter_list->erase(letter_list->begin() + a_column, letter_list->begin() + a_column + a_count - 1);

        endRemoveColumns();
        return true;
    }
    
    QVariant LetterTableModel::data(const QModelIndex& a_index, int a_role) const 
    {
        if (language == nullptr)
        {
            return QVariant();
        }
        
        switch (a_role)
        {
            case Qt::DisplayRole:
            {
                LetterData letter_data = language->getLetterList()->at(a_index.column());

                switch (a_index.row())
                {
                    case 0:
                    {
                        return letter_data.letter;
                    }
                    case 1:
                    {
                        return letter_data.count;
                    }
                    case 2:
                    {
                        return letter_data.points;
                    }
                }

                break;
            }
            case Qt::TextAlignmentRole:
            {
                return Qt::AlignCenter;
            }
            default:
            {
                break;
            }
        }
        
        return QVariant();
    }
    
    QVariant LetterTableModel::headerData(int a_section, Qt::Orientation a_orientation, int a_role) const 
    {
        switch (a_orientation)
        {
            case Qt::Orientation::Horizontal:
            {
                break;;
            }
            case Qt::Orientation::Vertical:
            {
                switch (a_role)
                {
                    case Qt::DisplayRole:
                    {
                        switch (a_section)
                        {
                            case 0:
                            {
                                return QString("Letter");
                            }
                            case 1:
                            {
                                return QString("Count");
                            }
                            case 2:
                            {
                                return QString("Points");
                            }
                        }
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }

        return QVariant();
    }
    
    void LetterTableModel::setLetterList(std::vector<LetterData>* a_letter_list)
    {
        if (!language->getLetterList()->empty())
        {
            removeColumns(0, language->getLetterList()->size());
        }
        
        if (!a_letter_list->empty())
        {
            insertColumns(0, a_letter_list->size());
            language->setLetterList(a_letter_list);
        }

        emit dataChanged(createIndex(0, 0), createIndex(2, columnCount() - 1), {Qt::DisplayRole});
        return;
    }
    
    void LetterTableModel::addLetter(LetterData a_letter_data)
    {
        insertColumn(columnCount());
        language->getLetterList()->back() = a_letter_data;
        emit dataChanged(createIndex(0, 0), createIndex(2, columnCount() - 1), {Qt::DisplayRole});
        return;
    }
    
    void LetterTableModel::removeLetter(int a_position)
    {
        removeColumn(a_position);
        emit dataChanged(createIndex(0, 0), createIndex(2, columnCount() - 1), {Qt::DisplayRole});
        return;
    }
}