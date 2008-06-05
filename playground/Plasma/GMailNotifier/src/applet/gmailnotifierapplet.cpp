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
#include <KStringHandler>

// Qt
#include <QtCore/QFile>
#include <QtGui/QGraphicsLinearLayout>


class GMailNotifierApplet::Private
{
public:
    // Layouts
    QGraphicsLinearLayout *layout;
    QGraphicsLinearLayout *headerLayout;
    QGraphicsLinearLayout *mailboxLayout;

    // Labels / Images
    Plasma::Label *gmailLogo;
    Plasma::Label *accountName;
    Plasma::Label *newMailCount;

    // Config
    QHash<QString, QString> config;
    GMailNotifierAppletConfig *configDialog;

    // Engine
    Plasma::DataEngine *engine;

    Private()
        : configDialog(0)
        , engine(0)
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
    d->layout = new QGraphicsLinearLayout(Qt::Vertical);
    Plasma::Applet::setLayout(d->layout);

    d->headerLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    d->headerLayout->setContentsMargins(5, 5, 5, 15);
    d->layout->addItem(d->headerLayout);

    d->mailboxLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    d->mailboxLayout->setContentsMargins(5, 0, 5, 0);
    d->layout->addItem(d->mailboxLayout);
    d->layout->addStretch(100);

    d->gmailLogo = new Plasma::Label(this);
    d->gmailLogo->setImage(":/images/gmail_logo.png");
    d->headerLayout->addStretch();
    d->headerLayout->addItem(d->gmailLogo);
    d->headerLayout->addStretch();

    d->accountName = new Plasma::Label(this);
    d->newMailCount = new Plasma::Label(this);
    d->accountName->setText("");
    d->newMailCount->setText("---");
    d->mailboxLayout->insertItem(0, d->accountName);
    d->mailboxLayout->insertStretch(1);
//    d->mailboxLayout->setItemSpacing(1, 20);
    d->mailboxLayout->insertItem(2, d->newMailCount);

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
} // ctor()

GMailNotifierApplet::~GMailNotifierApplet()
{
    delete d;
} // dtor()

void GMailNotifierApplet::init()
{
    d->engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!d->engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Unable to open the data engine!"));
        return;
    }

    readConfig();
} // init()


/*
** Public slots
*/
void GMailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(source)

    if (data.contains("Error")) {
        d->accountName->setText(data["error"].toString());
        d->newMailCount->setText(QString());
        return;
    }
    d->newMailCount->setText(data["fullcount"].toString());
} // dataUpdated()


/*
** Protected
*/
void GMailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    d->configDialog = new GMailNotifierAppletConfig(parent);
    parent->setButtons(KDialog::Ok | KDialog::Cancel | KDialog::Apply);
    parent->addPage(d->configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

    /*
    d->configDialog->setBackground(d->config["Background"]);
    d->configDialog->setLogin(d->config["Login"]);
    d->configDialog->setPassword(KStringHandler::obscure(d->config["Password"]));
    d->configDialog->setDisplayName(d->config["DisplayName"]);
    */
} // createConfigurationInterface()


/*
** Private slots
*/
void GMailNotifierApplet::configAccepted()
{
    /*
    config().writeEntry("Background", d->configDialog->background());
    config().writeEntry("Login", d->configDialog->login());
    config().writeEntry("Password", KStringHandler::obscure(d->configDialog->password()));
    config().writeEntry("DisplayName", d->configDialog->displayName());
    */

    KConfigGroup cg = config();
    save(cg);
    readConfig();
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
    /*
    QList<QVariant> list;
    list << 1 << 3 << 5 << 34;
    config().writeEntry("TEST", list);
    */
    qDebug() << config().readEntry("TEST", QList<QVariant>());
    d->config["Background"] = config().readEntry("Background", "Default");
    d->config["Login"] = config().readEntry("Login", QString());
    QString password = config().readEntry("Password", QString());
    d->config["Password"] = KStringHandler::obscure(password);
    d->config["DisplayName"] = config().readEntry("DisplayName", QString());

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

    setBackground(d->config["Background"]);

//    d->engine->connectSource(QString("%1:%2").arg(d->config["Login"]).arg(d->config["Password"]), this);

} // readConfig()

void GMailNotifierApplet::setBackground(const QString &background)
{
    Plasma::Applet::BackgroundHints hint;

    if (background == "Standard") {
        hint = StandardBackground;
    } else if (background == "Translucent") {
        hint = TranslucentBackground;
    } else if (background == "Shadowed") {
        hint = ShadowedBackground;
    } else { // Default
        hint = DefaultBackground;
    }

    Plasma::Applet::setBackgroundHints(hint);
} // setBackground()


#include "gmailnotifierapplet.moc"
