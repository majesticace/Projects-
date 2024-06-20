import RPi.GPIO as GPIO
import requests
import time

# Pin definitions
led_pin = 16

# Use "GPIO" pin numbering
GPIO.setmode(GPIO.BCM)

# Set LED pin as output
GPIO.setup(led_pin, GPIO.OUT)

# Set interval between requests
interval = 10

# Insert the information for the talkback service
talkback_ID = "51448"  # replace with your talkback ID
talkback_API = "H3Z70VWNS28QJMYS"  # replace with your talkback API key

# Note: Make sure you get the API key belonging to the TalkBack service, not any of the others
url = f"https://api.thingspeak.com/talkbacks/{talkback_ID}/commands.json?api_key={talkback_API}"

try:
    while True:
        time.sleep(interval)
        response = requests.get(url)
        data = response.json()

        if data == []:
            print("No alerts")
        else:
            data = data[0]  # Take the first element in data list which is a dictionary
            if data.get("command_string") == "TURN_ON":
                print("LED is ON")
                GPIO.output(led_pin, GPIO.HIGH)
                requests.delete(url)
            if data.get("command_string") == "TURN_OFF":
                print("LED is OFF")
                GPIO.output(led_pin, GPIO.LOW)
                requests.delete(url)

except KeyboardInterrupt:
    print("Connection ended!")

GPIO.cleanup()
