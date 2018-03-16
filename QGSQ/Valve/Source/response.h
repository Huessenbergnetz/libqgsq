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
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(VSR)

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
    QString getString();
    QUrl getUrl();

    template<typename T> T get()
    {
        T ret;
        if (read(reinterpret_cast<char *>(&ret), sizeof(T)) != sizeof(T)) {
            qCWarning(VSR, "Failed to get %lu byte(s) from position %lli.", sizeof(T), pos());
            ret = 0;
        }
        return ret;
    }

    bool event(QEvent *event) override;
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_RESPONSE_H
