#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>
#include "NeoPixelBusLg.h"

enum output_select
{
    output_ring,
    output_strip
};

class Utils
{
public:
    template <typename V, typename T, typename H>
    void DrawGradient(NeoPixelBusLg<V, T, H> *, RgbColor startColor, RgbColor finishColor, uint16_t startIndex, uint16_t finishIndex);
    void SelectOutput(output_select);
    void ToggleOutput();
    output_select GetSelectedOutput();

    Utils(int, int);
    ~Utils(){};

private:
    int _SelectPin1, _SelectPin2;

    output_select _selection = output_strip;
};

#include "Utils.ipp"
#endif