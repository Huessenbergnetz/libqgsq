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
#include <ostream>

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerInfoPrivate;

class QGSQ_LIBRARY ServerInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address NOTIFY addressChanged)
    Q_PROPERTY(quint16 queryPort READ queryPort NOTIFY queryPortChanged)
    Q_PROPERTY(bool goldSource READ isGoldSource NOTIFY goldSourceChanged)
    Q_PROPERTY(quint8 protocol READ protocol NOTIFY protocolChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString map READ map NOTIFY mapChanged)
    Q_PROPERTY(QString folder READ folder NOTIFY folderChanged)
    Q_PROPERTY(QString game READ game NOTIFY gameChanged)
    Q_PROPERTY(quint16 appId READ appId NOTIFY appIdChanged)
    Q_PROPERTY(quint8 players READ players NOTIFY playersChanged)
    Q_PROPERTY(quint8 maxPlayers READ maxPlayers NOTIFY maxPlayersChanged)
    Q_PROPERTY(quint8 bots READ bots NOTIFY botsChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::Type serverType READ serverType NOTIFY serverTypeChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::Environment environment READ environment NOTIFY environmentChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::Visibility visibility READ visibility NOTIFY visibilityChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::VAC vac READ vac NOTIFY vacChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::TheShipMode theShipMode READ theShipMode NOTIFY theShipModeChanged)
    Q_PROPERTY(quint8 theShipWitnesses READ theShipWitnesses NOTIFY theShipWitnessesChanged)
    Q_PROPERTY(quint8 theShipDuration READ theShipDuration NOTIFY theShipDurationChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(quint16 gamePort READ gamePort NOTIFY gamePortChanged)
    Q_PROPERTY(quint64 steamId READ steamId NOTIFY steamIdChanged)
    Q_PROPERTY(quint16 specPort READ specPort NOTIFY specPortChanged)
    Q_PROPERTY(QString specName READ specName NOTIFY specNameChanged)
    Q_PROPERTY(QStringList keywords READ keywords NOTIFY keywordsChanged)
    Q_PROPERTY(quint64 gameId READ gameId NOTIFY gameIdChanged)
    Q_PROPERTY(QUrl storeLink READ storeLink NOTIFY storeLinkChanged)
    Q_PROPERTY(bool isMod READ isMod NOTIFY isModChanged)
    Q_PROPERTY(QUrl modLink READ modLink NOTIFY modLinkChanged)
    Q_PROPERTY(QUrl modDownloadLink READ modDownloadLink NOTIFY modDownloadLinkChanged)
    Q_PROPERTY(quint32 modVersion READ modVersion NOTIFY modVersionChanged)
    Q_PROPERTY(quint32 modSize READ modSize NOTIFY modSizeChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::ModType modType READ modType NOTIFY modTypeChanged)
    Q_PROPERTY(QGSQ::Valve::Source::ServerInfo::ModDLLUsage modDll READ modDll NOTIFY modDllChanged)
public:
    enum Type : quint8 {
        Unspecified     = 0,
        Dedicated       = 100,
        NonDedicated    = 108,
        SourceTVRelay   = 112
    };
    Q_ENUM(Type)

    enum Environment : quint8 {
        Unknown = 0,
        Linux   = 108,
        Mac     = 109,
        Windows = 119
    };
    Q_ENUM(Environment)

    enum Visibility : quint8 {
        Public  = 0,
        Private = 1
    };
    Q_ENUM(Visibility)

    enum VAC : quint8 {
        Unsecured   = 0,
        Secured     = 1
    };
    Q_ENUM(VAC)

    enum TheShipMode : quint8 {
        Hunt                = 0,
        Elimination         = 1,
        Duel                = 2,
        Deathmatch          = 3,
        VIPTeam             = 4,
        TeamElimination     = 5,
        UnknownTheShipMode  = 255
    };
    Q_ENUM(TheShipMode)

    enum ModType : quint8 {
        SingleAndMultiplayerMod = 0,
        MultiplayerOnlyMod      = 1
    };
    Q_ENUM(ModType)

    enum ModDLLUsage : quint8 {
        UsesHalfLifeDll = 0,
        UsesOwnDll      = 1
    };
    Q_ENUM(ModDLLUsage)

    explicit ServerInfo(QObject *parent = nullptr);

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

    quint16 appId() const;

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

    QUrl storeLink() const;

    bool isMod() const;

    QUrl modLink() const;

    QUrl modDownloadLink() const;

    quint32 modVersion() const;

    quint32 modSize() const;

    ModType modType() const;

    ModDLLUsage modDll() const;

    QJsonObject toJson() const;

    int setRawData(const QByteArray &data);

    Q_INVOKABLE bool update(int timeout = 4000);

    Q_INVOKABLE void updateAsync(int timeout = 4000);

    Q_INVOKABLE bool query(const QString &address, quint16 queryPort = 27015, int timeout = 4000);

    Q_INVOKABLE void queryAsync(const QString &address, quint16 queryPort = 27015, int timeout = 4000);

    bool event(QEvent *event) override;

    static ServerInfo *fromRawData(const QByteArray &data, const QString &address, quint16 queryPort, QObject *parent = nullptr);

    static ServerInfo *get(const QString &address, quint16 queryPort, int timeout = 4000, QObject *parent = nullptr);

Q_SIGNALS:
    void addressChanged(const QString &address);
    void queryPortChanged(quint16 queryPort);
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
    void storeLinkChanged(const QUrl &storeLink);
    void isModChanged(bool isMod);
    void modLinkChanged(const QUrl &modLink);
    void modDownloadLinkChanged(const QUrl &modDownloadLink);
    void modVersionChanged(quint32 modVersion);
    void modSizeChanged(quint32 modSize);
    void modTypeChanged(ServerInfo::ModType modType);
    void modDllChanged(ServerInfo::ModDLLUsage modDll);

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

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerInfo &serverInfo);

QGSQ_LIBRARY std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::ServerInfo *serverInfo);

QGSQ_LIBRARY std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::ServerInfo &serverInfo);

#endif // QGSQ_VALVE_SOURCE_SERVERINFO_H
