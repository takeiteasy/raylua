local screenWidth = 1600
local screenHeight = 900

rl.InitWindow(screenWidth, screenHeight, "First Window");
rl.SetTargetFPS(60)

while not rl.WindowShouldClose() do
  rl.BeginDrawing()
  rl.ClearBackground({r = 100, g = 100, b = 100, a = 255})

  rl.DrawText("Congrats! You created your first window!", 600, 400, 20, {r = 200, g = 200, b = 0, a = 255})

  rl.EndDrawing()
end

rl.CloseWindow()
