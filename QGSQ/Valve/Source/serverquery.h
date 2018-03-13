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

#ifndef QGSQ_VALVE_SOURCE_SERVERQUERY_H
#define QGSQ_VALVE_SOURCE_SERVERQUERY_H

#include "qgsq_global.h"
#include "serverinfo.h"
#include <QObject>
#include <QHostAddress>

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerQueryPrivate;
class ServerInfo;

class QGSQ_LIBRARY ServerQuery : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ServerQuery)
    Q_DISABLE_COPY(ServerQuery)
public:
    explicit ServerQuery(QObject *parent = nullptr);

    ServerQuery(const QHostAddress &server, quint16 port, QObject *parent = nullptr);

    ~ServerQuery();

    bool isValid() const;

    QHostAddress server() const;
    void setServer(const QHostAddress &server);

    quint16 port() const;
    void setPort(quint16 port);

    int timeout() const;
    void setTimeout(int timeout);

    ServerInfo* getInfo(QObject *parent = nullptr) const;
    QByteArray getRawInfo() const;

protected:
    const QScopedPointer<ServerQueryPrivate> d_ptr;
    ServerQuery(ServerQueryPrivate &dd, QObject *parent = nullptr);
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_SERVERQUERY_H
