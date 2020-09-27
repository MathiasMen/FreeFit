#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QGridLayout>
#include <QPainter>
#include <QPen>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialSliderHandle : public QLabel
        {
        Q_OBJECT
        public:
            MaterialSliderHandle(int width, int height, QWidget* parent = nullptr);
            void setMinX(int x){minVal = x;}
            void setMaxX(int x){maxVal = x;}
            void setColor(std::string c);
        signals:
            void moved(int);
        protected:
            void mousePressEvent(QMouseEvent* e);
            void mouseMoveEvent(QMouseEvent* e);
            void mouseReleaseEvent(QMouseEvent* e);
        private:
            void updateCSS();
            
            int minVal;
            int maxVal;
            QPoint offset;
            bool pressed = false;
            std::string color;
        };

        class MaterialSlider : public QWidget
        {
        Q_OBJECT
        public:
            MaterialSlider(QWidget* parent = nullptr,int h_w = 10,int h_h = 20);
            void setMinValue(int v){min_mapped_value = v;}
            void setMaxValue(int v){max_mapped_value = v;}
            void setColor(std::string c);
        signals:
            void valuesChanged(int,int);
        public slots:
            void leftHandleMoved(int new_x);
            void rightHandleMoved(int new_x);
        protected:
            void paintEvent(QPaintEvent* e) override;
        private:
            void updateMappedValues();
            int lineStartPosX();
            int lineEndPosX();

            int handle_width;
            int handle_height;
            const int distance_line_to_border = 5;
            int minPosValue;
            int maxPosValue;

            double min_mapped_value;
            double max_mapped_value;
            double lower_mapped_value;
            double upper_mapped_value;
            MaterialSliderHandle* left_handle;
            MaterialSliderHandle* right_handle;
        };
    }
}