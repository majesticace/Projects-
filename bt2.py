import BlynkLib

# Replace 'DwPUJCN5puGU67UgogFRh' with your actual Blynk Auth Token
BLYNK_AUTH_TOKEN = 'hWRSpWldCWv-ha5aqmasPkbObCIvxNGX'

# Initialize Blynk
blynk = BlynkLib.Blynk(BLYNK_AUTH_TOKEN)

# Function to handle virtual pin 2 write events
@blynk.on("V2")
def v2_write_handler(value):
    print(f'Received value on Virtual Pin 2: {value[0]}')

# Function to print a message when Raspberry Pi is connected to Blynk
@blynk.on("connected")
def blynk_connected():
    print("Raspberry Pi connected to Blynk")
    # Read the initial value of virtual pin 2
    blynk.virtual_read(2)

# Run Blynk event loop
blynk.run()