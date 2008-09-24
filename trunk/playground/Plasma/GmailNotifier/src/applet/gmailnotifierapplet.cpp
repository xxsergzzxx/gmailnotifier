/*
** Copyright (C) 2008 Gilles CHAUVIN <gcnweb+kde@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2  of  the  License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be  useful, but
** WITHOUT  ANY  WARRANTY;  without  even  the   implied   warranty  of
** MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  GNU
** General Public License for more details.
**
** You should have received a copy of the GNU  General  Public  License
** along with  this  program;  if  not,  write  to  the  Free  Software
** Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

// Own
#include "gmailnotifierapplet.h"
#include "gmailnotifierdialog.h"

// Plasma
#include <Plasma/DataEngine>
#include <Plasma/Icon>

// Qt
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QPainter>


class GmailNotifierApplet::Private
{
public:
    Private()
        : icon(0)
        , engine(0)
        , dialog(0)
        , proxy(0)
        , layout(0)
    {
    }
    ~Private()
    {
    }

    Plasma::Icon *icon;
    Plasma::DataEngine *engine;
    GmailNotifierDialog *dialog;
    QGraphicsProxyWidget *proxy;
    QGraphicsLinearLayout *layout;
}; // Private


/*
** Public
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , d(new Private)
{
    kDebug();

    // Connect to the dataengine
    d->engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!d->engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
    delete d;
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();

    // Main layout, used both in desktop and panel mode
    d->layout = new QGraphicsLinearLayout(this);
    d->layout->setContentsMargins(0, 0, 0, 0);
    d->layout->setSpacing(0);
    Plasma::Applet::setLayout(d->layout);
} // init()


/*
** Protected
*/
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug();

    bool isSizeConstrained = (formFactor() == Plasma::Horizontal ||
                              formFactor() == Plasma::Vertical);
    if (constraints & Plasma::FormFactorConstraint) {
        if (isSizeConstrained) {
            if (!d->dialog) {
                d->dialog = new GmailNotifierDialog(GmailNotifierDialog::PanelArea, this);
            }
            kDebug() << d->dialog;
            setBackgroundHints(NoBackground);
            if (d->proxy) {
                d->layout->removeItem(d->proxy);
                delete d->proxy;
                d->proxy=0;
            }
            drawIcon("123");
        }
        else {
            if (d->icon) {
                d->layout->removeItem(d->icon);
                delete d->icon;
                d->icon = 0;
            }

            if (!d->dialog) {
                d->dialog = new GmailNotifierDialog(GmailNotifierDialog::DesktopArea, this);
            }
            d->proxy = new QGraphicsProxyWidget(this);
            d->proxy->setWidget(d->dialog->dialog());
            d->layout->addItem(d->proxy);

            resize(d->dialog->dialog()->size()/* + QSize(100, 100) */);
            Plasma::Applet::setMinimumSize(d->dialog->dialog()->minimumSizeHint() + QSize(30, 30));
        }
    }

    if (d->icon && constraints & Plasma::SizeConstraint) {
        drawIcon("123");
    }
} // constraintsEvent()


/*
** Protected Q_SLOTS
*/
void GmailNotifierApplet::onClickNotifier()
{
    kDebug();

    if (d->dialog->dialog()->isVisible()) {
        d->dialog->hide();
    } else {
        d->dialog->dialog()->move(popupPosition(d->dialog->dialog()->sizeHint()));
        d->dialog->show();
    }    
} // onClickNotifier()


/*
** Private
*/
void GmailNotifierApplet::drawIcon(const QString &text)
{
    kDebug() << text;

    // Remove any previously created icon
    if (d->icon) {
        d->layout->removeItem(d->icon);
        delete d->icon;
        d->icon = 0;
    }

    QPixmap srcImg(":/images/gmailnotifier_icon.png");
    QPixmap img = srcImg.scaled((int)geometry().width(),
                                (int)geometry().height(),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);

    if (!text.isEmpty()) {
        QPainter p(&img);
        QFont font(p.font());
        font.setBold(true);
        p.setFont(font);
        p.drawText(QRectF(0, img.height()/2, img.width(), img.height()/2), Qt::AlignCenter, text);
    }

    d->icon = new Plasma::Icon(img, QString(), this);
    connect(d->icon, SIGNAL(clicked()), this, SLOT(onClickNotifier()));
    d->icon->resize(geometry().size());

    Plasma::Applet::setAspectRatioMode(Plasma::ConstrainedSquare);
    d->layout->addItem(d->icon);
} // drawIcon()


#include "gmailnotifierapplet.moc"
