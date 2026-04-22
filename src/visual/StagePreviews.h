#pragma once

#include <QPixmap>
#include <QString>

class StagePreviews
{
public:
    static QPixmap createFaceImage(const QString& face);
};