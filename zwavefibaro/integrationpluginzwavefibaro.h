// SPDX-License-Identifier: GPL-3.0-or-later

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright (C) 2013 - 2024, nymea GmbH
* Copyright (C) 2024 - 2025, chargebyte austria GmbH
*
* This file is part of nymea-plugins-zwave.
*
* nymea-plugins-zwave is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* nymea-plugins-zwave is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with nymea-plugins-zwave. If not, see <https://www.gnu.org/licenses/>.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef INTEGRATIONPLUGINZWAVEFIBARO_H
#define INTEGRATIONPLUGINZWAVEFIBARO_H

#include "extern-plugininfo.h"

#include "../common/zwaveintegrationplugin.h"

#include <QTimer>


class IntegrationPluginZWaveFibaro: public ZWaveIntegrationPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "io.nymea.IntegrationPlugin" FILE "integrationpluginzwavefibaro.json")
    Q_INTERFACES(IntegrationPlugin)

public:
    explicit IntegrationPluginZWaveFibaro();

    QString name() const override;
    bool handleNode(ZWaveNode *node) override;

    void setupThing(ThingSetupInfo *info) override;
    void executeAction(ThingActionInfo *info) override;
    void thingRemoved(Thing *thing) override;

};

#endif // INTEGRATIONPLUGINZWAVEFIBARO_H
