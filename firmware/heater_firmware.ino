// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Lambert iGEM 2025 Team and contributors
//
// LANCET cartridge thermal controller firmware
// ----------------------------------------------------------------------------
// Target board: Arduino Uno R3 (ATmega328P, 16 MHz)
// Sensor:       DS18B20 1-Wire waterproof probe on D7
//               (4.7 kohm pull-up from D7 to +5 V)
// Actuator:     SparkFun 5 V resistive heating pad driven through an N-channel
//               MOSFET (IRLB8721 or equivalent). MOSFET gate on D9 with a
//               10 kohm pull-down to ground. Pad in series with the MOSFET
//               drain; pad supply rail = benchtop 3-24 V (we use ~10 V).
// Trigger:      Momentary pushbutton between D2 and GND, INPUT_PULLUP.
//
// Five-phase thermal protocol (paper Section 3.3):
//   1) 37 C / 70 min  -- RPA isothermal amplification
//   2) 46 C /  1 min  -- lower-threshold wax valve actuation (10:7 paraffin/oil)
//   3) 37 C / 25 min  -- Cas12a collateral cleavage
//   4) 51 C /  1 min  -- upper-threshold wax valve actuation (10:8 paraffin/oil)
//   5) 37 C / 30 min  -- reporter incubation
// Total elapsed time: ~127 min.
//
// Control law: simple proportional + bang-bang hybrid.
//   - Above set point: PWM = 0 (heater off).
//   - Below set point: PWM = clamp(Kp * error, 0, 255).
// This is intentionally minimal -- the physical thermal mass of the cartridge
// dominates loop dynamics, and a full PID adds tuning burden without measured
// benefit at the targets used here. See docs/validation_protocols.md for the
// fidelity numbers.
//
// Safety: a hard cutoff at 60 C drops PWM to zero, transitions to PHASE_FAULT,
// and latches there until power-cycle. The pad's intrinsic time constant gives
// us margin even if the MOSFET fails closed (heater max ~50 C unloaded at
// 5 V, but the supply may be higher -- always verify before walking away).
//
// Dependencies (install via Arduino Library Manager):
//   - OneWire by Paul Stoffregen (>= 2.3.7)
//   - DallasTemperature by Miles Burton (>= 3.9.0)
// ----------------------------------------------------------------------------

#include <OneWire.h>
#include <DallasTemperature.h>

// ----- Pin assignments ------------------------------------------------------
constexpr uint8_t PIN_TEMP_BUS  = 7;   // DS18B20 1-Wire data
constexpr uint8_t PIN_HEATER    = 9;   // MOSFET gate (PWM)
constexpr uint8_t PIN_BUTTON    = 2;   // Start button (active-low, INPUT_PULLUP)
constexpr uint8_t PIN_LED       = LED_BUILTIN;

// ----- Control parameters ---------------------------------------------------
constexpr float   KP            = 40.0f;   // PWM units per degree C of error
constexpr uint8_t PWM_MAX       = 255;
constexpr float   SAFETY_LIMIT  = 60.0f;   // Hard cutoff (paper spec)
constexpr uint16_t SAMPLE_PERIOD_MS = 1000; // 1 Hz control + log loop

// ----- Protocol definition --------------------------------------------------
struct Phase {
  const char* name;
  float       set_point_c;
  uint32_t    duration_ms;
};

constexpr Phase PROTOCOL[] = {
  { "RPA_37C_70min",       37.0f, 70UL * 60UL * 1000UL },
  { "LOWER_VALVE_46C_1min", 46.0f,  1UL * 60UL * 1000UL },
  { "CAS12A_37C_25min",     37.0f, 25UL * 60UL * 1000UL },
  { "UPPER_VALVE_51C_1min", 51.0f,  1UL * 60UL * 1000UL },
  { "REPORTER_37C_30min",   37.0f, 30UL * 60UL * 1000UL },
};
constexpr uint8_t NUM_PHASES = sizeof(PROTOCOL) / sizeof(PROTOCOL[0]);

// ----- Runtime state --------------------------------------------------------
enum RunState : uint8_t {
  STATE_IDLE,
  STATE_RUNNING,
  STATE_DONE,
  STATE_FAULT,
};

OneWire           one_wire(PIN_TEMP_BUS);
DallasTemperature sensors(&one_wire);

RunState  run_state          = STATE_IDLE;
uint8_t   current_phase_idx  = 0;
uint32_t  phase_start_ms     = 0;
uint32_t  last_tick_ms       = 0;

// ----- Helpers --------------------------------------------------------------

// Returns true on a clean rising-edge press (active-low button with debounce).
bool button_pressed() {
  static bool     last_level = HIGH;
  static uint32_t last_change_ms = 0;
  const uint32_t  DEBOUNCE_MS = 30;

  bool level = digitalRead(PIN_BUTTON);
  uint32_t now = millis();

  if (level != last_level && (now - last_change_ms) > DEBOUNCE_MS) {
    last_change_ms = now;
    bool was_press = (last_level == HIGH && level == LOW);
    last_level = level;
    return was_press;
  }
  return false;
}

// Reads the DS18B20. Returns DEVICE_DISCONNECTED_C (= -127.0) on failure.
float read_temperature_c() {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

// Proportional controller with bang-bang ceiling at the set point.
uint8_t compute_pwm(float measured_c, float set_point_c) {
  if (measured_c >= set_point_c) {
    return 0;
  }
  float error = set_point_c - measured_c;
  long  pwm   = static_cast<long>(KP * error);
  if (pwm < 0)         pwm = 0;
  if (pwm > PWM_MAX)   pwm = PWM_MAX;
  return static_cast<uint8_t>(pwm);
}

void enter_fault(const __FlashStringHelper* reason) {
  run_state = STATE_FAULT;
  analogWrite(PIN_HEATER, 0);
  digitalWrite(PIN_LED, LOW);
  Serial.print(F("FAULT: "));
  Serial.println(reason);
}

void log_status(float measured_c, float set_point_c, uint8_t pwm) {
  // CSV: millis,state,phase_idx,phase_name,set_c,measured_c,pwm
  Serial.print(millis());
  Serial.print(F(","));
  switch (run_state) {
    case STATE_IDLE:    Serial.print(F("IDLE"));    break;
    case STATE_RUNNING: Serial.print(F("RUNNING")); break;
    case STATE_DONE:    Serial.print(F("DONE"));    break;
    case STATE_FAULT:   Serial.print(F("FAULT"));   break;
  }
  Serial.print(F(","));
  Serial.print(current_phase_idx);
  Serial.print(F(","));
  if (run_state == STATE_RUNNING) {
    Serial.print(PROTOCOL[current_phase_idx].name);
  } else {
    Serial.print(F("-"));
  }
  Serial.print(F(","));
  Serial.print(set_point_c, 2);
  Serial.print(F(","));
  Serial.print(measured_c, 2);
  Serial.print(F(","));
  Serial.println(pwm);
}

void start_protocol() {
  run_state         = STATE_RUNNING;
  current_phase_idx = 0;
  phase_start_ms    = millis();
  digitalWrite(PIN_LED, HIGH);
  Serial.println(F("# Protocol start"));
}

// Advances to the next phase or finishes if the protocol is complete.
void advance_phase() {
  current_phase_idx++;
  if (current_phase_idx >= NUM_PHASES) {
    run_state = STATE_DONE;
    analogWrite(PIN_HEATER, 0);
    digitalWrite(PIN_LED, LOW);
    Serial.println(F("# Protocol complete"));
    return;
  }
  phase_start_ms = millis();
  Serial.print(F("# Advancing to phase "));
  Serial.print(current_phase_idx);
  Serial.print(F(": "));
  Serial.println(PROTOCOL[current_phase_idx].name);
}

// ----- Arduino entry points -------------------------------------------------

void setup() {
  pinMode(PIN_HEATER, OUTPUT);
  analogWrite(PIN_HEATER, 0);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED,    OUTPUT);
  digitalWrite(PIN_LED, LOW);

  Serial.begin(115200);
  while (!Serial) { ; } // wait for USB CDC on boards that need it

  sensors.begin();
  sensors.setResolution(12);          // 12-bit, ~750 ms conversion
  sensors.setWaitForConversion(true); // blocking; loop is 1 Hz so it's fine

  Serial.println(F("# LANCET heater firmware v0.1.0 -- MIT License"));
  Serial.println(F("# CSV header: millis,state,phase_idx,phase_name,set_c,measured_c,pwm"));
  Serial.println(F("# Press the start button to begin the five-phase protocol."));
}

void loop() {
  // Poll the button regardless of state so a press from IDLE starts the run
  // and a press from DONE re-arms.
  if (button_pressed()) {
    if (run_state == STATE_IDLE || run_state == STATE_DONE) {
      start_protocol();
    }
  }

  uint32_t now = millis();
  if ((now - last_tick_ms) < SAMPLE_PERIOD_MS) {
    return;
  }
  last_tick_ms = now;

  float measured_c = read_temperature_c();

  // Sensor sanity: DS18B20 returns -127 on bus failure.
  if (measured_c <= -100.0f) {
    enter_fault(F("DS18B20 disconnected or CRC error"));
    log_status(measured_c, 0.0f, 0);
    return;
  }

  // Hard safety cutoff -- highest priority once we have a valid reading.
  if (measured_c > SAFETY_LIMIT) {
    enter_fault(F("Temperature exceeded safety limit"));
    log_status(measured_c, 0.0f, 0);
    return;
  }

  float   set_point_c = 0.0f;
  uint8_t pwm         = 0;

  switch (run_state) {
    case STATE_RUNNING: {
      const Phase& phase = PROTOCOL[current_phase_idx];
      set_point_c = phase.set_point_c;
      pwm = compute_pwm(measured_c, set_point_c);
      analogWrite(PIN_HEATER, pwm);

      if ((now - phase_start_ms) >= phase.duration_ms) {
        advance_phase();
      }
      break;
    }
    case STATE_IDLE:
    case STATE_DONE:
    case STATE_FAULT:
    default:
      analogWrite(PIN_HEATER, 0);
      pwm = 0;
      break;
  }

  log_status(measured_c, set_point_c, pwm);
}
