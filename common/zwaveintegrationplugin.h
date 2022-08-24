/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
* Copyright 2013 - 2022, nymea GmbH
* Contact: contact@nymea.io
*
* This file is part of nymea.
* This project including source code and documentation is protected by
* copyright law, and remains the property of nymea GmbH. All rights, including
* reproduction, publication, editing and translation, are reserved. The use of
* this project is subject to the terms of a license agreement to be concluded
* with nymea GmbH in accordance with the terms of use of nymea GmbH, available
* under https://nymea.io/license
*
* GNU Lesser General Public License Usage
* Alternatively, this project may be redistributed and/or modified under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation; version 3. This project is distributed in the hope that
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this project. If not, see <https://www.gnu.org/licenses/>.
*
* For any further details and any questions please contact us under
* contact@nymea.io or see our FAQ/Licensing Information on
* https://nymea.io/license/faq
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
