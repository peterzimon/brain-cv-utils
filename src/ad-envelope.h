#ifndef AD_ENVELOPE_H_
#define AD_ENVELOPE_H_

#include <cstdint>

#include "calibration.h"
#include "led-controller.h"
#include "brain-io/audio-cv-in.h"
#include "brain-io/audio-cv-out.h"
#include "brain-io/pulse.h"
#include "brain-ui/leds.h"
#include "brain-ui/pots.h"

class AdEnvelope {
public:
	AdEnvelope();

	void init(brain::io::Pulse& pulse);
	void update(brain::ui::Pots& pots, brain::io::AudioCvIn& cv_in,
				brain::io::AudioCvOut& cv_out, brain::io::Pulse& pulse,
				Calibration& calibration, bool button_b_pressed,
				brain::ui::Leds& leds, LedController& led_controller);

private:
	enum class Stage : uint8_t {
		kIdle = 0,
		kAttack,
		kDecay
	};
	struct EnvelopeState {
		Stage stage;
		int32_t envelope_q15;
		uint32_t stage_start_us;
		uint32_t stage_duration_us;
		bool gate_prev_high;
	};

	static constexpr uint8_t kPotAttack = 0;
	static constexpr uint8_t kPotDecay = 1;
	static constexpr uint8_t kPotShape = 2;

	// Envelope output in Q15 fixed-point signal domain (0 = 0V, kQ15One = +5V)
	static constexpr int32_t kQ15One = 32768;

	// Max envelope time ~5 seconds in microseconds
	static constexpr uint32_t kMaxTimeUs = 5000000;

	// Minimum time to avoid division by zero (~1ms)
	static constexpr uint32_t kMinTimeUs = 1000;

	// Convert pot value (0-255) to time in microseconds (logarithmic)
	static uint32_t pot_to_time_us(uint8_t pot_value);

	// Apply shape curve to linear position (0..kQ15One)
	// shape_q15: 0 = linear, kQ15One = exponential
	static int32_t apply_shape(int32_t linear_pos_q15, uint16_t shape_q15, bool is_attack);
	static void trigger_envelope(EnvelopeState& state, uint32_t now_us, uint32_t attack_us);
	static bool process_envelope(EnvelopeState& state, uint32_t now_us, uint32_t decay_us,
								 uint16_t shape_q15);

	// State
	EnvelopeState envelope_a_;
	EnvelopeState envelope_b_;
	bool button_b_prev_;
	bool pulse_triggered_;
};

#endif  // AD_ENVELOPE_H_
