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
#include "gmailnotifierapplet.h"
// Plasma
#include <Plasma/DataEngine>
#include <Plasma/IconWidget>
#include <Plasma/ToolTipManager>
// KDE
#include <KDE/KConfigDialog>
#include <KDE/KDebug>
#include <KDE/KIcon>


/*
** public:
*/
GmailNotifierApplet::GmailNotifierApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args)
    , m_dialog(0)
    , m_configDialog(0)
    , m_icon(0)
{
    kDebug();

    Plasma::Applet::setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Plasma::Applet::setHasConfigurationInterface(true);

    // Initialize the widget
    (void)widget();
    resize(widget()->sizeHint());
} // ctor()

GmailNotifierApplet::~GmailNotifierApplet()
{
    kDebug();
} // dtor()

void GmailNotifierApplet::init()
{
    kDebug();
//    m_icon = new Plasma::IconWidget(KIcon("gmailnotifier", 0), QString(), this);
//    Plasma::PopupApplet::setPopupIcon(m_icon->icon());

    Plasma::PopupApplet::setPopupIcon("gmailnotifier");

} // init()

QWidget* GmailNotifierApplet::widget()
{
    if (!m_dialog) {
        m_dialog = new GmailNotifierDialog(this);
    }

    return m_dialog->widget();
} // widget()


/*
** public Q_SLOTS:
*/
/*
void GmailNotifierApplet::dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    kDebug();
} // dataUpdated()
*/

/*
** protected:
*/
/*
void GmailNotifierApplet::constraintsEvent(Plasma::Constraints constraints)
{
    kDebug();
} // constraintsEvent()
*/

void GmailNotifierApplet::createConfigurationInterface(KConfigDialog *parent)
{
    kDebug();

    m_configDialog = new GmailNotifierAppletConfig(Plasma::Applet::config(), parent);
    parent->setButtons(KDialog::Ok | KDialog::Cancel);
    parent->addPage(m_configDialog, parent->windowTitle(), icon());
    parent->setDefaultButton(KDialog::Ok);
    parent->showButtonSeparator(true);
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
} // createConfigurationInterface()


/*
** private Q_SLOTS:
*/
void GmailNotifierApplet::configAccepted()
{
    kDebug();

    KConfigGroup cg(m_configDialog->config());
    Plasma::Applet::save(cg);
} // configAccepted()


#include "gmailnotifierapplet.moc"
