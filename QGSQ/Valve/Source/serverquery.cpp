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
#include "player.h"
#include "response.h"
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(SQ, "qgsq.valve.source.serverquery")

using namespace QGSQ::Valve::Source;

ServerQuery::ServerQuery(QObject *parent) :
    QObject(parent), d_ptr(new ServerQueryPrivate)
{
    d_ptr->q_ptr = this;
}

ServerQuery::ServerQuery(const QString &server, quint16 port, QObject *parent) :
    QObject(parent), d_ptr(new ServerQueryPrivate)
{
    Q_D(ServerQuery);
    d->q_ptr = this;
    d->server = QHostAddress(server);
    d->port = port;
}

ServerQuery::ServerQuery(const QHostAddress &server, quint16 port, QObject *parent) :
    QObject(parent), d_ptr(new ServerQueryPrivate)
{
    Q_D(ServerQuery);
    d->q_ptr = this;
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

QString ServerQuery::server() const
{
    Q_D(const ServerQuery);
    return d->server.toString();
}

void ServerQuery::setServer(const QString &server)
{
    Q_D(ServerQuery);
    const QHostAddress _server(server);
    if (d->server != _server) {
        d->server = _server;
        Q_EMIT serverChanged(server);
        Q_EMIT validChanged(isValid());
    }
}

quint16 ServerQuery::port() const
{
    Q_D(const ServerQuery);
    return d->port;
}

void ServerQuery::setPort(quint16 port)
{
    Q_D(ServerQuery);
    if (d->port != port) {
        d->port = port;
        Q_EMIT portChanged(port);
        Q_EMIT validChanged(isValid());
    }
}

int ServerQuery::timeout() const
{
    Q_D(const ServerQuery);
    return d->timeout;
}

void ServerQuery::setTimeout(int timeout)
{
    Q_D(ServerQuery);
    if (d->timeout != timeout) {
        d->timeout = timeout;
        Q_EMIT timeoutChanged(timeout);
    }
}

ServerInfo *ServerQuery::getInfo(QObject *parent) const
{
    ServerInfo *si = nullptr;
    Q_D(const ServerQuery);

    const QByteArray ba = getRawInfo();
    if (Q_LIKELY(!ba.isEmpty())) {
        si = new ServerInfo(d->server.toString(), d->port, parent);
        si->setRawData(ba);
    }

    return si;
}

QByteArray ServerQuery::getRawInfo() const
{
    QByteArray ba;

    Q_D(const ServerQuery);

    qCInfo(SQ, "Start requesting server info (A2S_INFO) from %s:%u.", qUtf8Printable(d->server.toString()), d->port);

    const auto data = d->getRawData(QByteArrayLiteral("\xff\xff\xff\xffTSource Engine Query\0"));

    if (Q_UNLIKELY(data.isEmpty() || (!data.startsWith('I') || data.startsWith('m')))) {
        qCCritical(SQ, "Received invalid response to A2S_INFO query.");
        return ba;
    }

    qCInfo(SQ, "Finished requesting server info (A2S_INFO from %s:%u", qUtf8Printable(d->server.toString()), d->port);

    ba = data;

    return ba;
}

QHash<QString,QString> ServerQuery::getRules() const
{
    QHash<QString,QString> rules;

    auto data = getRawRules();

    if (Q_LIKELY(!data.isEmpty())) {
        Response res(&data);
        res.getCharacter(); // go over header
        const auto rulesCount = res.getUShort();
        if (rulesCount > 0) {
            for (int i = 0; i < rulesCount; ++i) {
                const QString name = res.getString();
                const QString value = res.getString();
                if (!name.isEmpty() && !value.isEmpty()) {
                    rules.insert(name, value);
                }
            }
        }
    }

    return rules;
}

QByteArray ServerQuery::getRawRules() const
{
    QByteArray ba;

    Q_D(const ServerQuery);

    qCInfo(SQ, "Start requesting server rules (A2S_RULES) from %s:%u.", qUtf8Printable(d->server.toString()), d->port);

    const auto challenge = d->getChallenge('V');

    if (Q_UNLIKELY(challenge.isEmpty())) {
        return ba;
    }

    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + 'V' + challenge;

    const auto data = d->getRawData(request);

    if (Q_UNLIKELY(data.isEmpty() || !data.startsWith('E'))) {
        qCCritical(SQ, "Received invalid response to A2S_RULES query.");
        return ba;
    }

    qCInfo(SQ, "Finished requesting server rules (A2S_RULES) from %s:%u", qUtf8Printable(d->server.toString()), d->port);

    ba = data;

    return ba;
}

QList<Player*> ServerQuery::getPlayers(QObject *parent) const
{
    QList<Player*> lst;

    auto data = getRawPlayers();

    if (!data.isEmpty()) {
        Response res(&data);
        res.getCharacter(); // go over header
        const auto count = res.getUByte();
        if (count > 0) {
            lst.reserve(count);
            for (int i = 0; i < count; ++i) {
                res.getUByte(); // index of player chunk
                const auto name = res.getString();
                const auto score = res.getLong();
                const auto duration = res.getFloat();
                lst.append(new Player(name, score, duration, parent));
            }
        }
    }

    return lst;
}

QByteArray ServerQuery::getRawPlayers() const
{
    QByteArray ba;

    Q_D(const ServerQuery);

    qCInfo(SQ, "Start requesting players (A2S_PLAYER) from %s:%u.", qUtf8Printable(d->server.toString()), d->port);

    const auto challenge = d->getChallenge('U');

    if (Q_UNLIKELY(challenge.isEmpty())) {
        return ba;
    }

    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + 'U' + challenge;

    const auto data = d->getRawData(request);

    if (Q_UNLIKELY(data.isEmpty() || !data.startsWith('D'))) {
        qCCritical(SQ, "Received invalid resposne to A2S_PLAYER query.");
        return ba;
    }

    qCInfo(SQ, "Finished requesting players (A2S_PLAYER) from %s:%u.", qUtf8Printable(d->server.toString()), d->port);

    ba = data;

    return ba;
}

QByteArray ServerQueryPrivate::getRawData(const QByteArray &request) const
{
    QByteArray ba;

    if (server.isNull()) {
        qCCritical(SQ, "Failed to send request, invalid host address.");
        return ba;
    }

    if (!port) {
        qCCritical(SQ, "Failed to send request, invalid query port.");
        return ba;
    }

    QUdpSocket udp;
    auto responseComplete = false;
    auto noTimeout = true;
    auto invalidData = false;

    qCDebug(SQ, "Sending request \"%s\" to %s:%u.", request.toHex().constData(), qUtf8Printable(server.toString()), port);
    if (Q_UNLIKELY(udp.writeDatagram(request, server, port) != request.size())) {
        qCCritical(SQ, "Failed to send request to %s:%u.", qUtf8Printable(server.toString()), port);
        return ba;
    }

    QByteArray rcvData;
    while (!responseComplete && noTimeout && !invalidData) {
        noTimeout = udp.waitForReadyRead(timeout);
        if (noTimeout) {
            const auto data = udp.receiveDatagram().data();
            qCDebug(SQ) << "Received data:" << data;
            const auto leftData = data.left(4);
            if (leftData == QByteArrayLiteral("\xff\xff\xff\xff")) {
                responseComplete = true;
                rcvData = data.mid(4);
            } else if (leftData == QByteArrayLiteral("\xfe\xff\xff\xff")) {

            } else {
                invalidData = true;
            }
        }
    }

    if (Q_UNLIKELY(invalidData)) {
        qCCritical(SQ, "Received invalid data.");
        return ba;
    }

    if (Q_UNLIKELY(!noTimeout)) {
        qCCritical(SQ, "Timeout within %ims while wating for reply.", timeout);
        return ba;
    }

    ba = rcvData;

    return ba;
}

QByteArray ServerQueryPrivate::getChallenge(char header) const
{
    QByteArray ba;

    qCInfo(SQ, "Requesting challenge with header '%c' from %s:%u.", header, qUtf8Printable(server.toString()), port);

    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + header + QByteArrayLiteral("\xff\xff\xff\xff");

    const auto data = getRawData(request);

    if (data.isEmpty() || (data.size() != 5) || (data.at(0) != 'A')) {
        qCCritical(SQ, "Received invalid response to challenge request.");
        return ba;
    }

    ba = data.mid(1, 4);

    return ba;
}

void ServerQueryPrivate::setRunning(bool _running)
{
    if (running != _running) {
        running = _running;
        Q_Q(ServerQuery);
        Q_EMIT q->runningChanged(running);
    }
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerQuery *serverQuery)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver);
    if (!serverQuery) {
        return dbg << QGSQ::Valve::Source::ServerQuery::staticMetaObject.className() << "(0x0)";
    }
    dbg.nospace() << serverQuery->metaObject()->className() << '(' << (const void *)serverQuery;
    dbg << ", Server: " << serverQuery->server();
    dbg << ", Port: " << serverQuery->port();
    dbg << ", Timeout: " << serverQuery->timeout() << "ms";
    dbg << ')';
    return dbg.maybeSpace();
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerQuery &serverQuery)
{
    return dbg << &serverQuery;
}

#include "moc_serverquery.cpp"
