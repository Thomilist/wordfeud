#include "ProposalInfo.hpp"

namespace wf
{
    ProposalInfo::ProposalInfo(
                QSize a_size,
                QWidget* a_parent)
        : QWidget(a_parent)
        , size(a_size)
    {
        setFixedWidth(size.width());
        setMouseTracking(true);

        grid_layout.addWidget(&proposed_play_label, 0, 0);
        grid_layout.addWidget(&proposed_play_value, 0, 1);

        proposed_play_label.setAlignment(Qt::AlignRight);

        grid_layout.setSpacing(0);
        setLayout(&grid_layout);
    }
    
    ProposalInfo::~ProposalInfo()
    { }
    
    void ProposalInfo::setProposedPlay(bool a_valid, int a_points)
    {
        valid = a_valid;
        points = a_points;
        repaint();

        return;
    }
    
    void ProposalInfo::paintEvent(QPaintEvent*)
    {
        if (valid)
        {
            proposed_play_value.setText(getPointsAsText());
        }
        else
        {
            proposed_play_value.setText("Invalid");
        }

        return;
    }
    
    QString ProposalInfo::getPointsAsText()
    {
        return QString::number(points) + " points";
    }
}