#pragma once

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QPainter>
#include <QRect>
#include <QStyle>
#include <QStyleOption>
#include <QTimer>
#include <QStateMachine>
#include <QState>
#include <QRegExp>

#include <functional>
#include <iostream>

namespace FreeFit
{
    namespace GUI
    {
        class MaterialTextField : public QLineEdit
        {
        Q_OBJECT
        public:
            MaterialTextField(QString t_t, QWidget* parent = nullptr);
            void highlightAsInvalid();
            void highlightAsValid();
            void setValidationFunction(std::function<bool(std::string)> f){validate_function = f;}
            bool validateText();
        protected:
            void paintEvent(QPaintEvent* ev) override;
            void focusInEvent(QFocusEvent* e) override;
            void focusOutEvent(QFocusEvent* e) override;
        private:
            void updateStyleSheet(){this->setStyleSheet(css_string);}
            void setDefaultPainterSettings(QPainter* painter);
            void setFocusedPainterSettings(QPainter* painter);
            void setNotFocusedPainterSettings(QPainter* painter);
            void drawStaticBaseLine(QPainter* painter);
            void drawAnimatedBaseLine(QPainter* painter);
            void drawStaticTextLabel(QPainter* painter);
            void drawAnimatedTextLabel(QPainter* painter);
            void focusGainedTextEnteredPaint(QPainter* painter);
            void focusGainedNoTextEnteredPaint(QPainter* painter);
            void focusLostTextEnteredPaint(QPainter* painter);
            void focusLostNoTextEnteredPaint(QPainter* painter);

            int lineAnimationCounter;
            int textAnimationCounter;
            QTimer* t_update;
            QString t;
            QString css_string;

            std::function<void(QPainter*)> currentPaintFunction;
            std::function<bool(std::string)> validate_function;
        signals:
            void focusGainedTextEntered();
            void focusGainedNoTextEntered();
            void focusLostTextEntered();
            void focusLostNoTextEntered();
        private slots:
            void incrementLineAnimationCounter();
            void incrementTextAnimationCounter();
        };
    }
}