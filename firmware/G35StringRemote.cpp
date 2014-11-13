/*
  G35: An Arduino library for GE Color Effects G-35 holiday lights.

  Copyright © 2011 The G35 Authors. Use, modification, and distribution are
  subject to the BSD license as described in the accompanying LICENSE file.

  Original version by Paul Martis (http://www.digitalmisery.com). See
  README for complete attributions.

  Special thanks to Richard <rkfoote@gmail.com> and "slinky" for the unrolled-
  loop protocol code that seems to work very well!
*/

#include "G35StringRemote.h"

G35StringRemote::G35StringRemote(uint8_t pin, uint8_t light_count,
                     uint8_t physical_light_count,
                     uint8_t bulb_zero, bool is_forward, char ip[], int port)
: G35(), pin_(pin), physical_light_count_(physical_light_count),
  bulb_zero_(bulb_zero), is_forward_(is_forward) {
  light_count_ = light_count;
  port_	= port;
  strncpy(ip_, ip, sizeof(ip_));
  //ip_[sizeof(ip_) - 1] = '\0';
  client_.begin(port_);
}

G35StringRemote::G35StringRemote(uint8_t pin, uint8_t light_count, char ip[], int port)
: G35(), pin_(pin), physical_light_count_(light_count),
  bulb_zero_(0), is_forward_(true) {
  light_count_ = light_count;
  port_	= port;
  strncpy(ip_, ip, sizeof(ip_));
  //ip_[sizeof(ip_) - 1] = '\0';
  client_.begin(port_);
  Serial.println(ip_);
}

void G35StringRemote::set_color(uint8_t bulb, uint8_t intensity, color_t color) {
  bulb += bulb_zero_;

  if (intensity > MAX_INTENSITY) {
    intensity = MAX_INTENSITY;
  }

  char fmt[14];
  sprintf(fmt, "S%02d%03d%03d%04d", pin_, bulb, intensity, color);
  fmt[sizeof(fmt) - 1] = '\0';
  Serial.println("Sending udp packet"); 
  client_.beginPacket(ip_, port_);
  client_.write(fmt);
  client_.endPacket();
  Serial.println("Sent packet");
}

void G35StringRemote::enumerate() {
  enumerate(is_forward_);
}

void G35StringRemote::enumerate(bool forward) {
  uint8_t count = physical_light_count_;
  uint8_t bulb = forward ? 0 : light_count_ - 1;
  int8_t delta = forward ? 1 : -1;
  while (count--) {
    set_color(bulb, MAX_INTENSITY, COLOR_RED);
    bulb += delta;
  }
}

void G35StringRemote::enumerate_forward() {
  enumerate(true);
}

void G35StringRemote::enumerate_reverse() {
  enumerate(false);
}

void G35StringRemote::do_test_patterns() {
  const uint8_t last_light = light_count_ - 1;

  // Cycle through primaries.
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_RED);
  delay(1000);
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_GREEN);
  delay(1000);
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_BLUE);
  delay(1000);

  // Tickle the ends. You should see three reds at the start, and three greens
  // at the end. This confirms that you've properly configured the strand
  // lengths and directions.
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 3; ++j) {
      set_color(BROADCAST_BULB, 0, COLOR_BLACK);
      set_color(j, MAX_INTENSITY, COLOR_RED);
      set_color(last_light - j, MAX_INTENSITY, COLOR_GREEN);
      delay(250);
    }
  }

  // Full white for five seconds. If you have power problems, they'll probably
  // show up here.
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_WHITE);
  delay(5000);

  // Cycle through secondaries.
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_YELLOW);
  delay(1000);
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_CYAN);
  delay(1000);
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_MAGENTA);
  delay(1000);

  // ... and get ready for the first program.
  fill_color(0, light_count_, MAX_INTENSITY, COLOR_BLACK);
}
