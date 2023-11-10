import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    ENTITY_CATEGORY_DIAGNOSTIC,
    UNIT_PASCAL,
)
from . import CONF_FLOW_METER_ID, FlowMeter

DEPENDENCIES = ["flow_meter"]

UNIT_LITER_PER_SEC = "l/s"
CONF_PRESSURE_AVG = "pressure_avg"
CONF_PRESSURE_ERROR = "pressure_error"
CONF_FLOW = "flow"
CONF_FLOW_AVG = "flow_avg"
CONF_FLOW_ERROR = "flow_error"

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_FLOW_METER_ID): cv.use_id(FlowMeter),
    cv.Optional(CONF_PRESSURE_AVG): sensor.sensor_schema(
        unit_of_measurement=UNIT_PASCAL,
        accuracy_decimals=1,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    cv.Optional(CONF_PRESSURE_ERROR): sensor.sensor_schema(
        unit_of_measurement=UNIT_PASCAL,
        accuracy_decimals=1,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    cv.Optional(CONF_FLOW): sensor.sensor_schema(
        unit_of_measurement=UNIT_LITER_PER_SEC,
        accuracy_decimals=1,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    cv.Optional(CONF_FLOW_AVG): sensor.sensor_schema(
        unit_of_measurement=UNIT_LITER_PER_SEC,
        accuracy_decimals=1,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
    cv.Optional(CONF_FLOW_ERROR): sensor.sensor_schema(
        unit_of_measurement=UNIT_LITER_PER_SEC,
        accuracy_decimals=1,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
    ),
}


async def to_code(config):
    hub = await cg.get_variable(config[CONF_FLOW_METER_ID])

    if pressure_avg_conf := config.get(CONF_PRESSURE_AVG):
        sens = await sensor.new_sensor(pressure_avg_conf)
        cg.add(hub.set_pressure_avg_sensor(sens))

    if pressure_error_conf := config.get(CONF_PRESSURE_ERROR):
        sens = await sensor.new_sensor(pressure_error_conf)
        cg.add(hub.set_pressure_error_sensor(sens))

    if flow_conf := config.get(CONF_FLOW):
        sens = await sensor.new_sensor(flow_conf)
        cg.add(hub.set_flow_sensor(sens))

    if flow_avg_conf := config.get(CONF_FLOW_AVG):
        sens = await sensor.new_sensor(flow_avg_conf)
        cg.add(hub.set_flow_avg_sensor(sens))

    if flow_error_conf := config.get(CONF_FLOW_ERROR):
        sens = await sensor.new_sensor(flow_error_conf)
        cg.add(hub.set_flow_error_sensor(sens))
