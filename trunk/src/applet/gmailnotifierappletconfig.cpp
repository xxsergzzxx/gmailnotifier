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
#include "defaults.h"
#include "gmailnotifierappletconfig.h"
// KDE
#include <KDE/KColorScheme>
#include <KDE/KDebug>
#include <KDE/KGlobal>
#include <KDE/KStringHandler>


/*
** public
*/
GmailNotifierAppletConfig::GmailNotifierAppletConfig(KConfigGroup cg, QWidget *parent)
    : QWidget(parent)
{
    kDebug();

    m_cg = cg;

    ui.setupUi(this);

    // Init CGUI
    initDialog();
} // ctor()

GmailNotifierAppletConfig::~GmailNotifierAppletConfig()
{
    kDebug();
} // dtor()

KConfigGroup GmailNotifierAppletConfig::config()
{
    kDebug();
    m_cg.writeEntry("Background", ui.comboBackground->currentText());
    m_cg.writeEntry("DisplayLogo", ui.cbDisplayLogo->isChecked());
    m_cg.writeEntry("PollingInterval", ui.spinPollingInterval->value());
    m_cg.writeEntry("IconTextColor", ui.kcbIconColor->color().name());
    m_cg.writeEntry("DialogTextColor", ui.kcbDialogColor->color().name());

    // Start cnt at "-1" in case the following "for" statement
    // is ignored (no accounts). That way its value cannot be more than 0
    // when we'll increment it later.
    int cnt=-1;
    for (int i=0; i<ui.listAccounts->count(); ++i) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        QVariantMap account(item->data(Qt::UserRole).toMap());
        QString prefix = QString("Account%1_").arg(i);
        m_cg.writeEntry(prefix+"Login", account["Login"].toString());
        m_cg.writeEntry(prefix+"Password", KStringHandler::obscure(account["Password"].toString()));
        m_cg.writeEntry(prefix+"Label", account["Label"].toString());
        m_cg.writeEntry(prefix+"Display", account["Display"].toString());
        m_cg.writeEntry(prefix+"DisableNotifications", account["DisableNotifications"].toBool());
        m_cg.writeEntry(prefix+"ExcludeFromTotalCount", account["ExcludeFromTotalCount"].toBool());
        cnt = i;
    }

    ++cnt;

    // Make sure we delete any remaining previously created accounts
    bool loop = true;
    while (loop == true) {
        QString prefix = QString("Account%1_").arg(cnt);
        QString login = m_cg.readEntry(prefix+"Login", QString());
        // No (more?) accounts
        if (login.isEmpty()) {
            loop = false;
            break;
        }
        m_cg.deleteEntry(prefix+"Login");
        m_cg.deleteEntry(prefix+"Password");
        m_cg.deleteEntry(prefix+"Label");
        m_cg.deleteEntry(prefix+"Display");
        m_cg.deleteEntry(prefix+"DisableNotifications");
        m_cg.deleteEntry(prefix+"ExcludeFromTotalCount");

        ++cnt;
    }

    return m_cg;
} // config()


/*
** private slots
*/
void GmailNotifierAppletConfig::on_leLogin_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
    adaptAddModifyButtonLabel();
} // on_leLogin_textChanged()

void GmailNotifierAppletConfig::on_leLabel_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
    adaptAddModifyButtonLabel();
} // on_leLabel_textChanged()

void GmailNotifierAppletConfig::on_lePassword_textChanged(const QString &text)
{
    kDebug();
    Q_UNUSED(text)
    setAddModifyButtonEnabled();
} // on_lePassword_textChanged()

void GmailNotifierAppletConfig::on_btnAddModify_clicked()
{
    kDebug();
    QVariantMap data;
    data["Login"] = ui.leLogin->text();
    data["Password"] = ui.lePassword->text();
    data["Label"] = ui.leLabel->text();
    data["Display"] = ui.leDisplay->text();
    data["DisableNotifications"] = ui.cbDisableNotifications->isChecked();
    data["ExcludeFromTotalCount"] = ui.cbExcludeFromTotalCount->isChecked();

    int pos = accountPosition(ui.leLogin->text(), ui.leLabel->text());
    if (pos == -1) {
        // Add
        addItemToList(data);
        setUpDownButtonsEnabled();
    } else {
        // Modify
        QListWidgetItem *item = ui.listAccounts->item(pos);
        item->setText(listItemText(data));
        item->setData(Qt::UserRole, data);
    }

    adaptAddModifyButtonLabel();
} // on_btnAddModify_clicked()

void GmailNotifierAppletConfig::on_btnDelete_clicked()
{
    kDebug();
//    ui.listAccounts->removeItemWidget(ui.listAccounts->currentItem());
    ui.listAccounts->takeItem(ui.listAccounts->currentRow());
    ui.listAccounts->setCurrentRow(-1);
    ui.btnDelete->setEnabled(false);
    adaptAddModifyButtonLabel();
    setUpDownButtonsEnabled();
} // on_btnDelete_clicked()

void GmailNotifierAppletConfig::on_btnUp_clicked()
{
    kDebug();
    moveItem(-1);

    setUpDownButtonsEnabled();
} // on_btnUp_clicked()

void GmailNotifierAppletConfig::on_btnDown_clicked()
{
    kDebug();
    moveItem(+1);

    setUpDownButtonsEnabled();
} // on_btnDown_clicked()

void GmailNotifierAppletConfig::on_listAccounts_itemPressed(QListWidgetItem *item)
{
    kDebug();
    QVariantMap data(item->data(Qt::UserRole).toMap());
    ui.leLogin->setText(data["Login"].toString());
    ui.lePassword->setText(data["Password"].toString());
    ui.leLabel->setText(data["Label"].toString());
    ui.leDisplay->setText(data["Display"].toString());
    ui.cbDisableNotifications->setChecked(data["DisableNotifications"].toBool());
    ui.cbExcludeFromTotalCount->setChecked(data["ExcludeFromTotalCount"].toBool());

    ui.btnDelete->setEnabled(true);
    setUpDownButtonsEnabled();
    adaptAddModifyButtonLabel();
} // on_listAccounts_itemPressed()

void GmailNotifierAppletConfig::on_spinPollingInterval_valueChanged(int value)
{
    kDebug();

    KColorScheme kcs(QPalette::Active, KColorScheme::View);
    QPalette pal(ui.spinPollingInterval->palette());

    if (value < 5) {
        pal.setColor(QPalette::Text, kcs.foreground(KColorScheme::NegativeText).color());
    } else {
        pal.setColor(QPalette::Text, kcs.foreground(KColorScheme::NormalText).color());
    }

    ui.spinPollingInterval->setPalette(pal);
} // on_spinPollingInterval_valueChanged() 

/*
** private
*/
void GmailNotifierAppletConfig::initDialog()
{
    // use of @gmail.com is forbidden in the UK and is replaced by @googlemail.com
    // http://mail.google.com/mail/help/intl/en-GB/googlemail.html
    if (KGlobal::locale()->country() == "gb") {
        ui.lblAtDomain->setText("@googlemail.com");
    } else {
        ui.lblAtDomain->setText("@gmail.com");
    }

    // Button icons
    ui.btnAddModify->setIcon(KIcon("list-add"));
    ui.btnDelete->setIcon(KIcon("list-remove"));
    ui.btnUp->setIcon(KIcon("arrow-up"));
    ui.btnDown->setIcon(KIcon("arrow-down"));

    // Display logo
    ui.cbDisplayLogo->setChecked(m_cg.readEntry("DisplayLogo", Defaults::displayLogo));

    // Polling Interval
    ui.spinPollingInterval->setValue(m_cg.readEntry("PollingInterval", Defaults::pollingInterval));

    // Background
    QStringList backgroundHints;
    backgroundHints << "Standard" << "Translucent" << "None";
    ui.comboBackground->addItems(backgroundHints);
    
    int pos = ui.comboBackground->findText(m_cg.readEntry("Background", Defaults::background), Qt::MatchExactly);
    ui.comboBackground->setCurrentIndex(pos);

    // Colors
    ui.kcbIconColor->setColor(m_cg.readEntry("IconTextColor", Defaults::iconTextColor));
    ui.kcbDialogColor->setColor(m_cg.readEntry("DialogTextColor", Defaults::dialogTextColor));

    // Accounts
    bool loop = true;
    int cnt=0;
    while (loop == true) {
        QString prefix = QString("Account%1_").arg(cnt);
        QString login = m_cg.readEntry(prefix+"Login", QString());
        // No (more?) accounts
        if (login.isEmpty()) {
            loop = false;
            break;
        }
        QVariantMap account;
        account["Login"] = login;
        account["Password"] = KStringHandler::obscure(m_cg.readEntry(prefix+"Password", QString()));
        account["Label"] = m_cg.readEntry(prefix+"Label", QString());
        account["Display"] = m_cg.readEntry(prefix+"Display", QString());
        account["DisableNotifications"] = m_cg.readEntry(prefix+"DisableNotifications", false);
        account["ExcludeFromTotalCount"] = m_cg.readEntry(prefix+"ExcludeFromTotalCount", false);

        addItemToList(account);

        ++cnt;
    }
} // initDialog()

void GmailNotifierAppletConfig::setAddModifyButtonEnabled()
{
    kDebug();
    bool enabled = (!ui.leLogin->text().isEmpty() &&
                    !ui.lePassword->text().isEmpty());
    ui.btnAddModify->setEnabled(enabled);
} // setAddModifyButtonEnabled()

void GmailNotifierAppletConfig::setUpDownButtonsEnabled()
{
    kDebug();
    bool enabled = (ui.listAccounts->count() >= 2 &&
                    ui.listAccounts->currentRow() != -1);

    bool upEnabled = (enabled && ui.listAccounts->currentRow() != 0);
    bool downEnabled = (enabled &&
                        ui.listAccounts->currentRow() + 1 != ui.listAccounts->count());

    ui.btnUp->setEnabled(upEnabled);
    ui.btnDown->setEnabled(downEnabled);
} // setUpDownButtonsEnabled()

void GmailNotifierAppletConfig::adaptAddModifyButtonLabel()
{
    kDebug();
    if (ui.leLogin->text().isEmpty() || ui.lePassword->text().isEmpty()) {
        return;
    }

    QString buttonText;
    QString toolTipText;
    if (accountPosition(ui.leLogin->text(), ui.leLabel->text()) != -1) {
        buttonText  = i18n("Modify");
        toolTipText = i18n("Modify this account settings");
        ui.btnAddModify->setIcon(KIcon("edit-rename"));
    } else {
        buttonText  = i18n("Add");
        toolTipText = i18n("Add this account to the list");
        ui.btnAddModify->setIcon(KIcon("list-add"));
    }

    if (buttonText != ui.btnAddModify->text().remove('&')) {
        ui.btnAddModify->setText(buttonText);
        ui.btnAddModify->setToolTip(toolTipText);
    }
} // adaptAddModifyButtonLabel()

QString GmailNotifierAppletConfig::listItemText(const QVariantMap &data)
{
    kDebug();
    QString itemText;
    QString label = (data["Label"].toString().isEmpty()) ? "inbox" : data["Label"].toString();
    if (data["Display"].toString().isEmpty()) {
        itemText = QString("%1/%2").arg(data["Login"].toString()).arg(label);
    } else {
        itemText = QString("%1").arg(data["Display"].toString());
    }

    return itemText;
} // listItemText()

void GmailNotifierAppletConfig::addItemToList(const QVariantMap &data)
{
    kDebug();
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(listItemText(data));
    item->setData(Qt::UserRole, data);
    ui.listAccounts->addItem(item);
//    ui.listAccounts->scrollToItem(item);
} // addItemToList()

int GmailNotifierAppletConfig::accountPosition(const QString &login, const QString &label)
{
    kDebug();
    int i=0;
    int pos=-1;
    while(i < ui.listAccounts->count()) {
        QListWidgetItem *item(ui.listAccounts->item(i));
        QVariantMap data(item->data(Qt::UserRole).toMap());
        if (login == data["Login"] && label == data["Label"]) {
            pos = i;
            break;
        }
        ++i;
    }

    return pos;
} // accountPosition()

void GmailNotifierAppletConfig::moveItem(const int &shift)
{
    kDebug();
    int pos = ui.listAccounts->currentRow();
    QListWidgetItem *item = ui.listAccounts->takeItem(pos);
    ui.listAccounts->insertItem(pos+shift, item);
    ui.listAccounts->setCurrentRow(pos+shift);
} // moveItem()

/*
QVariantMap GmailNotifierAppletConfig::QSM2QVM(const QMap<QString, QString> &data)
{
    kDebug();
    QVariantMap map;
    foreach(QString key, data.keys()) {
        map[key] = QVariant(data[key]);
    }

    return map;
} // QSM2QVM()
*/

/*
QMap<QString, QString> GmailNotifierAppletConfig::QVM2QSM(const QVariantMap &data)
{
    kDebug();
    QMap<QString, QString> map;
    foreach(QString key, data.keys()) {
        map[key] = data[key].toString();
    }

    return map;
} // QVM2QSM();
*/


#include "gmailnotifierappletconfig.moc"
