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

#ifndef __GMAILNOTIFIER_APPLET_H__
#define __GMAILNOTIFIER_APPLET_H__


// Plasma
#include <Plasma/Applet>


class GmailNotifierApplet : public Plasma::Applet
{
    Q_OBJECT

public:
    GmailNotifierApplet(QObject *parent, const QVariantList &args);
    ~GmailNotifierApplet();

protected:
    void constraintsEvent(Plasma::Constraints constraints);

private:
    class Private;
    Private * const d;

    void drawIcon();
};


K_EXPORT_PLASMA_APPLET(gmailnotifier, GmailNotifierApplet)


#endif // __GMAILNOTIFIER_APPLET_H__
