


```bash
python -m venv venv
# activate venv
pip install -r requirements.txt
```

```bash
pip-compile --upgrade -o requirements.txt requirements.in
pip-sync
```

```bash
esphome run dif-pressure-meter.yaml
esphome run dif-pressure-meter.yaml --device dif-pressure-meter.local
```

