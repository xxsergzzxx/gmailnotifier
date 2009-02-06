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
#include "gmailnotifierservice.h"
// Plasma
#include <Plasma/ServiceJob>
// KDE
#include <KDE/KDebug>


/*
** public:
*/
GmailNotifierService::GmailNotifierService(GmailNotifierSource *parent)
    : Plasma::Service(parent)
    , m_source(parent)
{
    kDebug();

    Plasma::Service::setName("gmailnotifierengine");
} // ctor()


GmailNotifierService::~GmailNotifierService()
{
//    kDebug();
} // dtor()


/*
** protected:
*/
Plasma::ServiceJob* GmailNotifierService::createJob(const QString &operation, QMap<QString, QVariant> &parameters)
{
    kDebug();

    if (operation == "auth") {
        m_source->setPassword(parameters.value("password").toString());
    }

    // Fail!
    return new Plasma::ServiceJob(m_source->account(), operation, parameters, this);
} // createJob()


#include "gmailnotifierservice.moc"
