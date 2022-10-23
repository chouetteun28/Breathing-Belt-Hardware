#include <ArduinoBLE.h>

int r;

BLEService customService("180C");


BLEIntCharacteristic ble_r("6eea5885-ed94-4731-b81a-00eb60d93b49", BLERead | BLENotify);


void setup() {
  // put your setup code here, to run once:

  Serial.begin(38400);
  while(!Serial && millis()<100){;}
  if (!BLE.begin()){
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1) {;} 
  }

  BLE.setLocalName("Arduino Potentiometer");
  BLE.setAdvertisedService(customService);

  customService.addCharacteristic(ble_r);

  BLE.addService(customService);

  BLE.advertise();
  Serial.println("Bluetooth devic is now active, waiting for connections...");
}



void loop() {
  // put your main code here, to run repeatedly:

  BLEDevice central = BLE.central();
  if (central)
  {
    Serial.print("Connected to central : ");
    Serial.println(central.address());
    while(central.connected())
    {
      BLE.poll();
      delay(50);

      r=analogRead(A0);
      ble_r.writeValue(r);

      Serial.println("Reading Sensors");
      Serial.println("Force : "+ String(r)+", saved_r : "+String(ble_r.value()));
      delay(10);
    }
  }

  Serial.print("Disconnected from central: "+central.address());

}
