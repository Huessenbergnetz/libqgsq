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

class QHostAddress;

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerQueryPrivate;
class ServerInfo;
class Player;

class QGSQ_LIBRARY ServerQuery : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString server READ server WRITE setServer NOTIFY serverChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
public:
    explicit ServerQuery(QObject *parent = nullptr);

    ServerQuery(const QString &server, quint16 port, QObject *parent = nullptr);

    ServerQuery(const QHostAddress &server, quint16 port, QObject *parent = nullptr);

    ~ServerQuery();

    bool isValid() const;
    bool isRunning() const;

    QString server() const;
    void setServer(const QString &server);

    quint16 port() const;
    void setPort(quint16 port);

    int timeout() const;
    void setTimeout(int timeout);

    ServerInfo* getInfo(QObject *parent = nullptr) const;
    QByteArray getRawInfo() const;
    Q_INVOKABLE void getRawInfoAsync();
    Q_INVOKABLE void getInfoAsync();

    QHash<QString, QString> getRules() const;
    QByteArray getRawRules() const;
    Q_INVOKABLE void getRawRulesAsync();
    Q_INVOKABLE void getRulesAsync();

    QList<Player*> getPlayers(QObject *parent = nullptr) const;
    QByteArray getRawPlayers() const;
    Q_INVOKABLE void getRawPlayersAsync();
    Q_INVOKABLE void getPlayersAsync();

Q_SIGNALS:
    void serverChanged(const QString &server);
    void portChanged(quint16 port);
    void timeoutChanged(int timeout);
    void validChanged(bool isValid);
    void runningChanged(bool running);

    void gotChallenge(const QByteArray &challenge);
    void gotRawInfo(const QByteArray &serverInfo);
    void gotInfo(ServerInfo *serverInfo);
    void gotRawRules(const QByteArray &rules);
    void gotRules(const QHash<QString,QString> &rules);
    void gotRawPlayers(const QByteArray &rules);
    void gotPlayers(const QList<Player*> &players);

protected:
    const QScopedPointer<ServerQueryPrivate> d_ptr;
    ServerQuery(ServerQueryPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(ServerQuery)
    Q_DECLARE_PRIVATE(ServerQuery)
};

}
}
}

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerQuery *serverQuery);

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerQuery &serverQuery);

#endif // QGSQ_VALVE_SOURCE_SERVERQUERY_H
