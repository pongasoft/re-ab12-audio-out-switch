format_version = "1.0"

rtc_bindings = {
  -- this will initialize the C++ object
  { source = "/environment/system_sample_rate", dest = "/global_rtc/init_instance" },
}

global_rtc = {
  init_instance = function(source_property_path, new_value)
    local new_no = jbox.make_native_object_rw("Instance", { new_value })
    jbox.store_property("/custom_properties/instance", new_no);
  end,
}

local rtInputSetupNotify = {
  "/audio_inputs/audioInputLeft/connected",
  "/audio_inputs/audioInputRight/connected",
  "/custom_properties/prop_mode",
  "/custom_properties/prop_bank",
  "/custom_properties/prop_bank_override_cv",
  "/custom_properties/prop_bank_cv_mapping",
  "/custom_properties/prop_state_single",
  "/custom_properties/prop_state_single_override_cv",
  "/custom_properties/prop_volume_change_filter",
  "/custom_properties/prop_state_single_keyboard_enabled",
  "/custom_properties/prop_soften",
  "/cv_inputs/cv_in_state_single/connected",
  "/cv_inputs/cv_in_state_single/value",
  "/cv_inputs/cv_in_bank/connected",
  "/cv_inputs/cv_in_bank/value",
  "/note_states/*"
}

local outputs = { "A1", "A2", "A3", "A4", "A5", "A6", "B1", "B2", "B3", "B4", "B5", "B6" }

for k, output in pairs(outputs) do
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/audio_outputs/audioOutputLeft" .. output .. "/connected"
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/audio_outputs/audioOutputRight" .. output .. "/connected"
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/cv_inputs/cv_in_gate_" .. output .. "/connected"
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/cv_inputs/cv_in_gate_" .. output .. "/value"
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/cv_outputs/cv_out_gate_" .. output .. "/connected"
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/custom_properties/prop_state_multi_" .. output
  rtInputSetupNotify[#rtInputSetupNotify + 1] = "/custom_properties/prop_volume_" .. output
end

rt_input_setup = {
  notify = rtInputSetupNotify
}

sample_rate_setup = {
	native = {
		22050,
		44100,
		48000,
		88200,
		96000,
		192000
	},
	
}
