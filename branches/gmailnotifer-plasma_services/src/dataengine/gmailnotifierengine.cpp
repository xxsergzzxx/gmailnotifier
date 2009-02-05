/*
** Copyright (C) 2008-2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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
#include "gmailnotifiersource.h"


/*
** public:
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
//    kDebug();
} // dtor()

Plasma::Service* GmailNotifierEngine::serviceForSource(const QString &name)
{
    kDebug();

    GmailNotifierSource *source = qobject_cast<GmailNotifierSource*>(Plasma::DataEngine::containerForSource(name));

    if (!source) {
        return Plasma::DataEngine::serviceForSource(name);
    }

    Plasma::Service *service = source->createService();
    service->setParent(this);
    return service;
} // serviceForSource()


/*
** protected:
*/
bool GmailNotifierEngine::sourceRequestEvent(const QString &request)
{
    kDebug();

    return updateSourceEvent(request);
} // sourceRequestEvent()

bool GmailNotifierEngine::updateSourceEvent(const QString &request)
{
    kDebug();

    QStringList tokens = request.split(':');
    if (tokens.count() != 2) {
        kWarning() << "Request should be of the type: <accountname>:<label>";
        return false;
    }
    QString accountName(tokens.at(0));
    QString labelName(tokens.at(1));

    if (accountName.isEmpty()) {
        kWarning() << "<accountname> cannot be empty";
        return false;
    }

    GmailNotifierSource *source = qobject_cast<GmailNotifierSource*>(Plasma::DataEngine::containerForSource(request));

    if (!source) {
        source = new GmailNotifierSource(accountName, labelName, this);
        source->setObjectName(request);
        Plasma::DataEngine::addSource(source);
    }

    source->update();

    return false; // We're async, return false
} // updateSourceEvent()


#include "gmailnotifierengine.moc"
