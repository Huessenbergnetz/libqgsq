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

#ifndef QGSQ_VALVE_SOURCE_SERVERINFO_H
#define QGSQ_VALVE_SOURCE_SERVERINFO_H

#include "qgsq_global.h"
#include <QObject>
#include <QJsonObject>
#include <QUrl>

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerInfoPrivate;

class QGSQ_LIBRARY ServerInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(quint16 queryPort READ queryPort CONSTANT)
    Q_PROPERTY(bool goldSource READ isGoldSource NOTIFY goldSourceChanged)
    Q_PROPERTY(quint8 protocol READ protocol NOTIFY protocolChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString map READ map NOTIFY mapChanged)
    Q_PROPERTY(QString folder READ folder NOTIFY folderChanged)
    Q_PROPERTY(QString game READ game NOTIFY gameChanged)
public:
    enum Type : quint8 {
        Unspecified,
        Dedicated,
        NonDedicated,
        SourceTVRelay
    };
    Q_ENUM(Type)

    enum Environment : quint8 {
        Unknown,
        Linux,
        Windows,
        Mac
    };
    Q_ENUM(Environment)

    enum Visibility : quint8 {
        Public = 0,
        Private = 1
    };
    Q_ENUM(Visibility)

    enum VAC : quint8 {
        Unsecured = 0,
        Secured = 1
    };
    Q_ENUM(VAC)

    enum TheShipMode : quint8 {
        Hunt,
        Elimination,
        Duel,
        Deathmatch,
        VIPTeam,
        TeamElimination,
        UnknownTheShipMode = 255
    };
    Q_ENUM(TheShipMode)

    explicit ServerInfo(const QString &address, quint16 queryPort = 27015, QObject *parent = nullptr);

    ~ServerInfo();

    QString address() const;

    quint16 queryPort() const;

    bool isGoldSource() const;

    quint8 protocol() const;

    QString name() const;

    QString map() const;

    QString folder() const;

    QString game() const;

    qint16 appId() const;

    quint8 players() const;

    quint8 maxPlayers() const;

    quint8 bots() const;

    Type serverType() const;

    Environment environment() const;

    Visibility visibility() const;

    VAC vac() const;

    TheShipMode theShipMode() const;

    quint8 theShipWitnesses() const;

    quint8 theShipDuration() const;

    QString version() const;

    quint16 gamePort() const;

    quint64 steamId() const;

    quint16 specPort() const;

    QString specName() const;

    QStringList keywords() const;

    quint64 gameId() const;

    bool isMod() const;

    QUrl modLink() const;

    QUrl modDownloadLink() const;

    quint32 modVersion() const;

    quint32 modSize() const;

    quint8 modType() const;

    quint8 modDll() const;

    QJsonObject toJson() const;

    int setRawData(const QByteArray &data);

    bool update(int timeout = 4000);

    static ServerInfo *fromRawData(const QByteArray &data, const QString &address, quint16 queryPort, QObject *parent = nullptr);

    static ServerInfo *get(const QString &address, quint16 queryPort, int timeout = 4000, QObject *parent = nullptr);

Q_SIGNALS:
    void goldSourceChanged(bool goldSource);
    void protocolChanged(quint8 protocol);
    void nameChanged(const QString &name);
    void mapChanged(const QString &map);
    void folderChanged(const QString &folder);
    void gameChanged(const QString &game);
    void appIdChanged(quint16 appId);
    void playersChanged(quint8 players);
    void maxPlayersChanged(quint8 maxPlayers);
    void botsChanged(quint8 bots);
    void serverTypeChanged(ServerInfo::Type type);
    void environmentChanged(ServerInfo::Environment environment);
    void visibilityChanged(ServerInfo::Visibility);
    void vacChanged(ServerInfo::VAC vac);
    void theShipModeChanged(ServerInfo::TheShipMode theShipMode);
    void theShipWitnessesChanged(quint8 theShipWitnesses);
    void theShipDurationChanged(quint8 theShipDuration);
    void versionChanged(const QString &version);
    void gamePortChanged(quint16 gamePort);
    void steamIdChanged(quint64 steamId);
    void specPortChanged(quint16 specPort);
    void specNameChanged(const QString &specName);
    void keywordsChanged(const QStringList &keywords);
    void gameIdChanged(quint64 gameId);
    void isModChanged(bool isMod);
    void modLinkChanged(const QUrl &modLink);
    void modDownloadLinkChanged(const QUrl &modDownloadLink);
    void modVersionChanged(quint32 modVersion);
    void modSizeChanged(quint32 modSize);
    void modTypeChanged(quint8 modType);
    void modDllChanged(quint8 modDll);

protected:
    const QScopedPointer<ServerInfoPrivate> d_ptr;
    ServerInfo(ServerInfoPrivate &dd, QObject *parent = nullptr);

private:
    Q_DISABLE_COPY(ServerInfo)
    Q_DECLARE_PRIVATE(ServerInfo)
};

}
}
}

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerInfo *serverInfo);

#endif // QGSQ_VALVE_SOURCE_SERVERINFO_H
