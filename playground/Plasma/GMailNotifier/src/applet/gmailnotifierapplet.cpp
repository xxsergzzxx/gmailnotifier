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
#include <Plasma/Label>

// Qt
#include <QtCore/QFile>
#include <QtGui/QGraphicsLinearLayout>


class GMailNotifierApplet::Private
{
public:
    Plasma::Label *gmailLogo;
    Plasma::Label *accountName;
    Plasma::Label *newMailCount;

    Private() {}
    ~Private() {}
}; // Private()


GMailNotifierApplet::GMailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , d(new Private)
{
    d->gmailLogo = new Plasma::Label(this);
    d->accountName = new Plasma::Label(this);
    d->newMailCount = new Plasma::Label(this);
    d->gmailLogo->setImage(":/images/gmail_logo.png");
    d->accountName->setText("Mailbox");
    d->newMailCount->setText("42");
} // ctor()


GMailNotifierApplet::~GMailNotifierApplet()
{
    delete d;
} // dtor()


void GMailNotifierApplet::init()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->addItem(d->gmailLogo);
    layout->addItem(d->accountName);
    layout->addItem(d->newMailCount);
    setLayout(layout);

    setMinimumSize(300, 100);
    setMaximumSize(300, 200);
} // init()


void GMailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug() << "constraintsEvent()" << constraints;
} // constraintsEvent()


#include "gmailnotifierapplet.moc"
