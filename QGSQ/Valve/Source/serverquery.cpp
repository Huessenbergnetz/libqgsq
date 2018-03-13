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

#include "serverquery_p.h"
#include "serverinfo.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(SQ, "qgsq.valve.source.serverquery")

using namespace QGSQ::Valve::Source;

ServerQuery::ServerQuery(QObject *parent) :
    QObject(parent), d_ptr(new ServerQueryPrivate)
{

}

ServerQuery::ServerQuery(const QHostAddress &server, quint16 port, QObject *parent) :
    QObject(parent), d_ptr(new ServerQueryPrivate)
{
    Q_D(ServerQuery);
    d->server = server;
    d->port = port;
}

ServerQuery::ServerQuery(ServerQueryPrivate &dd, QObject *parent) :
    QObject(parent), d_ptr(&dd)
{

}

ServerQuery::~ServerQuery()
{

}

bool ServerQuery::isValid() const
{
    Q_D(const ServerQuery);
    return (!d->server.isNull() && (d->port > 0));
}

QHostAddress ServerQuery::server() const
{
    Q_D(const ServerQuery);
    return d->server;
}

void ServerQuery::setServer(const QHostAddress &server)
{
    Q_D(ServerQuery);
    d->server = server;
}

quint16 ServerQuery::port() const
{
    Q_D(const ServerQuery);
    return d->port;
}

void ServerQuery::setPort(quint16 port)
{
    Q_D(ServerQuery);
    d->port = port;
}

int ServerQuery::timeout() const
{
    Q_D(const ServerQuery);
    return d->timeout;
}

void ServerQuery::setTimeout(int timeout)
{
    Q_D(ServerQuery);
    d->timeout = timeout;
}

ServerInfo *ServerQuery::getInfo(QObject *parent) const
{
    ServerInfo *si = nullptr;
    Q_D(const ServerQuery);

    const QByteArray ba = getRawInfo();
    if (!ba.isEmpty()) {
        si = new ServerInfo(d->server.toString(), d->port, parent);
        si->setRawData(ba);
    }

    return si;
}

QByteArray ServerQuery::getRawInfo() const
{
    QByteArray ba;

    Q_D(const ServerQuery);
    QUdpSocket udp;

    if (d->server.isNull()) {
        qCCritical(SQ, "Failed to send request, invalid host address.");
        return ba;
    }

    if (!d->port) {
        qCCritical(SQ, "Failed to send request, invalid query port.");
        return ba;
    }

    const QByteArray reqData = QByteArrayLiteral("\xff\xff\xff\xffTSource Engine Query\0");
    if (Q_UNLIKELY(udp.writeDatagram(reqData, d->server, d->port) != reqData.size())) {
        qCCritical(SQ, "Failed to send request to %s:%u.", qUtf8Printable(d->server.toString()), d->port);
        return ba;
    }

    if (Q_UNLIKELY(!udp.waitForReadyRead(d->timeout))) {
        qCCritical(SQ, "Timeout within %ims while wating for reply.", d->timeout);
        return ba;
    }

    const auto data = udp.receiveDatagram().data();
    if (Q_UNLIKELY(data.isEmpty() || !(data.startsWith(QByteArrayLiteral("\xff\xff\xff\xffI")) || data.startsWith(QByteArrayLiteral("\xff\xff\xff\xffm"))))) {
        qCCritical(SQ, "Received invalid response.");
        return ba;
    }

    ba = data;

    qCDebug(SQ) << "Received data:" << ba;

    return ba;
}

#include "moc_serverquery.cpp"
