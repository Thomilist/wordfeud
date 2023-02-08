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
        grid_layout.addWidget(&proposed_play_value, 1, 0);

        proposed_play_label.setAlignment(Qt::AlignCenter);
        proposed_play_value.setAlignment(Qt::AlignCenter);

        QFont value_font{"Monospace", size.height() / 7};
        value_font.setBold(true);
        proposed_play_value.setFont(value_font);

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