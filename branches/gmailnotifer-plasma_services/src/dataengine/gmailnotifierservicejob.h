/*
** Copyright (C) 2008 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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

#ifndef __GMAILNOTIFIERSERVICEJOB_H__
#define __GMAILNOTIFIERSERVICEJOB_H__

// Own
#include "gmailnotifiercontainer.h"

// Plasma
#include <Plasma/ServiceJob>

// KDE
#include <KDE/KUrl>

/*
class KJob;
namespace KIO
{
    class Job;
}
*/

class GmailNotifierServiceJob : public Plasma::ServiceJob
{
    Q_OBJECT

public:
    GmailNotifierServiceJob(GmailNotifierContainer *source,
                            const QMap<QString, QVariant> &parameters,
                            QObject *parent = 0);
    ~GmailNotifierServiceJob();

    void start();

private:
    KUrl m_url;
};


#endif // __GMAILNOTIFIERSERVICEJOB_H__
