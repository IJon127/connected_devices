let glassSize;
let waterVolume;
let glassFinData;
let glassArea;
// variable for the serialport object:
let portSelector;
let serial;

// this function is called when the page is loaded.
function setup(event) {
  //???????????????
  glassSize = document.getElementById("container");
  //   glassSize.addEventListener("change", setGlassSize);

  waterVolume = document.getElementById("water");
  //   waterVolume.addEventListener("change", setWaterVolume);

  // initialize the serialport object:
  serial = new p5.SerialPort();
  serial.on("list", printList);
  serial.on("data", serialEvent);
  serial.list(); // list the serial ports

  setGlassSize(); //???????????????????
  setWaterVolume(); //?????????????
}

function setGlassSize(i) {
  //???????????????
  let glassRawData = i;
  glassFinData = map(glassRawData, 20, 1024, 4, 10);
  glassArea = ((glassFinData * glassFinData) / 4) * 3.14;

  // get the span associated with it and change its text:
  let thisSpan = document.getElementById(glassDia);
  thisSpan.innerHTML = glassFinData;
  glassSize.style.width = glassFinData * 45;
}

function setWaterVolume(e) {
  // assume e is a number:
  let waterRawData = e;
  let waterFinData = map(waterRawData, 20, 1000, 0, 50.24);
  //get the span associated with it and change its text:
  let thisSpan = document.getElementById(waterVol);
  thisSpan.innerHTML = waterFinData;

  let waterheight = (waterFinData / glassArea) * 100;
  let thisBlock = document.getElementById(water);
  thisBlock.style.height = waterheight + "%";
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
    console.log(inData);
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
