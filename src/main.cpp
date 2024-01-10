#include <Arduino.h>

#define MEM_CLK_PIN   1
#define MEM_START_PIN 2
#define MEM_CONT_PIN  3
#define MEM_WE_PIN    4

void setup() {
  pinMode(2, INPUT);
  //attachInterrupt(2, on_clk_change, RISING);

  pinMode(3, INPUT);

  pinMode(5, OUTPUT);

  Serial.begin(4000000);
}

void loop() {
  uint8_t memory[64][16] = { 0 };
  uint8_t addr_hi = 0;
  uint8_t addr_lo = 0;

  memset(memory, 0x0, 64 * 16);

  /*for (int i = 0; i < 64; ++i) {
    memory[i][0] = 0x4;
    memory[i][1] = 0x5;
    memory[i][2] = 0x6;
  }*/

  /*memory[0][5] = 0x0;
  memory[10][5] = 0x0;*/

  //memory[5][7] = 0x3;

  int i = 0;

  bool prev_clk = false;
  bool writing = false;

  int num_accesses = 0;

  uint8_t report = 0xFF;

  while (1) {
    bool clk = digitalReadFast(MEM_CLK_PIN);

    if (!prev_clk && clk) {
      GPIO6_GDIR &= ~((0xF << 16) | (0x3 << 22));
      if (digitalReadFast(MEM_START_PIN)) {
        writing = digitalReadFast(MEM_WE_PIN);
        uint32_t psr = GPIO6_PSR;
        addr_hi = ((psr >> 16) & 0xF) | (((psr >> 22) & 0x3) << 4);
        addr_lo = 0;

        /*if (num_accesses >= 16) {
          num_accesses = 0;

          if (report != 0xFF) {
            Serial.write(0xFF);
            Serial.write(report);
            for (int i = 0; i < 16; ++i) {
              Serial.write(memory[report][i]);
            }
          }
          report = 0xFF;
        }*/

        /*if (num_accesses == 0 || num_accesses == 1) {
          Serial.write((uint8_t)addr_hi);
        }

        ++num_accesses;
        if (num_accesses == 32) {
          num_accesses = 0;
        }*/

        /*if (!writing) {
          Serial.write(num_pulses);
          num_pulses = 0;
        }*/

      } else if (digitalReadFast(MEM_CONT_PIN)) {
        uint32_t mem = ((addr_hi & 0x1F) < 20) ? memory[addr_hi][addr_lo] : 0x00;
        GPIO6_DR_CLEAR = (0xF << 16) | (0x3 << 22);
        GPIO6_DR_SET = ((mem & 0x7) << 16);


        if (writing) {
          uint32_t psr = GPIO6_PSR;
          memory[addr_hi][addr_lo] = ((psr >> 16) & 0x7);

          /*++num_accesses;
          if (memory[addr_hi][addr_lo] != 0) {
            report = addr_hi;
          }*/
        } else {
          GPIO6_GDIR |= (0x7 << 16);
        }

        ++addr_lo;
        addr_lo &= 0xF;
      }
    }
    prev_clk = clk;

    /*
    ++i;
    if (i == 1000000) {
      i = 0;
      //Serial.println(addr_hi);
      i = 0;
      for (int j = 18; j < 20; ++j) {
        bool row_present = false;
        for (int k = 0; k < 16; ++k) {
          if (memory[j][k] != 0) {
            row_present = true;
            break;
          }
        }

        if (row_present) {
          Serial.println(j);
        }
      }
    }
    */
  }
}