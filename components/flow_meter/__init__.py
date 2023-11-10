import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_NAME,
)

CODEOWNERS = ["@ardiloot"]
DEPENDENCIES = ["logger"]


CONF_FLOW_METER_ID = "flow_meter_id"
CONF_VALVES = "valves"
CONF_VALVE_SIZES = "sizes"
CONF_VALVE_SIZE = "size"
CONF_VALVE_S = "s"
CONF_VALVE_K = "k"

flow_meter_ns = cg.esphome_ns.namespace("flow_meter")
ValveType = flow_meter_ns.class_("ValveType")
FlowMeter = flow_meter_ns.class_("FlowMeter", cg.Component)

CONFIG_VALVE_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_VALVE_SIZE): cv.string,
        cv.Required(CONF_VALVE_S): [cv.float_],
        cv.Required(CONF_VALVE_K): [cv.float_],
    }
)


CONFIG_VALVE_TYPE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ValveType),
        cv.Required(CONF_NAME): cv.string,
        cv.Required(CONF_VALVE_SIZES): [CONFIG_VALVE_SCHEMA],
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(FlowMeter),
            cv.Required(CONF_VALVES): [CONFIG_VALVE_TYPE_SCHEMA],
        }
    ),
)


async def to_code(config):
    hub = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(hub, config)

    for valve_type_conf in config[CONF_VALVES]:
        vt = cg.new_Pvariable(valve_type_conf[CONF_ID])
        cg.add(vt.set_name(valve_type_conf[CONF_NAME]))
        for valve_size_conf in valve_type_conf[CONF_VALVE_SIZES]:
            cg.add(
                vt.add_size(
                    valve_size_conf[CONF_VALVE_SIZE],
                    valve_size_conf[CONF_VALVE_S],
                    valve_size_conf[CONF_VALVE_K],
                )
            )
        cg.add(hub.add_valve_type(vt))

    cg.add_library("robtillaart/Statistic", "1.0.5")
