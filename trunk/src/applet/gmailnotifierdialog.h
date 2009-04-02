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

/*
** Some parts were borrowed from the Device Notifier Applet
** and are (C) Alexis Ménard
*/


#ifndef __GMAILNOTIFIERDIALOG_H__
#define __GMAILNOTIFIERDIALOG_H__


// Own
#include "accounts.h"
// Plasma
#include <Plasma/DataEngine>
// QtGui
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
// QtCore
#include <QtCore/QObject>


class GmailNotifierDialog : public QObject
{
    Q_OBJECT

        
public:
    GmailNotifierDialog(QObject *parent=0);
    ~GmailNotifierDialog();

    QWidget* widget();

    // Show/Hide the Gmail logo
    void setDisplayLogo(const bool &visible);

    // Textcolor
    void setTextColor(const QColor &color="#FFFFFF");

    // Populate the widget with user's accounts
    void setAccounts(const Accounts &accounts);

    // Update the mail count
    void updateMailCount(const QString &source, const Plasma::DataEngine::Data &data);


private:
    void buildDialog();

    QWidget     *m_widget;

    QVBoxLayout *m_layoutMain;
    QGridLayout *m_layoutAccounts;

    QLabel      *m_logoGmail;
};


#endif // __GMAILNOTIFIERDIALOG_H__
