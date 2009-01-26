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
#include "gmailnotifierappletconfig.h"

// KDE
#include <KDE/KDebug>
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

    // Display logo
    ui.cbDisplayLogo->setChecked(m_cg.readEntry("DisplayLogo", true));

    // Polling Interval
    ui.spinPollingInterval->setValue(m_cg.readEntry("PollingInterval", 5));

    // Background
    QStringList backgroundHints;
    //backgroundHints << "Default" << "Standard" << "Translucent" << "Shadowed" << "None";
    backgroundHints << "Standard" << "Translucent";
    ui.comboBackground->addItems(backgroundHints);
    
    int pos = ui.comboBackground->findText(m_cg.readEntry("Background", "Standard"), Qt::MatchExactly);
    ui.comboBackground->setCurrentIndex(pos);

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
        QMap<QString, QString> account;
        account["Login"] = login;
        account["Password"] = KStringHandler::obscure(m_cg.readEntry(prefix+"Password", QString()));
        account["Label"] = m_cg.readEntry(prefix+"Label", QString());
        account["Display"] = m_cg.readEntry(prefix+"Display", QString());

        addItemToList(account);

        ++cnt;
    }
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
        cnt = i;
    }

    ++cnt;

    // Make sure we delete any remaining previously created entries
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
    QMap<QString, QString> data;
    data["Login"]    = ui.leLogin->text();
    data["Password"] = ui.lePassword->text();
    data["Label"]    = ui.leLabel->text();
    data["Display"]  = ui.leDisplay->text();

    int pos = accountPosition(ui.leLogin->text(), ui.leLabel->text());
    if (pos == -1) {
        // Add
        addItemToList(data);
        setUpDownButtonsEnabled();
    } else {
        // Modify
        QListWidgetItem *item = ui.listAccounts->item(pos);
        item->setText(listItemText(data));
        item->setData(Qt::UserRole, QSM2QVM(data));
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

    ui.btnDelete->setEnabled(true);
    setUpDownButtonsEnabled();
    adaptAddModifyButtonLabel();
} // on_listAccounts_itemPressed()


/*
** private
*/
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
    if (accountPosition(ui.leLogin->text(), ui.leLabel->text()) != -1) {
        buttonText = i18n("Modify");
    } else {
        buttonText = i18n("Add");
    }

    if (buttonText != ui.btnAddModify->text().remove('&')) {
        ui.btnAddModify->setText(buttonText);
    }
} // adaptAddModifyButtonLabel()

QString GmailNotifierAppletConfig::listItemText(const QMap<QString, QString> &data)
{
    kDebug();
    QString itemText;
    QString label = (data["Label"].isEmpty()) ? "inbox" : data["Label"];
    if (data["Display"].isEmpty()) {
        itemText = QString("%1/%2").arg(data["Login"]).arg(label);
    } else {
        itemText = QString("%1").arg(data["Display"]);
    }

    return itemText;
} // listItemText()

void GmailNotifierAppletConfig::addItemToList(const QMap<QString, QString> &data)
{
    kDebug();
    QListWidgetItem *item = new QListWidgetItem();
    item->setText(listItemText(data));
    item->setData(Qt::UserRole, QSM2QVM(data));
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

QVariantMap GmailNotifierAppletConfig::QSM2QVM(const QMap<QString, QString> &data)
{
    kDebug();
    QVariantMap map;
    foreach(QString key, data.keys()) {
        map[key] = QVariant(data[key]);
    }

    return map;
} // QSM2QVM()

QMap<QString, QString> GmailNotifierAppletConfig::QVM2QSM(const QVariantMap &data)
{
    kDebug();
    QMap<QString, QString> map;
    foreach(QString key, data.keys()) {
        map[key] = data[key].toString();
    }

    return map;
} // QVM2QSM();


#include "gmailnotifierappletconfig.moc"
