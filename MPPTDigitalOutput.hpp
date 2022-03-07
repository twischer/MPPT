#ifndef __MPPT_DIGITAL_OUTPUT_HPP__
#define __MPPT_DIGITAL_OUTPUT_HPP__

#include <Arduino.h>
#include <IMPPTOutput.hpp>

class MPPTDigitalOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

protected:
	virtual void writeHw(const uint8_t value) override {
                const uint8_t digitalValue = (value == 0) ? LOW : HIGH;
		digitalWrite(outPin, value);
	}

public:
	MPPTDigitalOutput(const uint8_t outPin, const bool inverted=false) : IMPPTOutput(inverted),
			outPin(outPin) {
		write(0);
		pinMode(outPin, OUTPUT);
	}
};
#endif /* __MPPT_DIGITAL_OUTPUT_HPP__ */

