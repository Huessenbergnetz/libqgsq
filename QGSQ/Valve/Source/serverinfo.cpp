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

#include "serverinfo_p.h"
#include "response.h"
#include "serverquery.h"

#include <QLoggingCategory>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>

Q_LOGGING_CATEGORY(SI, "qgsq.valve.source.serverinfo")

using namespace QGSQ::Valve::Source;

ServerInfo::ServerInfo(QObject *parent) : QObject(parent), d_ptr(new ServerInfoPrivate)
{
    d_ptr->q_ptr = this;
}

ServerInfo::ServerInfo(const QString &address, quint16 queryPort, QObject *parent) : QObject(parent), d_ptr(new ServerInfoPrivate)
{
    Q_D(ServerInfo);
    d->q_ptr = this;
    d->address = address;
    d->queryPort = queryPort;
}

ServerInfo::ServerInfo(ServerInfoPrivate &dd, QObject *parent) : QObject(parent), d_ptr(&dd)
{

}

ServerInfo::~ServerInfo()
{

}

QString ServerInfo::address() const
{
    Q_D(const ServerInfo);
    return d->address;
}

quint16 ServerInfo::queryPort() const
{
    Q_D(const ServerInfo);
    return d->queryPort;
}

bool ServerInfo::isGoldSource() const
{
    Q_D(const ServerInfo);
    return d->goldSource;
}

quint8 ServerInfo::protocol() const
{
    Q_D(const ServerInfo);
    return d->protocol;
}

QString ServerInfo::name() const
{
    Q_D(const ServerInfo);
    return d->name;
}

QString ServerInfo::map() const
{
    Q_D(const ServerInfo);
    return d->map;
}

QString ServerInfo::folder() const
{
    Q_D(const ServerInfo);
    return d->folder;
}

QString ServerInfo::game() const
{
    Q_D(const ServerInfo);
    return d->game;
}

quint16 ServerInfo::appId() const
{
    Q_D(const ServerInfo);
    return d->appId;
}

quint8 ServerInfo::players() const
{
    Q_D(const ServerInfo);
    return d->players;
}

quint8 ServerInfo::maxPlayers() const
{
    Q_D(const ServerInfo);
    return d->maxPlayers;
}

quint8 ServerInfo::bots() const
{
    Q_D(const ServerInfo);
    return d->bots;
}

QGSQ::Valve::Source::ServerInfo::Type ServerInfo::serverType() const
{
    Q_D(const ServerInfo);
    return d->serverType;
}

QGSQ::Valve::Source::ServerInfo::Environment ServerInfo::environment() const
{
    Q_D(const ServerInfo);
    return d->environment;
}

QGSQ::Valve::Source::ServerInfo::Visibility ServerInfo::visibility() const
{
    Q_D(const ServerInfo);
    return d->visibility;
}

QGSQ::Valve::Source::ServerInfo::VAC ServerInfo::vac() const
{
    Q_D(const ServerInfo);
    return d->vac;
}

QGSQ::Valve::Source::ServerInfo::TheShipMode ServerInfo::theShipMode() const
{
    Q_D(const ServerInfo);
    return d->theShipMode;
}

quint8 ServerInfo::theShipWitnesses() const
{
    Q_D(const ServerInfo);
    return d->theShipWitnesses;
}

quint8 ServerInfo::theShipDuration() const
{
    Q_D(const ServerInfo);
    return d->theShipDuration;
}

QString ServerInfo::version() const
{
    Q_D(const ServerInfo);
    return d->version;
}

quint16 ServerInfo::gamePort() const
{
    Q_D(const ServerInfo);
    return d->gamePort;
}

quint64 ServerInfo::steamId() const
{
    Q_D(const ServerInfo);
    return d->steamId;
}

quint16 ServerInfo::specPort() const
{
    Q_D(const ServerInfo);
    return d->specPort;
}

QString ServerInfo::specName() const
{
    Q_D(const ServerInfo);
    return d->specName;
}

QStringList ServerInfo::keywords() const
{
    Q_D(const ServerInfo);
    return d->keywords;
}

quint64 ServerInfo::gameId() const
{
    Q_D(const ServerInfo);
    return d->gameId;
}

QUrl ServerInfo::storeLink() const
{
    QUrl url;
    Q_D(const ServerInfo);
    const auto id = (d->appId > 0) ? d->appId : d->gameId;
    if (id > 0) {
        url.setScheme(QStringLiteral("http"));
        url.setHost(QStringLiteral("store.steampowered.com"));
        url.setPath(QLatin1String("/app/") + QString::number(id));
    }
    return url;
}

bool ServerInfo::isMod() const
{
    Q_D(const ServerInfo);
    return d->isMod;
}

QUrl ServerInfo::modLink() const
{
    Q_D(const ServerInfo);
    return d->modLink;
}

QUrl ServerInfo::modDownloadLink() const
{
    Q_D(const ServerInfo);
    return d->modDownloadLink;
}

quint32 ServerInfo::modVersion() const
{
    Q_D(const ServerInfo);
    return d->modVersion;
}

quint32 ServerInfo::modSize() const
{
    Q_D(const ServerInfo);
    return d->modSize;
}

QGSQ::Valve::Source::ServerInfo::ModType ServerInfo::modType() const
{
    Q_D(const ServerInfo);
    return d->modType;
}

QGSQ::Valve::Source::ServerInfo::ModDLLUsage ServerInfo::modDll() const
{
    Q_D(const ServerInfo);
    return d->modDll;
}

QJsonObject ServerInfo::toJson() const
{
    QJsonObject o;

    Q_D(const ServerInfo);
    o.insert(QStringLiteral("address"), QJsonValue(d->address));
    o.insert(QStringLiteral("queryPort"), QJsonValue(static_cast<int>(d->queryPort)));
    o.insert(QStringLiteral("isGoldSource"), QJsonValue(d->goldSource));
    o.insert(QStringLiteral("protocol"), QJsonValue(static_cast<int>(d->protocol)));
    o.insert(QStringLiteral("name"), QJsonValue(d->name));
    o.insert(QStringLiteral("map"), QJsonValue(d->map));
    o.insert(QStringLiteral("folder"), QJsonValue(d->folder));
    o.insert(QStringLiteral("game"), QJsonValue(d->game));
    if (!d->goldSource) {
        o.insert(QStringLiteral("appId"), QJsonValue(static_cast<int>(d->appId)));
    }
    o.insert(QStringLiteral("players"), QJsonValue(static_cast<int>(d->players)));
    o.insert(QStringLiteral("maxPlayers"), QJsonValue(static_cast<int>(d->maxPlayers)));
    o.insert(QStringLiteral("bots"), QJsonValue(static_cast<int>(d->bots)));
    switch (d->serverType) {
    case Dedicated:
        o.insert(QStringLiteral("serverType"), QJsonValue(QStringLiteral("d")));
        break;
    case NonDedicated:
        o.insert(QStringLiteral("serverType"), QJsonValue(QStringLiteral("l")));
        break;
    case SourceTVRelay:
        o.insert(QStringLiteral("serverType"), QJsonValue(QStringLiteral("p")));
        break;
    default:
        break;
    }
    switch (d->environment) {
    case Linux:
        o.insert(QStringLiteral("environment"), QJsonValue(QStringLiteral("l")));
        break;
    case Windows:
        o.insert(QStringLiteral("environment"), QJsonValue(QStringLiteral("w")));
        break;
    case Mac:
        o.insert(QStringLiteral("environment"), QJsonValue(QStringLiteral("m")));
        break;
    default:
        break;
    }

    o.insert(QStringLiteral("visibility"), QJsonValue(static_cast<int>(d->visibility)));
    o.insert(QStringLiteral("vac"), QJsonValue(static_cast<int>(d->vac)));

    if (d->appId == 2400) {
        o.insert(QStringLiteral("mode"), QJsonValue(static_cast<int>(d->theShipMode)));
        o.insert(QStringLiteral("witnesses"), QJsonValue(static_cast<int>(d->theShipWitnesses)));
        o.insert(QStringLiteral("duration"), QJsonValue(static_cast<int>(d->theShipDuration)));
    }

    if (!d->goldSource) {
        o.insert(QStringLiteral("version"), QJsonValue(d->version));
        o.insert(QStringLiteral("gamePort"), QJsonValue(static_cast<int>(d->gamePort)));
        if (d->steamId <= Q_UINT64_C(9007199254740992)) {
            o.insert(QStringLiteral("steamId"), QJsonValue(static_cast<qint64>(d->steamId)));
        } else {
            o.insert(QStringLiteral("steamId"), QJsonValue(QString::number(d->steamId)));
        }

        o.insert(QStringLiteral("specPort"), QJsonValue(static_cast<int>(d->specPort)));
        o.insert(QStringLiteral("specName"), QJsonValue(d->specName));
        o.insert(QStringLiteral("keywords"), QJsonArray::fromStringList(d->keywords));
        if (d->gameId <= Q_UINT64_C(9007199254740992)) {
            o.insert(QStringLiteral("gameId"), QJsonValue(static_cast<qint64>(d->gameId)));
        } else {
            o.insert(QStringLiteral("gameId"), QJsonValue(QString::number(d->gameId)));
        }
        o.insert(QStringLiteral("storeLink"), QJsonValue(storeLink().toString()));
    } else {
        o.insert(QStringLiteral("isMod"), QJsonValue(d->isMod));
        if (d->isMod) {
            o.insert(QStringLiteral("modLink"), QJsonValue(d->modLink.toString()));
            o.insert(QStringLiteral("modDownloadLink"), QJsonValue(d->modDownloadLink.toString()));
            o.insert(QStringLiteral("modVersion"), QJsonValue(static_cast<qint64>(d->modVersion)));
            o.insert(QStringLiteral("modSize"), QJsonValue(static_cast<qint64>(d->modSize)));
            o.insert(QStringLiteral("modTye"), QJsonValue(d->modType == ServerInfo::MultiplayerOnlyMod ? 1 : 0));
            o.insert(QStringLiteral("modDll"), QJsonValue(d->modDll == ServerInfo::UsesOwnDll ? 1 : 0));
        }
    }
    return o;
}

int ServerInfo::setRawData(const QByteArray &data)
{
    int pos = 0;
    if (Q_LIKELY(!data.isEmpty())) {
        QByteArray ba = data;
        Response res(&ba);
        const char header = res.getCharacter();
        if ((header == 'I') || (header == 'm')) {
            Q_D(ServerInfo);
            d->setGoldSource(header == 'm');

            if (!d->goldSource) {
                d->setProtocol(res.getUByte());
                d->setName(res.getString());
                d->setMap(res.getString());
                d->setFolder(res.getString());
                d->setGame(res.getString());
                d->setAppId(res.getUShort());
                d->setPlayers(res.getUByte());
                d->setMaxPlayers(res.getUByte());
                d->setBots(res.getUByte());
                d->setServerType(res.getCharacter());
                d->setEnvironment(res.getCharacter());
                d->setVisibility(res.getUByte() ? ServerInfo::Private : ServerInfo::Public);
                d->setVac(res.getUByte() ? ServerInfo::Secured : ServerInfo::Unsecured);
                if (d->appId == 2400) {
                    d->setTheShipMode(res.getUByte());
                    d->setTheShipWitnesses(res.getUByte());
                    d->setTheShipDuartion(res.getUByte());
                } else {
                    d->setTheShipWitnesses(ServerInfo::UnknownTheShipMode);
                    d->setTheShipWitnesses(0);
                    d->setTheShipDuartion(0);
                }
                d->setVersion(res.getString());

                const auto edf = res.getUByte();
                if (edf & 0x80) {
                    d->setGamePort(res.getUShort());
                } else {
                    d->setGamePort(0);
                }

                if (edf & 0x10) {
                    d->setSteamId(res.getULongLong());
                } else {
                    d->setSteamId(0);
                }

                if (edf & 0x40) {
                    d->setSpecPort(res.getUShort());
                    d->setSpecName(res.getString());
                } else {
                    d->setSpecPort(0);
                    d->setSpecName(QString());
                }

                if (edf & 0x20) {
                    const QString kws = res.getString();
                    if (!kws.isEmpty()) {
                        d->setKeywords(kws.split(kws.at(0), QString::SkipEmptyParts));
                    } else {
                        d->setKeywords(QStringList());
                    }
                } else {
                    d->setKeywords(QStringList());
                }

                if (edf & 0x01) {
                    d->setGameId(res.getULongLong());
                } else {
                    d->setGameId(0);
                }

                // only available in the gold source response
                d->setIsMod(false);
                d->setModLink(QUrl());
                d->setModDownloadLink(QUrl());
                d->setModVersion(0);
                d->setModSize(0);
                d->setModType(ServerInfo::SingleAndMultiplayerMod);
                d->setModDll(ServerInfo::UsesHalfLifeDll);
            } else {
                const QString addressAndPort = res.getString();
                const auto portSepIdx = addressAndPort.lastIndexOf(QLatin1Char(':'));
                if (portSepIdx > -1) {
                    d->setGamePort(addressAndPort.midRef(portSepIdx + 1).toUShort());
                } else {
                    d->setGamePort(0);
                }
                d->setName(res.getString());
                d->setMap(res.getString());
                d->setFolder(res.getString());
                d->setGame(res.getString());
                d->setPlayers(res.getUByte());
                d->setMaxPlayers(res.getUByte());
                d->setProtocol(res.getUByte());
                d->setServerType(res.getCharacter());
                d->setEnvironment(res.getCharacter());
                d->setVisibility(res.getUByte() ? ServerInfo::Private : ServerInfo::Public);
                d->setIsMod(res.getUByte() == 1);
                if (d->isMod) {
                    d->setModLink(res.getUrl());
                    d->setModDownloadLink(res.getUrl());
                    res.getUByte();
                    d->setModVersion(res.getULong());
                    d->setModSize(res.getULong());
                    d->setModType(res.getUByte() ? ServerInfo::MultiplayerOnlyMod : ServerInfo::SingleAndMultiplayerMod);
                    d->setModDll(res.getUByte() ? ServerInfo::UsesHalfLifeDll : ServerInfo::UsesOwnDll);
                }
                d->setVac(res.getUByte() ? ServerInfo::Secured : ServerInfo::Unsecured);
                d->setBots(res.getUByte());

                // only available in the new response format
                d->setAppId(0);
                d->setTheShipWitnesses(ServerInfo::UnknownTheShipMode);
                d->setTheShipWitnesses(0);
                d->setTheShipDuartion(0);
                d->setSteamId(0);
                d->setSpecPort(0);
                d->setSpecName(QString());
                d->setKeywords(QStringList());
                d->setGameId(0);
            }
            pos = res.pos();
        } else {
            qCCritical(SI, "Invalid response header: %c", header);
        }
    } else {
        qCCritical(SI, "Can not set empty raw data.");
    }

    return pos;
}

bool ServerInfo::update(int timeout)
{
    Q_D(ServerInfo);

    ServerQuery sq(d->address, d->queryPort);
    sq.setTimeout(timeout);

    const QByteArray data = sq.getRawInfo();
    if (Q_LIKELY(!data.isEmpty())) {
        return (setRawData(data) > 0);
    } else {
        return false;
    }
}

void ServerInfo::updateAsync(int timeout)
{
    Q_D(ServerInfo);

    auto sq = new ServerQuery(d->address, d->queryPort, this);
    sq->setTimeout(timeout);
    QObject::connect(sq, &ServerQuery::gotRawInfo, this, [this](const QByteArray &data){
        if (!data.isEmpty()) {
            setRawData(data);
        }
    });
    QObject::connect(sq, &ServerQuery::gotRawInfo, sq, &ServerQuery::deleteLater);
    sq->getRawInfoAsync();
}

bool ServerInfo::query(const QString &address, quint16 queryPort, int timeout)
{
    Q_D(ServerInfo);
    d->setAddress(address);
    d->setQueryPort(queryPort);

    ServerQuery sq(address, queryPort);
    sq.setTimeout(timeout);

    const QByteArray data = sq.getRawInfo();
    if (Q_LIKELY(!data.isEmpty())) {
        return (setRawData(data) > 0);
    } else {
        return false;
    }
}

void ServerInfo::queryAsync(const QString &address, quint16 queryPort, int timeout)
{
    Q_D(ServerInfo);
    d->setAddress(address);
    d->setQueryPort(queryPort);

    auto sq = new ServerQuery(address, queryPort, this);
    sq->setTimeout(timeout);
    QObject::connect(sq, &ServerQuery::gotRawInfo, this, [this](const QByteArray &data){
        if (!data.isEmpty()) {
            setRawData(data);
        }
    });
    QObject::connect(sq, &ServerQuery::gotRawInfo, sq, &ServerQuery::deleteLater);
    sq->getRawInfoAsync();
}

bool ServerInfo::event(QEvent *event)
{
    return QObject::event(event);
}

ServerInfo *ServerInfo::fromRawData(const QByteArray &data, const QString &address, quint16 queryPort, QObject *parent)
{
    ServerInfo *si = new ServerInfo(address, queryPort, parent);

    si->setRawData(data);

    return si;
}

ServerInfo *ServerInfo::get(const QString &address, quint16 queryPort, int timeout, QObject *parent)
{
    ServerInfo *si = new ServerInfo(address, queryPort, parent);

    ServerQuery sq(address, queryPort);
    sq.setTimeout(timeout);
    const QByteArray data = sq.getRawInfo();
    si->setRawData(data);

    return si;
}

void ServerInfoPrivate::setAddress(const QString &_address)
{
    Q_Q(ServerInfo);
    if (address != _address) {
        address = _address;
        Q_EMIT q->addressChanged(_address);
    }
}

void ServerInfoPrivate::setQueryPort(quint16 _queryPort)
{
    Q_Q(ServerInfo);
    if (queryPort != _queryPort) {
        queryPort = _queryPort;
        Q_EMIT q->queryPortChanged(_queryPort);
    }
}

void ServerInfoPrivate::setGoldSource(bool _goldSource)
{
    Q_Q(ServerInfo);
    if (goldSource != _goldSource) {
        goldSource = _goldSource;
        Q_EMIT q->goldSourceChanged(_goldSource);
    }
}

void ServerInfoPrivate::setProtocol(quint8 _protocol)
{
    Q_Q(ServerInfo);
    if (protocol != _protocol) {
        protocol = _protocol;
        Q_EMIT q->protocolChanged(_protocol);
    }
}

void ServerInfoPrivate::setName(const QString &_name)
{
    Q_Q(ServerInfo);
    if (name != _name) {
        name = _name;
        Q_EMIT q->nameChanged(_name);
    }
}

void ServerInfoPrivate::setMap(const QString &_map)
{
    Q_Q(ServerInfo);
    if (map != _map) {
        map = _map;
        Q_EMIT q->mapChanged(_map);
    }
}

void ServerInfoPrivate::setFolder(const QString &_folder)
{
    Q_Q(ServerInfo);
    if (folder != _folder) {
        folder = _folder;
        Q_EMIT q->folderChanged(_folder);
    }
}

void ServerInfoPrivate::setGame(const QString &_game)
{
    Q_Q(ServerInfo);
    if (game != _game) {
        game = _game;
        Q_EMIT q->gameChanged(_game);
    }
}

void ServerInfoPrivate::setAppId(quint16 _appId)
{
    Q_Q(ServerInfo);
    if (appId != _appId) {
        appId = _appId;
        Q_EMIT q->appIdChanged(_appId);
        Q_EMIT q->storeLinkChanged(q->storeLink());
    }
}

void ServerInfoPrivate::setPlayers(quint8 _players)
{
    Q_Q(ServerInfo);
    if (players != _players) {
        players = _players;
        Q_EMIT q->playersChanged(_players);
    }
}

void ServerInfoPrivate::setMaxPlayers(quint8 _maxPlayers)
{
    Q_Q(ServerInfo);
    if (maxPlayers != _maxPlayers) {
        maxPlayers = _maxPlayers;
        Q_EMIT q->maxPlayersChanged(_maxPlayers);
    }
}

void ServerInfoPrivate::setBots(quint8 _bots)
{
    Q_Q(ServerInfo);
    if (bots != _bots) {
        bots = _bots;
        Q_EMIT q->botsChanged(_bots);
    }
}

void ServerInfoPrivate::setServerType(ServerInfo::Type _type)
{
    Q_Q(ServerInfo);
    if (serverType != _type) {
        serverType = _type;
        Q_EMIT q->serverTypeChanged(_type);
    }
}

void ServerInfoPrivate::setServerType(char ch)
{
    if (ch == 'd' || ch == 'D') {
        setServerType(ServerInfo::Dedicated);
    } else if (ch == 'l' || ch == 'L') {
        setServerType(ServerInfo::NonDedicated);
    } else if (ch == 'p' || ch == 'P') {
        setServerType(ServerInfo::SourceTVRelay);
    } else {
        setServerType(ServerInfo::Unspecified);
    }
}

void ServerInfoPrivate::setEnvironment(ServerInfo::Environment _env)
{
    Q_Q(ServerInfo);
    if (environment != _env) {
        environment = _env;
        Q_EMIT q->environmentChanged(_env);
    }
}

void ServerInfoPrivate::setEnvironment(char ch)
{
    if (ch == 'l' || ch == 'L') {
        setEnvironment(ServerInfo::Linux);
    } else if (ch == 'w' || ch == 'W') {
        setEnvironment(ServerInfo::Windows);
    } else if (ch == 'o' || ch == 'm') {
        setEnvironment(ServerInfo::Mac);
    } else {
        setEnvironment(ServerInfo::Unknown);
    }
}

void ServerInfoPrivate::setVisibility(ServerInfo::Visibility _vis)
{
    Q_Q(ServerInfo);
    if (visibility != _vis) {
        visibility = _vis;
        Q_EMIT q->visibilityChanged(_vis);
    }
}

void ServerInfoPrivate::setVac(ServerInfo::VAC _vac)
{
    Q_Q(ServerInfo);
    if (vac != _vac) {
        vac = _vac;
        Q_EMIT q->vacChanged(_vac);
    }
}

void ServerInfoPrivate::setTheShipMode(ServerInfo::TheShipMode _mode)
{
    Q_Q(ServerInfo);
    if (theShipMode != _mode) {
        theShipMode = _mode;
        Q_EMIT q->theShipModeChanged(_mode);
    }
}

void ServerInfoPrivate::setTheShipMode(quint8 _mode)
{
    switch (_mode) {
    case 0:
        setTheShipMode(ServerInfo::Hunt);
        break;
    case 1:
        setTheShipMode(ServerInfo::Elimination);
        break;
    case 2:
        setTheShipMode(ServerInfo::Duel);
        break;
    case 3:
        setTheShipMode(ServerInfo::Deathmatch);
        break;
    case 4:
        setTheShipMode(ServerInfo::VIPTeam);
        break;
    case 5:
        setTheShipMode(ServerInfo::TeamElimination);
        break;
    default:
        setTheShipMode(ServerInfo::UnknownTheShipMode);
        break;
    }
}

void ServerInfoPrivate::setTheShipWitnesses(quint8 _witnesses)
{
    Q_Q(ServerInfo);
    if (theShipWitnesses != _witnesses) {
        theShipWitnesses = _witnesses;
        Q_EMIT q->theShipWitnessesChanged(_witnesses);
    }
}

void ServerInfoPrivate::setTheShipDuartion(quint8 _duration)
{
    Q_Q(ServerInfo);
    if (theShipDuration != _duration) {
        theShipDuration = _duration;
        Q_EMIT q->theShipDurationChanged(_duration);
    }
}

void ServerInfoPrivate::setVersion(const QString &_version)
{
    Q_Q(ServerInfo);
    if (version != _version) {
        version = _version;
        Q_EMIT q->versionChanged(_version);
    }
}

void ServerInfoPrivate::setGamePort(quint16 _port)
{
    Q_Q(ServerInfo);
    if (gamePort != _port) {
        gamePort = _port;
        Q_EMIT q->gamePortChanged(_port);
    }
}

void ServerInfoPrivate::setSteamId(quint64 _id)
{
    Q_Q(ServerInfo);
    if (steamId != _id) {
        steamId = _id;
        Q_EMIT q->steamIdChanged(_id);
    }
}

void ServerInfoPrivate::setSpecPort(quint16 _port)
{
    Q_Q(ServerInfo);
    if (specPort != _port) {
        specPort = _port;
        Q_EMIT q->specPortChanged(_port);
    }
}

void ServerInfoPrivate::setSpecName(const QString &_name)
{
    Q_Q(ServerInfo);
    if (specName != _name) {
        specName = _name;
        Q_EMIT q->specNameChanged(_name);
    }
}

void ServerInfoPrivate::setKeywords(const QStringList &_kws)
{
    Q_Q(ServerInfo);
    if (keywords != _kws) {
        keywords = _kws;
        Q_EMIT q->keywordsChanged(_kws);
    }
}

void ServerInfoPrivate::setGameId(quint64 _id)
{
    Q_Q(ServerInfo);
    if (gameId != _id) {
        gameId = _id;
        Q_EMIT q->gameIdChanged(_id);
        Q_EMIT q->storeLinkChanged(q->storeLink());
    }
}

void ServerInfoPrivate::setIsMod(bool _isMod)
{
    Q_Q(ServerInfo);
    if (isMod != _isMod) {
        isMod = _isMod;
        Q_EMIT q->isModChanged(_isMod);
    }
}

void ServerInfoPrivate::setModLink(const QUrl &_url)
{
    Q_Q(ServerInfo);
    if (modLink != _url) {
        modLink = _url;
        Q_EMIT q->modLinkChanged(_url);
    }
}

void ServerInfoPrivate::setModDownloadLink(const QUrl &_url)
{
    Q_Q(ServerInfo);
    if (modDownloadLink != _url) {
        modDownloadLink = _url;
        Q_EMIT q->modDownloadLinkChanged(_url);
    }
}

void ServerInfoPrivate::setModVersion(quint32 _version)
{
    Q_Q(ServerInfo);
    if (modVersion != _version) {
        modVersion = _version;
        Q_EMIT q->modVersionChanged(_version);
    }
}

void ServerInfoPrivate::setModSize(quint32 _size)
{
    Q_Q(ServerInfo);
    if (modSize != _size) {
        modSize = _size;
        Q_EMIT q->modSizeChanged(_size);
    }
}

void ServerInfoPrivate::setModType(ServerInfo::ModType _type)
{
    Q_Q(ServerInfo);
    if (modType != _type) {
        modType = _type;
        Q_EMIT q->modTypeChanged(_type);
    }
}

void ServerInfoPrivate::setModDll(ServerInfo::ModDLLUsage _dll)
{
    Q_Q(ServerInfo);
    if (modDll != _dll) {
        modDll = _dll;
        Q_EMIT q->modDllChanged(_dll);
    }
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerInfo *serverInfo)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver);
    if (!serverInfo) {
        return dbg << QGSQ::Valve::Source::ServerInfo::staticMetaObject.className() << "(0x0)";
    }
    dbg.nospace() << serverInfo->metaObject()->className() << '(' << (const void *)serverInfo;
    dbg << ", GoldSource: " << serverInfo->isGoldSource();
    dbg << ", Protocol version: " << serverInfo->protocol();
    dbg << ", Name: " << serverInfo->name();
    dbg << ", Map: " << serverInfo->map();
    dbg << ", Folder: " << serverInfo->folder();
    dbg << ", Game: " << serverInfo->game();
    if (!serverInfo->isGoldSource()) {
        dbg << ", AppID: " << serverInfo->appId();
    }
    dbg << ", Players: " << serverInfo->players();
    dbg << ", Max. Players: " << serverInfo->maxPlayers();
    dbg << ", Bots: " << serverInfo->bots();
    dbg << ", Type: " << serverInfo->serverType();
    dbg << ", Environment: " << serverInfo->environment();
    dbg << ", Visibility: " << serverInfo->visibility();
    dbg << ", VAC: " << serverInfo->vac();
    if (serverInfo->appId() == 2400) {
        dbg << ", Mode: " << serverInfo->theShipMode();
        dbg << ", Witnesses: " << serverInfo->theShipWitnesses();
        dbg << ", Duration: " << serverInfo->theShipDuration();
    }
    if (!serverInfo->isGoldSource()) {
        dbg << ", Version: " << serverInfo->version();
        if (serverInfo->gamePort()) {
            dbg << ", Game Port: " << serverInfo->gamePort();
        }
        if (serverInfo->steamId()) {
            dbg << ", SteamID: " << serverInfo->steamId();
        }
        if (serverInfo->specPort()) {
            dbg << ", Spectator Port: " << serverInfo->specPort();
        }
        if (!serverInfo->specName().isEmpty()) {
            dbg << ", Spectator Server Name: " << serverInfo->specName();
        }
        if (!serverInfo->keywords().isEmpty()) {
            dbg << ", Keywords: " << serverInfo->keywords();
        }
        if (serverInfo->gameId()) {
            dbg << ", GameID: " << serverInfo->gameId();
        }
        if (serverInfo->storeLink().isValid()) {
            dbg << ", Store Link: " << serverInfo->storeLink();
        }
    } else {
        if (serverInfo->gamePort()) {
            dbg << ", Game Port: " << serverInfo->gamePort();
        }
        dbg << ", IsMod: " << serverInfo->isMod();
        if (serverInfo->isMod()) {
            dbg << ", ModLink: " << serverInfo->modLink();
            dbg << ", ModDownloadLink: " << serverInfo->modDownloadLink();
            dbg << ", ModVersion: " << serverInfo->modVersion();
            dbg << ", ModSize: " << serverInfo->modSize();
            dbg << ", ModType: " << serverInfo->modType();
            dbg << ", ModDLL: " << serverInfo->modDll();
        }
    }
    dbg << ')';
    return dbg.maybeSpace();
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::ServerInfo &serverInfo)
{
    return dbg << &serverInfo;
}

std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::ServerInfo *serverInfo)
{
    return stream << qUtf8Printable(QString::fromUtf8(QJsonDocument(serverInfo->toJson()).toJson()));
}

std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::ServerInfo &serverInfo)
{
    return stream << &serverInfo;
}

#include "moc_serverinfo.cpp"
