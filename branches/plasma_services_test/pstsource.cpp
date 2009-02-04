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
#include "pstsource.h"
#include "pstservice.h"


class PSTSource::Private
{
public:
    Private() {}
    ~Private() {}

    QString account;
    QString password;
}; // Private()


/*
** Public
*/
PSTSource::PSTSource(const QString &account, QObject *parent)
    : Plasma::DataContainer(parent)
    , d(new Private)
{
    kDebug();

    d->account = account;
} // ctor()

PSTSource::~PSTSource()
{
    kDebug();
    delete d;
} // dtor()

Plasma::Service* PSTSource::createService()
{
    kDebug();

    return new PSTService(this);
} // createService()

void PSTSource::setPassword(const QString &password)
{
    d->password = password;
} // setPassword()

QString PSTSource::account() const
{
    return QString();
} // account()

QString PSTSource::password() const
{
    return QString();
} // password()

#include "pstsource.moc"
