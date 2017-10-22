#include "borovylo.h"

using namespace sl::cabl;

const Color kColor_Black{0};
const Color kColor_Red{0xff, 0, 0, 0xff};
const Color kColor_Yellow{0xff, 0xff, 0, 0x55};
const Color kColor_Blue{0xff, 0, 0, 0x77};


CablDevice::CablDevice(Borovylo* borovylo)
: borovylo(borovylo)
{
}


CablDevice::~CablDevice()
{
}


void CablDevice::buttonChanged(Device::Button button, bool buttonState, bool shiftState)
{
  std::string value = "Butt#" + std::to_string(static_cast<int>(button)) + (shiftState ? " SHIFT" : "");

  device()->setButtonLed(
    button,
    buttonState ? (shiftState ? kColor_Red : kColor_Yellow) : kColor_Black
  );

  device()->graphicDisplay(0)->black();
  device()->graphicDisplay(0)->putText(10, 10, value.c_str(), {0xff});

  borovylo->onCablDeviceButton(value);
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
