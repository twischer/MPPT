#ifndef __MPPT_HPP__
#define __MPPT_HPP__

#include <stdint.h>
#include <IMPPTOutput.hpp>

class MPPT {
public:
	enum State {
		STATE_INPUT_LOW,	/* Output disabled due to too small input voltage */
		STATE_INCREASING,
		STATE_DECREASING,
		STATE_OUTPUT_INVALID,	/* Battery voltage is outside all configured vaild ranges */
		STATE_OUTPUT_LOW,	/* Battery voltage is too low for selected range */
		STATE_OUTPUT_HIGH,	/* Battery voltgae is too high for selected range */
		STATE_MAX
	};

private:
	static const uint8_t PWM_UPDATE_DIFF = 1;

	IMPPTOutput& output1;
	IMPPTOutput* const output2;
	uint8_t pwm;
	uint8_t pwmLimit;
	uint32_t lastVoltage;
	uint32_t lastPower;

	void actState();

protected:
	enum State state;

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
			lastVoltage(0), lastPower(0), state(STATE_INPUT_LOW) {
		update(0, 0);
	}

	void update(const float voltage, const float power);

	float getPwmLevel() {
		return pwm * 100.0 / IMPPTOutput::maxValue;
	}

	enum State getState() {
		return state;
	}
};

#endif /* __MPPT_HPP__ */

