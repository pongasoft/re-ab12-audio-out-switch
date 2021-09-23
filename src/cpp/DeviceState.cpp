//
// Created by Yan Pujante on 5/23/15.
//

#include "DeviceState.h"

DeviceState::DeviceState() : fSingleStateKeySwitch(kNone)
{
  for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    fSwitchedOutputStates[i] = new SwitchedOutputState(fMotherboard.fSwitchedOutputs[i]);
  }

  for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    fSelectedSwitches[i] = false;
    fSelectedAndConnectedSwitches[i] = false;
  }
}

void DeviceState::init()
{
  fSingleStateKeySwitch = kNone;
}

void DeviceState::update(const DeviceState &rhs)
{
  fMotherboard.update(rhs.fMotherboard);
  for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    fSwitchedOutputStates[i]->update(*rhs.fSwitchedOutputStates[i]);
    fSelectedSwitches[i] = rhs.fSelectedSwitches[i];
    fSelectedAndConnectedSwitches[i] = rhs.fSelectedAndConnectedSwitches[i];
  }
  fSingleStateKeySwitch = rhs.fSingleStateKeySwitch;
}

bool DeviceState::onNoteReceived(const TJBox_PropertyDiff &iPropertyDiff)
{
  TJBox_Tag noteIndex = iPropertyDiff.fPropertyTag;
      
  int outputIndex = (noteIndex % 12) + static_cast<int>(kA1);

  JBOX_ASSERT(outputIndex >= kA1 && outputIndex < kEnd);

  SwitchedOutputState *output = fSwitchedOutputStates[outputIndex];
      
  bool cur = JBox::toJBoxFloat64(iPropertyDiff.fCurrentValue) != 0;
  bool prev = JBox::toJBoxFloat64(iPropertyDiff.fPreviousValue) != 0;

  if(cur != prev)
  {
    if(cur && isSingleMode() && isKeyboardEnabled())
      fSingleStateKeySwitch = output->getId();
    output->setNoteSelection(cur);
    return true;
  }

  return false;
}

bool DeviceState::afterMotherboardUpdate(bool motherboardStateChanged,
                                         DeviceState const &previousState)
{
  bool res = motherboardStateChanged;

  // adjust midi and volume
  for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    SwitchedOutputState *output = fSwitchedOutputStates[i];

    res |= output->afterMotherboardUpdate(motherboardStateChanged,
                                         fMotherboard.fPropVolumeChangeFilter.getValue());
  }

  if(motherboardStateChanged)
  {
    if(previousState.isKeyboardEnabled() && !isKeyboardEnabled())
      fSingleStateKeySwitch = kNone;

    // handle CV (single state)
    TJBox_Float64 stateSingleCV = previousState.fMotherboard.fPropStateSingleCV.getValue();

    // if there is UI override, it trumps everything
    if(fMotherboard.fPropStateSingle.isNotSameValue(previousState.fMotherboard.fPropStateSingle) ||
      fMotherboard.fPropStateSingleOverrideCV.isNotSameValue(previousState.fMotherboard.fPropStateSingleOverrideCV))
    {
      fSingleStateKeySwitch = kNone;
      stateSingleCV = 0;
    }
    else
    {
      // keyboard trumps cv (if enabled)
      if(previousState.fSingleStateKeySwitch != fSingleStateKeySwitch && isKeyboardEnabled())
      {
        stateSingleCV = fSingleStateKeySwitch;
      }
      else
      {
        // no longer connected => no state from CV
        if(!fMotherboard.fCVInStateSingle.isConnected() && previousState.fMotherboard.fCVInStateSingle.isConnected())
          stateSingleCV = fSingleStateKeySwitch;
        else
        {
          if(fMotherboard.fCVInStateSingle.isNotSameValue(previousState.fMotherboard.fCVInStateSingle)) // only when CV changes
          {
            TJBox_Int32 noteValue = fMotherboard.fCVInStateSingle.getValue();
            if(noteValue >= kMidiC0)
            {
              stateSingleCV = (noteValue % 12) + 1; // Cx maps to A1
            }
          }
        }
      }
    }

    res |= fMotherboard.fPropStateSingleCV.storeValueToMotherboardOnUpdate(stateSingleCV);

    // handle CV (bank)
    TJBox_Float64 bankCV = previousState.fMotherboard.fPropBankCV.getValue();

    if(!fMotherboard.fCVInBank.isConnected() ||
      fMotherboard.fPropBankOverrideCV.isNotSameValue(previousState.fMotherboard.fPropBankOverrideCV))
    {
      // not connected or UI forced change => revert to no CV
      bankCV = 0;
    }
    else
    if(fMotherboard.fCVInBank.isNotSameValue(previousState.fMotherboard.fCVInBank)) // only when CV changes
    {
      TJBox_Float64 value = fMotherboard.fCVInBank.getValue();

      switch(fMotherboard.fPropBankCVMapping.getValue())
      {
        case kBankCVMappingBipolar:
          if(value <= -1.0)
            bankCV = 1;
          else
          if(value < 0)
            bankCV = 2;
          else
          if(value < 1.0)
            bankCV = 3;
          else
            bankCV = 4;
          break;

        case kBankCVMappingUnipolar:
          if(value < 0.25)
            bankCV = 1;
          else
          if(value < 0.5)
            bankCV = 2;
          else
          if(value < 0.75)
            bankCV = 3;
          else
            bankCV = 4;
          break;

        case kBankCVMappingNote:
        {
          TJBox_Int32 note = JBox::toNote(value) % 12; // to bring back to 1 octave
          if(note < 3) // Cx - Dx
            bankCV = 1;
          else
          if(note < 6) // D#x - Fx
            bankCV = 2;
          else
          if(note < 9) // F#x - G#x
            bankCV = 3;
          else
            bankCV = 4; // Ax - Bx
        }
          break;

        default:
          JBOX_ASSERT_MESSAGE(true, "not reached");
      }
    }

    res |=
      fMotherboard.fPropBankCV.storeValueToMotherboardOnUpdate(bankCV);
  }

  if(res)
  {
    // compute state of switches
    for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
    {
      SwitchedOutputState *output = fSwitchedOutputStates[i];
      bool selected = isSelected(output);
      fSelectedSwitches[i] = selected;
      fSelectedAndConnectedSwitches[i] = selected && output->isConnected();

      output->adjustGateOut(selected);
    }
  }

  return res;
}

bool DeviceState::haveOutputsChanged(DeviceState const &previousState)
{
  for (int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    if(fSelectedAndConnectedSwitches[i] != previousState.fSelectedAndConnectedSwitches[i])
      return true;
  }

  return false;
}

//------------------------------------------------------------------------
// DeviceState::writeAudio
//------------------------------------------------------------------------
void DeviceState::writeAudio(StereoAudioBuffer &iAudioInputBuffer)
{
  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    SwitchedOutputState *output = fSwitchedOutputStates[i];

    if(fSelectedAndConnectedSwitches[i])
      output->writeAudio(iAudioInputBuffer);
  }
}

//------------------------------------------------------------------------
// DeviceState::writeAudio
//------------------------------------------------------------------------
void DeviceState::writeAudio(StereoAudioBuffer &iAudioInputBuffer,
                             DeviceState const &previousState,
                             XFade<kBatchSize> const &iXFade)
{
  for(int i = static_cast<int>(kA1); i < static_cast<int>(kEnd); ++i)
  {
    SwitchedOutputState *output = fSwitchedOutputStates[i];

    if(fSelectedAndConnectedSwitches[i])
    {
      // no cross-fading (switch was on and remained on)
      if(previousState.fSelectedAndConnectedSwitches[i])
        output->writeAudio(iAudioInputBuffer);
      else
      {
        // cross-fading from 0 to iAudioInputBuffer
        StereoAudioBuffer buf;
        buf.clear();
        buf.xFade(iXFade, iAudioInputBuffer, buf);
        output->writeAudio(buf);
      }
    }
    else
    {
      if(previousState.fSelectedAndConnectedSwitches[i])
      {
        // cross-fading from iAudioInputBuffer to 0
        StereoAudioBuffer buf;
        buf.clear();
        buf.xFade(iXFade, buf, iAudioInputBuffer);
        output->writeAudio(buf);
      }
      else
      {
        // nothing to do: switch was off and remained off
      }
    }
  }
}
