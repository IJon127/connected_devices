let winH = window.innerHeight;
let level1Btn = document.querySelector("#level1");
let level2Btn = document.querySelector("#level2");
let level3Btn = document.querySelector("#level3");

let playground = document.querySelector("#square_container");

function startGame(rowNum) {
  for (i = 0; i <= rowNum; i++) {
    let cell = document.createElement("div");
    cell.className = "square";
    playground.appendChild(cell);
  }
}
