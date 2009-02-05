/*
** Copyright (C) 2008-2009 Gilles CHAUVIN <gcnweb+gmailnotifier@gmail.com>
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
#include "gmailatomfeedparser.h"
#include "gmailnotifiersource.h"
// QtXml
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNode>
// QtCore
#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>


/*
** public:
*/
Plasma::DataEngine::Data GmailAtomFeedParser::parseFeed(const QByteArray &feed, GmailNotifierSource * const source)
{
    Plasma::DataEngine::Data data;
    QList<QVariant>          entries;

    QDomDocument doc("GmailATOMFeed");
    doc.setContent(feed);

    QDomElement de = doc.documentElement();
    QDomNode dn = de.firstChild();
    while(!dn.isNull())
    {
        if (dn.isElement()) {
            QDomElement de = dn.toElement();
            QString tag(de.tagName().toLower());

            if (tag == "title") {
                source->setData("title", de.text());
            }
            else if (tag == "tagline") {
                source->setData("tagline", de.text());
            }
            else if (tag == "fullcount") {
                source->setData("fullcount", de.text().toInt());
            }
            else if (tag == "link") {
                source->setData("link", QUrl(de.attribute("href"), QUrl::StrictMode));
            }
            else if (tag == "modified") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                source->setData("modified", dt);
            }
            else if (tag == "entry") {
                entries << parseEntry(dn);
            }

            dn = dn.nextSibling();
        }
    }

    if (entries.count() > 0) {
        source->setData("entries", entries);
    }

    return data;
} // parseFeed()

QVariantMap GmailAtomFeedParser::parseEntry(const QDomNode &node)
{
    QVariantMap entry;

    QDomNode dn = node.firstChild();
    while(!dn.isNull())
    {
        if (dn.isElement()) {
            QDomElement de = dn.toElement();
            QString tag(de.tagName().toLower());

            if (tag == "title") {
                entry["title"] = de.text();
            }
            else if (tag == "summary") {
                entry["summary"] = de.text();
            }
            else if (tag == "link") {
                entry["link"] = QUrl(de.attribute("href"), QUrl::StrictMode);
            }
            else if (tag == "modified") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                entry["modified"] = dt;
            }
            else if (tag == "issued") {
                QDateTime dt = QDateTime::fromString(de.text(), Qt::ISODate);
                dt.setTimeSpec(Qt::UTC);
                entry["issued"] = dt;
            }
            else if (tag == "id") {
                entry["id"] = de.text();
            }
            else if (tag == "author") {
                QDomNode dn = de.firstChild();
                while(!dn.isNull())
                {
                    if (dn.isElement()) {
                        QDomElement de = dn.toElement();
                        QString tagName(de.tagName().toLower());

                        if (tagName == "name") {
                            entry["author_name"] = de.text();
                        }
                        else if (tagName == "email") {
                            entry["author_email"] = de.text();
                        }
                    }

                    dn = dn.nextSibling();
                }
            } 
        }

        dn = dn.nextSibling();
    }

    return entry;
} // parseEntry()
