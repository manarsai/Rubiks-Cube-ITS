#pragma once
#include <QWidget>

class CubeNet : public QWidget
{
    Q_OBJECT

public:
    CubeNet(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};