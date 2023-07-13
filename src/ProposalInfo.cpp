#include "ProposalInfo.hpp"

namespace wf
{
    ProposalInfo::ProposalInfo(Settings* a_settings, QWidget* a_parent)
        : QWidget(a_parent)
        , settings(a_settings)
    {
        size = QSize{
            settings->getHandDimensions().width() * settings->getHandTileSize().width(),
            settings->getHandDimensions().height() * settings->getHandTileSize().height()
        };
        
        setFixedWidth(size.width());
        setMouseTracking(true);

        proposed_play_label.setMouseTracking(true);
        proposed_play_value.setMouseTracking(true);

        grid_layout.addWidget(&proposed_play_label, 0, 0);
        grid_layout.addWidget(&proposed_play_value, 1, 0);

        proposed_play_label.setAlignment(Qt::AlignCenter);
        proposed_play_value.setAlignment(Qt::AlignCenter);

        QFont value_font = settings->getMonospaceFont();
        value_font.setPointSize(size.height() / 7);
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
        if (points == 1)
        {
            return QString::number(points) + " point";
        }
        else
        {
            return QString::number(points) + " points";
        }
    }
}