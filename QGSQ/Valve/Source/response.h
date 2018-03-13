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

#ifndef QGSQ_VALVE_SOURCE_RESPONSE_H
#define QGSQ_VALVE_SOURCE_RESPONSE_H

#include <QBuffer>
#include <QUrl>

namespace QGSQ {
namespace Valve {
namespace Source {

class Response : public QBuffer
{
    Q_OBJECT
public:
    explicit Response(QByteArray *byteArray, QObject *parent = nullptr);

    bool checkHeader(const QByteArray &header = QByteArrayLiteral("\xff\xff\xff\xff"));
    char getCharacter();
    quint8 getUByte();
    quint16 getUShort();
    quint32 getULong();
    quint64 getULongLong();
    QString getString();
    QUrl getUrl();
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_RESPONSE_H
