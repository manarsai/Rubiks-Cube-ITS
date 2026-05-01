#include "StagePreviews.h"
#include <QPainter>
#include <vector>

QPixmap StagePreviews::createFaceImage(const QString& face)
{
    int size = 120;
    int cell = size / 3;

    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter p(&pixmap);

    auto getColor = [](QChar c) -> QColor {
        switch (c.toLatin1()) {
        case 'W': return Qt::white;
        case 'Y': return Qt::yellow;
        case 'R': return Qt::red;
        case 'O': return QColor(255, 165, 0);
        case 'B': return Qt::blue;
        case 'G': return Qt::green;
        default:  return Qt::gray;
        }
        };

    for (int i = 0; i < 9; i++)
    {
        int row = i / 3;
        int col = i % 3;

        QRect rect(col * cell, row * cell, cell, cell);

        p.fillRect(rect, getColor(face[i]));
        p.setPen(Qt::black);
        p.drawRect(rect);
    }

    return pixmap;
}
std::vector<QPixmap> StagePreviews::getStagePreviews(Stage stage)
{
    switch (stage)
    {
    case Stage::WHITE_CROSS:
        return {
            createFaceImage(" W "
                            "WWW"
                            " W "),
            createFaceImage("   "
                            "   "
                            "   "),
            createFaceImage("   "
                            "   "
                            "   ")
        };


    case Stage::F2L:
        return {
            createFaceImage("WWW"
                            "WWW"
                            "WWW"),
            createFaceImage("   "
                            "   "
                            "   "),
            createFaceImage("OOO"
                            "O O"
                            "OOO")
        };
    case Stage::OLL:
        return {
            createFaceImage("RRR"
                            "R R"
                            "RRR"),
            createFaceImage("BBB"
                            "B B"
                            "BBB"),
            createFaceImage("OOO"
                            "O O"
                            "OOO")
        };

    case Stage::PLL:
        return {
            createFaceImage("RRR"
                            "R R"
                            "RRR"),
            createFaceImage("BBB"
                            "B B"
                            "BBB"),
            createFaceImage("OOO"
                            "O O"
                            "OOO")
        };

    default:
        return {
            createFaceImage("RRR"
                            "R R"
                            "RRR"),
            createFaceImage("BBB"
                            "B B"
                            "BBB"),
            createFaceImage("OOO"
                            "O O"
                            "OOO")
        };
    }
}
