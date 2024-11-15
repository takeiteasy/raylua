local screenWidth = 1600
local screenHeight = 900

rl.InitWindow(screenWidth, screenHeight, "First Window");
rl.SetTargetFPS(60)

while not rl.WindowShouldClose() do
  rl.BeginDrawing()
  rl.ClearBackground(RAYWHITE)

  rl.DrawText("Congrats! You created your first window!", 600, 400, 20, RED)

  rl.EndDrawing()
end

rl.CloseWindow()