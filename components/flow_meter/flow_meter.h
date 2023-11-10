#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/macros.h"
#include "esphome/core/helpers.h"
#include "esphome/components/sensor/sensor.h"

#include "Statistic.h"
#include <vector>

namespace esphome {
namespace flow_meter {

enum struct State {
  IDLE,
  AVERAGING,
  STOPPED
};

class Valve {
  public:
    Valve(const std::string& size, const std::vector<float>& s, const std::vector<float>& k);
    bool next_valve_setting();
    bool previous_valve_setting();
    const std::string& get_size() const { return size_; }
    float get_setting() const { return setting_; }

    float interpolate_k() {
      if (setting_ <= s_.front())
        return k_.front();

      if (setting_ >= s_.back())
        return k_.back();

      // Seek for interval
      for (int i = 1; i < s_.size(); i++) {
        if (s_[i - 1] <= setting_ && s_[i] >= setting_) {
          const float slope = (k_[i] - k_[i - 1]) / (s_[i] - s_[i - 1]);
          const float interpolated_k = k_[i - 1] + slope * (setting_ - s_[i - 1]);
          return interpolated_k;
        }
      }
      return k_.back();
    }

    float calc_flow(float dp){
      
      if (std::fabs(dp) < 0.1f)
        return 0.0f; // Remove noise near zero pressure

      const float kv = interpolate_k();
      if (dp < 0.0f){
        return -kv * std::sqrt(-dp);
      } else {
        return kv * std::sqrt(dp);
      }
    }
    
  protected:
    std::string size_;
    std::vector<float> s_;
    std::vector<float> k_;
    float setting_ {0.0f};
};


class ValveType {
public:
  void set_name(const std::string& name);
  void add_size(const std::string& size, const std::vector<float>& s, const std::vector<float>& k);
  void next_valve_size();
  void previous_valve_size();
  const std::string& get_name() const { return name_; }
  const std::vector<Valve>& get_sizes() const { return sizes_; }
  std::vector<Valve>& get_sizes() { return sizes_; }
  Valve& active_valve() { return sizes_.at(size_index_); }
  const Valve& active_valve() const { return sizes_.at(size_index_); }

protected:
  std::string name_;
  std::vector<Valve> sizes_;
  int size_index_ {0};
};


class FlowMeter : public Component {
 public:
  void loop() override;
  float get_setup_priority() const override;
  void dump_config() override;

  void add_valve_type(ValveType* valve_type);
  void set_flow_sensor(sensor::Sensor* sensor) { flow_sensor_ = sensor; }
  void set_pressure_avg_sensor(sensor::Sensor* sensor) { pressure_avg_sensor_ = sensor; }
  void set_pressure_error_sensor(sensor::Sensor* sensor) { pressure_error_sensor_ = sensor; }
  void set_flow_avg_sensor(sensor::Sensor* sensor) { flow_avg_sensor_ = sensor; }
  void set_flow_error_sensor(sensor::Sensor* sensor) { flow_error_sensor_ = sensor; }

  void update(const float pressure);

  void goto_next_state();
  State get_state() const { return state_; }
  
  const std::string& active_valve_type_name();
  void next_valve_type();
  void previous_valve_type();
  
  const std::string& active_valve_size() const;
  void next_valve_size();
  void previous_valve_size();

  float active_valve_setting() const;
  bool next_valve_setting();
  bool previous_valve_setting();

  uint32_t get_sample_count() const { return pressure_stats_.count(); }

 protected:
  std::vector<ValveType*> valve_types_;
  int valve_type_index_ {0};

  State state_ {State::IDLE};
  statistic::Statistic<float, uint32_t, true> pressure_stats_;

  sensor::Sensor* flow_sensor_ {nullptr};
  sensor::Sensor* pressure_avg_sensor_ {nullptr};
  sensor::Sensor* pressure_error_sensor_ {nullptr};
  sensor::Sensor* flow_avg_sensor_ {nullptr};
  sensor::Sensor* flow_error_sensor_ {nullptr};

  ValveType& active_valve_type() { return *valve_types_.at(valve_type_index_); }
  const ValveType& active_valve_type() const { return *valve_types_.at(valve_type_index_); }
  Valve& active_valve() { return active_valve_type().active_valve(); }
  const Valve& active_valve() const { return active_valve_type().active_valve(); }

};

}  // namespace debug
}  // namespace esphome