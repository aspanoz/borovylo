#include "cablDevice/deviceFeedback.h"

// #include <algorithm>
// #include <cmath>
#include <sstream>

// #include <unmidify.hpp>

#include <cabl/gfx/TextDisplay.h>


namespace
{
  const sl::Color kColor_Black{0};
  const sl::Color kColor_Red{0xff, 0, 0, 0xff};
  const sl::Color kColor_Yellow{0xff, 0xff, 0, 0x55};
  const sl::Color kColor_Blue{0xff, 0, 0, 0x77};
}

namespace sl
{
  // using namespace midi;
  using namespace cabl;
  using namespace std::placeholders;

  // Init Display, draw default image
  void DeviceFeedback::initDevice()
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


  // Refresh display, put data from Reaper
  void DeviceFeedback::putValueToDisplay(double value)
  {
    std::string valueToDisplay = "REAPER: " + std::to_string(static_cast<int>(value * 0xff));

    device()->graphicDisplay(0)->black();
    device()->graphicDisplay(0)->putText(10, 10, valueToDisplay.c_str(), {0xff});
  }


  // @TODO: why I need render() method ?
  void DeviceFeedback::render()
  {
  }

} // namespace sl
