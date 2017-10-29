#include "borovylo.h"

using namespace sl::cabl;

const Color kColor_Black{0};
const Color kColor_Red{0xff, 0, 0, 0xff};
const Color kColor_Yellow{0xff, 0xff, 0, 0x55};
const Color kColor_Blue{0xff, 0, 0, 0x77};


CablDevice::CablDevice(Borovylo* borovylo)
: borovylo(borovylo)
{
  M_LOG("[CablDevice] constructor");
}


CablDevice::~CablDevice()
{
  M_LOG("[CablDevice] destructor");
  for(unsigned i = 0; i < device()->numOfGraphicDisplays(); i++)
  {
    device()->graphicDisplay(i)->black();
  }
  for(unsigned i = 0; i < device()->numOfLedArrays(); i++)
  {
    device()->ledArray(i)->setValue(0.5, kColor_Black);
  }
}


void CablDevice::buttonChanged(Device::Button button, bool buttonState, bool shiftState)
{
  int b = static_cast<int>(button);
  std::cout << "buttonChanged id: " << b << ", state: " << buttonState << '\n';
  std::string str = "Butt#" + std::to_string(b) + (buttonState ? " ON" : " OFF") + (shiftState ? " SHIFT" : "");
  device()->setButtonLed(
    button,
    buttonState ? (shiftState ? kColor_Red : kColor_Yellow) : kColor_Black
  );
  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, str.c_str(), {0xff});

  if (buttonState) borovylo->onCablDeviceButton(b);
}


void CablDevice::controlChanged(unsigned pot, double value, bool shiftPressed)
{
  std::string str = "Pot#" + std::to_string(static_cast<int>(pot)) + " " + std::to_string(static_cast<int>(value * 100)) + (shiftPressed ? " SHIFT" : "");
  device()->textDisplay(0)->putText(str.c_str(), 0);
  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, str.c_str(), {0xff});
  device()->ledArray(pot)->setValue(value, kColor_Red);
}


void CablDevice::keyChanged(unsigned index, double value, bool shiftPressed)
{
  std::string str = "Pad#" + std::to_string(static_cast<int>(value * 0xff)) + (shiftPressed ? " SHIFT" : "");
  device()->setKeyLed(index, {static_cast<uint8_t>(value * 0xff)});
  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, str.c_str(), {0xff});
}


void CablDevice::encoderChanged(unsigned encoder, bool valueIncreased, bool shiftPressed)
{
  std::string str = "Enc#" + std::to_string(static_cast<int>(encoder)) + ( valueIncreased ? " increased" : " decreased" ) + (shiftPressed ? " SHIFT" : "");
  device()->textDisplay(0)->putText(str.c_str(), 0);
  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, str.c_str(), {0xff});
}


void CablDevice::initDevice()
{
  for(unsigned i = 0; i < device()->numOfLedArrays(); i++)
  {
    device()->ledArray(i)->setValue(0.5, kColor_Blue);
  }

  for(unsigned i = 0; i < device()->numOfGraphicDisplays(); i++)
  {
    unsigned w = device()->graphicDisplay(i)->width();
    unsigned h = device()->graphicDisplay(i)->height();
    device()->graphicDisplay(i)->black();
    device()->graphicDisplay(i)->line(0, 0, w, h, {0xff});
    device()->graphicDisplay(i)->line(0, h, w, 0, {0xff});
    device()->graphicDisplay(i)->line(w/2, h, w/2, 0, {0xff});
    device()->graphicDisplay(i)->line(0, h/2, w, h/2, {0xff});
    device()->graphicDisplay(i)->rectangle(0, 0, w, h, {0xff});
    device()->graphicDisplay(i)->circle(w/2, h/2, w/2, {0xff});
    device()->graphicDisplay(i)->circle(w/2, h/2, h/2, {0xff});
  }
}


void CablDevice::render()
{
}
