#pragma once

#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QRadialGradient>

#include <functional>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialButton : public QPushButton
        {
            Q_OBJECT
            public:
                MaterialButton(QString t, QWidget* parent = nullptr);
                void setColor(std::string c);
            protected:
                void paintEvent(QPaintEvent* ev) override;
                void mousePressEvent(QMouseEvent* ev) override;
            private slots:
                void updateRippleData();
                void handleRippleFinished();
            private:
                void updateCSS();
                void animateRipple(QPainter* painter);

                QTimer* t_update;
                bool rippleIntensityIncreasing;
                const int rippleSteps = 10;
                int rippleOpacityCounter;
                int rippleRadiusCounter;
                QPointF click_pos;
                std::function<void(QPainter*)> currentPaintFunction;
                std::string color;
            signals:
                void rippleFinished();
        };
    }
}