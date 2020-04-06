#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <particlegfx.h>
#include <particle.h>
#include <particlefx.h>
#include "middlerpath.h"
#include "pongpath.h"
#include "ziggypath.h"

particle *fireflies[3];

// Matrix pin
#define MPIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, MPIN,
  NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
  //NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

#define FIREFLYFRAMERATE 100
particlegfx ffmatrix = particlegfx();

void setup() {
  // Each firefly has its own path
  
  // middlerpath keeps the firefly in the middle
  fireflies[0] = new particle((int16_t *) middlerpath, sizeof(middlerpath), middlerloopstart, 0, 0xff, 0, false);
  
  // pongpath moves the firefly left to right and back again
  fireflies[1] = new particle((int16_t *) pongpath, sizeof(pongpath), pongloopstart, 0, 0xff, 0, false);
  
  // ziggypath moves the firefly in a zig-zag fashion
  fireflies[2] = new particle((int16_t *) ziggypath, sizeof(ziggypath), ziggyloopstart, 0, 0xff, 0, false);

  for(int i = 0; i < 3; i++)
  {
    fireflies[i]->resetPoint();
    fireflies[i]->setEnabled(true);
  }
}

void loop() {
  static unsigned long timer;
  if(millis() - timer >= FIREFLYFRAMERATE)
  {
    timer = millis();
    matrix.fillScreen(0);
    for(int i = 0; i < 3; i++)
    {
      ffmatrix.drawParticle(matrix, fireflies[i], true);
      fireflies[i]->nextPoint();
    }
    matrix.show();
  }

}
