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

/*
** Some parts were borrowed from the Device Notifier Applet
** and are (C) Alexis Ménard
*/

#ifndef __GMAILNOTIFIER_APPLET_H__
#define __GMAILNOTIFIER_APPLET_H__


// Own
#include "gmailnotifierdialog.h"

// Plasma
#include <Plasma/Applet>
#include <Plasma/Icon>

// Qt
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsProxyWidget>


class GmailNotifierApplet : public Plasma::Applet
{
    Q_OBJECT

public:
    GmailNotifierApplet(QObject *parent, const QVariantList &args);
    ~GmailNotifierApplet();

    void init();

protected:
    void constraintsEvent(Plasma::Constraints constraints);

protected Q_SLOTS:
    void onClickNotifier();

private:
    void drawIcon(const QString &text="");

    Plasma::Icon          *m_icon;
    Plasma::DataEngine    *m_engine;
    GmailNotifierDialog   *m_dialog;
    QGraphicsProxyWidget  *m_proxy;
    QGraphicsLinearLayout *m_layout;
};


K_EXPORT_PLASMA_APPLET(gmailnotifier, GmailNotifierApplet)


#endif // __GMAILNOTIFIER_APPLET_H__
