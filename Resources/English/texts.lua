format_version = "1.0"

-- Note that remote name text lenghts are: 
--		internal_name = max 64 chars, 
--		short_name = max 8 chars, 
--		shortest_ui_name = max 4 chars

 -- english
texts = {
	["audio input main left"] = "Audio Input Left",
	["audio input main right"] = "Audio Input Right",
  ["propertyname prop_mode"] = "Mode Select",
  ["ric short_ui_name prop_mode"] = "ModeSel.",
  ["ric shortest_ui_name prop_mode"] = "MSel",
  ["ui_selector prop_mode_single"] = "Single",
  ["ui_selector prop_mode_multi"] = "Multiple",
  ["ui_selector prop_mode_midi1"] = "Instrument",
  ["propertyname prop_bank"] = "Bank Select",
  ["ric short_ui_name prop_bank"] = "BankSel.",
  ["ric shortest_ui_name prop_bank"] = "BSel",
  ["ui_selector prop_bank_none"] = "No Bank",
  ["ui_selector prop_bank_A"] = "Bank A",
  ["ui_selector prop_bank_B"] = "Bank B",
  ["ui_selector prop_bank_AB"] = "Bank A+B",
  ["propertyname prop_bank_cv_mapping"] = "Bank CV Mapping",
  ["ui_selector prop_bank_cv_mapping_unipolar"] = "Unipolar",
  ["ui_selector prop_bank_cv_mapping_bipolar"] = "Bipolar",
  ["ui_selector prop_bank_cv_mapping_note"] = "Note",
  ["propertyname prop_state_single"] = "Single Select",
  ["ric short_ui_name prop_state_single"] = "SwSingle",
  ["ric shortest_ui_name prop_state_single"] = "SwSi",
  ["propertyname prop_state_single_cv"] = "Switch State (single) from CV",
  ["propertyname cv_in_state_single"] = "CV In Switch State (single)",
  ["propertyname cv_in_bank"] = "CV In Bank",
  ["propertyname prop_state_radio_selector_NONE"] = "NONE",
  ["propertyname prop_volume_change_filter"] = "Volume Change Filter",
  ["propertyname prop_volume_change_filter on"] = "Volume Change Filter On",
  ["propertyname prop_volume_change_filter off"] = "Volume Change Filter Off",
  ["propertyname prop_soften"] = "Soften",
  ["ric short_ui_name prop_soften"] = "Soften",
  ["ric shortest_ui_name prop_soften"] = "Soft",
  ["propertyname prop_soften on"] = "Soften On",
  ["propertyname prop_soften off"] = "Soften Off",
  ["ui_type template prop_volume"] = "^0 dB",
  ["cd BankToggleTap"] = "Toggle Banks [A/B]",
  ["cd BankToggleTap_bank_B"] = "Select Bank [B]",
  ["cd handlBankToggleTapX_A"] = "Toggle Bank [A]",
  ["cd handlBankToggleTapX_B"] = "Toggle Bank [B]",
  ["ui_group volume"] = "Volume",
  ["ui_group state_multi"] = "Multi Select",

  ["propertyname prop_state_single_keyboard_enabled"] = "Keyboard (Single)",
  ["propertyname prop_state_single_keyboard_enabled on"] = "Enabled",
  ["propertyname prop_state_single_keyboard_enabled off"] = "Disabled",

  ["TBD"] = "TBD",
}

local outputs = { "A1", "A2", "A3", "A4", "A5", "A6", "B1", "B2", "B3", "B4", "B5", "B6" }

for k, output in pairs(outputs) do
  -- outputs
  texts["audioOutputLeft" .. output] = "Audio [" .. output .. "] Output Left"
  texts["audioOutputRight" .. output] = "Audio [" .. output .. "] Output Right"
  texts["propertyname cv_in_gate_" .. output] = "Gate In [" .. output .. "]"
  texts["propertyname cv_out_gate_" .. output] = "Gate Out [" .. output .. "]"

  texts["propertyname prop_state_multi_" .. output] = "Multi [" .. output .. "]"
  texts["ric short_ui_name prop_state_multi_" .. output] = "Multi" .. output
  texts["ric shortest_ui_name prop_state_multi_" .. output] = "Mu" .. output
  texts["propertyname prop_state_midi_" .. output] = "Midi [" .. output .. "]"
  texts["propertyname prop_state_radio_selector_" .. output] = "Single [" .. output .. "]"
  texts["propertyname prop_volume_" .. output] = "Volume [" .. output .. "]"
  texts["ric short_ui_name prop_volume_" .. output] = "Volume" .. output
  texts["ric shortest_ui_name prop_volume_" .. output] = "Vo" .. output

  texts["cd handleSwitchTap_multi_true_" .. output] = "Select [" .. output .. "]"
  texts["cd handleSwitchTap_multi_false_" .. output] = "Deselect [" .. output .. "]"
  texts["cd handleSwitchTap_single_true_" .. output] = "Select [" .. output .. "]"
  texts["cd handleSwitchTap_single_false_" .. output] = "Deselect [" .. output .. "]"
end
