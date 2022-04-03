#ifndef __MPPT_HPP__
#define __MPPT_HPP__

#include <stdint.h>
#include <IMPPTOutput.hpp>

class MPPT {
private:
	IMPPTOutput& output1;
	IMPPTOutput* const output2;
	uint8_t pwm;
	uint8_t pwmLimit;
	uint32_t lastVoltage;
	uint32_t lastPower;

protected:
	void setOutputs(const uint8_t value) {
		output1.write(value);
		if (output2 != nullptr) {
			output2->write(value);
		}
	}

	void setOutputLimit(const uint8_t limit) {
		if (pwm > limit) {
			pwm = limit;
			setOutputs(pwm);
		}
		pwmLimit = limit;
	}
public:
	MPPT(IMPPTOutput& output1, IMPPTOutput& output2) : MPPT(output1, &output2) {}

	MPPT(IMPPTOutput& output1, IMPPTOutput* const output2=nullptr) :
			output1(output1), output2(output2), pwm(0),
			pwmLimit(IMPPTOutput::maxValue),
			lastVoltage(0), lastPower(0) {
		update(0, 0);
	}

	void update(const float voltage, const float power);

	float getPwmLevel() {
		return pwm * 100.0 / IMPPTOutput::maxValue;
	}
};

#endif /* __MPPT_HPP__ */

