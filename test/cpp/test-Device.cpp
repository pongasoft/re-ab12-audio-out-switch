/*
 * Copyright (c) 2021 pongasoft
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 *
 * @author Yan Pujante
 */

#include <gtest/gtest.h>
#include <Device.h>
#include <re/mock/DeviceTesters.h>
#include <re_cmake_build.h>
#include <ProjectConstants.h>

using namespace re::mock;

auto newAB12SwitchTester()
{
  auto c = DeviceConfig<Device>::fromJBoxExport(RE_CMAKE_MOTHERBOARD_DEF_LUA, RE_CMAKE_REALTIME_CONTROLLER_LUA);
  return HelperTester<Device>(c);
}

// Device - Default
TEST(Device, Default)
{
  auto tester = newAB12SwitchTester();

  ASSERT_EQ(44100, tester.device()->getSampleRate());

  ASSERT_FALSE(tester.device().getBool("/custom_properties/prop_soften"));

  tester.nextFrame();

  ASSERT_FALSE(tester.device().getBool("/custom_properties/prop_soften"));
  ASSERT_EQ(1, tester.device().getNum("/custom_properties/prop_state_single"));
}

MockAudioDevice::buffer_type xfade(MockAudioDevice::buffer_type const &iFromBuffer,
                                   MockAudioDevice::buffer_type const &iToBuffer)
{
  MockAudioDevice::buffer_type res{};
  for(int i = 0; i < MockAudioDevice::NUM_SAMPLES_PER_FRAME; i++)
  {
    double f = static_cast<double>(i) / (MockAudioDevice::NUM_SAMPLES_PER_FRAME - 1.0);
    res[i] = static_cast<float>((iToBuffer[i] * f) + (iFromBuffer[i] * (1.0 - f)));
  }
  return res;
}

MockAudioDevice::StereoBuffer xfade(MockAudioDevice::StereoBuffer const &iFromBuffer,
                                    MockAudioDevice::StereoBuffer const &iToBuffer)
{
  return { .fLeft = xfade(iFromBuffer.fLeft, iToBuffer.fLeft), .fRight = xfade(iFromBuffer.fRight, iToBuffer.fRight) };
}

// Device - Outputs
TEST(Device, Outputs)
{
  auto tester = newAB12SwitchTester();

  auto input = tester.wireNewAudioSrc("audioInputLeft", "audioInputRight");

  auto a1o = tester.wireNewAudioDst("audioOutputLeftA1", "audioOutputRightA1");
  auto a3o = tester.wireNewAudioDst("audioOutputLeftA3", "audioOutputRightA3");
  auto b5o = tester.wireNewAudioDst("audioOutputLeftB5", "audioOutputRightB5");


  /*******************************/////////////******************************/
  /*                             < FIRST FRAME >                            */
  /*******************************/////////////******************************/

  tester.nextFrame(); // initializes the device

  ////////// Checks - outputs remain at 0/0 //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(1, tester.device().getNum("/custom_properties/prop_state_single"));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - input has sound //////////

  input->fBuffer = MockAudioDevice::buffer(1.0, 2.0);

  tester.nextFrame();

  ////////// Checks - a1 should output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - selecting switch a2 //////////

  tester.device().setNum("/custom_properties/prop_state_single", 2);

  tester.nextFrame();

  ////////// Checks - a2 is not connected //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - selecting switch b5 //////////

  tester.device().setNum("/custom_properties/prop_state_single", 11);

  tester.nextFrame();

  ////////// Checks - b5 should output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - selecting multi mode //////////

  tester.device().setNum("/custom_properties/prop_mode", 1);

  tester.nextFrame();

  ////////// Checks - no output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - selecting A1 A3 B2 //////////

  tester.device().setBool("/custom_properties/prop_state_multi_A1", true);
  tester.device().setBool("/custom_properties/prop_state_multi_A3", true);
  tester.device().setBool("/custom_properties/prop_state_multi_B2", true);

  tester.nextFrame();

  ////////// Checks - a1/a3 output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - turning soften on //////////

  tester.device().setBool("/custom_properties/prop_soften", true);

  tester.nextFrame();

  ////////// Checks - a1/a3 output (no change since no switch changed) //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - turning a1 off, b5 on //////////

  tester.device().setBool("/custom_properties/prop_state_multi_A1", false);
  tester.device().setBool("/custom_properties/prop_state_multi_B5", true);

  tester.nextFrame();

  ////////// Checks - a1 -> cross fade to 0, a3 remains the same, b5 cross fade to 1/2 //////////
  ASSERT_EQ(a1o->fBuffer, xfade(MockAudioDevice::buffer(1.0, 2.0), MockAudioDevice::buffer(0, 0)));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(b5o->fBuffer, xfade(MockAudioDevice::buffer(0, 0), MockAudioDevice::buffer(1.0, 2.0)));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - none //////////

  tester.nextFrame();

  ////////// Checks - a1 is now 0, a3 remains the same, b5 stays at 1/2 //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0.0, 0.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - turning soften off - switching to instrument mode //////////

  tester.device().setBool("/custom_properties/prop_soften", false);
  tester.device().setNum("/custom_properties/prop_mode", 2);

  tester.nextFrame();

  ////////// Checks - no output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - turning on A1 / A6 / B5 //////////

  // 69 is A440 (middle A)
  // A1 - C - 60 | A3 - D - 62 | B5 - A# - 70
  tester.device().setNoteInEvents(MockDevice::NoteEvents{}.noteOn(60).noteOn(65).noteOn(70));

  tester.nextFrame();

  ////////// Checks - A1 / B5 should output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(0, 0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));

  /*******************************////////////*******************************/
  /*                             < NEXT FRAME >                             */
  /*******************************////////////*******************************/
  ////////// Action - turning on A3 off B5 //////////

  // 69 is A440 (middle A)
  // A1 - C - 60 | A3 - D - 62 | B5 - A# - 70
  tester.device().setNoteInEvents(MockDevice::NoteEvents{}.noteOn(62).noteOff(70));

  tester.nextFrame();

  ////////// Checks - A1 / B5 should output //////////
  ASSERT_EQ(a1o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(a3o->fBuffer, MockAudioDevice::buffer(1.0, 2.0));
  ASSERT_EQ(b5o->fBuffer, MockAudioDevice::buffer(0, 0));

}
