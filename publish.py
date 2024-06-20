import paho.mqtt.publish as publish
import time
import board
import adafruit_dht

# Initialize the DHT device, with data pin connected to:
dhtDevice = adafruit_dht.DHT11(board.D12)

# Time between readings
interval = 60

# ThingSpeak Channel ID (numeric id, not the name)
channel_ID = "2440773"  # your channel ID

# Your MQTT credentials for the Raspberry Pi
client_ID = "Ay4KKR8cIy4uAhU0DCk5MyQ"  # MQTT device ID
username = "Ay4KKR8cIy4uAhU0DCk5MyQ"  # MQTT device username
password = "4FK4U3/zWKx/yWtV26DhohxL"  # MQTT device password

# hostname of the ThingSpeak MQTT broker
host = "mqtt3.thingspeak.com"

# Define the connection type as websockets and use port 80
t_transport = "websockets"
t_port = 80

# create a topic string to publish to the ThingSpeak channel
topic = "channels/" + channel_ID + "/publish"

while True:
    try:
        # Read Temperature and Humidity Values
        temperature_c = dhtDevice.temperature
        humidity = dhtDevice.humidity
    except Exception as e:
        print(e)

    # Check if temperature_c is not None before converting
    if temperature_c is not None:
        temperature = (temperature_c * 9.0 / 5.0) + 32.0
    else:
        print("Failed to read temperature")
        continue

    payload = f"field1={temperature}&field2={humidity}"

    # Publish Sensor Values
    try:
        publish.single(topic, payload, hostname=host, transport=t_transport, port=t_port, client_id=client_ID, auth={'username':username,'password':password})
        print("Temp: {:.1f} F, Humidity: {}% ".format(temperature, humidity))
        time.sleep(interval)
    except KeyboardInterrupt:
        print("Connection ended!")
        break
    except Exception as e:
        print(e)
        time.sleep(5)
