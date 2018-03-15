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

#ifndef QGSQ_VALVE_SOURCE_SERVERQUERY_P_H
#define QGSQ_VALVE_SOURCE_SERVERQUERY_P_H

#include "serverquery.h"
#include <QHostAddress>

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerQueryPrivate
{
public:
    ServerQueryPrivate() {}

    virtual ~ServerQueryPrivate() {}

    QByteArray getRawData(const QByteArray &request) const;
    QByteArray getChallenge(char header) const;

    Q_DECLARE_PUBLIC(ServerQuery)
    ServerQuery *q_ptr = nullptr;
    QHostAddress server;
    int timeout = 4000;
    quint16 port = 0;

private:
    Q_DISABLE_COPY(ServerQueryPrivate)
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_SERVERQUERY_P_H
