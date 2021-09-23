//
// Created by Yan Pujante on 5/27/15.
//


#ifndef __SwitchedOutput_H_
#define __SwitchedOutput_H_

#include "AudioSocket.h"
#include "ProjectConstants.h"
#include "Motherboard.h"

class SwitchedOutputState
{
public:
  explicit SwitchedOutputState(SwitchedOutput *iSwitchedOutput):
    fSwitchedOutput(iSwitchedOutput),
    fNoteCount(0),
    fVolume(1.0, true)
  {}

  void update(const SwitchedOutputState &rhs)
  {
    fVolume.update(rhs.fVolume);
  }

  inline bool isMultiSelected() const { return fSwitchedOutput->fPropStateMulti.getValue(); }
  inline bool isCVSelected() const { return fSwitchedOutput->fCVInGate.isConnected() && fSwitchedOutput->fCVInGate.getValue() != 0; }
  inline bool isNoteSelected() const { return fNoteCount > 0; }
  inline bool isMidiSelected() const { return isNoteSelected() || isCVSelected(); }
  inline bool isConnected() const { return fSwitchedOutput->fAudioOutput.isConnected(); }
  inline char const *getName() const { return fSwitchedOutput->fName; }
  inline ESwitchedOutput getId() const { return fSwitchedOutput->fId; }
  inline ESwitchBank getBank() const { return fSwitchedOutput->fBank; }

  void writeAudio(StereoAudioBuffer const &iStereoAudioBuffer);

  inline void adjustGateOut(bool isSelected)
  {
    if(fSwitchedOutput->fCVOutGate.isConnected())
      fSwitchedOutput->fCVOutGate.storeValueToMotherboardOnUpdate(isSelected ? 1.0 : 0.0);
  }

  inline bool afterMotherboardUpdate(bool motherboardStateChanged,
                                     bool volumeChangeFilter)
  {
    bool res = false;

    fVolume.setFilterOn(volumeChangeFilter);

    if(motherboardStateChanged)
    {
      res |=
        fSwitchedOutput->fPropStateMidi.storeValueToMotherboardOnUpdate(isMidiSelected() ? TJbox_TRUE : TJbox_FALSE);
    }

    res |= fVolume.adjustVolume(fSwitchedOutput->fPropVolume.getValue());

    return res;
  }

  inline void setNoteSelection(bool selected)
  {
    if(selected)
      fNoteCount++;
    else
      fNoteCount--;

    JBOX_ASSERT(fNoteCount >= 0);
  }

private:
  SwitchedOutput * const fSwitchedOutput;

  // count how many midi outputs we are getting for this switch (multiple notes (1 octave apart) can be held at the same
  // time)
  int fNoteCount;
  VolumeState fVolume;
};

#endif //__SwitchedOutput_H_
