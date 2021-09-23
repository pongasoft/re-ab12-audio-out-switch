//
// Created by Yan Pujante on 5/23/15.
//

#ifndef __pongasoft__DeviceState_h__
#define __pongasoft__DeviceState_h__


#include "JBoxPropertyManager.h"
#include "AudioSocket.h"
#include "ProjectConstants.h"
#include "Motherboard.h"
#include "SwitchedOutputState.h"

class DeviceState : public JBoxNoteListener
{
public:
  DeviceState();

  ~DeviceState()
  {
  }

  /*
   * Called to initialize the state on first batch
   */
  void init();

  inline bool isMultiMode() const
  {
    return getSwitchMode() == kModeMulti;
  }

  inline bool isSingleMode() const
  {
    return getSwitchMode() == kModeSingle;
  }

  inline bool isKeyboardEnabled() const
  {
    return fMotherboard.fPropStateSingleKeyboardEnabled.getValue();
  }

  inline ESwitchMode getSwitchMode() const
  {
    return fMotherboard.fPropMode.getValue();
  }

  inline ESwitchBank getSwitchBank() const
  {
    if(fMotherboard.fPropBankCV.getValue() > 0)
      return static_cast<ESwitchBank>(static_cast<int>(fMotherboard.fPropBankCV.getValue()) - 1);

    return fMotherboard.fPropBank.getValue();
  }

  inline ESwitchedOutput getSingleStateSwitch() const
  {
    return static_cast<ESwitchedOutput>(static_cast<int>(fMotherboard.fPropStateSingle.getValue()));
  };

  inline ESwitchedOutput getSingleStateCVSwitch() const
  {
    return static_cast<ESwitchedOutput>(static_cast<int>(fMotherboard.fPropStateSingleCV.getValue()));
  };

  virtual bool onNoteReceived(const TJBox_PropertyDiff &iPropertyDiff);

  bool afterMotherboardUpdate(bool motherboardStateChanged,
                              DeviceState const &previousState);

  bool haveOutputsChanged(DeviceState const &previousState);

  /**
   * Write the buffer to all outputs according to the state */
  void writeAudio(StereoAudioBuffer &iAudioInputBuffer);

  /**
   * Write the buffer to all outputs according to the state */
  void writeAudio(StereoAudioBuffer &iAudioInputBuffer,
                  DeviceState const &previousState,
                  XFade<kBatchSize> const &iXFade);

  void update(const DeviceState &rhs);

private:
  bool isSelected(SwitchedOutputState *output) const
  {
    ESwitchBank bank = getSwitchBank();

    if(bank != kBankAB && bank != output->getBank())
      return false;

    switch(getSwitchMode())
    {
      case kModeSingle:
        return output->getId() ==
          (getSingleStateCVSwitch() != kNone ? getSingleStateCVSwitch() : getSingleStateSwitch());

      case kModeMulti:
        return output->isMultiSelected();

      case kModeMidi1:
        return output->isMidiSelected();

      default:
        return false;
    }
  };

public:
  Motherboard fMotherboard;

private:
  SwitchedOutputState *fSwitchedOutputStates[kEnd];
  bool fSelectedSwitches[kEnd];
  bool fSelectedAndConnectedSwitches[kEnd];
  ESwitchedOutput fSingleStateKeySwitch;
};


#endif //__pongasoft__DeviceState_h__
