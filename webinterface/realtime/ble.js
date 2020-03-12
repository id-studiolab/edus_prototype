var encoderService = 'a196c876-de8c-4c47-ab5a-d7afd5ae7126';
var encoderCharacteristic = 'a196c876-de8c-4c47-ab5a-d7afd5ae7128';

var options = {};
options.optionalServices = [ encoderService ];
options.acceptAllDevices = true;

var connected = false;

var device;
var server;
var service;


async function connect() {
  try {
    console.log( 'Requesting Bluetooth Device...' );
    device = await navigator.bluetooth.requestDevice( options );

    console.log( 'Connecting to GATT Server...' );
    server = await device.gatt.connect();

    console.log( 'Getting encoder Service...' );
    service = await server.getPrimaryService( encoderService );


    const characteristic = await service.getCharacteristic( encoderCharacteristic );
    await characteristic.startNotifications();
    console.log( '> Notifications started' );
    characteristic.addEventListener( 'characteristicvaluechanged', handleNotifications );
    connected = true;
  } catch ( error ) {
    console.log( 'Argh! ' + error );
  }
}

var encoderData;

async function readValues() {
  try {
    const characteristic = await service.getCharacteristic( encoderCharacteristic );
    const value = await characteristic.readValue();
    encoderData = value.getInt8( 0 );
    console.log( encoderData );
  } catch ( error ) {
    console.log( 'Argh! ' + error );
  }
}

function handleNotifications( event ) {
  let value = event.target.value;
  if ( event.target.uuid == encoderCharacteristic ) {
    encoderData = value.getInt8( 0 );
  }
}