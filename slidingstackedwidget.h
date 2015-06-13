#ifndef SLIDINGSTACKEDWIDGET_H
#define SLIDINGSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QWidget>
#include <QDebug>
#include <QEasingCurve>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QMap>


enum mainViewsIdx
{
      MENU_WIDGET = 0
    , DIAGNOSTIC_WIDGET = 1
    , DATABASE_WIDGET = 2
    , CONFIG_WIDGET = 3
    , DATA_WIDGET = 4
    , SHOWRESULT_WIDGET = 5
};


class SlidingStackedWidget : public QStackedWidget
{
        Q_OBJECT

public:
        //! This enumeration is used to define the animation direction
        enum t_direction {
                LEFT2RIGHT,
                RIGHT2LEFT,
                TOP2BOTTOM,
                BOTTOM2TOP,
                AUTOMATIC
        };

        //! The Constructor and Destructor
        SlidingStackedWidget(QWidget *parent);
        ~SlidingStackedWidget(void);


public slots:
        //! Some basic settings API
        void setSpeed(int speed);   //animation duration in milliseconds
        void setAnimation(enum QEasingCurve::Type animationtype); //check out the QEasingCurve documentation for different styles
        void setVerticalMode(bool vertical=true);
        void setWrap(bool wrap);    //wrapping is related to slideInNext/Prev;it defines the behaviour when reaching last/first page

        //! The Animation / Page Change API
        void slideInNext();
        void slideInPrev();
        void slideInIdx(int idx, enum t_direction direction=AUTOMATIC);

        //extend base class func
        int addWidget(int mapViewIdx, QWidget* widget);
        void removeWidget(QWidget* widget);

signals:
        //! this is used for internal purposes in the class engine
        void animationFinished(void);

protected slots:
        //! this is used for internal purposes in the class engine
        void animationDoneSlot(void);

protected:
        //! this is used for internal purposes in the class engine
        void slideInWgt(QWidget * widget, enum t_direction direction=AUTOMATIC);

        QWidget *m_mainwindow;

        int m_speed;
        enum QEasingCurve::Type m_animationtype;
        bool m_vertical;
        int m_now;
        int m_next;
        bool m_wrap;
        QPoint m_pnow;
        bool m_active;

        QList<QWidget*> blockedPageList;

public:
        QMap<int, int> mappedViews;
};
#endif // SLIDINGSTACKEDWIDGET_H
