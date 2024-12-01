type Vector2 = {
  x: number;
  y: number;
};

function Vec2Norm(vec: Vector2): Vector2 {
  let norm = Math.sqrt(vec.x * vec.x + vec.y * vec.y);
  return { x: vec.x / norm, y: vec.y / norm };
}

function getRayLength(dir: Vector2, init: Vector2): number {
  let drawDistance = 10000;
  let len = 0;
  let unitScale: Vector2 = {
    x: Math.sqrt(1 + (dir.y / dir.x) * (dir.y / dir.x)),
    y: Math.sqrt(1 + (dir.x / dir.y) * (dir.x / dir.y)),
  };
  let diag: Vector2 = { x: 0, y: 0 };
  let pos: Vector2 = init;
  let cell = {
    row: Math.floor(pos.y / squareSize),
    col: Math.floor(pos.x / squareSize),
  };
  let step: Vector2 = { x: 0, y: 0 };

  while (len < drawDistance) {
    if (
      cell.row >= grid.length ||
      cell.col >= grid[0].length ||
      cell.row < 0 ||
      cell.col < 0 ||
      grid[cell.row][cell.col] === 1
    ) {
      break;
    }

    if (dir.x > 0) {
      step.x = 1;
      diag.x = unitScale.x * ((cell.col + 1) * squareSize - pos.x);
    } else {
      step.x = -1;
      diag.x = unitScale.x * (pos.x - cell.col * squareSize);
    }

    if (dir.y > 0) {
      step.y = 1;
      diag.y = unitScale.y * ((cell.row + 1) * squareSize - pos.y);
    } else {
      step.y = -1;
      diag.y = unitScale.y * (pos.y - cell.row * squareSize);
    }

    if (diag.x < diag.y) {
      len += diag.x;
      pos = { x: pos.x + dir.x * diag.x, y: pos.y + dir.y * diag.x };
      cell.col += step.x;
    } else {
      len += diag.y;
      pos = { x: pos.x + dir.x * diag.y, y: pos.y + dir.y * diag.y };
      cell.row += step.y;
    }
  }

  return len;
}

function drawGrid(ctx: CanvasRenderingContext2D) {
  for (let i = 0; i <= grid.length; i++) {
    ctx.beginPath();
    ctx.moveTo(i * squareSize, 0);
    ctx.lineTo(i * squareSize, gridHeight);
    ctx.stroke();
    ctx.moveTo(0, i * squareSize);
    ctx.lineTo(gridWidth, i * squareSize);
    ctx.stroke();
    ctx.closePath();
  }

  for (let row = 0; row < grid.length; row++) {
    for (let col = 0; col < grid[0].length; col++) {
      if (grid[row][col] === 1) {
        ctx.fillStyle = palette.wall;
        ctx.fillRect(
          col * squareSize,
          row * squareSize,
          squareSize,
          squareSize
        );
      }

      if (row === player.pos.row && col === player.pos.col) {
        ctx.fillStyle = palette.player;
        ctx.fillRect(
          col * squareSize,
          row * squareSize,
          squareSize,
          squareSize
        );
      }
    }
  }
}

// TODO: take Player as arg and calculate playerX playerY or something
function drawDirCam(
  ctx: CanvasRenderingContext2D,
  playerX: number,
  playerY: number
) {
  ctx.lineWidth = 1.5;
  ctx.beginPath();
  ctx.strokeStyle = "red";
  ctx.moveTo(playerX, playerY);
  ctx.lineTo(
    playerX + player.dir.x * squareSize,
    playerY + player.dir.y * squareSize
  );
  ctx.stroke();
  ctx.closePath();

  ctx.beginPath();
  ctx.strokeStyle = "blue";
  ctx.moveTo(
    playerX + player.dir.x * squareSize,
    playerY + player.dir.y * squareSize
  );
  ctx.lineTo(
    playerX + (player.dir.x + player.camera.x) * squareSize,
    playerY + (player.dir.y + player.camera.y) * squareSize
  );
  ctx.moveTo(
    playerX + player.dir.x * squareSize,
    playerY + player.dir.y * squareSize
  );
  ctx.lineTo(
    playerX + (player.dir.x - player.camera.x) * squareSize,
    playerY + (player.dir.y - player.camera.y) * squareSize
  );
  ctx.stroke();
  ctx.closePath();
}

function drawFrame(timestamp: number) {
  const ctx = canvas.getContext("2d");
  if (ctx === null) {
    throw new Error("ctx is null.");
  }
  ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height); // clear canvas
  ctx.fillStyle = palette.bg;
  ctx.strokeStyle = palette.fg;
  ctx.lineWidth = 0.5;

  ctx.fillRect(0, 0, ctx.canvas.width, ctx.canvas.height);

  drawGrid(ctx);

  let playerX = (player.pos.col + 0.5) * squareSize;
  let playerY = (player.pos.row + 0.5) * squareSize;

  let rayLen = getRayLength(player.dir, { x: playerX, y: playerY });

  ctx.beginPath();
  ctx.moveTo(playerX, playerY);
  ctx.lineTo(playerX + player.dir.x * rayLen, playerY + player.dir.y * rayLen);
  ctx.stroke();
  ctx.closePath();

  drawDirCam(ctx, playerX, playerY);

  // if (timestamp % 500 < 20) {
  //   let theta = Math.PI / 18;
  //   player.changeDir({
  //     x: player.dir.x * Math.cos(theta) - player.dir.y * Math.sin(theta),
  //     y: player.dir.x * Math.sin(theta) + player.dir.y * Math.cos(theta),
  //   });
  // }

  window.requestAnimationFrame(drawFrame);
}

const canvas = <HTMLCanvasElement>document.getElementById("canvas");
if (canvas === null) {
  throw new Error("Canvas is null.");
}

const VPadding = 50;
const HPadding = 50;
const width = window.innerWidth - HPadding;
const height = window.innerHeight - VPadding;
const grid = [
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 1, 0, 0, 1, 1, 1, 1, 1, 0],
  [0, 1, 0, 0, 0, 0, 1, 0, 0, 0],
  [1, 0, 0, 0, 0, 0, 1, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
  [1, 1, 0, 0, 0, 0, 1, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
  [0, 0, 0, 1, 1, 0, 0, 0, 0, 0],
  [0, 0, 0, 0, 0, 0, 1, 0, 0, 0],
];
const squareSize = height / 1 / grid.length;
const gridWidth = squareSize * grid.length;
const gridHeight = squareSize * grid.length;

class Player {
  pos;
  dir: Vector2;
  camera: Vector2;

  constructor(pos = { row: 5, col: 3 }, dir: Vector2 = { x: 1, y: 0.2 }) {
    this.pos = pos;
    this.dir = Vec2Norm(dir);
    // https://mathworld.wolfram.com/PerpendicularVector.html
    // Camera is same length as direction, so 90 deg FOV
    this.camera = { x: -this.dir.y, y: this.dir.x };
  }

  changeDir(dir: Vector2) {
    this.dir = dir;
    this.camera = { x: -this.dir.y, y: this.dir.x };
  }
}

let player = new Player();

const palette = {
  bg: "#D3D3D3",
  fg: "#000000",
  wall: "#4f2525",
  player: "#ADD8E6",
};

(() => {
  canvas.width = width;
  canvas.height = height;

  drawFrame(0);
})();
