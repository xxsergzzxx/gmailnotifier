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
#include "gmailnotifierappletconfig.h"

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
    void createConfigurationInterface(KConfigDialog *parent);

private Q_SLOTS:
    void onClickNotifier();
    void configAccepted();

private:
    void readConfig();
    void drawIcon(const QString &text="");

    Plasma::DataEngine *m_engine;

    QString m_cfgBackground;
    bool m_cfgDisplayLogo;
    uint m_cfgPollingInterval;
    QVariantList m_cfgAccounts;

    GmailNotifierDialog *m_dialog;
    GmailNotifierAppletConfig *m_configDialog;

    Plasma::Icon *m_icon;
    QGraphicsProxyWidget *m_proxy;
    QGraphicsLinearLayout *m_layout;
};


K_EXPORT_PLASMA_APPLET(gmailnotifier, GmailNotifierApplet)


#endif // __GMAILNOTIFIER_APPLET_H__
