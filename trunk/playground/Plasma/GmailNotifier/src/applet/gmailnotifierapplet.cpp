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
#include <Plasma/DataEngine>
#include <Plasma/Icon>


class GmailNotifierApplet::Private
{
public:
    Private()
        : icon(0)
        , engine(0)
    {
    }
    ~Private()
    {
    }

    Plasma::Icon *icon;
    Plasma::DataEngine *engine;
}; // Private


/*
** Public
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , d(new Private)
{
    kDebug();
    d->engine = Plasma::Applet::dataEngine("gmailnotifier");
    if (!d->engine->isValid()) {
        Plasma::Applet::setFailedToLaunch(true, i18n("Failed to open the data engine!"));
        return;
    }

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
    delete d;
} // dtor()


/*
** Protected
*/
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug();

    Q_UNUSED(constraints);

    switch (formFactor()) {
    case Plasma::Planar:
    case Plasma::MediaCenter:
        kDebug() << "Planar/MediaCenter";
        break;
    case Plasma::Horizontal:
    case Plasma::Vertical:
        kDebug() << "Horizontal/Vertical";
        drawIcon();
        break;
    }
} // constraintsEvent()


/*
** Private
*/
void GmailNotifierApplet::drawIcon()
{
    kDebug();

    if (d->icon) {
        delete d->icon;
        d->icon = 0;
    }
    d->icon = new Plasma::Icon(QIcon(":/images/gmailnotifier_icon.png"), QString(), this);
    d->icon->resize(geometry().size());
} // drawIcon()


#include "gmailnotifierapplet.moc"
