#include "include/materialtextfield.h"

namespace FreeFit
{
    namespace GUI
    {
        MaterialTextField::MaterialTextField(QString t_t, QWidget* parent)
            : t_update(new QTimer),QLineEdit(t_t,parent),t(t_t),lineAnimationCounter(0),textAnimationCounter(0)
        {
            css_string = QString("background-color:gainsboro; color:black; border: 0px; padding: 0px; padding-bottom: 2px; padding-top: 20px;");
            updateStyleSheet();
            this->setAttribute(Qt::WA_MacShowFocusRect, 0);
            this->setFrame(false);

            connect(this,&MaterialTextField::focusGainedTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedTextEnteredPaint,this,std::placeholders::_1);});
            connect(this,&MaterialTextField::focusGainedNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusGainedNoTextEnteredPaint,this,std::placeholders::_1);});
            connect(this,&MaterialTextField::focusLostTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostTextEnteredPaint,this,std::placeholders::_1);});
            connect(this,&MaterialTextField::focusLostNoTextEntered,[=](){currentPaintFunction = std::bind(&MaterialTextField::focusLostNoTextEnteredPaint,this,std::placeholders::_1);});
            connect(this,&QLineEdit::textEdited,this,&MaterialTextField::validateText);
        }

        void MaterialTextField::highlightAsInvalid()
        {
            css_string.replace(QRegExp("background-color:gainsboro;"),"background-color:indianred;");
            updateStyleSheet();
        }

        void MaterialTextField::highlightAsValid()
        {
            css_string.replace(QRegExp("background-color:indianred;"),"background-color:gainsboro;");
            updateStyleSheet();
        }

        bool MaterialTextField::validateText()
        {
            if(!validate_function(text().toStdString()))
            {
                highlightAsInvalid();
                return false;
            }
            else
            {
                highlightAsValid();
                return true;
            }
        }

        void MaterialTextField::setText(const QString& t)
        {
            QLineEdit::setText(t);
            emit focusLostTextEntered();
            update();
        }

        void MaterialTextField::paintEvent(QPaintEvent* ev)
        {
            QLineEdit::paintEvent(ev);

            QPainter painter(this);

            if (currentPaintFunction)
                currentPaintFunction(&painter);

        }

        void MaterialTextField::focusInEvent(QFocusEvent* e)
        {
            connect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
            connect(t_update,SIGNAL(timeout()),this,SLOT(incrementTextAnimationCounter()));
            connect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
            t_update->start(20);

            if (text() != "" && text() != t)
                emit focusGainedTextEntered();
            else
            {
                setText("");
                emit focusGainedNoTextEntered();
            }

            QLineEdit::focusInEvent(e);
        }

        void MaterialTextField::focusOutEvent(QFocusEvent* e)
        {
            t_update->stop();
            disconnect(t_update,SIGNAL(timeout()),this,SLOT(incrementLineAnimationCounter()));
            disconnect(t_update,SIGNAL(timeout()),this,SLOT(incrementTextAnimationCounter()));
            disconnect(t_update,SIGNAL(timeout()),this,SLOT(repaint()));
            lineAnimationCounter = 0;
            textAnimationCounter = 0;

            if (text() != "" && text() != t)
                emit focusLostTextEntered();
            else
            {
                setText(t);
                emit focusLostNoTextEntered();
            }

            QLineEdit::focusOutEvent(e);
        }

        void MaterialTextField::setDefaultPainterSettings(QPainter* painter)
        {
            QPen pen = painter->pen();
            pen.setWidth(2);
            QFont font = painter->font();
            font.setPixelSize(10);

            painter->setPen(pen);
            painter->setFont(font);
        }
        
        void MaterialTextField::setFocusedPainterSettings(QPainter* painter)
        {
            setDefaultPainterSettings(painter);
            QPen pen = painter->pen();
            pen.setColor(Qt::black);
            painter->setPen(pen);
        }

        void MaterialTextField::setNotFocusedPainterSettings(QPainter* painter)
        {
            setDefaultPainterSettings(painter);
            QPen pen = painter->pen();
            pen.setColor(Qt::gray);
            painter->setPen(pen);
        }

        void MaterialTextField::drawStaticBaseLine(QPainter* painter)
        {
            painter->drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
        }

        void MaterialTextField::drawAnimatedBaseLine(QPainter* painter)
        {
            if (this->rect().width()/2+(this->rect().width()/25)*lineAnimationCounter <= this->rect().width())
            {
                painter->drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2+(this->rect().width()/25)*lineAnimationCounter,this->rect().height()-1);
                painter->drawLine(this->rect().width()/2,this->rect().height()-1,this->rect().width()/2-(this->rect().width()/25)*lineAnimationCounter,this->rect().height()-1);
            }
            else
            {
                painter->drawLine(0,this->rect().height(),this->rect().width(),this->rect().height());
            }
        }

        void MaterialTextField::drawStaticTextLabel(QPainter* painter)
        {
            painter->drawText(0,painter->font().pixelSize(),t);
        }

        void MaterialTextField::drawAnimatedTextLabel(QPainter* painter)
        {
            if (this->rect().height() - 2*textAnimationCounter >= painter->font().pixelSize())
                painter->drawText(0,this->rect().height()-2*textAnimationCounter,this->t);
            else
                drawStaticTextLabel(painter);
        }

        void MaterialTextField::focusGainedTextEnteredPaint(QPainter* painter)
        {
            setFocusedPainterSettings(painter);
            drawAnimatedBaseLine(painter);
            drawStaticTextLabel(painter);
        }

        void MaterialTextField::focusGainedNoTextEnteredPaint(QPainter* painter)
        {
            setFocusedPainterSettings(painter);
            drawAnimatedBaseLine(painter);
            drawAnimatedTextLabel(painter);
        }

        void MaterialTextField::focusLostTextEnteredPaint(QPainter* painter)
        {
            setNotFocusedPainterSettings(painter);
            drawStaticBaseLine(painter);
            drawStaticTextLabel(painter);
        }

        void MaterialTextField::focusLostNoTextEnteredPaint(QPainter* painter)
        {
            setNotFocusedPainterSettings(painter);
            drawStaticBaseLine(painter);
        }

        void MaterialTextField::incrementLineAnimationCounter()
        {
            lineAnimationCounter += 1;
        }

        void MaterialTextField::incrementTextAnimationCounter()
        {
            textAnimationCounter += 1;
        }
    }
}