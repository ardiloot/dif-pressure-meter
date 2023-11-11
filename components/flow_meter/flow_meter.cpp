#include "flow_meter.h"

namespace esphome {
namespace flow_meter {

static const char *const TAG = "flow_meter";

Valve::Valve(const std::string &size, const std::vector<float> &s, const std::vector<float> &k)
    : size_(size), s_(s), k_(k), setting_(s.front()) {}

bool Valve::next_valve_setting() {
  if (setting_ >= s_.back())
    return false;
  setting_ = std::min(s_.back(), setting_ + 0.5f);
  return true;
}

bool Valve::previous_valve_setting() {
  if (setting_ <= s_.front())
    return false;
  setting_ = std::max(s_.front(), setting_ - 0.5f);
  return true;
}

void ValveType::set_name(const std::string &name) { name_ = name; }

void ValveType::add_size(const std::string &size, const std::vector<float> &s, const std::vector<float> &k) {
  sizes_.push_back(Valve(size, s, k));
}

void ValveType::next_valve_size() { size_index_ = size_index_ + 1 >= sizes_.size() ? 0 : size_index_ + 1; }

void ValveType::previous_valve_size() { size_index_ = size_index_ <= 0 ? sizes_.size() - 1 : size_index_ - 1; }

void FlowMeter::loop() {}

float FlowMeter::get_setup_priority() const { return setup_priority::LATE; }

void FlowMeter::dump_config() {
  ESP_LOGCONFIG(TAG, "Flow component:");

  ESP_LOGCONFIG(TAG, "  Valves:");
  for (const ValveType *valve_type : valve_types_) {
    for (const Valve &valve : valve_type->get_sizes()) {
      ESP_LOGCONFIG(TAG, "    %s-%s", valve_type->get_name().c_str(), valve.get_size().c_str());
    }
  }
  LOG_SENSOR("  ", "Pressure average", this->pressure_avg_sensor_);
  LOG_SENSOR("  ", "Pressure error", this->pressure_error_sensor_);
  LOG_SENSOR("  ", "Flow", this->flow_sensor_);
  LOG_SENSOR("  ", "Flow average", this->flow_avg_sensor_);
  LOG_SENSOR("  ", "Flow error", this->flow_error_sensor_);
}

void FlowMeter::add_valve_type(ValveType *valve_type) { valve_types_.push_back(valve_type); }

void FlowMeter::update(const float pressure) {
  ESP_LOGD(TAG, "update: %.1f Pa", pressure);

  if (this->flow_sensor_) {
    const float flow = active_valve().calc_flow(pressure);
    this->flow_sensor_->publish_state(flow);
  }

  if (state_ == State::AVERAGING) {
    pressure_stats_.add(pressure);
  }

  const float pressure_avg = pressure_stats_.average();
  const float pressure_error = pressure_stats_.pop_stdev() / std::sqrt(float(pressure_stats_.count()));
  const float flow_avg = active_valve().calc_flow(pressure_avg);
  const float flow_error = std::fabs(flow_avg - active_valve().calc_flow(pressure_avg + pressure_error));

  if (this->pressure_avg_sensor_) {
    this->pressure_avg_sensor_->publish_state(pressure_avg);
  }

  if (this->pressure_error_sensor_) {
    this->pressure_error_sensor_->publish_state(pressure_error);
  }

  if (this->flow_avg_sensor_) {
    this->flow_avg_sensor_->publish_state(flow_avg);
  }

  if (this->flow_error_sensor_) {
    this->flow_error_sensor_->publish_state(flow_error);
  }
}

void FlowMeter::goto_next_state() {
  switch (state_) {
    case State::IDLE:
      pressure_stats_.clear();
      state_ = State::AVERAGING;
      break;
    case State::AVERAGING:
      state_ = State::STOPPED;
      break;
    case State::STOPPED:
      pressure_stats_.clear();
      state_ = State::IDLE;
      break;
    default:
      break;
  }
}

const std::string &FlowMeter::active_valve_type_name() { return active_valve_type().get_name(); }

void FlowMeter::next_valve_type() {
  valve_type_index_ = valve_type_index_ + 1 >= valve_types_.size() ? 0 : valve_type_index_ + 1;
}

void FlowMeter::previous_valve_type() {
  valve_type_index_ = valve_type_index_ <= 0 ? valve_types_.size() - 1 : valve_type_index_ - 1;
}

const std::string &FlowMeter::active_valve_size() const { return active_valve().get_size(); }

void FlowMeter::next_valve_size() { active_valve_type().next_valve_size(); }

void FlowMeter::previous_valve_size() { active_valve_type().previous_valve_size(); }

float FlowMeter::active_valve_setting() const { return active_valve().get_setting(); }

bool FlowMeter::next_valve_setting() { return active_valve().next_valve_setting(); }

bool FlowMeter::previous_valve_setting() { return active_valve().previous_valve_setting(); }

}  // namespace flow_meter
}  // namespace esphome
