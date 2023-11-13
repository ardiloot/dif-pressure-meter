# Differential pressure (and air flow) meter

To measure and adjust air flow in air valves, diffusers or grilles accurate measurement of small pressure drop (differential pressure) is often required (usually in the range of -50 to 50 Pa). Pressure drop is used to calculate flow through air valve by using formula

$$q = k \cdot \sqrt{Δp}$$

where $q$ is flow (L/s), $Δp$ is measured pressure drop (Pa) and $k$ is the flow coefficient (k-factor) that can be found in the datasheet of the air valve (depends on the valve setting).

For example, Flakt Group air valves have nice illustrations :

![](imgs/flakt-kso-600.png)

Source: [KSO air valve product page](https://www.flaktgroup.com/en/products/air-management-atds/air-valves/exhaust-air-valves/kso-exhaust-air-valve/)


However, it is hard to find measurement equipment for such small pressures (±50 Pa). There are some professional options (e.g Fluke 922), but they cost around quite a bit.

Here, as an alternative, DIY handheld battery-powered differential pressure meter is built. It is based on [Sensirion SDP810-500Pa](https://www.sensirion.com/products/catalog/SDP810-500Pa) differential pressure sensor (measurement range ±500 Pa, zero point accuracy 0.1 Pa). As a brain, ESP32-WROOM-32E chip is used. Firmware is mainly based on [ESPHome](https://esphome.io/).

# Electrical

[Electrical design](electrical/) is made using KiCad 7.0 software. The main components are:
1. ESP32-WROOM-32E + USB to UART converter (`CH340C`)  - main compute
2. Sensirion SDP810-500Pa - I2C differential pressure sensor
3. 1.8" LCD ST7735S - screen
4. PKLCS1212E40A1-R1 - Buzzer for making sounds
5. 5 way buttons for navigation
6. Battery charging management (`TP4056`)

For details see:
* [Schematic](electrical/schematic.pdf)
* [Bill of materials (BOM)](electrical/bom.pdf)
* [Gerber manufacturing files for JLCPCB](electrical/gerbers/)

| **Front**                                                         | **Back**                                                          |
|-------------------------------------------------------------------|-------------------------------------------------------------------|
| <img src="imgs/pcb-model-front-600.png" height="75%" width="75%"> | <img src="imgs/pcb-model-back-600.png" height="75%" width="75%">  |
| <img src="imgs/pcb-front-600.png" height="75%" width="75%">       | <img src="imgs/pcb-back-600.png" height="75%" width="75%">        |

## Mechanical

The mechanical design is done using Onshape: [project files](https://cad.onshape.com/documents/3a0444f7efaf570fd867d2e7/w/0f2dabb567175f38ff258f90/e/ec28925e598c09357985fe26?renderMode=0&uiState=655145dc99c22b680c1f923b). The case is mostly 3D printed:
1. Front ([STL](mech/front.stl))
2. Back ([STL](mech/front.stl))
3. Screen spacers ([STL](mech/spacer.stl))
4. 1 mm thick Plexiglass for screen protection (37 mm x 50 mm)
5. M2x5 screws for screen mounting
6. DIN912 M3x12 screws for case


Printer parameters:
* Printer: Ender-3 V2 (Trianglelab DDB Extruder V2.0 + BLTouch)
* Slicer: Ultimaker Cura 4.11.0
* Filamant: eSun PLA+ 1.75mm 1kg Black
* Profile: Standard Quality
* Printing temperature: 210 °C
* Build Plate Temperature: 60 °C
* Initial layer horizontal expansion: -0.1 mm
* Generate support: yes

| **Powered on**                    | **Powered off**                  | **Case opened**                  |
|-----------------------------------|----------------------------------|----------------------------------|
| ![](imgs/dpm-power-on-600.png)    | ![](imgs/dpm-power-off-600.png)  | ![](imgs/case-opened-600.png)    |


## Installing firmware

Firmware is developed using [ESPHome](https://esphome.io/) with additional custom component for calculating flow. The [configuration file](dif-pressure-meter.yaml) can be edited to include more air valve types or to make any other changes if needed

To install precompiled firmware:
1. Download latest firmware (`.bin` file) from [releases page](https://**github**.com/ardiloot/dif-pressure-meter/releases/).
2. Connect with USB cable
3. Use https://web.esphome.io/ to prepare device for the first use and to flash the firmware

## Usage

![](imgs/dpm-usage.png)

## Development setup

This paragraph contains instructions for development of the code and can be skipped if not relevant.

To begin, create virtual environment (Python 3.11), activate and install requirements:

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
