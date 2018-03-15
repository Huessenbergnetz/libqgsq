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

#ifndef QGSQ_VALVE_SOURCE_PLAYER_P_H
#define QGSQ_VALVE_SOURCE_PLAYER_P_H

#include "player.h"

namespace QGSQ {
namespace Valve {
namespace Source {

class PlayerPrivate
{
public:
    PlayerPrivate() {}

    virtual ~PlayerPrivate() {}

    void setName(const QString &_name);
    void setScore(qint32 _score);
    void setDuration(float _duration);

    Q_DECLARE_PUBLIC(Player)
    Player *q_ptr = nullptr;
    QString name;
    qint32 score = 0;
    float duration = 0.0f;
};

}
}
}

#endif // QGSQ_VALVE_SOURCE_PLAYER_P_H
