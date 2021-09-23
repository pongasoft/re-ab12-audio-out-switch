format_version = "1.0"

local outputs = { "A1", "A2", "A3", "A4", "A5", "A6", "B1", "B2", "B3", "B4", "B5", "B6" }

local propStateMultiStartTag = 2000
local propStateMidiStartTag = 3000
local propVolumeStartTag = 4000

local documentOwnerProperties = {}
local rtOwnerProperties = {}
local guiOwnerProperties = {}

-- control the mode for selecting the outputs
-- 0 = single, 1 = multi, 2 = midi1
documentOwnerProperties["prop_mode"] = jbox.number {
  property_tag = 1000,
  default = 0,
  steps = 3,
  ui_name = jbox.ui_text("propertyname prop_mode"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_mode_single"),
    jbox.ui_text("ui_selector prop_mode_multi"),
    jbox.ui_text("ui_selector prop_mode_midi1"),
  })
}

local propStateRadioUISelector = {
  jbox.ui_text("propertyname prop_state_radio_selector_NONE")
}

local volumeGroupProperties = {}
local stateMultiGroupProperties = {}

for k, output in pairs(outputs) do

  -- contains the state of the individual switch when in multiple mode (one boolean per output)
  documentOwnerProperties["prop_state_multi_" .. output] = jbox.boolean{
    property_tag = propStateMultiStartTag + k - 1,
    default = false,
    ui_name = jbox.ui_text("propertyname prop_state_multi_" .. output),
    ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
  }

  stateMultiGroupProperties[#stateMultiGroupProperties + 1] = "/custom_properties/prop_state_multi_" .. output

  -- contains the state of the individual switch when in midi mode (one boolean per output)
  rtOwnerProperties["prop_state_midi_" .. output] = jbox.boolean{
    property_tag = propStateMidiStartTag + k - 1,
    default = false,
    ui_name = jbox.ui_text("propertyname prop_state_midi_" .. output),
    ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
  }
  -- number, nonlinear decibel:
  documentOwnerProperties["prop_volume_" .. output] = jbox.number{
    property_tag = propVolumeStartTag + k - 1,
    default = 0.7,
    ui_name = jbox.ui_text("propertyname prop_volume_" .. output),
    ui_type = jbox.ui_nonlinear{
      -- convert data range 0-1 to dB value using an x^3 curve. Data value 0.7 is 0 dB.
      data_to_gui = function(data_value)
        local gain = math.pow(data_value / 0.7, 3)
        local ui_value =  20 * math.log10(gain)
        return ui_value
      end,
      -- convert UI dB value to data range 0-1
      gui_to_data = function(gui_value)
        local data_value = math.pow(math.pow(10, gui_value / 20), 1/3) * 0.7
        return data_value
      end,
      units = {
        {min_value=0, unit = {template=jbox.ui_text("ui_type template prop_volume"), base=1}, decimals=2},
      },
    },
  }

  volumeGroupProperties[#volumeGroupProperties + 1] = "/custom_properties/prop_volume_" .. output

  -- label for switch
  guiOwnerProperties["prop_label_" .. output] = jbox.string {
    persistence = "patch",
    default = output
  }

  propStateRadioUISelector[#propStateRadioUISelector + 1] = jbox.ui_text("propertyname prop_state_radio_selector_" .. output)
end

-- contains the state of the switch when in single mode (1 - 12) / 0 is nothing selected
documentOwnerProperties["prop_state_single"] = jbox.number {
  property_tag = 1010,
  default = 1,
  steps = #outputs + 1,
  ui_name = jbox.ui_text("propertyname prop_state_single"),
  ui_type = jbox.ui_selector(propStateRadioUISelector)
}

-- contains the state of the switch coming from CV output (1 - 12) / 0 is nothing selected
rtOwnerProperties["prop_state_single_cv"] = jbox.number {
  default = 0,
  steps = #outputs + 1,
  ui_name = jbox.ui_text("propertyname prop_state_single_cv"),
  ui_type = jbox.ui_selector(propStateRadioUISelector)
}

-- this property is only used to tell c++ code to override cv (note that it is used as a toggle, not absolute value!)
documentOwnerProperties["prop_state_single_override_cv"] = jbox.boolean{
  property_tag = 1020,
  default = false,
  persistence="none",
  ui_name = jbox.ui_text("TBD"),
  ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
}

-- this property is a toggle to turn volume change filtering on or off
documentOwnerProperties["prop_volume_change_filter"] = jbox.boolean{
  property_tag = 1030,
  default = true,
  ui_name = jbox.ui_text("propertyname prop_volume_change_filter"),
  ui_type = jbox.ui_selector {
    jbox.ui_text("propertyname prop_volume_change_filter off"),
    jbox.ui_text("propertyname prop_volume_change_filter on")
  },
}

-- this property toggles soften mode (short xfade when outputs change)
documentOwnerProperties["prop_soften"] = jbox.boolean{
  property_tag = 1040,
  default = false,
  ui_name = jbox.ui_text("propertyname prop_soften"),
  ui_type = jbox.ui_selector {
    jbox.ui_text("propertyname prop_soften off"),
    jbox.ui_text("propertyname prop_soften on")
  },
}

-- control which bank is selected
-- 0 = none, 1 = A, 2 = B, 3 = A+B
documentOwnerProperties["prop_bank"] = jbox.number {
  property_tag = 1050,
  default = 3,
  steps = 4,
  ui_name = jbox.ui_text("propertyname prop_bank"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_bank_none"),
    jbox.ui_text("ui_selector prop_bank_A"),
    jbox.ui_text("ui_selector prop_bank_B"),
    jbox.ui_text("ui_selector prop_bank_AB"),
  })
}

-- this property is only used to tell c++ code to override cv (note that it is used as a toggle, not absolute value!)
documentOwnerProperties["prop_bank_override_cv"] = jbox.boolean{
  property_tag = 1060,
  default = false,
  persistence="none",
  ui_name = jbox.ui_text("TBD"),
  ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
}

-- contains the bank coming from CV output / 0 = no cv / 1 = none, 2 = A, 3 = B, 4 = A+B
rtOwnerProperties["prop_bank_cv"] = jbox.number {
  default = 0,
  steps = 5,
  ui_name = jbox.ui_text("propertyname prop_bank"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("TBD"),
    jbox.ui_text("TBD"),
    jbox.ui_text("TBD"),
    jbox.ui_text("TBD"),
    jbox.ui_text("TBD"),
  })
}

-- this property is no longer used (replaced by prop_bank_toggle_number)
guiOwnerProperties["prop_bank_toggle"] = jbox.boolean {
  default = false,
  persistence="none",
  ui_name = jbox.ui_text("TBD"),
  ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
}

-- this property is only used by the UI/custom display momentary button to display the right image
guiOwnerProperties["prop_bank_toggle_number"] = jbox.number {
  default = 0,
  steps = 2,
  persistence="none",
  ui_name = jbox.ui_text("TBD"),
  ui_type = jbox.ui_selector({jbox.UI_TEXT_OFF, jbox.UI_TEXT_ON})
}

-- control how to map prop bank cv to prop_bank property
-- 0 = unipolar, 1 = bipolar, 2 = note
documentOwnerProperties["prop_bank_cv_mapping"] = jbox.number {
  property_tag = 1070,
  default = 1, -- bipolar
  steps = 3,
  ui_name = jbox.ui_text("propertyname prop_bank_cv_mapping"),
  ui_type = jbox.ui_selector({
    jbox.ui_text("ui_selector prop_bank_cv_mapping_unipolar"),
    jbox.ui_text("ui_selector prop_bank_cv_mapping_bipolar"),
    jbox.ui_text("ui_selector prop_bank_cv_mapping_note"),
  })
}

-- this property enables keyboard in single mode (default is for backward compatibility)
documentOwnerProperties["prop_state_single_keyboard_enabled"] = jbox.boolean{
  property_tag = 1080,
  default = false,
  ui_name = jbox.ui_text("propertyname prop_state_single_keyboard_enabled"),
  ui_type = jbox.ui_selector {
    jbox.ui_text("propertyname prop_state_single_keyboard_enabled off"),
    jbox.ui_text("propertyname prop_state_single_keyboard_enabled on")
  },
}

--[[
Custom properties
--]]
custom_properties = jbox.property_set {
  gui_owner = {
    properties = guiOwnerProperties
  },

  document_owner = {
    properties = documentOwnerProperties
  },
	
  rtc_owner = {
    properties = {
      instance = jbox.native_object{ },
    }
  },
	
  rt_owner = {
    properties = rtOwnerProperties
  }
}

--[[ 
Outputs/Inputs
--]]


audio_outputs = {}
cv_inputs = {}
cv_outputs = {}

for k, output in pairs(outputs) do
  -- defines the audio socket (left)
  local leftSocket = "audioOutputLeft" .. output
  audio_outputs[leftSocket] = jbox.audio_output {
    ui_name = jbox.ui_text(leftSocket)
  }
  -- defines the audio socket (right)
  local rightSocket = "audioOutputRight" .. output
  audio_outputs[rightSocket] = jbox.audio_output {
    ui_name = jbox.ui_text(rightSocket)
  }
  -- it is a stereo pair => should be auto routed
  jbox.add_stereo_audio_routing_pair {
    left = "/audio_outputs/" .. leftSocket,
    right = "/audio_outputs/" .. rightSocket
  }

  -- defines the gate in cv socket
  cv_inputs["cv_in_gate_" .. output] = jbox.cv_input {
    ui_name = jbox.ui_text("propertyname cv_in_gate_" .. output)
  }
  -- defines the gate out cv socket
  cv_outputs["cv_out_gate_" .. output] = jbox.cv_output {
    ui_name = jbox.ui_text("propertyname cv_out_gate_" .. output)
  }
end

audio_inputs = {
  audioInputLeft = jbox.audio_input {
    ui_name = jbox.ui_text("audio input main left")
  },
  audioInputRight = jbox.audio_input {
    ui_name = jbox.ui_text("audio input main right")
  }
}

-- defines routing pairs for stereo (Reason will wire left and right automatically)

jbox.add_stereo_audio_routing_pair {
  left = "/audio_inputs/audioInputLeft",
  right = "/audio_inputs/audioInputRight"
}

-- Sockets on the back panel of the 45 that the host can auto-route to
jbox.add_stereo_audio_routing_target{
  signal_type = "normal",
  left = "/audio_inputs/audioInputLeft",
  right = "/audio_inputs/audioInputRight",
  auto_route_enable = true
}

-- defines the cv socket
cv_inputs["cv_in_state_single"] = jbox.cv_input {
  ui_name = jbox.ui_text("propertyname cv_in_state_single")
}

cv_inputs["cv_in_bank"] = jbox.cv_input {
  ui_name = jbox.ui_text("propertyname cv_in_bank")
}

-- groupings
ui_groups = {
  {
    ui_name = jbox.ui_text("ui_group state_multi"),
    properties = stateMultiGroupProperties
  },
  {
    ui_name = jbox.ui_text("ui_group volume"),
    properties = volumeGroupProperties
  },
}

-- allow for automation
midi_implementation_chart = {
  midi_cc_chart = {
    [12] = "/custom_properties/prop_state_single",
    [13] = "/custom_properties/prop_bank",
    [14] = "/custom_properties/prop_mode",
    [15] = "/custom_properties/prop_soften",
  }
}

local volumeCCChart = 16
local stateMultiCCChart = 40

remote_implementation_chart = {
  ["/custom_properties/prop_state_single"] = {
    internal_name = "Switch State Single",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_state_single"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_state_single")
  },
  ["/custom_properties/prop_bank"] = {
    internal_name = "A/B Bank Selection",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_bank"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_bank")
  },
  ["/custom_properties/prop_mode"] = {
    internal_name = "A/B Mode",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_mode"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_mode")
  },
  ["/custom_properties/prop_soften"] = {
    internal_name = "Soften",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_soften"),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_soften")
  },
}

for k, output in pairs(outputs) do
  midi_implementation_chart["midi_cc_chart"][volumeCCChart + k - 1] = "/custom_properties/prop_volume_" .. output
  remote_implementation_chart["/custom_properties/prop_volume_" .. output] = {
    internal_name = "Volume [" .. output .. "]",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_volume_" .. output),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_volume_" .. output)
  }
end

for k, output in pairs(outputs) do
  midi_implementation_chart["midi_cc_chart"][stateMultiCCChart + k - 1] = "/custom_properties/prop_state_multi_" .. output
  remote_implementation_chart["/custom_properties/prop_state_multi_" .. output] = {
    internal_name = "Multi [" .. output .. "]",
    short_ui_name = jbox.ui_text("ric short_ui_name prop_state_multi_" .. output),
    shortest_ui_name = jbox.ui_text("ric shortest_ui_name prop_state_multi_" .. output)
  }
end