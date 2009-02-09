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


// Own
#include "gmailnotifierdialog.h"
// KDE
#include <KDE/KDebug>
// QtGui
#include <QtGui/QWidget>


/*
** Public
*/
GmailNotifierDialog::GmailNotifierDialog(QObject *parent)
    : QObject(parent)
    , m_widget(new QWidget)
{
    kDebug();
} // ctor()

GmailNotifierDialog::~GmailNotifierDialog()
{
    kDebug();
} // dtor()

QWidget* GmailNotifierDialog::widget()
{
    kDebug();
    return m_widget;
} // widget();


#include "gmailnotifierdialog.moc"
