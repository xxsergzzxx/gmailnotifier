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
#include "gmailnotifierappletconfig.h"

// Qt
#include <QtCore/QDebug>


class GMailNotifierAppletConfig::Private
{
public:
    Private() {}
    ~Private() {}
}; // Private()


GMailNotifierAppletConfig::GMailNotifierAppletConfig(QWidget *parent)
    : QWidget(parent)
    , d(new Private)
{
    ui.setupUi(this);

    // Background Hints
    QStringList backgroundHints;
    backgroundHints << "Default" << "Standard" << "Translucent" << "Shadowed" << "None";
    ui.comboBackground->addItems(backgroundHints);
} // ctor()

GMailNotifierAppletConfig::~GMailNotifierAppletConfig()
{
    delete d;
} // dtor()

void GMailNotifierAppletConfig::setBackground(const QString &background)
{
    int idx = ui.comboBackground->findText(background);
    ui.comboBackground->setCurrentIndex(idx);
} // setBackground()

QString GMailNotifierAppletConfig::background() const
{
    return ui.comboBackground->currentText();
} // background()

void GMailNotifierAppletConfig::setLogin(const QString &login)
{
    ui.leditLogin->setText(login);
} // setLogin()

QString GMailNotifierAppletConfig::login() const
{
    return ui.leditLogin->text();
} // login()

void GMailNotifierAppletConfig::setPassword(const QString &password)
{
    ui.leditPassword->setText(password);
} // setPassword()

QString GMailNotifierAppletConfig::password() const
{
    return ui.leditPassword->text();
} // password()

void GMailNotifierAppletConfig::setDisplayName(const QString &displayName)
{
    ui.leditDisplayName->setText(displayName);
} // setDisplayName()

QString GMailNotifierAppletConfig::displayName() const
{
    return ui.leditDisplayName->text();
} // displayName()

#include "gmailnotifierappletconfig.moc"
