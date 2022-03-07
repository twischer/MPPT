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
	if (switchedOff) {
		return;
	}

	selectOutputRange(outVoltage);

	if (selectedOutputRange == nullptr) {
		setOutputs(0);
	} else if (outVoltage < selectedOutputRange->minVoltage ||
			outVoltage > selectedOutputRange->maxVoltage) {
		setOutputs(0);
		switchedOff = true;
	} else {
		MPPT::update(inVoltage, inPower);
	}
}

