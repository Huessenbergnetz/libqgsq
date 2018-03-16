/* libqgsq - Qt based library to query game servers
 * Copyright (C) 2018 Huessenbergnetz / Matthias Fehring
 * https://github.com/Huessenbergnetz/libqgsq
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "response.h"
#include <QDataStream>

Q_LOGGING_CATEGORY(VSR, "qgsq.valve.source.response")

using namespace QGSQ::Valve::Source;

Response::Response(QByteArray *byteArray, QObject *parent) : QBuffer(byteArray, parent)
{
    if (Q_UNLIKELY(!open(QBuffer::ReadOnly))) {
        qCCritical(VSR, "Failed to open buffer in readonly mode.");
    }
}

bool Response::checkHeader(const QByteArray &header)
{
    bool ok = false;

    const auto h = read(header.size());
    if (h == header) {
        ok = true;
    }

    return ok;
}

char Response::getCharacter()
{
    char ch;

    if (Q_UNLIKELY(!getChar(&ch))) {
        qCWarning(VSR, "Failed to get character from position %lli.", pos());
    }

    return ch;
}

QString Response::getString()
{
    QString str;

    const auto nullTermPos = static_cast<qint64>(buffer().indexOf('\0', pos()));
    if ((nullTermPos > -1) && (nullTermPos != pos())) {
        str = QString::fromUtf8(read(nullTermPos - pos() + 1));
    }

    return str;
}

QUrl Response::getUrl()
{
    QUrl url;

    url.setUrl(getString());

    return url;
}

bool Response::event(QEvent *event)
{
    return QBuffer::event(event);
}

#include "moc_response.cpp"
