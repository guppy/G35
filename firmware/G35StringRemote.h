/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.
  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.
*/

#ifndef INCLUDE_G35_STRINGREMOTE_H
#define INCLUDE_G35_STRINGREMOTE_H

#include "G35.h"

// A G35String knows how to talk to a real GE Color Effects light string.
// In particular, it implements the set_color() method of the G35 interface.
//
// These strings need enumerate() to be called before anything else, so that
// each bulb has an individual address. In many cases, your lights will work
// as expected even if you forget to call enumerate(), because most programs
// happen to do the same thing as enumerate() -- namely, they send commands
// to all bulbs starting with bulb #0 and ending with bulb #N-1. If your
// light programs look right but fractured, it's because you forgot to call
// enumerate().
class G35StringRemote : public G35 {
 public:
  // |pin|: the Arduino pin driving this string's data line.
  // |light_count|: the number of visible, physical bulbs on the string.
  // |physical_light_count|: the number of physical bulbs on the string.
  // |bulb_zero|: the index of the first bulb (almost always zero).
  // |is_forward|: true if the closest bulb to the plug has the lowest index.
  G35StringRemote(uint8_t pin, uint8_t light_count, uint8_t physical_light_count,
            uint8_t bulb_zero, bool is_forward, char ip[], int port);
  G35StringRemote(uint8_t pin, uint8_t light_count, char ip[], int port);

  // Implementation of G35 interface.
  virtual uint16_t get_light_count() { return light_count_; }
  void set_color(uint8_t led, uint8_t intensity, color_t color);

  // Initialize lights by giving them each an address.
  void enumerate();

  // Displays known-good patterns. Useful to prevent insanity during hardware
  // debugging.
  void do_test_patterns();

 protected:
  virtual uint8_t get_broadcast_bulb() { return BROADCAST_BULB; }

 private:
  uint8_t pin_;
  uint8_t physical_light_count_;
  uint8_t bulb_zero_;
  bool is_forward_;
  byte ip_[4] = {192, 168, 79, 95};
  int port_;
  UDP client_;

  enum {
    MAX_INTENSITY = 0xcc,
    BROADCAST_BULB = 63,
  };

  // Initialize lights by giving them each an address. enumerate_forward()
  // numbers the bulb closest to the controller 0, and enumerate_reverse()
  // numbers the farthest bulb 0.
  void enumerate(bool reverse);
  void enumerate_forward();
  void enumerate_reverse();
};

#endif  // INCLUDE_G35_STRINGREMOTE_H
