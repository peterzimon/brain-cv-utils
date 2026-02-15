#include "cv-utils.h"

#include <stdio.h>

#include "brain-common/brain-common.h"

CvUtils::CvUtils()
	: button_a_(BRAIN_BUTTON_1),
	  button_b_(BRAIN_BUTTON_2),
	  current_mode_(Mode::kAttenuverter) {}

void CvUtils::init() {
	// Initialize buttons
	button_a_.init();
	button_b_.init();

	button_a_.set_on_press([this]() {
		on_button_a_press();
	});

	button_b_.set_on_single_tap([this]() {
		next_mode();
	});

	// Initialize LEDs
	leds_.init();
	leds_.startup_animation();

	// Initialize pots (3 pots, 8-bit resolution for smooth control)
	brain::ui::PotsConfig pot_config = brain::ui::create_default_config(3, 8);
	pot_config.simple = true;
	pots_.init(pot_config);

	// Initialize CV I/O
	cv_in_.init();
	cv_out_.init();
	cv_out_.set_coupling(brain::io::AudioCvOutChannel::kChannelA, brain::io::AudioCvOutCoupling::kDcCoupled);
	cv_out_.set_coupling(brain::io::AudioCvOutChannel::kChannelB, brain::io::AudioCvOutCoupling::kDcCoupled);

	// Initialize pulse I/O
	pulse_.begin();

	// Set initial mode
	set_mode(Mode::kAttenuverter);

	printf("CV Utils initialized\n");
}

void CvUtils::update() {
	// Poll hardware
	button_a_.update();
	button_b_.update();
	pots_.scan();
	cv_in_.update();

	// Dispatch to current mode
	switch (current_mode_) {
		case Mode::kAttenuverter:
			update_attenuverter();
			break;
		case Mode::kPrecisionAdder:
			update_precision_adder();
			break;
		case Mode::kSlew:
			update_slew();
			break;
		case Mode::kAdEnvelope:
			update_ad_envelope();
			break;
	}
}

void CvUtils::next_mode() {
	uint8_t next = (static_cast<uint8_t>(current_mode_) + 1) % kNumModes;
	set_mode(static_cast<Mode>(next));
}

void CvUtils::set_mode(Mode mode) {
	current_mode_ = mode;
	update_mode_leds();
	printf("Mode: %d\n", static_cast<int>(mode));
}

void CvUtils::update_mode_leds() {
	// LEDs 0-3 indicate current mode (one lit)
	for (uint8_t i = 0; i < kNumModes; i++) {
		if (i == static_cast<uint8_t>(current_mode_)) {
			leds_.on(i);
		} else {
			leds_.off(i);
		}
	}
}

// ---------- Mode stubs (Phase 2-5 will implement these) ----------

void CvUtils::update_attenuverter() {
	// TODO: Phase 2
}

void CvUtils::update_precision_adder() {
	// TODO: Phase 3
}

void CvUtils::update_slew() {
	// TODO: Phase 4
}

void CvUtils::update_ad_envelope() {
	// TODO: Phase 5
}

void CvUtils::on_button_a_press() {
	// Dispatch to mode-specific handler
	switch (current_mode_) {
		case Mode::kAttenuverter:
			// No action in attenuverter mode
			break;
		case Mode::kPrecisionAdder:
			// TODO: Reset offsets to 0V
			break;
		case Mode::kSlew:
			// TODO: Toggle linked mode
			break;
		case Mode::kAdEnvelope:
			// TODO: Manual trigger
			break;
	}
}
