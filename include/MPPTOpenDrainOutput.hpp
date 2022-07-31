#ifndef __MPPT_OPEN_DRAIN_OUTPUT_HPP__
#define __MPPT_OPEN_DRAIN_OUTPUT_HPP__

#ifdef RASPBERRYPI
#include <wiringPi.h>
#else
#include <Arduino.h>
#endif

#include <IMPPTOutput.hpp>

class MPPTOpenDrainOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

protected:
	virtual void writeHw(const uint8_t value) override {
		if (value) {
			digitalWrite(outPin, inverted ? LOW : HIGH);
			pinMode(outPin, OUTPUT);
		} else {
			pinMode(outPin, INPUT);
		}
	}

public:
	MPPTOpenDrainOutput(const uint8_t outPin, const bool inverted=false) : IMPPTOutput(inverted),
			outPin(outPin) {
		write(0);
	}

	virtual void write(const uint8_t value) override {
		writeHw(value);
	}
};
#endif /* __MPPT_OPEN_DRAIN_OUTPUT_HPP__ */

