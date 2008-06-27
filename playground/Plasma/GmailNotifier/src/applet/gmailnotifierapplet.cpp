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

// KDE
#include <KDE/KStringHandler>


/*
** Public
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_engine(0), m_configDialog(0)
    , m_layoutMain(0), m_layoutContents(0)
{
    kDebug();
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
    readConfig();
    setBackground();
    setLayout();
    setPasswords();

    // Request data...
    foreach (QVariant account, m_cfgAccounts) {
        QVariantMap data(account.toMap());
        QString label = (data["Label"].toString().isEmpty()) ? "inbox" : data["Label"].toString();
        QString request = QString("%1:%2").arg(data["Login"].toString()).arg(label);
        m_engine->connectSource(request, this, (m_cfgPollingInterval*1000*60), Plasma::NoAlignment);
    }
} // init()


/*
** Public slots
*/
void GmailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();

    if (data.contains("Error")) {
        m_count[source]->setText("Err.");
        return;
    }

    m_count[source]->setText(data["fullcount"].toString());
    qDebug() << source << data["fullcount"].toString();
} // dataUpdated()


/*
** Protected
*/
void GmailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();
    m_configDialog = new GmailNotifierAppletConfig(parent);
    //parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    //connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    QVariantMap data;
    data["Background"]      = m_cfgBackground;
    data["DisplayLogo"]     = m_cfgDisplayLogo;
    data["PollingInterval"] = m_cfgPollingInterval;
    data["Accounts"]        = m_cfgAccounts;
    m_configDialog->importConfig(data);
} // createConfigurationInterface()


/*
** Private slots
*/
void GmailNotifierApplet::configAccepted()
{
    kDebug();
    QVariantMap data = m_configDialog->exportConfig();
    config().writeEntry("Background", data["Background"]);
    config().writeEntry("DisplayLogo", data["DisplayLogo"]);
    config().writeEntry("PollingInterval", data["PollingInterval"]);

    config().writeEntry("Accounts", data["Accounts"].toList().count());
    QVariantList accounts(data["Accounts"].toList());
    int i=1;
    foreach(QVariant data, accounts) {
        QVariantMap account(data.toMap());
        QString prefix = QString("Account%1_").arg(i);
        config().writeEntry(prefix+"Login", account["Login"]);
        config().writeEntry(prefix+"Password", KStringHandler::obscure(account["Password"].toString()));
        config().writeEntry(prefix+"Label", account["Label"]);
        config().writeEntry(prefix+"Display", account["Display"]);
        ++i;
    }

    KConfigGroup cg = config();
    save(cg);

    init();
} // configAccepted()

void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug() << "constraintsEvent()" << constraints;
} // constraintsEvent()


/*
** Private
*/
void GmailNotifierApplet::readConfig()
{
    kDebug();
    m_cfgBackground      = config().readEntry("Background", "Standard");
    m_cfgDisplayLogo     = config().readEntry("DisplayLogo", true);
    m_cfgPollingInterval = config().readEntry("PollingInterval", 5);
    int accounts         = config().readEntry("Accounts", 0);

    m_cfgAccounts.clear();
    for (int i=1; i <= accounts; ++i) {
        QString prefix = QString("Account%1_").arg(i);
        QVariantMap data;
        data["Login"] = config().readEntry(prefix+"Login", QString());
        data["Password"] = KStringHandler::obscure(config().readEntry(prefix+"Password", QString()));
        data["Label"] = config().readEntry(prefix+"Label", QString());
        data["Display"] = config().readEntry(prefix+"Display", QString());

        m_cfgAccounts << data;
    }
} // readConfig()

void GmailNotifierApplet::setBackground()
{
    kDebug();
    Plasma::Applet::BackgroundHints hint;

    if (m_cfgBackground == "Standard") {
        hint = StandardBackground;
    } else if (m_cfgBackground == "Translucent") {
        hint = TranslucentBackground;
    } else if (m_cfgBackground == "Shadowed") {
        hint = ShadowedBackground;
    } else if (m_cfgBackground == "None") {
        hint = NoBackground;
    } else { // Default
        hint = DefaultBackground;
    }

    Plasma::Applet::setBackgroundHints(hint);
} // setBackground()

void GmailNotifierApplet::setLayout()
{
    kDebug();
    // Cleanup
    if (m_layoutContents) {
        int i = m_layoutContents->count()-1;
        while (i>-1) {
            QGraphicsLayoutItem *item = m_layoutContents->itemAt(i);
            m_layoutContents->removeAt(i);
            delete item;
            --i;
        }
    }

    // Main layout
    if (!m_layoutMain) {
        m_layoutMain = new QGraphicsLinearLayout(Qt::Vertical);
        Plasma::Applet::setLayout(m_layoutMain);
    }

    // Contents
    if (!m_layoutContents) {
        m_layoutContents = new QGraphicsGridLayout;
        m_layoutMain->addItem(m_layoutContents);
        m_layoutMain->addStretch(0xFF);
    }

    // Gmail Logo
    if (m_cfgDisplayLogo) {
        Plasma::Label *imgGmailLogo = new Plasma::Label(this);
        imgGmailLogo->setImage(":/images/gmail_logo.png");
        QLabel *logo = imgGmailLogo->nativeWidget();
        logo->setAlignment(Qt::AlignCenter);
        m_layoutContents->addItem(imgGmailLogo, 0, 0, 1, 2);
        m_layoutContents->setRowSpacing(0, 15);
    }

    // Applet Not configured
    if (!m_cfgAccounts.count()) {
        Plasma::Label *warning = new Plasma::Label(this);
        warning->setText(i18n("Not configured yet!"));
        QLabel *label = warning->nativeWidget();
        label->setAlignment(Qt::AlignCenter);
        m_layoutContents->addItem(warning, 1, 0, 1, 2);
        m_layoutContents->addItem(new Plasma::Label(this), 2, 1); // Second "dummy" column
        return;
    }

    // Accounts
    for (int i=0; i<m_cfgAccounts.count(); ++i) {
        QVariantMap account = m_cfgAccounts.at(i).toMap();
        QString display;
        if (!account["Display"].toString().isEmpty()) {
            display = account["Display"].toString();
        } else {
            QString label;
            if (account["Label"].toString().isEmpty()) {
                label = "inbox";
            } else {
                label = account["Label"].toString();
            }
            display = QString("%1/%2").arg(account["Login"].toString()).arg(label);
        }

        Plasma::Label *accountName = new Plasma::Label(this);
        Plasma::Label *accountCount = new Plasma::Label(this);
        QLabel *accountCountNW = accountCount->nativeWidget();
        accountCountNW->setAlignment(Qt::AlignRight);
        accountName->setText(display);
        accountCount->setText("-----");
        m_layoutContents->addItem(accountName, i+1, 0);
        m_layoutContents->addItem(accountCount, i+1, 1);

        QString mboxLabel = (account["Label"].toString().isEmpty()) ? "inbox" : account["Label"].toString();
        QString request = QString("%1:%2").arg(account["Login"].toString()).arg(mboxLabel);
        m_count[request] = accountCount;
    }
} // setLayout()

void GmailNotifierApplet::setPasswords() const
{
    kDebug();
    QVariantMap passwordList;

    foreach (QVariant account, m_cfgAccounts) {
        QVariantMap data(account.toMap());
        passwordList.insert(data["Login"].toString(), data["Password"]);
    }

    m_engine->setProperty("passwords", passwordList);
} // setPasswords()

#include "gmailnotifierapplet.moc"
