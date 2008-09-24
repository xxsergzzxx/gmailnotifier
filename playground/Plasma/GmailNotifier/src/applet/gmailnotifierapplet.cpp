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

// Plasma
#include <Plasma/DataEngine>

// Qt
#include <QtGui/QPainter>


/*
** Public
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_icon(0), m_engine(0), m_dialog(0), m_proxy(0), m_layout(0)
{
    kDebug();

    // Connect to the dataengine
    m_engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!m_engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();

    // Main layout, used both in desktop and panel mode
    m_layout = new QGraphicsLinearLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    Plasma::Applet::setLayout(m_layout);
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
            if (m_proxy) {
                m_layout->removeItem(m_proxy);
                delete m_proxy;
                m_proxy=0;
            }
            m_dialog = new GmailNotifierDialog(GmailNotifierDialog::PanelArea, this);
            kDebug() << m_dialog;
            setBackgroundHints(NoBackground);
            drawIcon();
        }
        else {
            if (m_icon) {
                m_layout->removeItem(m_icon);
                delete m_icon;
                m_icon = 0;
            }
            m_dialog = new GmailNotifierDialog(GmailNotifierDialog::DesktopArea, this);
            m_proxy = new QGraphicsProxyWidget(this);
            m_proxy->setWidget(m_dialog->dialog());
            m_layout->addItem(m_proxy);

            resize(m_dialog->dialog()->size()/* + QSize(100, 100) */);
            Plasma::Applet::setMinimumSize(m_dialog->dialog()->minimumSizeHint() + QSize(30, 30));
        }
    }

    if (m_icon && constraints & Plasma::SizeConstraint) {
        drawIcon();
    }
} // constraintsEvent()


/*
** Protected Q_SLOTS
*/
void GmailNotifierApplet::onClickNotifier()
{
    kDebug();

    if (m_dialog->dialog()->isVisible()) {
        m_dialog->hide();
    } else {
        m_dialog->dialog()->move(popupPosition(m_dialog->dialog()->sizeHint()));
        m_dialog->show();
    }    
} // onClickNotifier()


/*
** Private
*/
void GmailNotifierApplet::drawIcon(const QString &text)
{
    kDebug() << text;

    // Remove any previously created icon
    if (m_icon) {
        m_layout->removeItem(m_icon);
        delete m_icon;
        m_icon = 0;
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
        p.setPen(QColor("#0057AE"));
        p.drawText(QRectF(0, img.height()/2, img.width(), img.height()/2), Qt::AlignCenter, text);
    }

    m_icon = new Plasma::Icon(img, QString(), this);
    connect(m_icon, SIGNAL(clicked()), this, SLOT(onClickNotifier()));
    m_icon->resize(geometry().size());

    Plasma::Applet::setAspectRatioMode(Plasma::ConstrainedSquare);
    m_layout->addItem(m_icon);
} // drawIcon()


#include "gmailnotifierapplet.moc"
