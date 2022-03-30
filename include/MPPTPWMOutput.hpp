#ifndef __MPPT_PWM_OUTPUT_HPP__
#define __MPPT_PWM_OUTPUT_HPP__

#ifdef RASPBERRYPI
#include <wiringPi.h>
#else
#include <Arduino.h>
#endif

#include <IMPPTOutput.hpp>

class MPPTPWMOutput : public IMPPTOutput {
private:
	const uint8_t outPin;

protected:
	virtual void writeHw(const uint8_t value) override {
		pwmWrite(outPin, value);
	}

public:
	MPPTPWMOutput(const uint8_t outPin, const bool inverted=false) : IMPPTOutput(inverted),
			outPin(outPin) {
		pinMode(outPin, PWM_OUTPUT);
		pwmSetMode(PWM_MODE_MS);
		/* pwmFrequency in Hz = 19.2e6 Hz / pwmClock / pwmRange */
		pwmSetClock(2);
		pwmSetRange(maxValue);
		write(0);
	}
};
#endif /* __MPPT_PWM_OUTPUT_HPP__ */

