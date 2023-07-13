#ifndef __LETTERTABLEMODEL_H__
#define __LETTERTABLEMODEL_H__

#include <vector>

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>

#include "ForwardDeclarations.hpp"

#include "Language.hpp"
#include "Letter.hpp"

namespace wf
{
    class LetterTableModel : public QAbstractTableModel
    {
        Q_OBJECT

        public:
            LetterTableModel(Language* a_language, QObject* a_parent = nullptr);
            ~LetterTableModel();

            int rowCount(const QModelIndex& a_parent = QModelIndex()) const override;
            int columnCount(const QModelIndex& a_parent = QModelIndex()) const override;
            bool insertColumns(int a_column, int a_count, const QModelIndex& a_parent = QModelIndex()) override;
            bool removeColumns(int a_column, int a_count, const QModelIndex& a_parent = QModelIndex()) override;
            QVariant data(const QModelIndex& a_index, int a_role = Qt::DisplayRole) const override;
            QVariant headerData(int a_section, Qt::Orientation a_orientation, int a_role = Qt::DisplayRole) const override;
            void setLetterList(std::vector<LetterData>* a_letter_list);

        public slots:
            void addLetter(LetterData a_letter_data);
            void removeLetter(int a_position);
        
        private:
            Language* language;
    };
}

#endif // __LETTERTABLEMODEL_H__