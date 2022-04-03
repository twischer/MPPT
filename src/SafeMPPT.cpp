#include <SafeMPPT.hpp>

void SafeMPPT::selectOutputRange(const float outVoltage) {
	if (selectedOutputRange == nullptr) {
		for (auto range: outputRanges) {
			if (range.minVoltage <= outVoltage &&
					outVoltage <= range.maxVoltage) {
				selectedOutputRange = &range;
				setOutputLimit(range.maxOutputLevel);
				break;
			}
		}
	}
}

bool SafeMPPT::addValidOutputRange(const float minVoltage, const float maxVoltage,
		const uint8_t maxOutputLevel) {
	OutputRange range = {minVoltage, maxVoltage, maxOutputLevel};
	if (!range.isValid()) {
		return false;
	}

	for (auto other: outputRanges) {
		if (!range.isDisjunct(other)) {
			return false;
		}
	}

	outputRanges.push_back(range);
	return true;
}

void SafeMPPT::update(const float inVoltage, const float inPower, const float outVoltage) {
	/* keep output switched off when output under/over voltage was detected */
	switch (state) {
	case STATE_OUTPUT_LOW:
	case STATE_OUTPUT_HIGH:
		return;
	}

	selectOutputRange(outVoltage);

	if (selectedOutputRange == nullptr) {
		state = STATE_OUTPUT_INVALID;
		setOutputs(0);
	} else if (outVoltage < selectedOutputRange->minVoltage) {
		state = STATE_OUTPUT_LOW;
		setOutputs(0);
	} else if (outVoltage > selectedOutputRange->maxVoltage) {
		state = STATE_OUTPUT_HIGH;
		setOutputs(0);
	} else {
		MPPT::update(inVoltage, inPower);
	}
}

