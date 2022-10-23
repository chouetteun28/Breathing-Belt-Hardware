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

  BLE.setLocalName("Arduino Slider Resistor");
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
      delay(10);

      r=analogRead(A0);

      // Filtering
      float filteredValue = 0;
      for (int i=0; i<100; i++){
        filteredValue += analogRead(A0);
        delayMicroseconds(100);
      }
      filteredValue /= 100;
      // Filtering End
      
      ble_r.writeValue(r);

      Serial.println("Reading Sensors");
      Serial.println("Force : "+ String(r)+", saved_r : "+String(ble_r.value()));
      
      Serial.println ("");
      Serial.println (filteredValue);
      
      delay(10);
    }
  }

  Serial.print("Disconnected from central: "+central.address());

}
