/*
** Copyright (C) 2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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
#include "pstengine.h"

// KDE


class PSTEngine::Private
{
public:
    Private() {}
    ~Private() {}
}; // Private()


/*
** Public
*/
PSTEngine::PSTEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
    , d(new Private)
{
    kDebug();
} // ctor()

PSTEngine::~PSTEngine()
{
    kDebug();
    delete d;
} // dtor()


/*
** Protected
*/
void PSTEngine::init()
{
    kDebug();
} // init()

bool PSTEngine::sourceRequestEvent(const QString &request)
{
    kDebug();

    return true;
} // sourceRequestEvent()

bool PSTEngine::updateSourceEvent(const QString &request)
{
    kDebug();

    return true;
} // updateSourceEvent()


#include "pstengine.moc"
