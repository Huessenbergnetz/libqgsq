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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QLoggingCategory>
#include <QJsonDocument>
#include <QEventLoop>

#include <iostream>

#include <QGSQ/Valve/Source/serverquery.h>
#include <QGSQ/Valve/Source/serverinfo.h>
#include <QGSQ/Valve/Source/player.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("Huessenbergnetz"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("huessenbergnetz.de"));
    QCoreApplication::setApplicationName(QStringLiteral("qgsqtestapp"));
    QCoreApplication::setApplicationVersion(QStringLiteral(VERSION));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption server(QStringList({QStringLiteral("s"), QStringLiteral("server")}), QStringLiteral("Server to connect to."), QStringLiteral("address"));
    parser.addOption(server);

    QCommandLineOption port(QStringList({QStringLiteral("p"), QStringLiteral("port")}), QStringLiteral("Port to connect to. (Default: 27015)"), QStringLiteral("port"), QStringLiteral("27015"));
    parser.addOption(port);

    QCommandLineOption getInfo(QStringLiteral("get-info"), QStringLiteral("Get server information."));
    parser.addOption(getInfo);

    QCommandLineOption getInfoAsync(QStringLiteral("get-info-async"), QStringLiteral("Get server informatino asynchronous."));
    parser.addOption(getInfoAsync);

    QCommandLineOption getRules(QStringLiteral("get-rules"), QStringLiteral("Get server rules"));
    parser.addOption(getRules);

    QCommandLineOption getRulesAsync(QStringLiteral("get-rules-async"), QStringLiteral("Get server rules aynchronous."));
    parser.addOption(getRulesAsync);

    QCommandLineOption getPlayers(QStringLiteral("get-players"), QStringLiteral("Get players currently on the server."));
    parser.addOption(getPlayers);

    QCommandLineOption getPlayersAsync(QStringLiteral("get-players-async"), QStringLiteral("Get players currently on the server asnychronous."));
    parser.addOption(getPlayersAsync);

    QCommandLineOption enableDebug(QStringLiteral("debug"), QStringLiteral("Enable debug output."));
    parser.addOption(enableDebug);

    parser.process(app);

    if (parser.isSet(enableDebug)) {
        QLoggingCategory::setFilterRules(QStringLiteral("qgsq.*.debug=true"));
    } else {
        QLoggingCategory::setFilterRules(QStringLiteral("qgsq.*.debug=false"));
    }

    if (parser.isSet(server)) {

        if (parser.isSet(getInfo)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());
            QScopedPointer<QGSQ::Valve::Source::ServerInfo> si(sq.getInfo());

            std::cout << "ServerInfo:\n" << si.data();
        }

        if (parser.isSet(getInfoAsync)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());
            QEventLoop loop;
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotInfo, &loop, &QEventLoop::quit);
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotInfo, &sq, [](QGSQ::Valve::Source::ServerInfo *si) {
                std::cout << "ServerInfo:\n" << si;
                delete si;
            });
            sq.getInfoAsync();
            loop.exec();
        }

        if (parser.isSet(getRules)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());

            qDebug() << sq.getRules();
        }

        if (parser.isSet(getRulesAsync)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());
            QEventLoop loop;
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotRules, &loop, &QEventLoop::quit);
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotRules, &sq, [](const QHash<QString,QString> &rules){
                qDebug() << rules;
            });
            sq.getRulesAsync();
            loop.exec();
        }

        if (parser.isSet(getPlayers)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());

            QObject o;
            qDebug() << sq.getPlayers(&o);
        }

        if (parser.isSet(getPlayersAsync)) {
            QGSQ::Valve::Source::ServerQuery sq(parser.value(server), parser.value(port).toUShort());
            QEventLoop loop;
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotPlayers, &loop, &QEventLoop::quit);
            QObject::connect(&sq, &QGSQ::Valve::Source::ServerQuery::gotPlayers, &sq, [](const QList<QGSQ::Valve::Source::Player*> &players){
                qDebug() << players;
                qDeleteAll(players);
            });
            sq.getPlayersAsync();
            loop.exec();
        }

    } else {
        parser.showHelp(1);
    }

    return 0;
}
