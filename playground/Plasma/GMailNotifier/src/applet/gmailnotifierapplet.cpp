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
#include "gmailnotifierappletconfig.h"

// Plasma
#include <Plasma/Label>

// KDE
#include <KConfigDialog>

// Qt
#include <QtGui/QGraphicsGridLayout>
#include <QtGui/QGraphicsLinearLayout>


class GMailNotifierApplet::Private
{
public:
    // Engine
    Plasma::DataEngine *engine;

    // Config
    QString      cfgBackground;
    bool         cfgDisplayLogo;
    QVariantList cfgAccounts;
    GMailNotifierAppletConfig *configDialog;

    // Layouts
    QGraphicsLinearLayout *layoutMain;
    QGraphicsLinearLayout *layoutLogo;
    QGraphicsLinearLayout *layoutNotConfigured;
    QGraphicsGridLayout   *layoutAccounts;

    // Labels / Images
    Plasma::Label *lblGMailLogo;
    Plasma::Label *lblNotConfigured;

    Private()
        : engine(0), configDialog(0)
        , layoutMain(0), layoutLogo(0), layoutNotConfigured(0), layoutAccounts(0)
    {}
    ~Private() {}
}; // Private()


/*
** Public
*/
GMailNotifierApplet::GMailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , d(new Private)
{
    d->engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!d->engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
} // ctor()

GMailNotifierApplet::~GMailNotifierApplet()
{
    delete d;
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
    Q_UNUSED(source)
    Q_UNUSED(data)

    /*
    if (data.contains("Error")) {
        d->accountName->setText(data["error"].toString());
        d->newMailCount->setText(QString());
        return;
    }
    d->newMailCount->setText(data["fullcount"].toString());
    */
} // dataUpdated()


/*
** Protected
*/
void GMailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    d->configDialog = new GMailNotifierAppletConfig(parent);
    //parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(d->configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    //connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    QVariantMap data;
    data["Background"]  = d->cfgBackground;
    data["DisplayLogo"] = d->cfgDisplayLogo;
    data["Accounts"]    = d->cfgAccounts;
    d->configDialog->importConfig(data);
} // createConfigurationInterface()


/*
** Private slots
*/
void GMailNotifierApplet::configAccepted()
{
    QVariantMap data = d->configDialog->exportConfig();
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
    d->cfgBackground  = config().readEntry("Background", "Default");
    d->cfgDisplayLogo = config().readEntry("DisplayLogo", true);
    int accounts   = config().readEntry("Accounts", 0);

    d->cfgAccounts.clear();
    for (int i=1; i <= accounts; ++i) {
        QString prefix = QString("Account%1_").arg(i);
        QVariantMap data;
        data["Login"] = config().readEntry(prefix+"Login", QString());
        data["Password"] = config().readEntry(prefix+"Password", QString());
        data["Label"] = config().readEntry(prefix+"Label", QString());
        data["Display"] = config().readEntry(prefix+"Display", QString());

        d->cfgAccounts << data;
    }

    /*
    if (!d->config["DisplayName"].isEmpty()) {
        d->accountName->setText(d->config["DisplayName"]);
    } else {
        if (!d->config["Login"].isEmpty() || !d->config["Password"].isEmpty()) {
            d->accountName->setText(QString("%1@gmail.com").arg(d->config["Login"]));
        } else {
            d->accountName->setText(i18n("Not configured yet!"));
            d->newMailCount->setText("");
        }
    }

    setBackground(d->cfgBackground);
    */
} // readConfig()

void GMailNotifierApplet::setBackground()
{
    Plasma::Applet::BackgroundHints hint;

    if (d->cfgBackground == "Standard") {
        hint = StandardBackground;
    } else if (d->cfgBackground == "Translucent") {
        hint = TranslucentBackground;
    } else if (d->cfgBackground == "Shadowed") {
        hint = ShadowedBackground;
    } else if (d->cfgBackground == "None") {
        hint = NoBackground;
    } else { // Default
        hint = DefaultBackground;
    }

    Plasma::Applet::setBackgroundHints(hint);
} // setBackground()

void GMailNotifierApplet::setLayout()
{
    d->layoutMain = new QGraphicsLinearLayout(Qt::Vertical);
    Plasma::Applet::setLayout(d->layoutMain);

    // GMail logo
    /*
    if (d->cfgDisplayLogo) {
        if (!d->layoutLogo && !d->lblGMailLogo) {
            d->layoutLogo = new QGraphicsLinearLayout(Qt::Horizontal);
            d->layoutLogo->setContentsMargins(5, 5, 5, 15);
            d->layoutMain->addItem(d->layoutLogo);

            d->lblGMailLogo = new Plasma::Label(this);
            d->lblGMailLogo->setImage(":/images/gmail_logo.png");
            d->layoutLogo->addStretch();
            d->layoutLogo->addItem(d->lblGMailLogo);
            d->layoutLogo->addStretch();
        }
    } else {
        if (d->lblGMailLogo) {
            delete d->lblGMailLogo; d->lblGMailLogo = 0;
        }
        if (d->layoutLogo) {
            delete d->layoutLogo; d->layoutLogo = 0;
        }
    }
    */

    /*
    if (!d->cfgAccounts.count()) {
        d->layoutNotConfigured = new QGraphicsLinearLayout(Qt::Horizontal);
        d->layoutMain->addItem(d->layoutNotConfigured);

        d->lblNotConfigured = new Plasma::Label(this);
        d->lblNotConfigured->setText(i18n("Not configured yet!"));
        d->layoutNotConfigured->addStretch();
        d->layoutNotConfigured->addItem(d->lblNotConfigured);
        d->layoutNotConfigured->addStretch();
    } else {
        if (d->lblNotConfigured) {
            delete d->lblNotConfigured;
            d->lblNotConfigured = 0;
        }
        if (d->layoutNotConfigured) {
            delete d->layoutNotConfigured;
            d->layoutNotConfigured = 0;
        }
    }
    */

    /*
    d->mailboxLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    d->mailboxLayout->setContentsMargins(5, 0, 5, 0);
    d->layoutMain->addItem(d->mailboxLayout);
    d->layoutMain->addStretch(100);

    d->accountName = new Plasma::Label(this);
    d->newMailCount = new Plasma::Label(this);
    d->accountName->setText("");
    d->newMailCount->setText("---");
    d->mailboxLayout->insertItem(0, d->accountName);
    d->mailboxLayout->insertStretch(1);
//    d->mailboxLayout->setItemSpacing(1, 20);
    d->mailboxLayout->insertItem(2, d->newMailCount);
    */

//    d->layoutMain->activate();
    d->layoutMain->addStretch();
} // setLayout()


#include "gmailnotifierapplet.moc"
