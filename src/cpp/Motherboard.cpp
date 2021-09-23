//
// Created by Yan Pujante on 5/27/15.
//

#include "Motherboard.h"

Motherboard::Motherboard():
    fAudioInput("audioInputLeft", "audioInputRight"),
    fCVInStateSingle("cv_in_state_single"),
    fCVInBank("cv_in_bank"),
    fNoteStates(),
    fPropMode("/custom_properties/prop_mode"),
    fPropStateSingle("/custom_properties/prop_state_single"),
    fPropStateSingleCV("/custom_properties/prop_state_single_cv"),
    fPropStateSingleOverrideCV("/custom_properties/prop_state_single_override_cv"),
    fPropStateSingleKeyboardEnabled("/custom_properties/prop_state_single_keyboard_enabled"),
    fPropVolumeChangeFilter("/custom_properties/prop_volume_change_filter"),
    fPropSoften("/custom_properties/prop_soften"),
    fPropBank("/custom_properties/prop_bank"),
    fPropBankOverrideCV("/custom_properties/prop_bank_override_cv"),
    fPropBankCV("/custom_properties/prop_bank_cv"),
    fPropBankCVMapping("/custom_properties/prop_bank_cv_mapping")
{
  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); i++) {
    fSwitchedOutputs[i] = new SwitchedOutput(static_cast<ESwitchedOutput>(i));
  }
}

Motherboard::~Motherboard()
{
  // Implementation note: dynamic allocation is automatically freed when the RE is removed
  // => destructor only called when unit testing
  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); i++)
    delete fSwitchedOutputs[i];
}

void Motherboard::update(Motherboard const &rhs)
{
  fAudioInput = rhs.fAudioInput;
  fCVInStateSingle = rhs.fCVInStateSingle;
  fCVInBank = rhs.fCVInBank;
  fPropMode = rhs.fPropMode;
  fPropStateSingle = rhs.fPropStateSingle;
  fPropStateSingleCV = rhs.fPropStateSingleCV;
  fPropStateSingleOverrideCV = rhs.fPropStateSingleOverrideCV;
  fPropVolumeChangeFilter = rhs.fPropVolumeChangeFilter;
  fPropSoften = rhs.fPropSoften;
  fPropBank = rhs.fPropBank;
  fPropBankOverrideCV = rhs.fPropBankOverrideCV;
  fPropBankCV = rhs.fPropBankCV;
  fPropBankCVMapping = rhs.fPropBankCVMapping;
  fPropStateSingleKeyboardEnabled = rhs.fPropStateSingleKeyboardEnabled;

  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    *fSwitchedOutputs[i] = *rhs.fSwitchedOutputs[i];
  }
}

void Motherboard::registerForUpdate(JBoxPropertyManager &manager)
{
  fAudioInput.registerForUpdate(manager);
  fCVInStateSingle.registerForUpdate(manager);
  fCVInBank.registerForUpdate(manager);

  manager.registerNoteStates(fNoteStates);

  manager.registerForInit(fPropStateSingleCV);
  manager.registerForInit(fPropBankCV);

  manager.registerForUpdate(fPropMode, PROP_MODE_TAG);
  manager.registerForUpdate(fPropStateSingle, PROP_STATE_SINGLE_TAG);
  manager.registerForUpdate(fPropStateSingleOverrideCV, PROP_STATE_SINGLE_OVERRIDE_CV_TAG);
  manager.registerForUpdate(fPropStateSingleKeyboardEnabled, PROP_STATE_SINGLE_KEYBOARD_ENABLED_TAG);
  manager.registerForUpdate(fPropVolumeChangeFilter, PROP_VOLUME_CHANGE_FILTER_TAG);
  manager.registerForUpdate(fPropSoften, PROP_SOFTEN_TAG);
  manager.registerForUpdate(fPropBank, PROP_BANK_TAG);
  manager.registerForUpdate(fPropBankOverrideCV, PROP_BANK_OVERRIDE_CV_TAG);
  manager.registerForUpdate(fPropBankCVMapping, PROP_BANK_CV_MAPPING_TAG);

  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    fSwitchedOutputs[i]->registerForUpdate(manager, static_cast<ESwitchedOutput>(i));
  }
}

