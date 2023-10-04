#include "Utils.hpp"
#include "NeoPixelBusLg.h"

Utils::Utils(int Sel_1, int Sel_2)
{
    Utils::_SelectPin1 = Sel_1;
    Utils::_SelectPin2 = Sel_2;
};

template <typename V, typename T, typename H>
void Utils::DrawGradient(NeoPixelBusLg<V, T, H> *_bus, RgbColor startColor, RgbColor finishColor,
                         uint16_t startIndex, uint16_t finishIndex)
{
    uint16_t delta = finishIndex - startIndex;

    for (uint16_t index = startIndex; index < finishIndex; index++)
    {
        float progress = static_cast<float>(index - startIndex) / delta;
        RgbColor color = RgbColor::LinearBlend(startColor, finishColor, progress);

        _bus->SetPixelColor(index, color);
    }
}

output_select Utils::GetSelectedOutput()
{
    return Utils::_selection;
}

void Utils::SelectOutput(output_select _selection)
{
    noInterrupts();
    uint32_t temp = GPO;
    if (_selection == output_strip)
    {
        temp &= ~(1 << Utils::_SelectPin1);
        temp |= 1 << Utils::_SelectPin2;
        Utils::_selection = output_strip;
    }
    else
    {
        temp &= ~(1 << Utils::_SelectPin2);
        temp |= 1 << Utils::_SelectPin1;
        Utils::_selection = output_ring;
    }
    GPO = temp;
    interrupts();
}

void Utils::ToggleOutput()
{
    if (Utils::_selection == output_strip)
    {
        Utils::SelectOutput(output_ring);
        Utils::_selection = output_ring;
    }
    else
    {
        Utils::SelectOutput(output_strip);
        Utils::_selection = output_strip;
    }
}