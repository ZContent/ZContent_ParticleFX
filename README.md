# ZContent_ParticleFX
Point of light effects with an Adadfruit NeoPixel matrix

This Arduino library uses an Adafruit NeoPixel matrix to create animated 
special effects for points of light. These "particles" can be used singlely 
or chained together to create complex animations like fireworks, fairies 
(i.e. "Tinker Bell"), fireflies, and fire.

When effects are chained together, they can run in series or in parallel 
using the wait option.
For example, take a simple animation using the Move effect to move a particle 
from point A to point B. Add to this The Dim effect in parallel, which
creates the animation of a particle moving from point A to point B while the
brightness decreases so the particle is invisible by the time it reaches
point B. Add to this the Pause effect to delay the start of the dim until
the point is perhaps half way between point A and Point B. The Pause and
Dim effects would run in series while the Move effect would run in parallel
with the Pause and Dim effects.
This is similar to the way a shooting star appears in the sky.

## Classes

These are main classes to the ParticleFX library:

particle - This is the "point of light" that is controlled by the effects

particleEffect - The main class where all effects are derived from (see Effects
section below)

particleEffectList - This class contains the list of effects that are applied
to the particle

particlegfx - This class draws the particles onto the pixel matrix

## Effects

Here is a list of the effects that are available in the libray:

effectDim(particle *particle, bool wait, uint16_t brightness, int16_t speed)

effectPause(particle *particle, bool wait, uint16_t speed)

effectsetColor(particle *particle, bool wait, uint8_t red, uint8_t green, uint8_t blue)

effectMove(particle *particle, bool wait, int16_t movetox, int16_t movetoy, uint16_t speed = 1, bool moveback = false)

effectFloat(particle *particle, bool wait, uint16_t speed = 1, uint16_t amplitude = 4)

effectMotionBlur(particle *particle, bool wait, particlegfx *particlegfx, Adafruit_NeoMatrix *matrix, uint16_t framecount, float dimvalue = .7)

effectTwinkle(particle *particle, bool wait, uint16_t speed = 10, float dimvalue = .5 )

## Library Requirements

In addition to this library, the following GitHub libraries are needed in 
order to use this library.

Adafruit_GFX - https://github.com/adafruit/Adafruit-GFX-Library

Adafruit_NeoMatrix - https://github.com/adafruit/Adafruit_NeoMatrix

Adafruit_NeoPixel - https://github.com/adafruit/Adafruit_NeoPixel

## Hardware Requirements

This library was tested using the Adafruit NeoPixel NeoMatrix 8x8 board.
Other matrix boards may work but they have not been tested.

The M0 and NRF52480 based Feathers from Adafruit have been tested using
this library. Other Feathers may work but have not been tested.

## Examples

Examples can be found in the examples folder and a are a great way to see
how to use the library. This project and documentation
is under development and more examples are expected to be added in the future.

More animation examples and project development can be found on the author's 
twitter account @cogliano.

