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
  let untiScale: Vector2 = {
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
      diag.x = untiScale.x * ((cell.col + 1) * squareSize - pos.x);
    } else {
      step.x = -1;
      diag.x = untiScale.x * (pos.x - cell.col * squareSize);
    }

    if (dir.y > 0) {
      step.y = 1;
      diag.y = untiScale.y * ((cell.row + 1) * squareSize - pos.y);
    } else {
      step.y = -1;
      diag.y = untiScale.y * (pos.y - cell.row * squareSize);
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

const canvas = <HTMLCanvasElement>document.getElementById("canvas");
if (canvas === null) {
  throw new Error("Canvas is null.");
}

const padding = 50;
const width = window.innerHeight - padding;
const height = window.innerHeight - padding;
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
const squareSize = width / grid.length;
const gridWidth = squareSize * grid.length;
const gridHeight = squareSize * grid.length;

let player = {
  pos: { row: 5, col: 3 },
  dir: Vec2Norm({ x: 1, y: 0.2 }),
  camera: { x: 1, y: 0 },
};

let palette = {
  bg: "#D3D3D3",
  fg: "#000000",
  wall: "#4f2525",
  player: "#ADD8E6",
};

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

  let playerX = (player.pos.col + 0.5) * squareSize;
  let playerY = (player.pos.row + 0.5) * squareSize;

  let rayLen = getRayLength(player.dir, { x: playerX, y: playerY });

  ctx.beginPath();
  ctx.moveTo(playerX, playerY);
  ctx.lineTo(playerX + player.dir.x * rayLen, playerY + player.dir.y * rayLen);
  ctx.stroke();
  ctx.closePath();

  // console.log(timestamp % 1000);
  if (timestamp % 500 < 20) {
    let theta = Math.PI / 12;
    player.dir = {
      x: player.dir.x * Math.cos(theta) - player.dir.y * Math.sin(theta),
      y: player.dir.x * Math.sin(theta) + player.dir.y * Math.cos(theta),
    };
  }

  window.requestAnimationFrame(drawFrame);
}

(() => {
  canvas.width = width;
  canvas.height = height;

  // console.log(`Before: ${player.dir.x}, ${player.dir.y}`);

  // console.log(`After: ${player.dir.x}, ${player.dir.y}`);

  drawFrame(0);
})();
