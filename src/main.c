// SPDX-License-Identifier: MIT
// Copyright 2024 Algovoid

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    pin_t pin_1A;
    pin_t pin_1B;
    pin_t pin_1C;
    pin_t pin_1D;

    pin_t pin_2A;
    pin_t pin_2B;
    pin_t pin_2C;
    pin_t pin_2D;

    pin_t pin_1Y;
    pin_t pin_2Y;

} chip_state_t;

uint8_t subGate1;
uint8_t subGate2;
uint8_t subGate3;
uint8_t subGate4;

static void set_logic(chip_state_t *chip) {
  subGate1 = pin_read(chip->pin_1A) & pin_read(chip->pin_1B) ;
  subGate2 = pin_read(chip->pin_1C) & pin_read(chip->pin_1D);
  subGate3 = pin_read(chip->pin_2A) & pin_read(chip->pin_2B) ;
  subGate4 = pin_read(chip->pin_2C) & pin_read(chip->pin_2D);

  pin_write(chip->pin_1Y, !( subGate1 | subGate2 ) );
  pin_write(chip->pin_2Y, !( subGate3 | subGate4 ) );
}


static void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t*)user_data;
  set_logic(chip);
  // uncomment to see gates and output states
  /*
  printf("Gate 1 :  %d\n" , subGate1);
  printf("Gate 2 :  %d\n" , subGate2);
  printf("Gate 3 :  %d\n" , subGate3);
  printf("Gate 4 :  %d\n" , subGate4);
  printf("Output State 1Y :  %u\n" , pin_read(chip->pin_1Y));
  printf("Output State 2Y :  %u\n" , pin_read(chip->pin_2Y));  */
}


void chip_init() {
  printf("Initialize sn74s51 chip!\n");
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->pin_1A = pin_init("1A", INPUT);
  chip->pin_1B = pin_init("1B", INPUT);
  chip->pin_1C = pin_init("1C", INPUT);
  chip->pin_1D = pin_init("1D", INPUT);

  chip->pin_2A = pin_init("2A", INPUT);
  chip->pin_2B = pin_init("2B", INPUT);
  chip->pin_2C = pin_init("2C", INPUT);
  chip->pin_2D = pin_init("2D", INPUT);

  chip->pin_1Y = pin_init("1Y", OUTPUT);
  chip->pin_2Y = pin_init("2Y", OUTPUT);

  pin_write(chip->pin_1A, LOW);
  pin_write(chip->pin_1B, LOW);

  pin_write(chip->pin_1C, LOW);
  pin_write(chip->pin_1D, LOW);

  pin_write(chip->pin_2A, LOW);
  pin_write(chip->pin_2B, LOW);

  pin_write(chip->pin_2C, LOW);
  pin_write(chip->pin_2D, LOW);

  const pin_watch_config_t config = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };

  pin_watch(chip->pin_1A, &config);
  pin_watch(chip->pin_1B, &config);

  pin_watch(chip->pin_1C, &config);
  pin_watch(chip->pin_1D, &config);

  pin_watch(chip->pin_2A, &config);
  pin_watch(chip->pin_2B, &config);

  pin_watch(chip->pin_2C, &config);
  pin_watch(chip->pin_2D, &config);  

  set_logic(chip);
}