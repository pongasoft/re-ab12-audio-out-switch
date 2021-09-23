format_version = "2.0"

local outputs = { "A1", "A2", "A3", "A4", "A5", "A6", "B1", "B2", "B3", "B4", "B5", "B6" }

-- front Widgets
frontWidgets = {

  -- device name / tape
  jbox.device_name {
    graphics = {
      node = "TapeFront",
    },
  },

  -- prop_mode
  jbox.radio_button{
    graphics = {
      node = "ModeSingleButton",
    },
    value = "/custom_properties/prop_mode",
    index = 0,
  },

  jbox.radio_button{
    graphics = {
      node = "ModeMultiButton",
    },
    value = "/custom_properties/prop_mode",
    index = 1,
  },

  jbox.radio_button{
    graphics = {
      node = "ModeMidi1Button",
    },
    value = "/custom_properties/prop_mode",
    index = 2,
  },

  -- prop_bank
  jbox.custom_display{
    graphics = {
      node = "BankToggle",
    },
    background = jbox.image{path = "BankToggleCustomDisplayBackground"},
    display_width_pixels = math.floor(160 / 5),
    display_height_pixels = math.floor(145 / 5),
    draw_function = "NoOpDraw",
    gesture_function ="BankToggleGesture",
    values = {
      "/custom_properties/prop_bank",
      "/custom_properties/prop_bank_cv",
      "/custom_properties/prop_bank_override_cv",
      "/custom_properties/prop_bank_toggle_number"
    },
  },

  jbox.static_decoration{
    graphics = { node = "BankToggleReleased" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank_toggle_number",
    visibility_values = { 0 }
  },

  jbox.static_decoration{
    graphics = { node = "BankTogglePressed" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank_toggle_number",
    visibility_values = { 1 }
  },

  jbox.custom_display{
    graphics = {
      node = "BankToggleA",
    },
    background = jbox.image{path = "BankToggleACustomDisplayBackground"},
    display_width_pixels = math.floor(100 / 5),
    display_height_pixels = math.floor(90 / 5),
    draw_function = "NoOpDraw",
    gesture_function ="BankToggleGestureA",
    values = {
      "/custom_properties/prop_bank",
      "/custom_properties/prop_bank_cv",
      "/custom_properties/prop_bank_override_cv",
    },
  },

  jbox.static_decoration{
    graphics = { node = "BankToggleAOff" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank",
    visibility_values = { 0, 2 }
  },

  jbox.static_decoration{
    graphics = { node = "BankToggleAOn" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank",
    visibility_values = { 1, 3 }
  },

  jbox.custom_display{
    graphics = {
      node = "BankToggleB",
    },
    background = jbox.image{path = "BankToggleBCustomDisplayBackground"},
    display_width_pixels = math.floor(100 / 5),
    display_height_pixels = math.floor(90 / 5),
    draw_function = "NoOpDraw",
    gesture_function ="BankToggleGestureB",
    values = {
      "/custom_properties/prop_bank",
      "/custom_properties/prop_bank_cv",
      "/custom_properties/prop_bank_override_cv",
    },
  },

    -- 0 = none, 1 = A, 2 = B, 3 = A+B

  jbox.static_decoration{
    graphics = { node = "BankToggleBOff" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank",
    visibility_values = { 0, 1 }
  },

  jbox.static_decoration{
    graphics = { node = "BankToggleBOn" },
    blend_mode = "normal",
    visibility_switch = "/custom_properties/prop_bank",
    visibility_values = { 2, 3 }
  },


  -- prop_soften
  jbox.toggle_button {
    graphics = {
      node = "SoftenButton",
    },
    value = "/custom_properties/prop_soften"
  },

  -- prop_state_single_cv
  jbox.sequence_meter {
    graphics = {
      node = "SingleStateCVLED",
    },
    value = "/custom_properties/prop_state_single_cv"
  },

  -- prop_bank_cv
  jbox.sequence_meter {
    graphics = {
      node = "BankCVLED",
    },
    value = "/custom_properties/prop_bank_cv"
  },

}

-- front
front = jbox.panel {
  graphics = {
    node = "Bg",
  },
  widgets = frontWidgets,
}

-- backWidgets
backWidgets = {

  -- device name / tape
  jbox.device_name {
    graphics = {
      node = "TapeBack",
    },
  },

  -- placeholder
  jbox.placeholder {
    graphics = {
      node = "Placeholder",
    },
  },

  -- audio input
  jbox.audio_input_socket {
    graphics = {
      node = "audioInputStereoPairLeft",
    },
    socket = "/audio_inputs/audioInputLeft",
  },
  jbox.audio_input_socket {
    graphics = {
      node = "audioInputStereoPairRight",
    },
    socket = "/audio_inputs/audioInputRight",
  },

  -- cv input
  jbox.cv_input_socket{
    graphics = {
      node = "CVInStateSingle",
    },
    socket = "/cv_inputs/cv_in_state_single",
  },

  jbox.toggle_button {
    graphics = {
      node = "StateSingleKeyboardEnabled",
    },
    value = "/custom_properties/prop_state_single_keyboard_enabled"
  },

  jbox.cv_input_socket{
    graphics = {
      node = "CVInBank",
    },
    socket = "/cv_inputs/cv_in_bank",
  },

  jbox.toggle_button {
    graphics = {
      node = "VolumeChangeFilter",
    },
    value = "/custom_properties/prop_volume_change_filter"
  },

  jbox.sequence_fader {
    graphics = {
      node = "BankCV",
    },
    value = "/custom_properties/prop_bank_cv_mapping",
    orientation = "vertical",
    inverted = true,
    handle_size = 43
  },
}

foldedFrontWidgets = {
  -- device name / tape
  jbox.device_name {
    graphics = {
      node = "TapeFoldedFront",
    },
  },
}

folded_front = jbox.panel {
  graphics = {
    node = "Bg",
  },

  widgets = foldedFrontWidgets
}

for k, output in pairs(outputs) do
  -- audio outputs
  backWidgets[#backWidgets + 1] = jbox.audio_output_socket {
    graphics = {
      node = "audioOutputStereoPairLeft" .. output,
    },
    socket = "/audio_outputs/audioOutputLeft" .. output,
  }
  backWidgets[#backWidgets + 1] = jbox.audio_output_socket {
    graphics = {
      node = "audioOutputStereoPairRight" .. output,
    },
    socket = "/audio_outputs/audioOutputRight" .. output,
  }

  -- cv gate in
  backWidgets[#backWidgets + 1] = jbox.cv_input_socket{
    graphics = {
      node = "CVInGate" .. output,
    },
    socket = "/cv_inputs/cv_in_gate_" .. output,
  }

  -- cv gate out
  backWidgets[#backWidgets + 1] = jbox.cv_output_socket{
    graphics = {
      node = "CVOutGate" .. output,
    },
    socket = "/cv_outputs/cv_out_gate_" .. output,
  }

  -- volume knobs
  frontWidgets[#frontWidgets + 1] = jbox.analog_knob{
    graphics= {
      node="Volume" .. output,
      hit_boundaries = {left = 0, top = 0, right = 0, bottom = 40}
    },
    value="/custom_properties/prop_volume_" .. output,
  }

  -- switch custom display
  frontWidgets[#frontWidgets + 1] = jbox.custom_display{
    graphics = {
      node = "Switch" .. output,
    },
    background = jbox.image{path = "SwitchCustomDisplayBackground"},
    display_width_pixels = math.floor(100 / 5),
    display_height_pixels = math.floor(95 / 5),
    draw_function = "SwitchDraw" .. output,
    gesture_function ="SwitchGesture" .. output,
    values = {
      "/custom_properties/prop_mode",
      "/custom_properties/prop_state_single",
      "/custom_properties/prop_state_single_cv",
      "/custom_properties/prop_state_single_override_cv",
      "/custom_properties/prop_state_multi_" .. output,
      "/custom_properties/prop_state_midi_" .. output,
      "/custom_properties/prop_bank",
      "/custom_properties/prop_bank_cv",
    },
  }

  -- label
  frontWidgets[#frontWidgets + 1] = jbox.value_display{
    graphics = {
      node = "Label" .. output,
    },
    value = "/custom_properties/prop_label_" .. output,
    text_color = { 38, 152, 162 },
    text_style = "Small LCD font",
  }

  -- switch custom display (folded front)
  foldedFrontWidgets[#foldedFrontWidgets + 1] = jbox.custom_display{
    graphics = {
      node = "SwitchFolded" .. output,
    },
    background = jbox.image{path = "SwitchCustomDisplayBackground"},
    display_width_pixels = math.floor(100 / 5),
    display_height_pixels = math.floor(95 / 5),
    draw_function = "SwitchDraw" .. output,
    gesture_function ="SwitchGesture" .. output,
    values = {
      "/custom_properties/prop_mode",
      "/custom_properties/prop_state_single",
      "/custom_properties/prop_state_single_cv",
      "/custom_properties/prop_state_single_override_cv",
      "/custom_properties/prop_state_multi_" .. output,
      "/custom_properties/prop_state_midi_" .. output,
      "/custom_properties/prop_bank",
      "/custom_properties/prop_bank_cv",
    },
  }
end

back = jbox.panel {
  graphics = {
    node = "Bg"
  },
  widgets = backWidgets
}


folded_back = jbox.panel {
  graphics = {
    node = "Bg",
  },
  cable_origin = {
    node = "CableOrigin",
  },
  widgets = {

    -- device name / tape
    jbox.device_name {
      graphics = {
        node = "TapeFoldedBack",
      },
    },

  },
}
