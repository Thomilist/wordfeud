#ifndef __MODIFIERPATTERN_H__
#define __MODIFIERPATTERN_H__

#include <algorithm>
#include <random>
#include <utility>
#include <vector>

#include <QString>

#include "ForwardDeclarations.hpp"

#include "Modifier.hpp"

#include "ModifierDistribution.hpp"

namespace wf
{
    class ModifierPattern
    {
        public:
            ModifierPattern();
            ~ModifierPattern();

            void setDistribution(ModifierDistribution a_distribution);
            void setDistribution(QString a_distribution);
            const std::vector<Modifier*> get();
            ModifierDistribution getDistribution() const;
            const QString getDistributionAsText() const;
            const std::vector<std::pair<ModifierDistribution, QString>> getAllDistributions() const;
            void reset();

        private:
            ModifierDistribution current_distribution = ModifierDistribution::WordfeudDefault;
            std::vector<std::pair<ModifierDistribution, QString>> all_distributions =
            {
                {ModifierDistribution::WordfeudDefault, "Wordfeud (default)"},
                {ModifierDistribution::WordfeudRandom, "Wordfeud (random)"},
                {ModifierDistribution::ScrabbleDefault, "Scrabble (default)"},
                {ModifierDistribution::ScrabbleRandom, "Scrabble (random)"}
            };
            std::vector<Modifier> wordfeud_default_pattern =
            {
                ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter,
                ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None,
                ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
                ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
                ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::Start, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter,
                ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
                ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
                ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, 
                ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None,
                ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter
            };
            std::vector<Modifier> scrabble_default_pattern =
            {
                ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
                ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
                ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter,
                ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
                ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::Start, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
                ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None,
                ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::None,
                ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter,
                ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None,
                ModifierType::None, ModifierType::DoubleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleWord, ModifierType::None,
                ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::TripleWord, ModifierType::None, ModifierType::None, ModifierType::None, ModifierType::DoubleLetter, ModifierType::None, ModifierType::None, ModifierType::TripleWord,
            };
            std::default_random_engine rng;
    };
}

#endif // __MODIFIERPATTERN_H__