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

// KDE
#include <KDE/KConfigDialog>

// Qt
#include <QtGui/QPainter>
#include <QtCore/QTime>


/*
** Public
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_engine(0), m_dialog(0), m_configDialog(0)
    , m_icon(0), m_proxy(0), m_layout(0)
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
    m_layout = new QGraphicsLinearLayout();
//    m_layout->setObjectName("Main QGraphicsLinearLayout");
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    Plasma::Applet::setLayout(m_layout);
} // init()


/*
** Public Q_SLOTS
*/
void GmailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();

    kDebug() << QTime::currentTime();
    kDebug() << source << data;

    /*
    if (data.contains("Error")) {
        kDebug() << "ERROR";
        return;
    }
    */

    if (m_validSources.contains(source)) {
        m_totalUnreadMailCount[source] = data["fullcount"].toUInt();
        m_dialog->updateMailCount(source, data);
    } else {
        kDebug() << source << "isn't a valid source!";
    }

    drawIcon();

} // dataUpdated()


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
                m_proxy = 0;
            }

            m_dialog = new GmailNotifierDialog(GmailNotifierDialog::PanelArea, this);
            setBackgroundHints(NoBackground);

            m_icon = new Plasma::Icon(this);
            connect(m_icon, SIGNAL(clicked()), this, SLOT(onClickNotifier()));
            m_layout->addItem(m_icon);

            Plasma::Applet::setAspectRatioMode(Plasma::ConstrainedSquare);

            drawIcon();
        } else {
            if (m_icon) {
                m_layout->removeItem(m_icon);
                delete m_icon;
                m_icon = 0;
            }
            m_dialog = new GmailNotifierDialog(GmailNotifierDialog::DesktopArea, this);
            m_proxy = new QGraphicsProxyWidget(this);
            m_proxy->setWidget(m_dialog->widget());
            m_layout->addItem(m_proxy);

            readjustSize();
        }

        initApplet();
    }

    if (m_icon && (constraints & Plasma::SizeConstraint)) {
        drawIcon();
    }

} // constraintsEvent()

void GmailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();
    m_configDialog = new GmailNotifierAppletConfig(Plasma::Applet::config(), parent);
    parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
} // createConfigurationInterface()


/*
** Private Q_SLOTS
*/
void GmailNotifierApplet::onClickNotifier()
{
    kDebug();

    if (m_dialog->widget()->isVisible()) {
        m_dialog->hide();
    } else {
        m_dialog->widget()->move(popupPosition(m_dialog->widget()->sizeHint()));
        m_dialog->show();
    }    
} // onClickNotifier()

void GmailNotifierApplet::configAccepted()
{
    kDebug();

    KConfigGroup cg(m_configDialog->config());
    Plasma::Applet::save(cg);

    // Reinit applet
    initApplet();

    // Redraw icon
    drawIcon();
} // configAccepted()


/*
** Private
*/
void GmailNotifierApplet::initApplet()
{
    kDebug();

    bool isSizeConstrained = (formFactor() == Plasma::Horizontal ||
                              formFactor() == Plasma::Vertical);

    // Set applet background
    // Do not change background when in "panel" Form Factor
    if (!isSizeConstrained) {
        QString background(config().readEntry("Background", "Standard"));
        Plasma::Applet::BackgroundHints hint;
        if (background == "Standard") {
            hint = StandardBackground;
        } else if (background == "Translucent") {
            hint = TranslucentBackground;
        } else if (background == "Shadowed") {
            hint = ShadowedBackground;
        } else if (background == "None") {
            hint = NoBackground;
        } else { // Default
            hint = DefaultBackground;
        }
        Plasma::Applet::setBackgroundHints(hint);
    }

    // Read account informations
    QList<QMap<QString, QString> > accountList;
    bool loop = true;
    int cnt = 0;
    while (loop == true) {
        QString prefix = QString("Account%1_").arg(cnt);
        QString login = config().readEntry(prefix+"Login", QString());
        // No (more?) accounts
        if (login.isEmpty()) {
            loop = false;
            break;
        }
        QMap<QString, QString> account;
        account["Login"] = login;
        account["Password"] = config().readEntry(prefix+"Password", QString());
        account["Label"] = config().readEntry(prefix+"Label", QString());
        account["Display"] = config().readEntry(prefix+"Display", QString());
        accountList.append(account);

        ++cnt;
    }

    m_dialog->setAccounts(accountList, m_totalUnreadMailCount);
    m_dialog->setDisplayLogo(config().readEntry("DisplayLogo", true));

    // Send passwords to the data engine
    // FIXME: Possible security hole here !!!
    QVariantMap passwordList;
    QList<QMap<QString, QString> >::ConstIterator it;
    for (it = accountList.constBegin(); it != accountList.constEnd(); ++it) {
        passwordList.insert(it->value("Login"), it->value("Password"));
    }
    m_engine->setProperty("passwords", passwordList);


    // Request data
    m_validSources.clear();
    for (it = accountList.constBegin(); it != accountList.constEnd(); ++it) {
        QString label = (it->value("Label").isEmpty()) ? "inbox" : it->value("Label");
        QString request = QString("%1:%2").arg(it->value("Login")).arg(label);
        m_engine->connectSource(request,
                                this,
                                (1000*60*config().readEntry("PollingInterval", 5)),
                                Plasma::NoAlignment);
        m_validSources << request;
    }

    // Remove sources that aren't used anymore from the total counter
    foreach (QString source, m_engine->sources()) {
        if (!m_validSources.contains(source)) {
            //kDebug() << "Disconnecting unused source" << source;
            //m_engine->disconnectSource(source, this);
            m_totalUnreadMailCount.remove(source);
        }
    }

    if (!isSizeConstrained) {
        readjustSize();
    }
} // initApplet()

void GmailNotifierApplet::drawIcon()
{
    kDebug();

    if (!m_icon) {
        return;
    }

    QPixmap srcImg(":/images/gmailnotifier_icon.png");
    QPixmap img = srcImg.scaled((int)geometry().width(),
                                (int)geometry().height(),
                                Qt::KeepAspectRatio,
                                Qt::SmoothTransformation);

    // Compute total number of unread mails
    uint totalUnreadMailCount=0;
    QMap<QString, uint>::ConstIterator it;
    for (it = m_totalUnreadMailCount.constBegin(); it != m_totalUnreadMailCount.constEnd(); ++it) {
        totalUnreadMailCount += it.value();
    }

    QPainter p(&img);
    QFont font(p.font());
    font.setBold(true);
    p.setFont(font);
    p.setPen(QColor("#0057AE"));
    p.drawText(QRectF(0, img.height()/2, img.width(), img.height()/2), Qt::AlignCenter, QString("%1").arg(totalUnreadMailCount));

    m_icon->setIcon(img);
    m_icon->resize(geometry().size());
    m_icon->update(); // Force icon to be refreshed
} // drawIcon()

void GmailNotifierApplet::readjustSize()
{
//    m_dialog->widget()->adjustSize();
//    Plasma::Applet::resize(m_dialog->widget()->minimumSizeHint()/* + QSize(100, 100) */);
    //Plasma::Applet::setMinimumSize(m_dialog->widget()->minimumSizeHint() + QSize(20, 20));
    Plasma::Applet::setMinimumSize(m_dialog->widget()->minimumSizeHint());
} // readjustSize()


#include "gmailnotifierapplet.moc"
