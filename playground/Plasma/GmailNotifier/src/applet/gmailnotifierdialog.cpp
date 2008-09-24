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
#include "gmailnotifierdialog.h"

// Plasma
#include <Plasma/Dialog>

// KDE
#include <KDE/KDebug>

// Qt
#include <QtGui/QLabel>


class GmailNotifierDialog::Private
{
public:
    Private()
        : widget(0)
    {
    }
    ~Private()
    {
    }

    QWidget *widget;
    QLabel *label;
}; // Private


/*
** Public
*/
GmailNotifierDialog::GmailNotifierDialog(DialogArea area, QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    kDebug();
    buildDialog(area);
} // ctor()

GmailNotifierDialog::~GmailNotifierDialog()
{
    kDebug();
    delete d;
} // dtor()

QWidget * GmailNotifierDialog::dialog()
{
    kDebug();
    return d->widget;
} // dialog()

void GmailNotifierDialog::show()
{
    kDebug();
    d->widget->show();
} // show()

void GmailNotifierDialog::hide()
{
    kDebug();
    d->widget->hide();
} // hide;

/*
** Private
*/
void GmailNotifierDialog::buildDialog(DialogArea area)
{
    kDebug();
    switch (area) {
    case DesktopArea:
        d->widget = new QWidget();
//        d->widget->setBackgroundRole(QPalette::Shadow);
        break;
    case PanelArea:
        d->widget = new Plasma::Dialog();
//        d->widget->setWindowFlags(Qt::Popup);
        break;
    }

    d->label = new QLabel("TAISTE", d->widget);
    kDebug() << d->label->text();
} // buildDialog()


#include "gmailnotifierdialog.moc"
