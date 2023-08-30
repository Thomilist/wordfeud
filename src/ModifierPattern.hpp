#ifndef __MODIFIERPATTERN_H__
#define __MODIFIERPATTERN_H__

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include <QSize>
#include <QString>
#include <QStringBuilder>

#include <nlohmann/json.hpp>

#include "ForwardDeclarations.hpp"

#include "Modifier.hpp"
#include "VirtualBoard.hpp"
#include "VirtualTile.hpp"

#include "ModifierType.hpp"

namespace wf
{
    class ModifierPattern
    {
        public:
            ModifierPattern();
            ModifierPattern(const QString& a_modifier_pattern);
            ~ModifierPattern();

            static const QString getPath(const QString& a_modifier_pattern);

            const std::vector<Modifier> get(bool a_shuffle = false);
            void setName(const QString& a_name);
            const QString getName(bool a_shuffled = false) const;
            const QString getPath() const;
            void setInternal(bool a_state);
            bool isInternal() const;
            const QSize& getBoardDimensions() const;
            void loadFromBoard(VirtualBoard* a_board);
            void loadFromJSON(const QString& a_modifier_pattern);
            void exportToJSON();
            void exportToJSON(const QString& a_file_path);

        private:
            QString name;
            bool internal = false;
            QSize dimensions;
            std::vector<Modifier> modifiers;
            std::default_random_engine rng;
    };
}

#endif // __MODIFIERPATTERN_H__