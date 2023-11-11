# Differential pressure and flow sensor

## Mechanical

| **Powered on**                    | **Powered off**                  | **Case opened**                  |
|-----------------------------------|----------------------------------|----------------------------------|
| ![](imgs/dpm-power-on-600.png)    | ![](imgs/dpm-power-off-600.png)  | ![](imgs/case-opened-600.png)    |

# Electrical

| **Front**                                             | **Back**                                              |
|-------------------------------------------------------|-------------------------------------------------------|
| <img src="imgs/pcb-model-front-600.png" height="400"> | <img src="imgs/pcb-model-back-600.png" height="400">  |
| <img src="imgs/pcb-front-600.png" height="400">       | <img src="imgs/pcb-back-600.png" height="400">       |

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
