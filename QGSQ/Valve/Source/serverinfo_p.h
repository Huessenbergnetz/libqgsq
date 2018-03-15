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

#ifndef QGSQ_VALVE_SOURCE_SERVERINFO_P_H
#define QGSQ_VALVE_SOURCE_SERVERINFO_P_H

#include "serverinfo.h"

namespace QGSQ {
namespace Valve {
namespace Source {

class ServerInfoPrivate
{
public:
    ServerInfoPrivate() {}

    virtual ~ServerInfoPrivate() {}

    void setAddress(const QString &_address);
    void setQueryPort(quint16 _queryPort);
    void setGoldSource(bool _goldSource);
    void setProtocol(quint8 _protocol);
    void setName(const QString &_name);
    void setMap(const QString &_map);
    void setFolder(const QString &_folder);
    void setGame(const QString &_game);
    void setAppId(quint16 _appId);
    void setPlayers(quint8 _players);
    void setMaxPlayers(quint8 _maxPlayers);
    void setBots(quint8 _bots);
    void setServerType(ServerInfo::Type _type);
    void setServerType(char ch);
    void setEnvironment(ServerInfo::Environment _env);
    void setEnvironment(char ch);
    void setVisibility(ServerInfo::Visibility _vis);
    void setVac(ServerInfo::VAC _vac);
    void setTheShipMode(ServerInfo::TheShipMode _mode);
    void setTheShipMode(quint8 _mode);
    void setTheShipWitnesses(quint8 _witnesses);
    void setTheShipDuartion(quint8 _duration);
    void setVersion(const QString &_version);
    void setGamePort(quint16 _port);
    void setSteamId(quint64 _id);
    void setSpecPort(quint16 _port);
    void setSpecName(const QString &_name);
    void setKeywords(const QStringList &_kws);
    void setGameId(quint64 _id);
    void setIsMod(bool _isMod);
    void setModLink(const QUrl &_url);
    void setModDownloadLink(const QUrl &_url);
    void setModVersion(quint32 _version);
    void setModSize(quint32 _size);
    void setModType(ServerInfo::ModType _type);
    void setModDll(ServerInfo::ModDLLUsage _dll);

    Q_DECLARE_PUBLIC(ServerInfo)
    quint64 steamId = 0;
    quint64 gameId = 0;
    QStringList keywords;
    QString address;
    QString name;
    QString map;
    QString folder;
    QString game;
    QString version;
    QString specName;
    QUrl modLink;
    QUrl modDownloadLink;
    ServerInfo *q_ptr = nullptr;
    quint32 modVersion = 0;
    quint32 modSize = 0;
    quint16 appId = 0;
    quint16 gamePort = 0;
    quint16 specPort = 0;
    quint16 queryPort = 0;
    quint8 protocol = 0;
    quint8 players = 0;
    quint8 maxPlayers = 0;
    quint8 bots = 0;
    quint8 theShipWitnesses = 0;
    quint8 theShipDuration = 0;
    ServerInfo::ModType modType = ServerInfo::SingleAndMultiplayerMod;
    ServerInfo::ModDLLUsage modDll = ServerInfo::UsesHalfLifeDll;
    ServerInfo::Type serverType = ServerInfo::Unspecified;
    ServerInfo::Environment environment = ServerInfo::Unknown;
    ServerInfo::Visibility visibility = ServerInfo::Public;
    ServerInfo::VAC vac = ServerInfo::Unsecured;
    ServerInfo::TheShipMode theShipMode = ServerInfo::UnknownTheShipMode;
    bool goldSource = false;
    bool isMod = false;

private:
    Q_DISABLE_COPY(ServerInfoPrivate)
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_SERVERINFO_P_H
