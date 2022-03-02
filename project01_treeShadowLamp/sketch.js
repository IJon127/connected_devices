const serviceUuid = "3922bb2e-0000-4c31-b690-5ee7074abc1f";
let myCharacteristic;
let myValue = 0;
let lampBLE;

let ax = 0;
let ay = 0;
let az = 1;
let vid = [];
let vidOpacity = [];

function setup() {
  // Create a p5ble class
  lampBLE = new p5ble();
  createCanvas(600, 400);
  textSize(20);
  textAlign(CENTER, CENTER);

  // Create a 'Connect' button
  const connectButton = createButton("Connect");
  connectButton.mousePressed(connectToBle);

  // video setting
  for (let i = 0; i < 4; i++) {
    vid[i] = createVideo("assets/vid" + i + ".mp4");
    vid[i].speed(0.6);
    vid[i].loop();
    vid[i].hide();
  }
}

function draw() {
  vidOpacity[0] = map(az, 1.02, 0.8, 50, 255);
  vidOpacity[1] = map(ax, 0, 0.4, 0, 255);
  vidOpacity[2] = map(ay, 0, 0.4, 0, 255);

  blendMode(BLEND);
  background(0);
  blendMode(HARD_LIGHT);
  for (let i = 0; i < 3; i++) {
    tint(255, vidOpacity[0]);
    image(vid[i], 0, 0, width, height);
  }

  text(myValue, 100, 100);
}

// BLE functions =======================================

function connectToBle() {
  // Connect to a device by passing the service UUID
  lampBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log("error: ", error);
  console.log("characteristics: ", characteristics);
  myCharacteristic = characteristics[0];
  // Read the value of the first characteristic
  lampBLE.read(myCharacteristic, "string", gotValue);
}

// A function that will be called once got values
function gotValue(error, value) {
  if (error) console.log("error: ", error);
  console.log("value: ", value);
  myValue = value;

  let acceleroData = split(myValue, ",");
  ax = acceleroData[0];
  ay = acceleroData[1];
  az = acceleroData[2];

  // After getting a value, call p5ble.read() again to get the value again
  lampBLE.read(myCharacteristic, "string", gotValue);
}

/* **************************************************************************
Tree Shadow Lamp 
Uses p5.ble.js example:
https://itpnyu.github.io/p5ble-website/docs/getstarted.html 

Video sources:
0: https://youtu.be/_ajwVI6mWnk
1: https://youtu.be/71fUdsctODA
2: https://youtu.be/eWQ594i7mNo
3: https://youtu.be/NWTJ_mhoQ38

created 25 Feb 2022
modified 01 Mar 2022 
by I-Jon Hsieh
************************************************************************** */
