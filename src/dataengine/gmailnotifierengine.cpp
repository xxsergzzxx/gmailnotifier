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
#include "gmailnotifierengine.h"
#include "gmailnotifiercontainer.h"

/*
** Public
*/
GmailNotifierEngine::GmailNotifierEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
{
    kDebug();

    // 1 min ought to be enough for anybody :)
    Plasma::DataEngine::setMinimumPollingInterval(1000 * 60 * 1);
} // ctor()


GmailNotifierEngine::~GmailNotifierEngine()
{
    kDebug();
} // dtor()

Plasma::Service* GmailNotifierEngine::serviceForSource(const QString &name)
{
    kDebug();

    GmailNotifierContainer *source = qobject_cast<GmailNotifierContainer *>(Plasma::DataEngine::containerForSource(name));

    if (!source) {
        return Plasma::DataEngine::serviceForSource(name);
    }

    Plasma::Service *service = source->createService();
    service->setParent(this);
    return service;
} // serviceForSource()


/*
** Protected
*/
bool GmailNotifierEngine::sourceRequestEvent(const QString &request)
{
    kDebug();

    return updateSourceEvent(request);
} // sourceRequestEvent()

bool GmailNotifierEngine::updateSourceEvent(const QString &request)
{
    kDebug();

    // FIXME: Import working code from previous dataengine
    return true;
} // updateSourceEvent()


#include "gmailnotifierengine.moc"
