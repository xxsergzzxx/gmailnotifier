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
#include "defaults.h"
#include "gmailnotifierapplet.h"
// Plasma
#include <Plasma/Service>
// KDE
#include <KDE/KConfigDialog>
#include <KDE/KNotification>
#include <KDE/KNotifyConfigWidget>
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
    , m_appletConfigured(false)
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

    // Store the current form factor
    m_formFactor = Plasma::Applet::formFactor();

    // Connect to the dataengine
    m_engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!m_engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    // Init the applet
    initApplet();

    // Paint Icon
    paintIcon();
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

    kDebug() << qPrintable(QString("[%1] Receiving data update for source \"%2\" (cnt: %3)").arg(QTime::currentTime().toString()).arg(source).arg(data.count()));

    if (data.count() != 0) {
        // Create list of new emails
        QVariantList newEntries;
        QVariant entry;
        foreach (entry, data.value("entries").toList()) {
            if ( !m_entries[source].contains(entry) ) { newEntries.append(entry); }
        }
        
        // If there are new entries we send a notification
        if ( !newEntries.isEmpty() ) {    
            QString message = "";
            
            // Check number of sources
            if (m_unreadMailCount.size() == 1) {
                message = "<table>";
            } else {
                // FIXME: This should likely use the "Display" value for the source, if it exists.
                message = i18n("<table><tr><td><b>Source</b>: </td><td>%1</td></tr>").arg(source);
            }
            
            // Check number of new entries
            if ( newEntries.size() == 1 ) {
                message += i18n("<tr><td><b>From</b>: </td><td>%1</td></tr><tr><td><b>Subject</b>: </td><td>%2</td></tr></table>").arg(newEntries.at(0).toMap()["author"].toMap()["name"].toString()).arg(newEntries.at(0).toMap()["title"].toString());
            } else {
                message += i18n("<tr><td colspan=2>You have <b>%1</b> new messages.</td></tr></table>").arg(newEntries.size());
            }
                        
            // Fire notification
            KNotification::event("new-mail-arrived",
                message,
                QPixmap(),
                0,
                KNotification::CloseOnTimeout,
                KComponentData("plasma-applet-gmailnotifier", "plasma-applet-gmailnotifier", KComponentData::SkipMainComponentRegistration)
                );
        }
        
        // Update dialog and icon
        m_unreadMailCount[source] = data.value("fullcount").toUInt();
        m_entries[source] = data.value("entries").toList();
        m_dialog->updateMailCount(source, data);
        paintIcon();
    }
} // dataUpdated()


/*
** protected:
*/
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::FormFactorConstraint) {
        // Ignore if previous form factor was the same
        if (Plasma::Applet::formFactor() == m_formFactor) {
            return;
        }

        // Form factor just changed from !Vertical or !Horizontal to
        // Vertical or Horizontal
        if ((m_formFactor != Plasma::Horizontal &&
             m_formFactor != Plasma::Vertical) &&
            (Plasma::Applet::formFactor() == Plasma::Horizontal ||
             Plasma::Applet::formFactor() == Plasma::Vertical)) {

            // Prevent the applet from displaying the "Configure..." button
            // This forces the displaying of the wrench icon
            if (!m_appletConfigured) {
                setConfigurationRequired(false);
                setConfigurationRequired(true);
            } else {        
                // Paint the icon    
                paintIcon();
            }
        }

        // Store the new form factor
        m_formFactor = Plasma::Applet::formFactor();
    }
} // constraintsEvent()

void GmailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();

    m_configDialog = new GmailNotifierAppletConfig(Plasma::Applet::config(), parent);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    
    // Add in KNotifyConfigWidget
    KNotifyConfigWidget *notifyWidget = new KNotifyConfigWidget(parent);
    notifyWidget->setApplication("plasma-applet-gmailnotifier", QString::null, QString::null);
    KPageWidgetItem *notifyPage = parent->addPage(notifyWidget, i18n("Notifications"), "Notifications");
    notifyPage->setIcon(KIcon("preferences-desktop-notification"));
    connect(parent, SIGNAL(okClicked()), notifyWidget, SLOT(save()));
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

    // Paint icon
    paintIcon();
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
        QString background(config().readEntry("Background", Defaults::background));
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
    m_appletConfigured = (accountList.size() != 0);
    Plasma::Applet::setConfigurationRequired(!m_appletConfigured);

    m_dialog->setAccounts(accountList, m_unreadMailCount);
    m_dialog->setDisplayLogo(config().readEntry("DisplayLogo", Defaults::displayLogo));

    m_dialog->setTextColor(config().readEntry("DialogTextColor", Defaults::dialogTextColor));

    // Request data
    QStringList validSources;
    QList<QMap<QString, QString> >::ConstIterator it;
    for (it = accountList.constBegin(); it != accountList.constEnd(); ++it) {
        QString request = QString("%1:%2").arg(it->value("Login")).arg(it->value("Label"));
        m_engine->connectSource(request,
                                this,
                                (1000*60*config().readEntry("PollingInterval", Defaults::pollingInterval)),
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
            m_entries.remove(source);
        } else {
            // Make sure that m_unreadMailCount is initialized for each source
            if ( !m_unreadMailCount.contains(source) ) { m_unreadMailCount[source] = 0; }
        }
    }
} // initApplet()

void GmailNotifierApplet::paintIcon()
{
    kDebug();

    // We don't need to update the icon if we don't live in a panel
    // or we're not configured yet
    if (Plasma::Applet::formFactor() == Plasma::Planar ||
        Plasma::Applet::formFactor() == Plasma::MediaCenter ||
        !m_appletConfigured) {
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
    int fontSize = 40;

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
    p.setPen(config().readEntry("IconTextColor", Defaults::iconTextColor));
    p.drawText(QRectF(0, icon.height()/2, icon.width(), icon.height()/2),
               Qt::AlignCenter, mailCount);
    p.end();

    // Set the icon
    Plasma::PopupApplet::setPopupIcon(icon);
} // drawIcon()


#include "gmailnotifierapplet.moc"
