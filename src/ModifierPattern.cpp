#include "ModifierPattern.hpp"


namespace wf
{
    ModifierPattern::ModifierPattern()
        : rng(std::default_random_engine{})
    { }

    ModifierPattern::ModifierPattern(const QString& a_modifier_pattern)
        : rng(std::default_random_engine{})
    {
        setName(a_modifier_pattern);
        loadFromJSON(a_modifier_pattern);
    }
    
    ModifierPattern::~ModifierPattern()
    { }
    
    const QString ModifierPattern::getPath(const QString& a_modifier_pattern)
    {
        return QString() % "resources/boards/" % a_modifier_pattern % ".json";
    }
    
    const std::vector<Modifier> ModifierPattern::get(bool a_shuffle)
    {
        std::vector<Modifier> pattern;
        
        for (auto& modifier : modifiers)
        {
            pattern.push_back(modifier);
        }

        if (a_shuffle)
        {
            std::shuffle(pattern.begin(), pattern.end(), rng);

            int middle_index = pattern.size() / 2;

            if (pattern.at(middle_index).getType() != ModifierType::Start)
            {
                for (auto& modifier : pattern)
                {
                    if (modifier.getType() == ModifierType::Start)
                    {
                        std::swap(modifier, pattern.at(middle_index));
                        break;
                    }
                }
            }
        }
        
        return pattern;
    }
    
    void ModifierPattern::setName(const QString& a_name)
    {
        name = a_name;
        return;
    }
    
    const QString ModifierPattern::getName(bool a_shuffled) const
    {
        return a_shuffled ? QString() % name % " (shuffled)" : name;
    }
    
    const QString ModifierPattern::getPath() const
    {
        if (internal)
        {
            return QString() % ":/boards/" % name % ".json";
        }
        else
        {
            return QString() % "resources/boards/" % name % ".json";
        }
    }
    
    void ModifierPattern::setInternal(bool a_state)
    {
        internal = a_state;
        return;
    }
    
    bool ModifierPattern::isInternal() const
    {
        return internal;
    }
    
    const QSize& ModifierPattern::getBoardDimensions() const
    {
        return dimensions;
    }
    
    void ModifierPattern::loadFromBoard(VirtualBoard* a_board)
    {
        modifiers.clear();

        dimensions.setHeight(a_board->getGridDimensions().height());
        dimensions.setWidth(a_board->getGridDimensions().width());

        for (int row = 0; row < dimensions.height(); ++row)
        {
            for (int column = 0; column < dimensions.width(); ++column)
            {
                VirtualTile* tile = a_board->getTileAtPosition(column, row);
                
                if (tile == nullptr)
                {
                    continue;
                }

                if (tile->getModifier() == nullptr || tile->getModifier()->getType() == ModifierType::None)
                {
                    Modifier modifier;
                    modifier.setType(ModifierType::None);
                    modifiers.push_back(modifier);
                }
                else
                {
                    modifiers.push_back(*tile->getModifier());
                }
            }
        }

        return;
    }
    
    void ModifierPattern::loadFromJSON(const QString& a_modifier_pattern)
    {
        std::ifstream json_file{getPath(a_modifier_pattern).toStdString()};

        if (json_file.is_open())
        {
            try
            {
                nlohmann::json json_data = nlohmann::json::parse(json_file);

                dimensions.setHeight(json_data["dimensions"]["height"]);
                dimensions.setWidth(json_data["dimensions"]["width"]);

                modifiers.clear();

                for (int row = 0; row < dimensions.height(); ++row)
                {
                    for (int column = 0; column < dimensions.width(); ++column)
                    {
                        modifiers.push_back(json_data["modifiers"][row][column]);
                    }
                }
            }
            catch (nlohmann::json::exception& e)
            {
                std::cerr
                << "An error occured while reading board from file \""
                << getPath(a_modifier_pattern).toStdString()
                << "\":\n"
                << e.what()
                << "\n";
            }
        }

        return;
    }
    
    void ModifierPattern::exportToJSON()
    {
        exportToJSON(QString() % "resources/export/boards/" % name % "_export.json");
    }
    
    void ModifierPattern::exportToJSON(const QString& a_file_path)
    {
        nlohmann::json json_data;

        json_data["dimensions"]["height"] = dimensions.height();
        json_data["dimensions"]["width"] = dimensions.width();

        int row = 0;
        int column = 0;

        for (const auto& modifier : modifiers)
        {
            json_data["modifiers"][row][column] = modifier;

            if (++column >= dimensions.width())
            {
                column = 0;
                ++row;
            }
        }

        std::ofstream json_file{a_file_path.toStdString()};

        if (json_file.is_open())
        {
            json_file << json_data.dump(2);
        }

        return;
    }
}