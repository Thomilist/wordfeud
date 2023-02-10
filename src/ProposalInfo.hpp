#ifndef __PROPOSALINFO_H__
#define __PROPOSALINFO_H__

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QLabel>
#include <QPaintEvent>
#include <QString>

#include "ForwardDeclarations.hpp"
#include "Settings.hpp"

namespace wf
{
    class ProposalInfo : public QWidget
    {
        public:
            ProposalInfo(
                Settings* a_settings,
                QWidget* a_parent = nullptr);
            ~ProposalInfo();

            void setProposedPlay(bool a_valid, int a_points);
        
        private:
            void paintEvent(QPaintEvent* a_event);
            QString getPointsAsText();

            Settings* settings;
            QSize size;
            QGridLayout grid_layout;
            QLabel proposed_play_label{"Proposed play: "};
            QLabel proposed_play_value{"0 points"};
            bool valid = true;
            int points = 0;
    };
}

#endif // __PROPOSALINFO_H__