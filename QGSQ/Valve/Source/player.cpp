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

#include "player_p.h"
#include <QDebug>
#include <QJsonDocument>

using namespace QGSQ::Valve::Source;

Player::Player(QObject *parent) : QObject(parent), d_ptr(new PlayerPrivate)
{
    d_ptr->q_ptr = this;
}

Player::Player(const QString &name, qint32 score, float duration, QObject *parent) :
    QObject(parent), d_ptr(new PlayerPrivate)
{
    Q_D(Player);
    d->name = name;
    d->score = score;
    d->duration = duration;
    d->q_ptr = this;
}

Player::~Player()
{

}

QString Player::name() const
{
    Q_D(const Player);
    return d->name;
}

qint32 Player::score() const
{
    Q_D(const Player);
    return d->score;
}

float Player::duration() const
{
    Q_D(const Player);
    return d->duration;
}

void Player::setData(const QString &name, qint32 score, float duration)
{
    Q_D(Player);
    d->setName(name);
    d->setScore(score);
    d->setDuration(duration);
}

QJsonObject Player::toJson() const
{
    QJsonObject o;
    Q_D(const Player);
    o.insert(QStringLiteral("name"), QJsonValue(d->name));
    o.insert(QStringLiteral("score"), QJsonValue(d->score));
    o.insert(QStringLiteral("duration"), QJsonValue(d->duration));
    return o;
}

void PlayerPrivate::setName(const QString &_name)
{
    if (name != _name) {
        name = _name;
        Q_Q(Player);
        Q_EMIT q->nameChanged(_name);
    }
}

void PlayerPrivate::setScore(qint32 _score)
{
    if (score != _score) {
        score = _score;
        Q_Q(Player);
        Q_EMIT q->scoreChanged(_score);
    }
}

void PlayerPrivate::setDuration(float _duration)
{
    if (duration != _duration) {
        duration = _duration;
        Q_Q(Player);
        Q_EMIT q->durationChanged(_duration);
    }
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::Player *player)
{
    QDebugStateSaver saver(dbg);
    Q_UNUSED(saver);
    if (!player) {
        return dbg << QGSQ::Valve::Source::Player::staticMetaObject.className() << "(0x0)";
    }
    dbg.nospace() << player->metaObject()->className() << '(' << (const void*)player;
    dbg << ", Name: " << player->name();
    dbg << ", Score: " << player->score();
    dbg << ", Duration: " << player->duration();
    dbg << ')';
    return dbg.maybeSpace();
}

QDebug operator<<(QDebug dbg, const QGSQ::Valve::Source::Player &player)
{
    return dbg << &player;
}

std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::Player *player)
{
    return stream << qUtf8Printable(QString::fromUtf8(QJsonDocument(player->toJson()).toJson()));
}

std::ostream& operator<<(std::ostream &stream, const QGSQ::Valve::Source::Player &player)
{
    return stream << &player;
}

#include "moc_player.cpp"
