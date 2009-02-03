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

/*
** PST DATAENGINE (PST = Plasma Services Test)
*/

#ifndef __PST_DATAENGINE_H__
#define __PST_DATAENGINE_H__

// Plasma
#include <Plasma/DataEngine>
#include <Plasma/Service>


class PSTEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    PSTEngine(QObject *parent, const QVariantList &args);
    ~PSTEngine();
    Plasma::Service* serviceForSource(const QString &source);

protected:
    void init();
    bool sourceRequestEvent(const QString &request);
    bool updateSourceEvent(const QString &request);

private:
    class Private;
    Private * const d;
};


K_EXPORT_PLASMA_DATAENGINE(pstengine, PSTEngine)


#endif // __PST_DATAENGINE_H__
