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
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>


class GmailNotifierDialog::Private
{
public:
    Private()
        : widget(0)
        , lblLogo(0)
        , layoutMain(0)
        , layoutMails(0)
    {
    }
    ~Private()
    {
    }

    QWidget *widget;
    QLabel *lblLogo;
    QVBoxLayout *layoutMain;
    QGridLayout *layoutMails;
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
//        d->widget->setBackgroundRole(QPalette::Window);
        d->widget->setAttribute(Qt::WA_NoSystemBackground);
        break;
    case PanelArea:
        d->widget = new Plasma::Dialog();
        d->widget->setWindowFlags(Qt::Popup);
        break;
    }

    // Main layout
    d->layoutMain = new QVBoxLayout(d->widget);
    d->layoutMain->setSpacing(0);
    d->layoutMain->setMargin(10);
    d->lblLogo = new QLabel(d->widget);
    d->lblLogo->setPixmap(QPixmap(":/images/gmail_logo.png"));
    d->lblLogo->setAlignment(Qt::AlignCenter);
    d->layoutMain->addWidget(d->lblLogo);

    d->layoutMain->addSpacerItem(new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    d->layoutMails = new QGridLayout(d->widget);
    d->layoutMails->setSpacing(5);
    d->layoutMails->setMargin(0);

    for (int i=0; i<=10; ++i) {
        QLabel *lblL = new QLabel(d->widget);
        QLabel *lblR = new QLabel(d->widget);

        lblL->setText(QString("<font color=\"white\">Label_Left_%1</font>").arg(i));
        lblR->setText(QString("<font color=\"white\">Label_Right_%1</font>").arg(i));

        d->layoutMails->addWidget(lblL, i, 0, Qt::AlignLeft | Qt::AlignVCenter);
        d->layoutMails->addWidget(lblR, i, 1, Qt::AlignRight | Qt::AlignVCenter);
    }

    d->layoutMain->addItem(d->layoutMails);

    d->layoutMain->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
} // buildDialog()


#include "gmailnotifierdialog.moc"
