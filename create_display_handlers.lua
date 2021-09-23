local outputs = { "A1", "A2", "A3", "A4", "A5", "A6", "B1", "B2", "B3", "B4", "B5", "B6" }

local input = {}

for k, output in pairs(outputs) do
  input[#input + 1] = string.format("SwitchGesture%s = gestureFunction{ on_tap = \"SwitchTap%s\" }", output, output)
  input[#input + 1] = string.format("SwitchTap%s = switchTapFunction(%d, \"%s\")", output, k, output)
  input[#input + 1] = string.format("SwitchDraw%s = switchDrawFunction(%d, \"%s\")", output, k, string.sub(output, 1, 1))
end

for k, input in pairs(input) do
  print(input)
end