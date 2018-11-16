var lastMillis = 0;
var interval = 500;

var accelerometerResetValues = [];
var accelerometerDisplacementsValues = [];

var wristVector;
var handVector;

var wristRotation=0;
var handRotation=0;

var varErrorX;
var varErrorY;
var varErrorZ;

var distanceFromPerfectAlignment=0;
var distanceFromLine=0;

var minAngle=0;
var maxAngle=25;


var xspacing = 16;    // Distance between each horizontal location
var w;                // Width of entire wave
var theta = 0.0;      // Start angle at 0
var amplitude = 100; // Height of wave
var period = 1000.0;   // How many pixels before the wave repeats
var dx;               // Value for incrementing x
var yvalues;  // Using an array to store height values for the wave

var ellipseY;

var canvas;


function setup() {
	canvas = createCanvas(displayWidth,displayHeight);

	w = width+16;
dx = (TWO_PI / period) * xspacing;
yvalues = new Array(floor(w/xspacing));

ellipseY=height/2;
}


function draw() {
	background('#ffdab9');
	calcWave();
  renderWave();

	drawTarget();
	calculateDisplacement();
	calculateAngles();
	calculateError();
	drawError();
	//drawArm();
}

function resetPosition() {
	accelerometerResetValues = accelerometerData.slice();
	//console.log(accelerometerResetValues);
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
	var ellipseBrightness = map(distanceFromPerfectAlignment,0,200,255,0);

	var desiredPosition= map(wristRotation,minAngle,maxAngle,amplitude*2,0);
	desiredPosition=height/2+desiredPosition-amplitude;

	noStroke();
	fill(100,200,200,ellipseBrightness);
 	ellipseY= lerp(ellipseY, desiredPosition, 0.05);

	ellipse(width / 2 , ellipseY, 80, 80);
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

function calculateAngles(){
	wristVector = calculateVectors(accelerometerDisplacementsValues[5], accelerometerDisplacementsValues[4]);
	wristVector.setMag(100);

	handVector = calculateVectors(accelerometerDisplacementsValues[2], accelerometerDisplacementsValues[1])
	handVector.setMag(100);
	var wristHand = p5.Vector.add(handVector, wristVector);

	wristRotation = wristVector.x;
	wristRotation = map(wristRotation, 0, 100, 90, 0);

	handRotation = handVector.x;
	handRotation = map(handRotation, 0, 100, 90, 0);

	//console.log(wristRotation,handRotation*);
}

function calculateError(){
	varErrorX=abs(wristVector.x - handVector.x);
	varErrorY=abs(wristVector.y - handVector.y);
	varErrorZ=abs(wristVector.z - handVector.z);
	distanceFromPerfectAlignment=varErrorX+varErrorY+varErrorZ;

	var centerCurveY=yvalues[Math.round(yvalues.length/2)]+height/2;
	//ellipse(width/2,centerCurveY,50,50 );
	distanceFromLine=abs(ellipseY-centerCurveY);
}

function drawError(){
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
var advancementSpeed=map(distanceFromPerfectAlignment,0,200,0.02 ,0.001)

	if (distanceFromLine<50){
	theta += advancementSpeed;
	}

	console.log(theta);
  // For every x value, calculate a y value with sine function
  var x = theta;
  for (var i = 0; i < yvalues.length; i++) {
    yvalues[i] = sin(x)*amplitude;
    x+=dx;
  }
}

function renderWave() {
  // A simple way to draw the wave with an ellipse at each location
	noFill();
	stroke(255,255,0);
	strokeWeight(3);

	beginShape();
	curveVertex(0, height/2+yvalues[0]);

  for (var x = 0; x < yvalues.length; x++) {
		curveVertex(x*xspacing, height/2+yvalues[x]);

    //ellipse(x*xspacing, height/2+yvalues[x], 16, 16);
  }
	endShape();
}
