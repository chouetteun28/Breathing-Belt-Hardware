#include <ArduinoBLE.h>

int p, q;

BLEService customService("180C");


BLEIntCharacteristic ble_p("6eea5885-ed94-4731-b81a-00eb60d93b49", BLERead | BLENotify);
BLEIntCharacteristic ble_q("03d0cffe-bd7f-4d17-ba39-86e57adb5ddd", BLERead | BLENotify);
//BLEIntCharacteristic ble_z("c8411116-479e-4bc7-a216-1e3c22d32809", BLERead | BLENotify);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(38400);
  while(!Serial && millis()<100){;}
  if (!BLE.begin()){
    Serial.println("BLE failed to Initiate");
    delay(500);
    while (1) {;} 
  }

  BLE.setLocalName("Arduino Environment Joystick");
  BLE.setAdvertisedService(customService);

  customService.addCharacteristic(ble_p);
  customService.addCharacteristic(ble_q);
  //customService.addCharacteristic(ble_z);

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

      p=analogRead(A0);
      q=analogRead(A1);
      //z=digitalRead(5);

      ble_p.writeValue(p);
      ble_q.writeValue(q);
      //ble_z.writeValue(z);

      Serial.println("Reading Sensors");
      Serial.println("Force : "+ String(p)+", saved_p : "+String(ble_p.value()));
      Serial.println("Force : "+ String(q)+", saved_q : "+String(ble_q.value()));
      //Serial.println("y : "+String(y));
      //Serial.println("z : "+String(z));
      delay(10);
    }
  }

  Serial.print("Disconnected from central: "+central.address());

}
