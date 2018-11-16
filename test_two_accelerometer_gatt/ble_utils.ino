void setupGatt() {

  ble.println("AT+GATTCLEAR");
  ble.waitForOK();

  ble.print("AT+GATTADDSERVICE="   );
  ble.print("UUID128="             );
  ble.print(IMU_SERVICE_UUID);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_1_x_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();
  
  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_1_y_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_1_z_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_2_x_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();
  
  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_2_y_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTADDCHAR="       );
  ble.print("UUID128="              );
  ble.print(IMU_2_z_CHARACTERISTIC_UUID); ble.print(',');
  ble.print("PROPERTIES=0x10"          ); ble.print(',');
  ble.print("MIN_LEN=1"                ); ble.print(','); 
  ble.print("DATATYPE=3"               ); ble.print(',');
  ble.print("VALUE=0"             ); 
  ble.println();
  ble.waitForOK();

}

void initBluetooth() {
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));
  if ( !ble.begin(VERBOSE_MODE) ) {
    Serial.println(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }

  Serial.println(F("Performing a factory reset: "));
  if ( ! ble.factoryReset() ) {
    Serial.println(F("Couldn't factory reset"));
  }

  ble.echo(true);

  //set the name of the device
  ble.print("AT+GAPDEVNAME="        );
  ble.print(LOCAL_NAME               );
  ble.println();

  //makes the device connectable;
  ble.print("AT+GAPCONNECTABLE=1");
  ble.println();
  ble.waitForOK();

  Serial.println("Requesting Bluefruit info:");
  ble.info();
  ble.waitForOK();

  ble.print("AT+GATTCLEAR");
  ble.println();
  ble.waitForOK();

  if ( !ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) ) {
    Serial.println( F("Callback requires at least 0.7.0") );
  }
}

void advertizeGatt() {
  //start advertizing the properties
  ble.print("AT+GAPSTARTADV");
  ble.println();
  ble.waitForOK();
}

void getGATTList() {
  ble.print("AT+GATTLIST"   );
  ble.println();
  ble.waitForOK();
}

void connected(void) {
  Serial.println( F("Connected") );
}

void disconnected(void) {
  Serial.println( F("Disconnected") );
}

void updateAccelerometerValues(int x1, int y1, int z1, int x2, int y2, int z2  ) {
  ble.print("AT+GATTCHAR=");
  ble.print(1);
  ble.print(',');
  ble.print(x1);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTCHAR=");
  ble.print(2);
  ble.print(',');
  ble.print(y1);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTCHAR=");
  ble.print(3);
  ble.print(',');
  ble.print(z1);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTCHAR=");
  ble.print(4);
  ble.print(',');
  ble.print(x2);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTCHAR=");
  ble.print(5);
  ble.print(',');
  ble.print(y2);
  ble.println();
  ble.waitForOK();

  ble.print("AT+GATTCHAR=");
  ble.print(6);
  ble.print(',');
  ble.print(z2);
  ble.println();
  ble.waitForOK();

}

