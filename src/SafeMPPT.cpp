#include <SafeMPPT.hpp>
#include <iterator>

void SafeMPPT::selectOutputRange(const float outVoltage) {
	if (selectedOutputRange == nullptr) {
		for (auto& range: outputRanges) {
			if (range.minVoltage <= outVoltage &&
					outVoltage <= range.maxVoltage) {
				selectedOutputRange = &range;
				/* enable the output if available */
				if (range.output) {
					range.output->write(IMPPTOutput::maxValue);
				}
				break;
			}
		}
	}
}

bool SafeMPPT::addValidOutputRange(const float minVoltage, const float maxVoltage,
		IMPPTOutput* const output) {
	OutputRange range = {minVoltage, maxVoltage, output};
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
	} else if (outVoltage < selectedOutputRange->minVoltage) {
		state = STATE_OUTPUT_LOW;
	} else if (outVoltage > selectedOutputRange->maxVoltage) {
		state = STATE_OUTPUT_HIGH;
	}

	switch (state) {
	case STATE_INPUT_LOW:
	case STATE_INCREASING:
	case STATE_DECREASING:
		MPPT::update(inVoltage, inPower);
		break;
	default:
		switchOff();
		for (auto& range: outputRanges) {
			/* disable the output if available */
			if (range.output) {
				range.output->write(0);
			}
		}
		break;
	}
}

