# edus_prototype

Please find in this repository all the code of the edus prototypes.

## Prototype A - 2 9axis accelerometers with ble.

#### how does it work
- This prototype uses two BNO055 modules by adafruit to measure wrist movements.
- Rotation data from the two sensors is streamed via bluetooth low energy and analyzed by a client side web application written in p5.js.
- The application is available here https://id-studiolab.github.io/edus_prototype/webinterface/realtime/.
- No data are saved or analyzed when the device is not connected to the browser.
- Once the user completes the exercise the datas about the performances are saved into json format to [a cloud service](http://api.jsonbin.io/b/5bf2c80773474c2f8d989838)
- Collected data are then visualized [here](https://id-studiolab.github.io/edus_prototype/webinterface/longLog/)


#### How to use it
- Open [this link](https://id-studiolab.github.io/edus_prototype/webinterface/realtime/) in your browser. Only Chrome!
- Wear the device.
	- make sure that the wires come out of the bracelet from the side of your hand.
	- make sure you are holding the hand piece with your palm touching the application
- Turn on the device -> a red light will start blinking.
- Click **connect** button to search for the device.
- From the dropdown menu select EDUS and click pair
- wait for the device to connect.
- Put the arm into rest position and press start
- Move your arm up and down to follow the curve with the ball.
- Do the exercise until the end.

#### What do I see
- the color of the ball fades when the user is not in the correct neutral position.
- The graph under the main curve shows how much error you are producing.
- the progress bar indicates how much exercise you have left.


Web interface accessible here
https://id-studiolab.github.io/edus_prototype/webinterface/realtime/

Once the data are saved, they can be visualized Here
https://id-studiolab.github.io/edus_prototype/webinterface/longLog/
