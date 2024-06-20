{\rtf1\ansi\ansicpg1252\cocoartf2761
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 from flask import Flask, render_template\
import RPi.GPIO as GPIO\
import Adafruit_DHT\
\
app = Flask(__name__)\
\
# Set up GPIO pins\
GPIO.setmode(GPIO.BCM)\
fan_pin = 17  # GPIO pin connected to the fan\
heat_pin = 18  # GPIO pin connected to the heating pads\
\
GPIO.setup(fan_pin, GPIO.OUT)\
GPIO.setup(heat_pin, GPIO.OUT)\
\
# DHT sensor setup\
DHT_SENSOR = Adafruit_DHT.DHT11\
DHT_PIN = 4  # GPIO pin connected to the DHT sensor\
\
# Function to read temperature and humidity from DHT sensor\
def read_sensor():\
    humidity, temperature = Adafruit_DHT.read_retry(DHT_SENSOR, DHT_PIN)\
    return humidity, temperature\
\
@app.route("/")\
def index():\
    humidity, temperature = read_sensor()\
    return render_template("index.html", temperature=temperature, humidity=humidity)\
\
@app.route("/heat")\
def heat():\
    GPIO.output(heat_pin, GPIO.HIGH)\
    return "Heating activated"\
\
@app.route("/cool")\
def cool():\
    GPIO.output(fan_pin, GPIO.HIGH)\
    return "Cooling activated"\
\
if __name__ == "__main__":\
    app.run(debug=True, host="0.0.0.0")\
}