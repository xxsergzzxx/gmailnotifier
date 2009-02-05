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
#include "gmailnotifierservicejob.h"

// KDE
#include <KDE/KDebug>


/*
** Public
*/
GmailNotifierServiceJob::GmailNotifierServiceJob(GmailNotifierContainer *source, const QMap<QString, QVariant> &parameters, QObject *parent)
    : Plasma::ServiceJob(source->account(), "update", parameters, parent)
{
    kDebug();

    m_url = "https://mail.google.com:443/mail/feed/atom/";
} // ctor()


GmailNotifierServiceJob::~GmailNotifierServiceJob()
{
    kDebug();
} // dtor()


void GmailNotifierServiceJob::start()
{
    kDebug();
} // start()


#include "gmailnotifierservicejob.moc"
