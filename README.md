# Differential pressure (and air flow) meter

To measure and adjust air flow in air valves, diffusers or grilles accurate measurement of small pressure drop (differential pressure) is often required (usually in the range of -50 to 50 Pa). Pressure drop is used to calculate flow through air valve by using formula

$$q = k \cdot \sqrt{Δp}$$

where $q$ is flow (`L/s`), $Δp$ is measured pressure drop (`Pa`) and $k$ is the flow coefficient (k-factor) that can be found in the datasheet of the air valve (depends on the valve setting).

For example, Flakt Group air valves have nice illustrations :

![](imgs/flakt-kso-600.png)

Source: [KSO air valve product page](https://www.flaktgroup.com/en/products/air-management-atds/air-valves/exhaust-air-valves/kso-exhaust-air-valve/)


However, it is hard to find measurement equipment for such small pressures (±50 Pa). There are expensive professional options (e.g Fluke 922), but they cost around quite a bit.

Here, as an alternative, DIY differential pressure meter is built. It is based on [Sensirion SDP810-500Pa](https://www.sensirion.com/products/catalog/SDP810-500Pa) differential pressure sensor (measurement range ±500 Pa, zero point accuracy 0.1 Pa). As a brain, ESP32-WROOM-32E chip is used. Firmware is mainly based on [ESPHome](https://esphome.io/).


## Mechanical

| **Powered on**                    | **Powered off**                  | **Case opened**                  |
|-----------------------------------|----------------------------------|----------------------------------|
| ![](imgs/dpm-power-on-600.png)    | ![](imgs/dpm-power-off-600.png)  | ![](imgs/case-opened-600.png)    |

# Electrical

| **Front**                                                         | **Back**                                                          |
|-------------------------------------------------------------------|-------------------------------------------------------------------|
| <img src="imgs/pcb-model-front-600.png" height="75%" width="75%"> | <img src="imgs/pcb-model-back-600.png" height="75%" width="75%">  |
| <img src="imgs/pcb-front-600.png" height="75%" width="75%">       | <img src="imgs/pcb-back-600.png" height="75%" width="75%">        |

## Installing firmware

1. Download latest firmware (`.bin` file) from [releases page](https://**github**.com/ardiloot/dif-pressure-meter/releases/).
2. Connect with USB cable
3. Use https://web.esphome.io/ to prepare device for the first use and to flash the firmware

## Development setup

Create virtual environment (Python 3.11), activate and install requirements:

```bash
python -m venv venv
source venv/bin/activate # or on Windows: .\venv\Scripts\activate
pip install -r requirements.txt
```

To compile and upload firmware, run:

```bash
esphome run dif-pressure-meter.yaml
```

Lint checks:

```bash
yamllint $(git ls-files '*.yaml')
pylint $(git ls-files '*.py')
clang-format-13 --dry-run --Werror $(git ls-files '*.cpp' '*.h')
```

If requirements change or need to be updated, run:

```bash
pip-compile --upgrade -o requirements.txt requirements.in
pip-sync
```
