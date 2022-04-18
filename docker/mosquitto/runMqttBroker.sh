mkdir -p $(pwd)/data
mkdir -p $(pwd)/log

docker run --rm -d \
-p 1883:1883 -p 9001:9001 \
-v $(pwd)/mosquitto.conf:/mosquitto/config/mosquitto.conf \
-v $(pwd)/data:/mosquitto/data \
-v $(pwd)/log:/mosquitto/log \
--name sonoff_mosquitto \
eclipse-mosquitto