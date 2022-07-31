#include "MPPT.hpp"
#include "IMPPTOutput.hpp"

void MPPT::update(const float voltage, const float power)
{
	const float hysteresis = 0.1;

	if (voltage < 10.0) {
		state = STATE_INPUT_LOW;
	} else if (pwm > (IMPPTOutput::maxValue - PWM_UPDATE_DIFF)) {
		state = STATE_DECREASING;
	} else if (pwm < PWM_UPDATE_DIFF) {
		state = STATE_INCREASING;
	} else if ( (power + hysteresis) < lastPower) {
		lastPower = power;
		switch (state) {
		case STATE_INCREASING:
			state = STATE_DECREASING;
			break;
		default:
			state = STATE_INCREASING;
			break;
		}
	} else if (power > lastPower) {
		lastPower = power;
		/* keep current state */
	}

	actState();
}

void MPPT::actState()
{
	switch (state) {
	case STATE_INCREASING:
		if (pwm > (IMPPTOutput::maxValue - PWM_UPDATE_DIFF)) {
			pwm = IMPPTOutput::maxValue;
		} else {
			pwm += PWM_UPDATE_DIFF;
		}
		break;
	default:
		/* required in case voltage too small */
		if (pwm <= PWM_UPDATE_DIFF) {
			pwm = 0;
		} else {
			pwm -= PWM_UPDATE_DIFF;
		}
		break;
	}

	setOutputs(pwm);
}

