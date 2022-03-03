const serviceUuid = "3922bb2e-0000-4c31-b690-5ee7074abc1f";
let acceleroCharacteristic;
let gyroCharacteristic;
let angleCharacteristic;
let acceleroValue = 0;
let gyroValue = 0;
let angleValue = 0;

let lampBLE;

let accelero = [0, 0, 1];
let gyro = [0, 0, 0];

let vid = [];
let vidOpacity = [150, 20, 20, 20];

let moving = false;

function setup() {
  // Create a p5ble class
  lampBLE = new p5ble();
  createCanvas(1200, 800);
  textSize(20);
  // textAlign(CENTER, CENTER);

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
  for (let i = 0; i < 3; i++) {
    if (abs(gyro[i]) > 3) {
      vidOpacity[i + 1] += random(5);
      if (vidOpacity[i + 1] > 255) {
        vidOpacity[i + 1] = 255;
      }
    } else {
      vidOpacity[i + 1] -= 4;
      if (vidOpacity[i + 1] < 20) {
        vidOpacity[i + 1] = 0;
      }
    }
  }

  blendMode(BLEND);
  background(60);
  blendMode(HARD_LIGHT);

  tint(255, vidOpacity[0]);
  image(vid[0], 0, 0, width, height); // background video

  for (let i = 1; i < 4; i++) {
    tint(255, vidOpacity[i]);
    image(vid[i], 0, 0, width, height);
  }

  text(acceleroValue, 100, 100);
  text(gyroValue, 100, 150);
  text(angleValue, 100, 200);
}

// BLE functions =======================================

function connectToBle() {
  // Connect to a device by passing the service UUID
  lampBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log("error: ", error);
  // console.log("characteristics: ", characteristics);
  acceleroCharacteristic = characteristics[0];
  gyroCharacteristic = characteristics[1];
  angleCharacteristic = characteristics[2];

  lampBLE.read(acceleroCharacteristic, "string", gotAcceleroValue);
  lampBLE.read(gyroCharacteristic, "string", gotGyroValue);
  lampBLE.read(angleCharacteristic, gotAngleValue);
}

// A function that will be called once got values
function gotAcceleroValue(error, value) {
  if (error) console.log("error: ", error);
  // console.log("value: ", value);
  acceleroValue = value;

  let eachData = split(value, ",");
  for (let i = 0; i < accelero.length; i++) {
    accelero[i] = eachData[i];
  }

  lampBLE.read(acceleroCharacteristic, "string", gotAcceleroValue);
}

function gotGyroValue(error, value) {
  if (error) console.log("error: ", error);
  // console.log("value: ", value);
  gyroValue = value;

  let eachData = split(value, ",");
  for (let i = 0; i < gyro.length; i++) {
    gyro[i] = eachData[i];
  }

  lampBLE.read(gyroCharacteristic, "string", gotGyroValue);
}

function gotAngleValue(error, value) {
  if (error) console.log("error: ", error);
  // console.log("value: ", value);
  angleValue = floor(map(value, 0, 255, 0, 360));

  lampBLE.read(angleCharacteristic, gotAngleValue);
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
modified 03 Mar 2022 
by I-Jon Hsieh
************************************************************************** */
