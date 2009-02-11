/*
** Copyright (C) 2008-2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
**
** $Id$
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
#include <Plasma/Service>
// KDE
#include <KDE/KConfigDialog>
#include <KDE/KStringHandler>
// QtGui
#include <QtGui/QPainter>


/*
** public:
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
    , m_engine(0)
    , m_dialog(new GmailNotifierDialog(this))
    , m_configDialog(0)
{
    kDebug();

    // Applet defaults
    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
    m_icon = KIcon(Plasma::Applet::icon()); // Applet icon from .desktop file
    Plasma::PopupApplet::setPopupIcon(m_icon);
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();

    // Connect to the dataengine
    m_engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!m_engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    // Init the applet
    initApplet();
} // init()

QWidget* GmailNotifierApplet::widget()
{
    return m_dialog->widget();
} // widget()


/*
** public Q_SLOTS:
*/
void GmailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();

    // FIXME: This is wrong....
    if (data.contains("fullcount")) {
        m_unreadMailCount[source] = data.value("fullcount").toUInt();
        m_dialog->updateMailCount(source, data);
        paintIcon();
    }
} // dataUpdated()


/*
** protected:
*/
void GmailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();

    m_configDialog = new GmailNotifierAppletConfig(Plasma::Applet::config(), parent);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);

    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
} // createConfigurationInterface()


/*
** private Q_SLOTS:
*/
void GmailNotifierApplet::configAccepted()
{
    kDebug();

    KConfigGroup cg(m_configDialog->config());
    Plasma::Applet::save(cg);

    // Reinit applet
    initApplet();
} // configAccepted()

/*
** private:
*/
void GmailNotifierApplet::initApplet()
{
    kDebug();

    // Set applet background (only if we are not in a panel)
    if (Plasma::Applet::formFactor() == Plasma::Planar ||
        Plasma::Applet::formFactor() == Plasma::MediaCenter) {
        QString background(config().readEntry("Background", "Standard"));
        Plasma::Applet::BackgroundHints hint;
        if (background == "Standard") {
            hint = StandardBackground;
        } else if (background == "Translucent") {
            hint = TranslucentBackground;
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

    // If the account list is empty, set the applet in unconfigured state
    bool configured = (accountList.size() == 0);
    Plasma::Applet::setConfigurationRequired(configured);

    m_dialog->setAccounts(accountList, m_unreadMailCount);
    m_dialog->setDisplayLogo(config().readEntry("DisplayLogo", true));

    // Request data
    QStringList validSources;
    QList<QMap<QString, QString> >::ConstIterator it;
    for (it = accountList.constBegin(); it != accountList.constEnd(); ++it) {
        QString request = QString("%1:%2").arg(it->value("Login")).arg(it->value("Label"));
        m_engine->connectSource(request,
                                this,
                                (1000*60*config().readEntry("PollingInterval", 5)),
                                Plasma::NoAlignment);
        Plasma::Service *service = m_engine->serviceForSource(request);
        KConfigGroup cg = service->operationDescription("auth");
        cg.writeEntry("password", KStringHandler::obscure(it->value("Password")));
        service->startOperationCall(cg);

        validSources << request;
    }

    // Remove sources that aren't used anymore
    foreach (QString source, m_engine->sources()) {
        if (!validSources.contains(source)) {
            m_engine->disconnectSource(source, this);
            m_unreadMailCount.remove(source);
        }
    }

    paintIcon();
} // initApplet()

void GmailNotifierApplet::paintIcon()
{
    kDebug();

    // We don't need to update the icon if we don't live in a panel
    if (Plasma::Applet::formFactor() == Plasma::Planar ||
        Plasma::Applet::formFactor() == Plasma::MediaCenter) {
        return;
    }

    // Compute total amount/number of unread e-mails
    uint totalUnreadMailCount=0;
    QMap<QString, uint>::ConstIterator it;
    for (it = m_unreadMailCount.constBegin(); it != m_unreadMailCount.constEnd(); ++it) {
        totalUnreadMailCount += it.value();
    }

    // Text to display
    QString mailCount(QString("%1").arg(totalUnreadMailCount));
    // Text font size
    int fontSize = 45;

    // Draw the total unread mail count over the icon
    int size = KIconLoader::SizeEnormous;   // 128
    QPixmap icon(size, size);
    icon = KIconLoader::global()->loadIcon(Plasma::Applet::icon(), KIconLoader::NoGroup, size);

    QPainter p(&icon);
    QFont font(p.font()); // TODO: Use font from KDE global settings
    font.setPointSize(fontSize);
    font.setBold(true);

    // Reduce the font size when needed...
    QFontMetrics fm(font);
    while (fm.width(mailCount) > icon.width()-10 && fontSize > 0) {
        --fontSize;
        kDebug() << "Reducing font size:" << fontSize;
        font.setPointSize(fontSize);
        fm = font;
    }

    p.setFont(font);
    p.setPen(QColor("#0057AE")); // TODO: User configurable color
    p.drawText(QRectF(0, icon.height()/2, icon.width(), icon.height()/2),
               Qt::AlignCenter, mailCount);
    p.end();

    // Set the icon
    Plasma::PopupApplet::setPopupIcon(icon);
} // drawIcon()


#include "gmailnotifierapplet.moc"
