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

#include "zwaveintegrationplugin.h"

#include <hardware/zwave/zwavehardwareresource.h>


ZWaveIntegrationPlugin::ZWaveIntegrationPlugin(ZWaveHardwareResource::HandlerType handlerType, const QLoggingCategory &loggingCategory):
    m_handlerType(handlerType),
    m_dc(loggingCategory.categoryName())
{

}

ZWaveIntegrationPlugin::~ZWaveIntegrationPlugin()
{

}

void ZWaveIntegrationPlugin::init()
{
    hardwareManager()->zwaveResource()->registerHandler(this, m_handlerType);
}

void ZWaveIntegrationPlugin::handleRemoveNode(ZWaveNode *node)
{
    foreach (Thing *thing, m_thingNodes.keys(node)) {
        emit autoThingDisappeared(thing->id());

        // Removing it from our map to prevent a loop that would ask the zigbee network to remove this node (see thingRemoved())
        m_thingNodes.remove(thing);
    }
}

void ZWaveIntegrationPlugin::thingRemoved(Thing *thing)
{
    m_thingNodes.remove(thing);
}

bool ZWaveIntegrationPlugin::manageNode(Thing *thing)
{
    Q_UNUSED(thing)
    QUuid networkUuid = thing->paramValue(thing->thingClass().paramTypes().findByName("networkUuid").id()).toUuid();
    quint8 nodeId = thing->paramValue(thing->thingClass().paramTypes().findByName("nodeId").id()).toUInt();

    ZWaveNode *node = m_thingNodes.value(thing);
    if (!node) {
        node = hardwareManager()->zwaveResource()->claimNode(this, networkUuid, nodeId);
    }

    if (!node) {
        return false;
    }

    m_thingNodes.insert(thing, node);

    // Update connected state
    thing->setStateValue("connected", node->reachable());
    connect(node, &ZWaveNode::reachableChanged, thing, [thing](bool reachable){
        thing->setStateValue("connected", reachable);
    });

    // Update signal strength
    thing->setStateValue("signalStrength", node->linkQuality());
    connect(node, &ZWaveNode::linkQualityChanged, thing, [thing](quint8 linkQuality){
        thing->setStateValue("signalStrength", linkQuality);
    });

    // Update battery level
    if (thing->thingClass().hasStateType("batteryLevel")) {
        thing->setStateValue("batteryLevel", node->value(ZWaveValue::GenreUser, ZWaveValue::CommandClassBattery, 1, 0, ZWaveValue::TypeByte).value().toUInt());
        thing->setStateValue("batteryCritical", thing->stateValue("batteryLevel").toUInt() < 10);
        connect(node, &ZWaveNode::valueChanged, thing, [thing](const ZWaveValue &value){
            if (value.genre() == ZWaveValue::GenreUser && value.commandClass() == ZWaveValue::CommandClassBattery && value.instance() == 1 && value.index() == 0 && value.type() == ZWaveValue::TypeByte) {
                thing->setStateValue("batteryLevel", value.value().toUInt());
                thing->setStateValue("batteryCritical", thing->stateValue("batteryLevel").toUInt() < 10);
            }
        });
    }

    return true;
}

Thing *ZWaveIntegrationPlugin::thingForNode(ZWaveNode *node)
{
    return m_thingNodes.key(node);
}

ZWaveNode *ZWaveIntegrationPlugin::nodeForThing(Thing *thing)
{
    return m_thingNodes.value(thing);
}

void ZWaveIntegrationPlugin::createThing(const ThingClassId &thingClassId, ZWaveNode *node, const ParamList &additionalParams)
{
    Q_UNUSED(node)
    ThingDescriptor descriptor(thingClassId, node->productName());
    QString deviceClassName = supportedThings().findById(thingClassId).displayName();

    ParamList params;
    ThingClass tc = supportedThings().findById(thingClassId);
    params.append(Param(tc.paramTypes().findByName("networkUuid").id(), node->networkUuid().toString()));
    params.append(Param(tc.paramTypes().findByName("nodeId").id(), node->nodeId()));
    params.append(additionalParams);
    descriptor.setParams(params);
    emit autoThingsAppeared({descriptor});
}

