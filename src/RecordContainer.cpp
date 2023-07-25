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
        qRegisterMetaTypeStreamOperators<Score>("Score");
        load();
    }
    
    RecordContainer::~RecordContainer()
    {
        save();
    }
    
    void RecordContainer::addToScoreCount(PlayerType a_player_type, int a_count)
    {
        int* count = nullptr;
        
        switch (a_player_type)
        {
            case PlayerType::AI:
            {
                count = &ai_scores;
                break;
            }
            case PlayerType::Human:
            {
                count = &human_scores;
                break;
            }
        }
        
        *count += a_count;

        if (*count < 0)
        {
            *count = 0;
        }

        return;
    }
    
    int RecordContainer::getScoreCount(PlayerType a_player_type) const
    {
        switch (a_player_type)
        {
            case PlayerType::AI:
            {
                return ai_scores;
            }
            case PlayerType::Human:
            {
                return human_scores;
            }
        }

        return 0;
    }
    
    void RecordContainer::load()
    {
        ai_scores = value("meta/ai_scores", "0").toInt();
        human_scores = value("meta/human_scores", "0").toInt();
        
        beginGroup("scores");

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
        setValue("meta/ai_scores", ai_scores);
        setValue("meta/human_scores", human_scores);

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