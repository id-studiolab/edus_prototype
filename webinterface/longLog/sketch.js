var dataFromCloud;
var loaded = false;

var gridrows = 7;
var gridcols = 6;

var canvas;

var startDate = new Date('Novembre 16, 2018 00:00:00');

var spacingX;
var spacingY;

var bg;

var x_off = 0;
var y_off = 1.3;
var z_off = 2.1;

var px_offset = 80; // amplitude

var NOISE_SCALE = 20;

function preload() {
	getSJsonData()
	bg = loadImage("assets/orecchiette.jpg");
}


function setup() {
	canvas = createCanvas(windowWidth, windowHeight);
}

function draw() {
	background('#ffdab9');
	drawGrid();
	if (loaded) {
		dataViz();
	}
}


const _MS_PER_DAY = 1000 * 60 * 60 * 24;

// a and b are javascript Date objects
function dateDiffInDays(a, b) {
	// Discard the time and time-zone information.
	const utc1 = Date.UTC(a.getFullYear(), a.getMonth(), a.getDate());
	const utc2 = Date.UTC(b.getFullYear(), b.getMonth(), b.getDate());

	return Math.floor((utc2 - utc1) / _MS_PER_DAY);
}


function dataVizEllipse() {
	for (var i = 0; i < dataFromCloud.length; i++) {
		var date = new Date(dataFromCloud[i].time);
		difference = dateDiffInDays(startDate, date);

		positionX = difference % spacingX;
		positionY = Math.floor(difference / spacingX)

		stroke(0);
		var color = map(dataFromCloud[i].totalErro, 0, 2000, 0, 255);
		noFill();
		var size = map(dataFromCloud[i].totalErro, 0, 10000, 20, (spacingX + spacingY) / 2);
		ellipse(positionX * spacingX + spacingX / 2, positionY * spacingY + spacingY / 2, size, size);
	}
}

function dataViz() {
	for (var i = 0; i < dataFromCloud.length; i++) {
		var date = new Date(dataFromCloud[i].time);
		difference = dateDiffInDays(startDate, date);

		positionX = difference % spacingX;
		positionY = Math.floor(difference / spacingX)

		z_off += 0.001;
		x_off += 0.001;
		y_off += 0.001;


		stroke('yellow');

		fill(255,100);
		//more error bigger circle
		var size = map(dataFromCloud[i].totalErro, 0, 10000, (spacingX + spacingY) / 4, 0);

		// draw shape:
		push();
		translate(positionX * spacingX + spacingX / 2, positionY * spacingY + spacingY / 2);

		//more error more noise
		vertices_amount = 20
		r = size / 2;

		NOISE_SCALE=map(dataFromCloud[i].totalErro, 0, 10000, 10, 90);

		px_offset=map(dataFromCloud[i].totalErro, 0, 10000, 90, 10);

		beginShape();
		for (var a = 0; a < TWO_PI; a += TWO_PI / vertices_amount) {
			var x = r * sin(a);
			var y = r * cos(a);

			var new_x = x + (
				noise(
					((x_off + x) / NOISE_SCALE),
					((y_off + y) / NOISE_SCALE),
					z_off) * px_offset * sin(a));

			var new_y = y + (
				noise(
					((x_off + x) / NOISE_SCALE),
					((y_off + y) / NOISE_SCALE),
					z_off) * px_offset * cos(a))
			vertex(new_x, new_y);
		}
		endShape(CLOSE);

		pop();

	}
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
		dataFromCloud = data;
		console.log(dataFromCloud);
		loaded = true;
	});
}



function drawGrid() {
	noStroke();
	spacingX = width / (gridrows);
	spacingY = height / (gridcols);

	for (var i = 0; i < gridrows; i++) {
		for (var j = 0; j < gridcols; j++) {
			ellipse(i * spacingX + spacingX / 2, j * spacingY + spacingY / 2, 5, 5);
		}
	}
}

window.onresize = function() {
	var w = window.innerWidth;
	var h = window.innerHeight;
	canvas.size(w, h);
	width = w;
	height = h;
};
