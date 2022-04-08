#ifndef __MPPT_DIGITAL_OUTPUT_HPP__
#define __MPPT_DIGITAL_OUTPUT_HPP__

#ifdef RASPBERRYPI
#include <wiringPi.h>
#else
#include <Arduino.h>
#endif

#include <IMPPTOutput.hpp>

class MPPTDigitalOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

protected:
	virtual void writeHw(const uint8_t value) override {
		digitalWrite(outPin, value);
	}

public:
	MPPTDigitalOutput(const uint8_t outPin, const bool inverted=false) : IMPPTOutput(inverted),
			outPin(outPin) {
		write(0);
		pinMode(outPin, OUTPUT);
	}

	virtual void write(const uint8_t value) override {
		if (inverted) {
			writeHw((value == 0) ? HIGH : LOW);
		} else {
			writeHw((value == 0) ? LOW : HIGH);
		}
	}
};
#endif /* __MPPT_DIGITAL_OUTPUT_HPP__ */

