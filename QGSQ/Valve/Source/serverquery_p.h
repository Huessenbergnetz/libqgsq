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
#include <QUdpSocket>
#include <QTimer>

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerQueryPrivate
{
public:
    ServerQueryPrivate() {}

    virtual ~ServerQueryPrivate() {}

    QByteArray getRawData(const QByteArray &request) const;
    void getRawDataAsync(const QByteArray &request);
    void onUdpReadyRead();
    QByteArray getChallenge(char header) const;
    void getChallengeAsync(char header);
    void setRunning(bool _running);
    void processRcvData();
    void processServerInfo();
    void processRulesChallenge(const QByteArray &challenge);
    void processRules(const QByteArray &data);
    QHash<QString,QString> extractRules(const QByteArray &data) const;
    void processPlayersChallenge(const QByteArray &challenge);
    void processPlayers(const QByteArray &data);
    QList<Player*> extractPlayers(const QByteArray &data, QObject *parent = nullptr) const;


    Q_DECLARE_PUBLIC(ServerQuery)
    ServerQuery *q_ptr = nullptr;
    QUdpSocket *udp = nullptr;
    QTimer *timeoutTimer = nullptr;
    QMetaObject::Connection serverInfoCon;
    QMetaObject::Connection gotChallengeToProcessRulesChallengeCon;
    QMetaObject::Connection rulesCon;
    QMetaObject::Connection gotChallengeToProcessPlayersChallengeCon;
    QMetaObject::Connection playersCon;
    QHostAddress server;
    QByteArray rcvData;
    int timeout = 4000;
    quint16 port = 0;
    bool running = false;
    bool responseComplete = false;

private:
    Q_DISABLE_COPY(ServerQueryPrivate)
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_SERVERQUERY_P_H
