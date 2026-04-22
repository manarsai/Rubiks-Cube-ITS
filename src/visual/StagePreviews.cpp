#include "StagePreviews.h"
#include <QPainter>

QPixmap StagePreviews::createFaceImage(const QString& face)
{
    int size = 120;
    int cell = size / 3;

    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter p(&pixmap);

    auto getColor = [](QChar c) -> QColor {
        switch (c.toLatin1()) {
        case 'W': return QColor(Qt::white);
        case 'Y': return QColor(Qt::yellow);
        case 'R': return QColor(Qt::red);
        case 'O': return QColor(255, 165, 0);
        case 'B': return QColor(Qt::blue);
        case 'G': return QColor(Qt::green);
        default:  return QColor(Qt::black);
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