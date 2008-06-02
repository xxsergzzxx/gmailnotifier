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

#ifndef __GMAILNOTIFIER_APPLET_CONFIG_H__
#define __GMAILNOTIFIER_APPLET_CONFIG_H__


// Own
#include "ui_gmailnotifierappletconfig.h"

// Qt
#include <QtGui/QWidget>


class GMailNotifierAppletConfig : public QWidget
{
    Q_OBJECT

public:
    GMailNotifierAppletConfig(QWidget *parent = 0);
    ~GMailNotifierAppletConfig();

    void setBackground(const QString &background);
    QString background() const;
    void setLogin(const QString &login);
    QString login() const;
    void setPassword(const QString &password);
    QString password() const;
    void setDisplayName(const QString &displayName);
    QString displayName() const;

private:
    Ui::GMailNotifierAppletConfig ui;

    class Private;
    Private * const d;
};


#endif // __GMAILNOTIFIER_APPLET_CONFIG_H__
