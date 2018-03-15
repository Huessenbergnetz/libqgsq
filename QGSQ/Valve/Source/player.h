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

#ifndef QGSQ_VALVE_SOURCE_PLAYER_H
#define QGSQ_VALVE_SOURCE_PLAYER_H

#include "qgsq_global.h"
#include <QObject>
#include <QJsonObject>
#include <ostream>

namespace QGSQ {
namespace Valve {
namespace Source {

class PlayerPrivate;

class QGSQ_LIBRARY Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(qint32 score READ score NOTIFY scoreChanged)
    Q_PROPERTY(float duartion READ duration NOTIFY durationChanged)
public:
    explicit Player(QObject *parent = nullptr);

    Player(const QString &name, qint32 score, float duration, QObject *parent = nullptr);

    ~Player();

    QString name() const;
    qint32 score() const;
    float duration() const;

    void setData(const QString &name, qint32 score, float duration);

    QJsonObject toJson() const;

    bool event(QEvent *event) override;

Q_SIGNALS:
    void nameChanged(const QString &name);
    void scoreChanged(qint32 score);
    void durationChanged(float duration);

private:
    const QScopedPointer<PlayerPrivate> d_ptr;
    Q_DISABLE_COPY(Player)
    Q_DECLARE_PRIVATE(Player)
};

}
}
}

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::Player *player);

QGSQ_LIBRARY QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::Player &player);

QGSQ_LIBRARY std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::Player *player);

QGSQ_LIBRARY std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::Player &player);
#endif // QGSQ_VALVE_SOURCE_PLAYER_H
