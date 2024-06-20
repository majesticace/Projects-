import time
import board
import adafruit_dht

# Initialize the DHT device, with data pin connected to:
dhtDevice = adafruit_dht.DHT11(board.D12)

# Print the temperature and humidity values to the terminal
temperature_c = dhtDevice.temperature
humidity = dhtDevice.humidity

# Convert the temperature to Fahrenheit
temperature_f = temperature_c * 9.0 / 5.0 + 32.0

print(f"Temp: {temperature_c:.1f}C, {temperature_f:.1f}F, Humidity: {humidity}%")
