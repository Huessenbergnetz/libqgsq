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
#include <QLoggingCategory>
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

    const QByteArray h = read(header.size());
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

quint8 Response::getUByte()
{
    quint8 byte = 0;
    char ch;
    if (Q_LIKELY(getChar(&ch))) {
        byte = static_cast<quint8>(ch);
    } else {
        qCWarning(VSR, "Failed to get byte (quint8) from position %lli.", pos());
    }

    return byte;
}

quint16 Response::getUShort()
{
    quint16 ret = 0;

    char ch1;
    char ch2;

    if (Q_LIKELY(getChar(&ch1))) {
        if (Q_LIKELY(getChar(&ch2))) {
            ret = static_cast<quint16>(ch1) | static_cast<quint16>(ch2) << 8;
        } else {
            qCWarning(VSR, "Failed to get second char of a short at position %lli.", pos());
        }
    } else {
        qCWarning(VSR, "Failed to get first char of a short at position %lli.", pos());
    }

    return ret;
}

qint32 Response::getLong()
{
    qint32 ret = 0;

    QDataStream ds(this);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds >> ret;

    return ret;
}

quint32 Response::getULong()
{
    quint32 ret = 0;

    QDataStream ds(this);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds >> ret;

    return ret;
}

quint64 Response::getULongLong()
{
    quint64 ret = 0;

    QDataStream ds(this);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
    ds >> ret;

    return ret;
}

float Response::getFloat()
{
    float ret = 0.0f;

    QDataStream ds(this);
    ds.setByteOrder(QDataStream::LittleEndian);
    ds >> ret;

    return ret;
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
