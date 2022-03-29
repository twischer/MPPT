#ifndef __MPPT_HPP__
#define __MPPT_HPP__

#include <stdint.h>
#include <initializer_list>
#include <vector>
#include <functional>
#include <IMPPTOutput.hpp>

class MPPT {
private:
	std::vector<std::reference_wrapper<IMPPTOutput>> outputs;
	uint8_t pwm;
	uint8_t pwmLimit;
	uint32_t lastVoltage;
	uint32_t lastPower;

protected:
	void setOutputs(const uint8_t value) {
		for (IMPPTOutput& output: outputs) {
			output.write(value);
		}
	}

	void setOutputLimit(const uint8_t limit) {
		pwmLimit = limit;
	}
public:
	MPPT(IMPPTOutput& output) : MPPT({output}) {}
	MPPT(IMPPTOutput& output1, IMPPTOutput& output2) : MPPT({output1, output2}) {}

	MPPT(std::initializer_list<std::reference_wrapper<IMPPTOutput>> outputs) :
			outputs(outputs), pwm(0), pwmLimit(IMPPTOutput::maxValue),
			lastVoltage(0), lastPower(0) {
		update(0, 0);
	}

	void update(const float voltage, const float power);

	float getPwmLevel() {
		return pwm * 100.0 / IMPPTOutput::maxValue;
	}
};

#endif /* __MPPT_HPP__ */

