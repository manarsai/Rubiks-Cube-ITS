#pragma once

#include <QPixmap>
#include <vector>
#include "src/core/domain/Stages.h"

class StagePreviews
{
public:
    static QPixmap createFaceImage(const QString& face);

    static std::vector<QPixmap> getStagePreviews(Stage stage);
};