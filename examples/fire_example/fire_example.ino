#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <particlegfx.h>
#include <particle.h>
#include <particlefx.h>

// Matrix pin
#define MPIN 6

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, MPIN,
  NEO_MATRIX_TOP + NEO_MATRIX_RIGHT +
  //NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

#define FIREFRAMERATE 40
#define MAXFIREPARTICLES 8

// comment out line below to see animation without motion-blur
#define MOTIONBLUR

#define COLOR_R 0xff
#define COLOR_G 0xd0
#define COLOR_B 0x60

particle *fire[MAXFIREPARTICLES];
particleEffectList *fireEffects[MAXFIREPARTICLES];
effectMove *fireMoveEffect[MAXFIREPARTICLES];
effectMotionBlur *fireMotionBlurEffect[MAXFIREPARTICLES];
effectPause *firePauseEffect[MAXFIREPARTICLES];
effectPause *fireEndPauseEffect[MAXFIREPARTICLES];
effectDim *fireDimEffect[MAXFIREPARTICLES];
particlegfx firematrix = particlegfx();

void setup() {
  // initialize fire
  for(int i =0; i < MAXFIREPARTICLES; i++)
  {
    fire[i] = new particle(NULL, 0, 0, COLOR_R, COLOR_G, COLOR_B, true);
    fire[i]->setGamma(true);
    fire[i]->setPointX(0);
    fire[i]->setPointY(78);
    fire[i]->setEnabled(false);
    fire[i]->setBrightness(1.0);
    
    fireEffects[i] = new particleEffectList();
    #ifdef MOTIONBLUR
      fireEffects[i]->appendEffect(fireMotionBlurEffect[i] = new effectMotionBlur(fire[i], false, &firematrix, &matrix, 6, 1.0));
    #endif
    // random wait to start flame
    fireEffects[i]->appendEffect(firePauseEffect[i] = new effectPause(fire[i], true, random(0,8)));
    // move flame up
    fireEffects[i]->appendEffect(fireMoveEffect[i] = new effectMove(fire[i], true, 8, 78, 6));
    // pause before dimming flame
    fireEffects[i]->appendEffect(fireEndPauseEffect[i] = new effectPause(fire[i], false, 5));
    // dim flame
    fireEffects[i]->appendEffect(fireDimEffect[i] = new effectDim(fire[i], true, 0., 5));
  }
}

void drawFlame(bool flameon)
{
  matrix.fillScreen(0);

  for(int i = 0; i < MAXFIREPARTICLES; i++)
  {
    fire[i]->setEnabled(true);
    if(fireEffects[i]->getNext() != NULL)
    {
      particleEffectList *listitem = fireEffects[i]->getNext();
      bool done = false;
      int listcount = 0;
      do
      {
        //Serial.println(String(i) + ": doEffect " + String(listcount) + ": " + String(listitem->getItem()->effectName()));
        if(!listitem->getItem()->doEffect())
        {
          listcount++;
          listitem = listitem->getNext();
          if(listitem == NULL)
            done = true;
        }
        else
          done = true;
      }
      while(!done);
      if(fireDimEffect[i]->isDone())         
      {

        // reset fire effects
        int startx = 0;
        int test = random(0,10);
        if(test < 9)
        {
          // inner flame (top of bell curve)
          startx = 24 + random(0,2)*8;
        }
        else
        {
          // outer flame (lower bell curve)
          startx = 16 + random(0,2)*24;
        }
        fire[i]->setPointX(startx);
        fire[i]->setPointY(64);
        fireMoveEffect[i]->setPointX(startx);
        fireMoveEffect[i]->setPointY(random(0,24));
        firePauseEffect[i]->setSpeed(random(0,6));
        // 3-5 looks good
        fireEndPauseEffect[i]->setSpeed(5);
        fireDimEffect[i]->setSpeed(4);
        #ifdef MOTIONBLUR
        fireMotionBlurEffect[i]->reset();
        #endif
        fire[i]->setBrightness(1.0);
        if(!flameon)
        {
          fire[i]->setPointY(78);
          fireMoveEffect[i]->setPointY(78);
        }
      }
    }
    firematrix.drawParticle(matrix, fire[i]);
  }
  matrix.show();
}

void loop() {
  static unsigned long timer;
  if(millis() - timer >= FIREFRAMERATE )
  {
    drawFlame(true);
    timer = millis();
  }
}
