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


/*
** public:
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
    , m_dialog(new GmailNotifierDialog(this))
    , m_configDialog(0)
    , m_icon(0)
{
    kDebug();

    // Applet defaults
    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setConfigurationRequired(true);
    Plasma::Applet::setHasConfigurationInterface(true);
    Plasma::PopupApplet::setPopupIcon("gmailnotifier");

    // Connect to the dataengine
    m_engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!m_engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();

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

    if (data.contains("fullcount")) {
        m_unreadMailCount[source] = data.value("fullcount").toUInt();
        m_dialog->updateMailCount(source, data);
    }
} // dataUpdated()


/*
** protected:
*/
/*
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug();
} // constraintsEvent()
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

    // Set applet background
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

    // Remove sources that aren't used anymore from the total counter
    foreach (QString source, m_engine->sources()) {
        if (!validSources.contains(source)) {
            m_engine->disconnectSource(source, this);
            m_unreadMailCount.remove(source);
        }
    }
} // initApplet()


#include "gmailnotifierapplet.moc"
