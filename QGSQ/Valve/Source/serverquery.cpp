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

bool ServerQuery::isRunning() const
{
    Q_D(const ServerQuery);
    return d->running;
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

void ServerQuery::getInfoAsync()
{
    Q_D(ServerQuery);
    d->serverInfoCon = QObject::connect(this, &QGSQ::Valve::Source::ServerQuery::gotRawInfo, [d](){d->processServerInfo();});
    getRawInfoAsync();
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

void ServerQuery::getRawInfoAsync()
{
    Q_D(ServerQuery);
    d->getRawDataAsync(QByteArrayLiteral("\xff\xff\xff\xffTSource Engine Query\0"));
}

QHash<QString,QString> ServerQuery::getRules() const
{
    QHash<QString,QString> rules;

    auto data = getRawRules();

    Q_D(const ServerQuery);
    rules = d->extractRules(data);

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

void ServerQuery::getRawRulesAsync()
{
    Q_D(ServerQuery);
    d->gotChallengeToProcessRulesChallengeCon = QObject::connect(this, &QGSQ::Valve::Source::ServerQuery::gotChallenge, [d](const QByteArray &challenge){d->processRulesChallenge(challenge);});
    d->getChallengeAsync('V');
}

void ServerQuery::getRulesAsync()
{
    Q_D(ServerQuery);
    d->rulesCon = QObject::connect(this, &QGSQ::Valve::Source::ServerQuery::gotRawRules, [d](const QByteArray &rules){d->processRules(rules);});
    getRawRulesAsync();
}

QList<Player*> ServerQuery::getPlayers(QObject *parent) const
{
    QList<Player*> lst;

    const auto data = getRawPlayers();

    Q_D(const ServerQuery);
    lst = d->extractPlayers(data, parent);

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

void ServerQuery::getRawPlayersAsync()
{
    Q_D(ServerQuery);
    d->gotChallengeToProcessPlayersChallengeCon = QObject::connect(this, &QGSQ::Valve::Source::ServerQuery::gotChallenge, [d](const QByteArray &challenge){d->processPlayersChallenge(challenge);});
    d->getChallengeAsync('U');
}

void ServerQuery::getPlayersAsync()
{
    Q_D(ServerQuery);
    d->playersCon = QObject::connect(this, &QGSQ::Valve::Source::ServerQuery::gotRawPlayers, [d](const QByteArray &players){d->processPlayers(players);});
    getRawPlayersAsync();
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

void ServerQueryPrivate::getChallengeAsync(char header)
{
    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + header + QByteArrayLiteral("\xff\xff\xff\xff");
    getRawDataAsync(request);
}

void ServerQueryPrivate::setRunning(bool _running)
{
    if (running != _running) {
        running = _running;
        Q_Q(ServerQuery);
        Q_EMIT q->runningChanged(running);
    }
}

void ServerQueryPrivate::getRawDataAsync(const QByteArray &request)
{
    responseComplete = false;
    rcvData.clear();
    if (!udp) {
        udp = new QUdpSocket(q_ptr);
        QObject::connect(udp, &QUdpSocket::readyRead, q_ptr, [this](){onUdpReadyRead();});
    }

    timeoutTimer = new QTimer(q_ptr);
    timeoutTimer->setInterval(timeout + 100);
    timeoutTimer->setTimerType(Qt::VeryCoarseTimer);
    QObject::connect(timeoutTimer, &QTimer::timeout, q_ptr, [this](){
        delete udp;
        udp = nullptr;
        timeoutTimer->deleteLater();
        timeoutTimer = nullptr;
        qCCritical(SQ, "Timeout within %ims while wating for reply.", timeout);
    });

    timeoutTimer->start();

    qCDebug(SQ, "Sending request \"%s\" to %s:%u.", request.toHex().constData(), qUtf8Printable(server.toString()), port);
    if (Q_UNLIKELY(udp->writeDatagram(request, server, port) != request.size())) {
        qCCritical(SQ, "Failed to send request to %s:%u.", qUtf8Printable(server.toString()), port);
    }
}

void ServerQueryPrivate::onUdpReadyRead()
{
    if (udp) {
        while (!responseComplete && udp->hasPendingDatagrams()) {
            const auto data = udp->receiveDatagram().data();
            const auto leftData = data.left(4);
            if (leftData == QByteArrayLiteral("\xff\xff\xff\xff")) {
                responseComplete = true;
                rcvData = data.mid(4);
                processRcvData();
            } else if (leftData == QByteArrayLiteral("\xfe\xff\xff\xff")) {

            } else {
//                invalidData = true;
            }
        }
        if (responseComplete) {
            delete timeoutTimer;
            timeoutTimer = nullptr;
        }
    }
}

void ServerQueryPrivate::processRcvData()
{
    const auto header = rcvData.at(0);
    Q_Q(ServerQuery);
    bool deleteSocket = true;
    if ((header == 'I') || (header == 'm')) {
        Q_EMIT q->gotRawInfo(rcvData);
    } else if (header == 'A') {
        deleteSocket = false;
        Q_EMIT q->gotChallenge(rcvData.mid(1, 4));
    } else if (header == 'E') {
        Q_EMIT q->gotRawRules(rcvData);
    } else if (header == 'D') {
        Q_EMIT q->gotRawPlayers(rcvData);
    }
    if (deleteSocket) {
        delete udp;
        udp = nullptr;
    }
}

void ServerQueryPrivate::processServerInfo()
{
    auto si = ServerInfo::fromRawData(rcvData, server.toString(), port);
    QObject::disconnect(serverInfoCon);
    Q_Q(ServerQuery);
    Q_EMIT q->gotInfo(si);
}

void ServerQueryPrivate::processRulesChallenge(const QByteArray &challenge)
{
    QObject::disconnect(gotChallengeToProcessRulesChallengeCon);
    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + 'V' + challenge;
    getRawDataAsync(request);
}

void ServerQueryPrivate::processRules(const QByteArray &data)
{
    const auto rules = extractRules(data);
    QObject::disconnect(rulesCon);
    Q_Q(ServerQuery);
    Q_EMIT q->gotRules(rules);
}

void ServerQueryPrivate::processPlayersChallenge(const QByteArray &challenge)
{
    QObject::disconnect(gotChallengeToProcessPlayersChallengeCon);
    const QByteArray request = QByteArrayLiteral("\xff\xff\xff\xff") + 'U' + challenge;
    getRawDataAsync(request);
}

void ServerQueryPrivate::processPlayers(const QByteArray &data)
{
    const auto players = extractPlayers(data);
    QObject::disconnect(playersCon);
    Q_Q(ServerQuery);
    Q_EMIT q->gotPlayers(players);
}

QHash<QString,QString> ServerQueryPrivate::extractRules(const QByteArray &data) const
{
    QHash<QString,QString> rules;

    if (Q_LIKELY(!data.isEmpty())) {
        auto _data = data;
        Response res(&_data);
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

QList<Player*> ServerQueryPrivate::extractPlayers(const QByteArray &data, QObject *parent) const
{
    QList<Player*> lst;

    if (!data.isEmpty()) {
        auto _data = data;
        Response res(&_data);
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
