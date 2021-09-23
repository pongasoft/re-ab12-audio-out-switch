#include "Device.h"

#ifndef  __phdsp__
#include <iostream>
#endif // !__phdsp__

Device::Device(int iSampleRate): CommonDevice(), fSampleRate{iSampleRate}, fFirstBatch(true)
{
  JBOX_TRACE("Device()");

#ifndef  __phdsp__
  JBOX_TRACE("Local 45 XCode Mode!!!");
#endif // !__phdsp__

  fCurrentDeviceState.fMotherboard.registerForUpdate(fJBoxPropertyManager);
}

Device::~Device()
{
  // nothing to do
  JBOX_TRACE("~Device()");
}

void Device::renderBatch(TJBox_PropertyDiff const iPropertyDiffs[], TJBox_UInt32 iDiffCount)
{
  bool stateChanged = false;

  if(fFirstBatch)
  {
    doInitDevice(iPropertyDiffs, iDiffCount);
    stateChanged = true;
    fFirstBatch = false;
  }
  else
  {
    if(iDiffCount > 0)
    {
      stateChanged |= fJBoxPropertyManager.onUpdate(iPropertyDiffs, iDiffCount);

      stateChanged |= fJBoxPropertyManager.onNotesUpdate(iPropertyDiffs, iDiffCount, &fCurrentDeviceState);
    }
  }

  stateChanged = fCurrentDeviceState.afterMotherboardUpdate(stateChanged, fPreviousDeviceState);

  stateChanged |= doRenderBatch(stateChanged);

  if(stateChanged)
    fPreviousDeviceState.update(fCurrentDeviceState);
}

bool Device::doRenderBatch(bool propertyStateChange)
{
  bool stateChanged = propertyStateChange;

  if(!fCurrentDeviceState.fMotherboard.fAudioInput.isConnected())
  {
    return stateChanged;
  }

  bool soften =
    stateChanged && // if state has not changed then outputs have not changed
      fCurrentDeviceState.fMotherboard.fPropSoften.getValue() && // is soften turned on?
      fCurrentDeviceState.haveOutputsChanged(fPreviousDeviceState); // have outputs actually changed?

  if(soften)
    JBOX_TRACE("softening");

  StereoAudioBuffer buf;
  fCurrentDeviceState.fMotherboard.fAudioInput.readAudio(buf);
  if(buf.isSilent())
    return stateChanged;

  if(soften)
    fCurrentDeviceState.writeAudio(buf, fPreviousDeviceState, fXFade);
  else
    fCurrentDeviceState.writeAudio(buf);

  return stateChanged;
}

void Device::doInitDevice(TJBox_PropertyDiff const iPropertyDiffs[], TJBox_UInt32 iDiffCount)
{
  JBOX_TRACE("doInitDevice()");

  // initialize properties
  fJBoxPropertyManager.initProperties();

  // processes the updates
  fJBoxPropertyManager.onUpdate(iPropertyDiffs, iDiffCount);

  // processes the notes
  fJBoxPropertyManager.onNotesUpdate(iPropertyDiffs, iDiffCount, &fCurrentDeviceState);

  // initialize current device
  fCurrentDeviceState.init();

  // copy to previous state to initialize it too!
  fPreviousDeviceState.update(fCurrentDeviceState);
}
