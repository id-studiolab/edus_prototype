var IMUData = [ , 0, 0, 0, 0, 0 ];

var imuService = '00110011-4455-6677-8899-aabbccddeeff';

var imucharacteristic = [
  'a196c876-de8c-4c47-ab5a-d7afd5ae7128',
  'a196c876-de8c-4c47-ab5a-d7afd5ae7129',
  'a196c876-de8c-4c47-ab5a-d7afd5ae7130',
  'a196c876-de8c-4c47-ab5a-d7afd5ae7131',
  'a196c876-de8c-4c47-ab5a-d7afd5ae7132',
  'a196c876-de8c-4c47-ab5a-d7afd5ae7133',
]

var options = {};
options.optionalServices = [ imuService ];
options.acceptAllDevices = true;

var connected = false;

var device;
var server;
var service;


var accelerometerData = [];

async function connect() {
  try {
    console.log( 'Requesting Bluetooth Device...' );
    device = await navigator.bluetooth.requestDevice( options );

    console.log( 'Connecting to GATT Server...' );
    server = await device.gatt.connect();

    console.log( 'Getting accelerometer Service...' );
    service = await server.getPrimaryService( imuService );

    for ( var i = 0; i < 6; i++ ) {
      const characteristic = await service.getCharacteristic( imucharacteristic[ i ] );
      await characteristic.startNotifications();
      console.log( '> Notifications started' );
      characteristic.addEventListener( 'characteristicvaluechanged', handleNotifications );
      connected = true;
    }
  } catch ( error ) {
    console.log( 'Argh! ' + error );
  }
}


async function readValues() {
  try {

    for ( var i = 0; i < 6; i++ ) {
      const characteristic = await service.getCharacteristic( imucharacteristic[ i ] );
      const value = await characteristic.readValue();
      accelerometerData[ i ] = value.getInt8( 0 );
    }
    console.log( accelerometerData );
  } catch ( error ) {
    console.log( 'Argh! ' + error );
  }
}


function handleNotifications( event ) {
  let value = event.target.value;
  for ( var i = 0; i < 6; i++ ) {
    if ( event.target.uuid == imucharacteristic[ i ] ) {
      accelerometerData[ i ] = value.getInt8( 0 );
    }
  }

  // console.log(event.target.uuid);
  // console.log(value.getInt8(0));
  //let a = [];
  // Convert raw data bytes to hex values just for the sake of showing something.
  // In the "real" world, you'd use data.getUint8, data.getUint16 or even
  // TextDecoder to process raw data bytes.
  // for (let i = 0; i < value.byteLength; i++) {
  //   a.push('0x' + ('00' + value.getUint8(i).toString(16)).slice(-2));
  // }
  // log('> ' + a.join(' '));
}

// function readValues(){
// 	console.log(characteristics[0]);
// 	if (characteristics[0]!=null){
// 		console.log(characteristics[0].readValue());
// 	}
// }