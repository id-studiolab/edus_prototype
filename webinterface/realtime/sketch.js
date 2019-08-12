var lastMillis = 0;
var interval = 500;

var accelerometerResetValues = [];
var accelerometerDisplacementsValues = [];

var wristVector;
var handVector;

var wristRotation = 0;
var handRotation = 0;

var varErrorX;
var varErrorY;
var varErrorZ;

var distanceFromPerfectAlignment = 0;
var distanceFromLine = 0;

var minAngle = 0;
var maxAngle = 25;


var xspacing = 16; // Distance between each horizontal location
var w; // Width of entire wave
var theta = 0.0; // Start angle at 0
var amplitude = 100; // Height of wave
var period = 1000.0; // How many pixels before the wave repeats
var dx; // Value for incrementing x
var yvalues; // Using an array to store height values for the wave

var ellipseY;

var canvas;

var numberOfMovementsToPerform = 20;
var movementsDone = 0;

var errorArray = [];

var gameStatus = 0;

var resetButton;
var connectButton;


var errorDataXSToSave = .4;
var lastTimeSaved = 0;

var gameAdvancement = 0;

var activityData = {}

var dataFromCloud;

function preload(){
	getSJsonData();
}

function setup() {
	canvas = createCanvas(windowWidth, windowHeight);
	console.log(width, height);

	w = width + 16;
	dx = (TWO_PI / period) * xspacing;
	yvalues = new Array(floor(w / xspacing));

	ellipseY = height / 2;

	resetButton = createButton('start');
	resetButton.mousePressed(resetPosition);
	resetButton.hide();
	resetButton.center()
	connectButton = createButton('connect');
	connectButton.mousePressed(connect);
	connectButton.center()
	//resetButton.hide();


	textAlign(CENTER, CENTER);

}

function draw() {
	background('#ffdab9');

	if (gameStatus == 0) {
		text("connect your device", width / 2, height / 2 - 50);
		if (connected) {
			gameStatus = 1;
			connectButton.hide();
		}

	} else if (gameStatus == 1) {
		resetButton.show();
		text("place the arm into rest position", width / 2, height / 2 - 50);
	} else if (gameStatus == 2) {
		calcWave();
		renderWave();
		drawTarget();
		calculateDisplacement();
		calculateAngles();
		calculateError();
		drawError();
		drawArm();
		drawProgressBar();
		drawErrorGraph();

		if (gameAdvancement > 1) {
			gameStatus = 3;
			activityData.time = Date();
			activityData.errorLog = errorArray;
			var totalErro = 0;
			for (var i = 0; i < errorArray.length; i++) {
				totalErro += errorArray[i];
			}
			activityData.totalErro = totalErro;
			saveJsonData(activityData);
			console.log(activityData);
		}
	} else if (gameStatus == 3) {
		text("well Done!", width / 2, height / 2);
		drawErrorGraph();
	}

}

function saveJsonData(data) {
	dataFromCloud.push(data);
	console.log(dataFromCloud);

	var url = 'https://api.jsonbin.io/b/5bf2c80773474c2f8d989838';
	httpDo(url, {
		method: "put",
		body: JSON.stringify(dataFromCloud),
		headers: new Headers({
			'Content-Type': 'application/json',
			'secret-key': '$2a$10$fbal4IkI0rAQHd7rN1nyLO9CCSzHVatN4RBZikSgiJ8rZt9d1Y.OC',
		})
	}, function(data) {
		console.log(data);
	});
}

function getSJsonData() {
	var url = 'https://api.jsonbin.io/b/5bf2c80773474c2f8d989838/latest  ';
	httpDo(url, {
		method: "get",
		headers: new Headers({
			'Content-Type': 'application/json',
			'secret-key': '$2a$10$fbal4IkI0rAQHd7rN1nyLO9CCSzHVatN4RBZikSgiJ8rZt9d1Y.OC',
		})
	}, function(data) {
		console.log(data);
		dataFromCloud=data;
	});
}


function drawErrorGraph() {
	//console.log(errorArray.length);
	var progress = map(gameAdvancement, 0, 1, 0, width - 100);
	var dataSpacing = progress / errorArray.length;
	stroke('grey');
	noFill();
	strokeWeight(1);
	beginShape();
	for (var i = 0; i < errorArray.length; i++) {
		vertex(i * dataSpacing + 50, height - 60 - errorArray[i]);
	}
	endShape();

}

function drawErrorGraphCurved() {
	var progress = map(gameAdvancement, 0, 1, 0, 360);
	var dataSpacing = progress / errorArray.length;
	stroke(0);
	noFill();
	strokeWeight(1);
	push();
	translate(100, 100);
	beginShape();
	for (var i = 0; i < errorArray.length; i++) {
		push();
		//	console.log(dataSpacing*i);
		rotate(radians(dataSpacing * i));
		vertex(0, height / 4 * 3 - errorArray[i]);
		pop();
	}
	endShape();
	pop();
}

function drawProgressBar() {
	movementsDone = theta / (2 * Math.PI);
	//console.log(movementsDone);
	stroke('#f08080');
	fill(255);
	rect(50, height - 50, width - 100, 10, 20);
	fill('#f08080')
	stroke('#f08080');
	var pbarSize = map(movementsDone, 0, numberOfMovementsToPerform, 0, width - 100);
	gameAdvancement = map(movementsDone, 0, numberOfMovementsToPerform, 0, 1)
	rect(50, height - 50, pbarSize, 10, 20);

}

function resetPosition() {
	accelerometerResetValues = accelerometerData.slice();
	//console.log(accelerometerResetValues);
	theta = 0;
	movementsDone = 0;

	gameStatus = 2;
	resetButton.hide();
}

function calculateDisplacement() {
	for (var i = 0; i < 6; i++) {
		accelerometerDisplacementsValues[i] = accelerometerData[i] - accelerometerResetValues[i];
		//console.log(accelerometerData[i], accelerometerResetValues[i]);
	}
}

calculateVectors = function(yaw, pitch) {
	var x = cos(radians(yaw)) * cos(radians(pitch))
	var y = sin(radians(yaw)) * cos(radians(pitch))
	var z = sin(radians(pitch))

	var vector = createVector(x, y, z);
	return vector;
}


function drawTarget() {
	var ellipseBrightness = map(distanceFromPerfectAlignment, 0, 200, 255, 0);

	var desiredPosition = map(wristRotation, minAngle, maxAngle, amplitude * 2, 0);
	desiredPosition = height / 2 + desiredPosition - amplitude;

	noStroke();
	fill(100, 200, 200, ellipseBrightness);
	ellipseY = lerp(ellipseY, desiredPosition, 0.05);

	ellipse(width / 2, ellipseY, 80, 80);
}

function drawArm() {
	translate(0, 400);
	push();
	rotate(radians(360 - wristRotation));
	fill("green");
	line(0, 0, 100, 0);
	push();
	translate(100, 0);
	fill("blue");
	line(0, 0, 100, 0);
	pop();
	pop();
}

function calculateAngles() {
	wristVector = calculateVectors(accelerometerDisplacementsValues[5], accelerometerDisplacementsValues[4]);
	wristVector.setMag(100);

	handVector = calculateVectors(accelerometerDisplacementsValues[2], accelerometerDisplacementsValues[1])
	handVector.setMag(100);
	var wristHand = p5.Vector.add(handVector, wristVector);

	wristRotation = wristVector.x;
	wristRotation = map(wristRotation, 0, 100, 90, 0);

	handRotation = handVector.x;
	handRotation = map(handRotation, 0, 100, 90, 0);

	console.log(wristRotation,handRotation);
}

function calculateError() {
	varErrorX = abs(wristVector.x - handVector.x);
	varErrorY = abs(wristVector.y - handVector.y);
	varErrorZ = abs(wristVector.z - handVector.z);
	distanceFromPerfectAlignment = varErrorX + varErrorY + varErrorZ;

	var centerCurveY = yvalues[Math.round(yvalues.length / 2)] + height / 2;
	//ellipse(width/2,centerCurveY,50,50 );
	distanceFromLine = abs(ellipseY - centerCurveY);

	if (millis() - lastTimeSaved > errorDataXSToSave * 1000) {
		errorArray.push(distanceFromLine + distanceFromPerfectAlignment);
		lastTimeSaved = millis();
		//console.log(distanceFromLine);
	}

}

function drawError() {
	stroke('yellow');
	//line(0, 0, 0, abs(100 - wristVector.x));

	stroke('red');
	line(0, 30, varErrorX, 30);
	stroke('green');
	line(0, 40, varErrorY, 40);
	stroke('blue');
	line(0, 50, varErrorZ, 50);

}

function calcWave() {
	// Increment theta (try different values for
	// 'angular velocity' here)
	var advancementSpeed = map(distanceFromPerfectAlignment, 0, 200, 0.02, 0.001)

	if (distanceFromLine < 50) {
		theta += advancementSpeed;
	}

	//console.log(theta);
	// For every x value, calculate a y value with sine function
	var x = theta;
	for (var i = 0; i < yvalues.length; i++) {
		yvalues[i] = sin(x) * amplitude;
		x += dx;
	}
}

function renderWave() {
	// A simple way to draw the wave with an ellipse at each location
	noFill();
	stroke(255, 255, 0);
	strokeWeight(3);

	beginShape();
	curveVertex(0, height / 2 + yvalues[0]);

	for (var x = 0; x < yvalues.length; x++) {
		curveVertex(x * xspacing, height / 2 + yvalues[x]);
		//ellipse(x*xspacing, height/2+yvalues[x], 16, 16);
	}
	endShape();
}

window.onresize = function() {
	var w = window.innerWidth;
	var h = window.innerHeight;
	canvas.size(w, h);
	width = w;
	height = h;
};
