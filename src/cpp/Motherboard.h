//
// Created by Yan Pujante on 5/27/15.
//


#ifndef __Motherboard_H_
#define __Motherboard_H_

#include "AudioSocket.h"
#include "ProjectConstants.h"
#include "CVSocket.h"
#include "Volume.h"

const TJBox_Tag PROP_MODE_TAG = 1000;
const TJBox_Tag PROP_STATE_SINGLE_TAG = 1010;
const TJBox_Tag PROP_STATE_SINGLE_OVERRIDE_CV_TAG = 1020;
const TJBox_Tag PROP_VOLUME_CHANGE_FILTER_TAG = 1030;
const TJBox_Tag PROP_SOFTEN_TAG = 1040;
const TJBox_Tag PROP_BANK_TAG = 1050;
const TJBox_Tag PROP_BANK_OVERRIDE_CV_TAG = 1060;
const TJBox_Tag PROP_BANK_CV_MAPPING_TAG = 1070;
const TJBox_Tag PROP_STATE_SINGLE_KEYBOARD_ENABLED_TAG = 1080;
const TJBox_Tag PROP_STATE_MULTI_START_TAG = 2000;
const TJBox_Tag PROP_VOLUME_START_TAG = 4000;

// start at C3
const TJBox_Tag PROP_NOTE_STATE_START_TAG = 60;

class SwitchedOutput
{
public:
  explicit SwitchedOutput(ESwitchedOutput iId) :
    fId{iId},
    fBank{bank(iId)},
    fName{StaticString<3>::printf("%s%d", fBank == kBankA ? "A" : "B", number(fId))},
    fAudioOutput(jbox::PropertyName::printf("audioOutputLeft%s", fName), jbox::PropertyName::printf("audioOutputRight%s", fName)),
    fCVInGate(jbox::PropertyName::printf("cv_in_gate_%s", fName)),
    fCVOutGate(jbox::PropertyName::printf("cv_out_gate_%s", fName)),
    fPropStateMulti(jbox::PropertyPath::printf("/custom_properties/prop_state_multi_%s", fName)),
    fPropStateMidi(jbox::PropertyPath::printf("/custom_properties/prop_state_midi_%s", fName)),
    fPropVolume(jbox::PropertyPath::printf("/custom_properties/prop_volume_%s", fName))
  {
    DCHECK_F(fId >= kA1 && fId <= kB6);
  }

  SwitchedOutput(SwitchedOutput const &other) = delete;

  SwitchedOutput &operator=(const SwitchedOutput &iOther) {
    fAudioOutput = iOther.fAudioOutput;
    fCVInGate = iOther.fCVInGate;
    fCVOutGate = iOther.fCVOutGate;
    fPropStateMulti = iOther.fPropStateMulti;
    fPropStateMidi = iOther.fPropStateMidi;
    fPropVolume = iOther.fPropVolume;
    return *this;
  }


  void registerForUpdate(JBoxPropertyManager &manager, ESwitchedOutput idx)
  {
    fAudioOutput.registerForUpdate(manager);
    fCVInGate.registerForUpdate(manager);
    fCVOutGate.registerForUpdate(manager);
    fPropStateMulti.registerForUpdate(manager, PROP_STATE_MULTI_START_TAG + idx - kA1);
    manager.registerForInit(fPropStateMidi);
    fPropVolume.registerForUpdate(manager, PROP_VOLUME_START_TAG + idx - kA1);
  }


  static constexpr ESwitchBank bank(ESwitchedOutput iOutput) {
    DCHECK_F(iOutput >= kA1 && iOutput <= kB6);
    return iOutput < kB1 ? kBankA : kBankB;
  }

  static constexpr int number(ESwitchedOutput iOutput) {
    DCHECK_F(iOutput >= kA1 && iOutput <= kB6);
    return static_cast<int>(iOutput) - (iOutput < kB1 ? 0 : 6);
  }

public:
  const ESwitchedOutput fId;
  const ESwitchBank fBank;
  const StaticString<3> fName;
  StereoOutPair fAudioOutput;
  CVInSocket fCVInGate;
  CVOutSocket fCVOutGate;
  BooleanJBoxProperty fPropStateMulti;
  BooleanJBoxProperty fPropStateMidi;
  VolumeCubeJBoxProperty fPropVolume;
};

class Motherboard
{
public:
  Motherboard();
  ~Motherboard();

  void registerForUpdate(JBoxPropertyManager &manager);
  void update(Motherboard const &rhs);

public:
  StereoInPair fAudioInput;
  CVInNote fCVInStateSingle;
  CVInSocket fCVInBank;

  JBoxNoteStates fNoteStates; // any note on the keyboard
  ESwitchModeJBoxProperty fPropMode;
  Float64JBoxProperty fPropStateSingle; // stepped number starts at 0 (0 means nothing selected)
  Float64JBoxProperty fPropStateSingleCV; // stepped number starts at 0
  BooleanJBoxProperty fPropStateSingleOverrideCV; // this property acts as a toggle!!!
  BooleanJBoxProperty fPropStateSingleKeyboardEnabled; // whether keyboard is enabled on not in single mode
  BooleanJBoxProperty fPropVolumeChangeFilter; // whether to filter on volume change or not
  BooleanJBoxProperty fPropSoften; // whether to xfade or not
  ESwitchBankJBoxProperty fPropBank;
  BooleanJBoxProperty fPropBankOverrideCV; // this property acts as a toggle!!!
  Float64JBoxProperty fPropBankCV; // 0 means no CV, then 1-4 is ESwitchBank + 1
  EBankCVMappingJBoxProperty fPropBankCVMapping;

  SwitchedOutput *fSwitchedOutputs[kEnd];
};


#endif //__Motherboard_H_
