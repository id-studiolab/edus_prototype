var dataFromCloud;
var loaded=false;

var gridrows=7;
var gridcols=6;

var canvas;

var startDate=new Date('Novembre 16, 2018 00:00:00');

var spacingX;
var spacingY;

var bg;


function preload(){
	getSJsonData()
	bg = loadImage("assets/orecchiette.jpg");

}


function setup() {
  canvas=createCanvas(windowWidth,windowHeight);
}

function draw() {
  background(255);
	drawGrid();
	if (loaded){
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


function dataViz(){
	for (var i=0; i<dataFromCloud.length; i++){
	 var date=new Date(dataFromCloud[i].time);
	 difference = dateDiffInDays(startDate, date);

	 positionX=difference%spacingX;
	 positionY=Math.floor(difference/spacingX)

	 stroke(0);
	 var color=map(dataFromCloud[i].totalErro,0,2000,0,255);
	 fill(color,color);
	 var size =map(dataFromCloud[i].totalErro,0,2000,20,(spacingX+spacingY)/2);
	 ellipse(positionX*spacingX+spacingX/2, positionY*spacingY+spacingY/2,size,size );
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
		dataFromCloud=data;
		console.log(dataFromCloud);
		loaded=true;
	});
}



function drawGrid(){
	spacingX= width/(gridrows);
	spacingY= height/(gridcols);

	for (var i=0; i<gridrows; i++){
		for (var j=0; j<gridcols; j++){
			ellipse(i*spacingX+spacingX/2,j*spacingY+spacingY/2,5,5 );
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
