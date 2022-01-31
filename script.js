let portSelector;
let glassSize;
let waterVolume;
// variable for the serialport object:
let serial;
// previous state of the serial input from the button:
let lastButtonState = 0;

// this function is called when the page is loaded.
// element event listeners are  added here:
function setup(event) {
  glassSize = document.getElementById("container");
  glassSize.addEventListener("change", setGlassSize);

  waterVolume = document.getElementById("fanSpeed");
  waterVolume.addEventListener("change", setWaterVolume);

  // initialize the serialport object:
  serial = new p5.SerialPort();
  serial.on("list", printList);
  serial.on("data", serialEvent);
  serial.list(); // list the serial ports

  //   setGlassSize();
  //   setWaterVolume();
}

function setGlassSize() {
  // change its value, depending on its current value:
  if (glassSize.value == "on") {
    glassSize.value = "off";
  } else {
    glassSize.value = "on";
  }
  // get the span associated with it and change its text:
  let thisSpan = document.getElementById(glassSize.id + "Val");
  thisSpan.innerHTML = "Power is " + glassSize.value;
}

function setWaterVolume(e) {
  // assume e is a number:
  var currentValue = e;
  // but if it's an object instead, it's because
  // the slider change event called this function.
  // Extract the number from it:
  if (typeof e == "object") {
    currentValue = e.target.value;
  }
  //get the span associated with it and change its text:
  let thisSpan = document.getElementById(waterVolume.id + "Val");
  thisSpan.innerHTML = currentValue;
}

// make a serial port selector object:
function printList(portList) {
  portSelector = document.getElementById("portSelector");
  for (var i = 0; i < portList.length; i++) {
    var option = document.createElement("option");
    option.text = portList[i];
    portSelector.add(option);
  }
  portSelector.addEventListener("change", openPort);
}

function openPort() {
  let item = portSelector.value;
  // if there's a port open, close it:
  if (serial.serialport != null) {
    serial.close();
  }
  // open the new port:
  serial.open(item);
}

function serialEvent() {
  // read a line of incoming data:
  var inData = serial.readLine();
  // if the line is not empty, parse it to JSON:
  if (inData) {
    var sensors = JSON.parse(inData);
    setGlassSize(sensors.pot);
    setWaterVolume(sensors.fsr);
  }
}

window.addEventListener("DOMContentLoaded", setup);

/***************************************************************************************
 *    Title: <title of program/source code>
 *    Author: <author(s) names>
 *    Date: <date>
 *    Code version: <code version>
 *    Availability: <where it's located>
 *
 ***************************************************************************************/

/***************************************************************************************
 *    Title: GraphicsDrawer source code
 *    Author: Igoe, T
 *    Date: 2021
 *    Code version: 2.0
 *    Availability: http://www.graphicsdrawer.com
 *
 ***************************************************************************************/
