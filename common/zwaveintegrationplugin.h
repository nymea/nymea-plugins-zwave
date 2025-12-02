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

#ifndef ZWAVEINTEGRATIONPLUGIN_H
#define ZWAVEINTEGRATIONPLUGIN_H

#include <integrations/integrationplugin.h>
#include <hardware/zwave/zwavehandler.h>
#include <hardware/zwave/zwavehardwareresource.h>

#include <QObject>

class ZWaveIntegrationPlugin: public IntegrationPlugin, public ZWaveHandler
{
    Q_OBJECT

public:
    explicit ZWaveIntegrationPlugin(ZWaveHardwareResource::HandlerType handlerType, const QLoggingCategory &loggingCategory);
    virtual ~ZWaveIntegrationPlugin() override;

    virtual void init() override;
    virtual void handleRemoveNode(ZWaveNode *node) override;
    virtual void thingRemoved(Thing *thing) override;

protected:
    bool manageNode(Thing *thing);
    Thing *thingForNode(ZWaveNode *node);
    ZWaveNode *nodeForThing(Thing *thing);

    void createThing(const ThingClassId &thingClassId, ZWaveNode *node, const ParamList &additionalParams = ParamList());

private:
    QHash<Thing*, ZWaveNode*> m_thingNodes;

    ZWaveHardwareResource::HandlerType m_handlerType = ZWaveHardwareResource::HandlerTypeVendor;
    QLoggingCategory m_dc;

};

#endif
