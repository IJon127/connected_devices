let lampBLE;
const serviceUuid = "3922bb2e-0000-4c31-b690-5ee7074abc1f";
let gyroCharacteristic;
let gyroValue = 0;
let gyro = [0, 0, 0];

/*
let acceleroCharacteristic;
let angleCharacteristic;
let acceleroValue = 0;
let angleValue = 0;
let accelero = [0, 0, 1];
*/

let vid = [];
let vidOpacity = [30, 20, 20, 20];

let bgmSound;
let leafSound;
let bgmVol = 0;
let leafVol = 0;

function preload() {
  bgmSound = loadSound("assets/bgmSound.wav");
  leafSound = loadSound("assets/leafSound.wav");
}

function setup() {
  // Create a p5ble class
  lampBLE = new p5ble();
  createCanvas(windowWidth, (windowWidth * 2) / 3);
  textSize(20);

  // Create a 'Connect' button
  const connectButton = createButton("Connect");
  connectButton.mousePressed(connectToBle);

  // video setting
  for (let i = 0; i < 4; i++) {
    vid[i] = createVideo("assets/vid" + i + ".mp4");
    vid[i].speed(0.6);
    vid[i].play();
    vid[i].loop();
    vid[i].hide();
  }
}

function draw() {
  for (let i = 0; i < 3; i++) {
    if (abs(gyro[i]) > 5) {
      vidOpacity[i + 1] += random(5);
      leafVol += 0.003;
      if (vidOpacity[i + 1] > 255) vidOpacity[i + 1] = 255;
      if (leafVol > 1) leafVol = 1;
    } else {
      vidOpacity[i + 1] -= 12;
      leafVol -= 0.006;
      if (vidOpacity[i + 1] < 20) vidOpacity[i + 1] = 20;
      if (leafVol < 0) leafVol = 0;
    }
  }

  blendMode(BLEND);
  background(20);
  blendMode(HARD_LIGHT);

  // background video------
  tint(255, vidOpacity[0]);
  image(vid[0], 0, 0, width, height);
  // videos changing with gyroscope data-----
  for (let i = 1; i < 4; i++) {
    tint(255, vidOpacity[i]);
    image(vid[i], 0, 0, width, height);
  }

  /*
  text(gyroValue, 100, 150);
  text(acceleroValue, 100, 100);
  text(angleValue, 100, 200);
  */

  //set audio volume----
  bgmSound.setVolume(bgmVol);
  leafSound.setVolume(leafVol);
  // leafSound.pan(accelero[1]);
  bgmVol += 0.01;
  if (bgmVol > 0.2) bgmVol = 0.2;
}

// BLE functions ===============================================

function connectToBle() {
  // Connect to a device by passing the service UUID
  lampBLE.connect(serviceUuid, gotCharacteristics);
  bgmVol = 0;
  bgmSound.loop();
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log("error: ", error);
  gyroCharacteristic = characteristics[0];
  lampBLE.read(gyroCharacteristic, "string", gotGyroValue);

  /*
  acceleroCharacteristic = characteristics[1];
  angleCharacteristic = characteristics[2];
  lampBLE.read(acceleroCharacteristic, "string", gotAcceleroValue);
  lampBLE.read(angleCharacteristic, gotAngleValue);
  */

  leafSound.loop();
}

// A function that will be called once got values

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

/*
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

function gotAngleValue(error, value) {
  if (error) console.log("error: ", error);
  // console.log("value: ", value);
  angleValue = floor(map(value, 0, 255, 0, 360));

  lampBLE.read(angleCharacteristic, gotAngleValue);
}
*/

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
modified 08 Mar 2022 
by I-Jon Hsieh
************************************************************************** */
