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


#ifndef __GMAILNOTIFIERDIALOG_H__
#define __GMAILNOTIFIERDIALOG_H__


// Qt
#include <QtCore/QObject>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>


class GmailNotifierDialog : public QObject
{
    Q_OBJECT

public:
    enum DialogArea
    {
        DesktopArea,
        PanelArea
    };

    GmailNotifierDialog(DialogArea area, QObject *parent=0);
    ~GmailNotifierDialog();

    QWidget * dialog();
    void show();
    void hide();

private:
    void buildDialog(DialogArea area);

    QWidget     *m_widget;
    QLabel      *m_lblLogo;
    QVBoxLayout *m_layoutMain;
    QGridLayout *m_layoutMails;
};


#endif // __GMAILNOTIFIERDIALOG_H__
