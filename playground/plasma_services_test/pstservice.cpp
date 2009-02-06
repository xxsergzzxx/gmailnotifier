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
#include "pstservice.h"
#include "pstsource.h"


class PSTService::Private
{
public:
    Private() {}
    ~Private() {}
}; // Private()


/*
** Public
*/
PSTService::PSTService(PSTSource *parent)
    : Plasma::Service(parent)
    , d(new Private)
{
    kDebug();

    setName("pst"); // --> "pst.operations" file
} // ctor()

PSTService::~PSTService()
{
    kDebug();
    delete d;
} // dtor()


/*
** Protected
*/
Plasma::ServiceJob* PSTService::createJob(const QString &operation, QMap<QString, QVariant> &parameters)
{
    kDebug();
    kDebug() << operation;
    kDebug() << parameters;

    //return new Plasma::ServiceJob(m_source->account(), operation, parameters, this);
    return new Plasma::ServiceJob("DUMMY", operation, parameters, this);
} // createJob()


#include "pstservice.moc"
