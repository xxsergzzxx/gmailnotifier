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
#include <Plasma/DataEngine>
#include <Plasma/IconWidget>
// KDE
#include <KDE/KDebug>
#include <KDE/KIcon>


#include <QtGui/QWidget>    // REMOVEME


/*
** public:
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
{
    kDebug();

    Plasma::IconWidget *icon = new Plasma::IconWidget(KIcon("gmailnotifier",NULL), QString());
    Plasma::PopupApplet::setPopupIcon(icon->icon());
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();
} // init()


QWidget* GmailNotifierApplet::widget()
{
    return new QWidget();
} // widget()


/*
** public Q_SLOTS:
*/
void GmailNotifierApplet::dataUpdated(const QString &/*source*/, const Plasma::DataEngine::Data &/*data*/)
{
    kDebug();
} // dataUpdated()


/*
** protected:
*/
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints /*constraints*/)
{
    kDebug();
} // constraintsEvent()

void GmailNotifierApplet::createConfigurationInterface(KConfigDialog * /*parent*/)
{
    kDebug();
} // createConfigurationInterface()


#include "gmailnotifierapplet.moc"
