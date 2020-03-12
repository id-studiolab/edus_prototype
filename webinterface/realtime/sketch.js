function setup() {
  canvas = createCanvas( windowWidth, windowHeight );
  console.log( width, height );

  w = width + 16;

  ellipseY = height / 2;

  resetButton = createButton( 'start' );
  resetButton.hide();
  resetButton.center()
  connectButton = createButton( 'connect' );
  connectButton.mousePressed( connect );
  connectButton.center()
  //resetButton.hide();

  textAlign( CENTER, CENTER );
  rectMode( CENTER );
}

function draw() {
  background( '#ffdab9' );
  if ( encoderData != undefined ) {
    rect( width / 2, height / 2, encoderData, encoderData );
  }
  console.log( encoderData );
}


window.onresize = function() {
  var w = window.innerWidth;
  var h = window.innerHeight;
  canvas.size( w, h );
  width = w;
  height = h;
};