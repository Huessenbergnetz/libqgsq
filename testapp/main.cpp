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

#include <stdio.h>

#include <QGSQ/Valve/Source/serverquery.h>
#include <QGSQ/Valve/Source/serverinfo.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("Huessenbergnetz"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("huessenbergnetz.de"));
    QCoreApplication::setApplicationName(QStringLiteral("qgsqtestapp"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.0.1"));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption server(QStringList({QStringLiteral("s"), QStringLiteral("server")}), QStringLiteral("Server to connect to."), QStringLiteral("address"));
    parser.addOption(server);

    QCommandLineOption port(QStringList({QStringLiteral("p"), QStringLiteral("port")}), QStringLiteral("Port to connect to. (Default: 27015)"), QStringLiteral("port"), QStringLiteral("27015"));
    parser.addOption(port);

    QCommandLineOption getInfo(QStringLiteral("get-info"), QStringLiteral("Get server inforamtion."));
    parser.addOption(getInfo);

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
            QGSQ::Valve::Source::ServerQuery sq(QHostAddress(parser.value(server)), parser.value(port).toUShort());
            QScopedPointer<QGSQ::Valve::Source::ServerInfo> si(sq.getInfo());

            qDebug() << si.data();
        }

    } else {
        parser.showHelp(1);
    }

    return 0;
}
