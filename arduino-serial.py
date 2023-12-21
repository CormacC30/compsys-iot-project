import os
import serial
import firebase_admin
from firebase_admin import credentials, db, storage
import json
from datetime import datetime

from decouple import config

FIREBASE_CREDENTIALS = config("FIREBASE_CREDENTIALS")
FIREBASE_DATABASE_URL = config("FIREBASE_DATABASE_URL")
SERIAL_PORT = config("SERIAL_PORT")

#initialize firebase admin sdk
cred = credentials.Certificate(FIREBASE_CREDENTIALS)
firebase_admin.initialize_app(cred, {"databaseURL": FIREBASE_DATABASE_URL})


from datetime import datetime

def update_firebase(data):
    try:
        data["timestamp"] = datetime.now().isoformat()
        ref = db.reference("sensors")

        new_data_ref = ref.push()
        new_data_ref.set(data)
        print("Data stored in Firebase.")
    except Exception as e:
        print(f"Error updating Firebase: {e}")

# Replace '/dev/ttyUSB0' with your Arduino's serial port
serial_port = serial.Serial(SERIAL_PORT, 115200, timeout=1)


try:
    while True:
        # Read data from Arduino
        sensorData = serial_port.readline().decode('utf-8').strip()
        print(f'Received data: {sensorData}')

        # Parse received JSON data
        try:
            data = json.loads(sensorData)
            print("Received Data: ", data)

            # Update Firebase with the received data
            update_firebase(data)
        except json.JSONDecodeError as json_error:
            print(f"Error decoding JSON: {json_error}")

except KeyboardInterrupt:
    print('Serial communication stopped.')
finally:
    serial_port.close()
