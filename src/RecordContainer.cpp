#include "RecordContainer.hpp"


namespace wf
{
    RecordContainer::RecordContainer(
        QString a_organisation,
        QString a_application,
        QObject* a_parent)
        : QSettings(
            a_organisation,
            a_application,
            a_parent)
    {
        qRegisterMetaType<Score>("Score");
        load();
    }
    
    RecordContainer::~RecordContainer()
    {
        save();
    }
    
    int RecordContainer::getScoreCount(const Score& a_score) const
    {
        int count = 0;
        
        for (const auto& score : *this)
        {
            if (Score::sameGameSetup(a_score, score))
            {
                ++count;
            }
        }

        return count;
    }
    
    void RecordContainer::load()
    {
        beginGroup("scores");

        std::vector<Score>::clear();

        int size = beginReadArray("array");
        resize(size);

        for (int index = 0; index < size; ++index)
        {
            setArrayIndex(index);
            (*this)[index] = value("score").value<Score>();
        }

        endArray();
        endGroup();

        return;
    }
    
    void RecordContainer::save()
    {
        beginGroup("scores");

        int index = 0;
        beginWriteArray("array");

        for (auto score : *this)
        {
            setArrayIndex(index++);
            setValue("score", QVariant::fromValue(score));
        }

        endArray();
        endGroup();

        return;
    }
}