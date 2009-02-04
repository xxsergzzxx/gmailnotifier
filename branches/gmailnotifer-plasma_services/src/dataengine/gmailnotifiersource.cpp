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
#include "gmailnotifiersource.h"
#include "gmailnotifierservice.h"


/*
** Public
*/
GmailNotifierSource::GmailNotifierSource(const QString &accountName, const QString &labelName, QObject *parent)
    : Plasma::DataContainer(parent)
{
    kDebug();
    m_url.setUrl(QString("https://mail.google.com:443/mail/feed/atom/%1").arg(labelName), QUrl::StrictMode);
    m_url.setUserName(accountName);

    kDebug() << m_url;

    update();
} // ctor()


GmailNotifierSource::~GmailNotifierSource()
{
    kDebug();
} // dtor()

/*
 * Not sure this is really necessary ?!
Plasma::Service* GmailNotifierSource::createService()
{
    kDebug();

    return new GmailNotifierService(this);
} // createService()
*/

void GmailNotifierSource::update()
{
    kDebug();
    // DO SOMETHING !!
} // update()


/*
void GmailNotifierSource::setPassword(const QString &password)
{
    kDebug();
    m_url.setPassword(password);
    update();
} // setPassword()
*/

/*
QString GmailNotifierSource::account() const
{
    kDebug();

    return m_url.userName();
} // account()
*/

#include "gmailnotifiersource.moc"
