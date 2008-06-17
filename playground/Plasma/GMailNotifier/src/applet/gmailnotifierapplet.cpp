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


/*
** Public
*/
GMailNotifierApplet::GMailNotifierApplet(QObject *parent, const QVariantList &args)
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

GMailNotifierApplet::~GMailNotifierApplet()
{
    kDebug();
} // dtor()

void GMailNotifierApplet::init()
{
    kDebug();
    readConfig();
    setBackground();
    setLayout();
} // init()


/*
** Public slots
*/
void GMailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();
    Q_UNUSED(source)
    Q_UNUSED(data)

    /*
    if (data.contains("Error")) {
        m_accountName->setText(data["error"].toString());
        m_newMailCount->setText(QString());
        return;
    }
    m_newMailCount->setText(data["fullcount"].toString());
    */
} // dataUpdated()


/*
** Protected
*/
void GMailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();
    m_configDialog = new GMailNotifierAppletConfig(parent);
    //parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    //connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    QVariantMap data;
    data["Background"]  = m_cfgBackground;
    data["DisplayLogo"] = m_cfgDisplayLogo;
    data["Accounts"]    = m_cfgAccounts;
    m_configDialog->importConfig(data);
} // createConfigurationInterface()


/*
** Private slots
*/
void GMailNotifierApplet::configAccepted()
{
    kDebug();
    QVariantMap data = m_configDialog->exportConfig();
    config().writeEntry("Background", data["Background"]);
    config().writeEntry("DisplayLogo", data["DisplayLogo"]);

    config().writeEntry("Accounts", data["Accounts"].toList().count());
    QVariantList accounts(data["Accounts"].toList());
    int i=1;
    foreach(QVariant data, accounts) {
        QVariantMap account(data.toMap());
        QString prefix = QString("Account%1_").arg(i);
        config().writeEntry(prefix+"Login", account["Login"]);
        config().writeEntry(prefix+"Password", account["Password"]);
        config().writeEntry(prefix+"Label", account["Label"]);
        config().writeEntry(prefix+"Display", account["Display"]);
        ++i;
    }

    KConfigGroup cg = config();
    save(cg);

    init();
} // configAccepted()

void GMailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug() << "constraintsEvent()" << constraints;
} // constraintsEvent()


/*
** Private
*/
void GMailNotifierApplet::readConfig()
{
    kDebug();
    m_cfgBackground  = config().readEntry("Background", "Standard");
    m_cfgDisplayLogo = config().readEntry("DisplayLogo", true);
    int accounts   = config().readEntry("Accounts", 0);

    m_cfgAccounts.clear();
    for (int i=1; i <= accounts; ++i) {
        QString prefix = QString("Account%1_").arg(i);
        QVariantMap data;
        data["Login"] = config().readEntry(prefix+"Login", QString());
        data["Password"] = config().readEntry(prefix+"Password", QString());
        data["Label"] = config().readEntry(prefix+"Label", QString());
        data["Display"] = config().readEntry(prefix+"Display", QString());

        m_cfgAccounts << data;
    }
} // readConfig()

void GMailNotifierApplet::setBackground()
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

void GMailNotifierApplet::setLayout()
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

    // GMail Logo
    if (m_cfgDisplayLogo) {
        Plasma::Label *imgGMailLogo = new Plasma::Label(this);
        imgGMailLogo->setImage(":/images/gmail_logo.png");
        QLabel *logo = imgGMailLogo->nativeWidget();
        logo->setAlignment(Qt::AlignCenter);
        m_layoutContents->addItem(imgGMailLogo, 0, 0, 1, 2);
        m_layoutContents->setRowSpacing(0, 15);
    }

    // Accounts
    if (!m_cfgAccounts.count()) {
        Plasma::Label *warning = new Plasma::Label(this);
        warning->setText(i18n("Not configured yet!"));
        QLabel *label = warning->nativeWidget();
        label->setAlignment(Qt::AlignCenter);
        m_layoutContents->addItem(warning, 1, 0, 1, 2);
        m_layoutContents->addItem(new Plasma::Label(this), 2, 1); // Second "dummy" column
    } else {
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
            QLabel *label = accountCount->nativeWidget();
            label->setAlignment(Qt::AlignRight);
            accountName->setText(display);
            accountCount->setText("----");
            m_layoutContents->addItem(accountName, i+1, 0);
            m_layoutContents->addItem(accountCount, i+1, 1);
        }
    }
} // setLayout()


#include "gmailnotifierapplet.moc"
