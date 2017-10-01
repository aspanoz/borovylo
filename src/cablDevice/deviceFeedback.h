#pragma once

#include <future>
#include <cstdint>

#include <cabl/cabl.h>

namespace sl
{
  using namespace cabl;

  class DeviceFeedback : public Client
  {
    public:
      void initDevice() override;
      void render() override;

      void putValueToDisplay(double value);
  };
}
