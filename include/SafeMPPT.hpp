#ifndef __SAFE_MPPT_HPP__
#define __SAFE_MPPT_HPP__

#include <stdint.h>
#include <vector>
#include <MPPT.hpp>
#include <IMPPTOutput.hpp>

/**
 * The MPPT output will be disabled as long as no valid range for the output voltage is given or
 * the output voltage was outside the selected range.
 * Multiple ranges can be defined with calling  addValidOutputRange().
 * A range will be selected in the first call of update() depending on the given outVoltage.
 * The output will be disabled for ever when the outVoltage was once outside the selected range.
 * This error condition can only be reset by recreating the object of this class.
 * The selection of a range is also persistent until recreation of the object of this class.
 */
class SafeMPPT : public MPPT {
private:
	class OutputRange {
	public:
		const float minVoltage;
		const float maxVoltage;
		IMPPTOutput* const output;

		bool isValid() {
			return (minVoltage < maxVoltage);
		}

		/**
		 * This method only works in case both OutputRange objects are valid and
		 * checked by isValid()
		 */
		bool isDisjunct(OutputRange& other) {
			return (other.maxVoltage < minVoltage ||
				maxVoltage < other.minVoltage);
		}
	};

	std::vector<OutputRange> outputRanges;
	OutputRange* selectedOutputRange;

	void selectOutputRange(const float outVoltage);

public:
	template<typename... Args>
	SafeMPPT(Args&... args) : MPPT(args...), outputRanges(), selectedOutputRange(nullptr) {
	}

	bool addValidOutputRange(const float minVoltage, const float maxVoltage,
			IMPPTOutput& output) {
		return addValidOutputRange(minVoltage, maxVoltage, &output);
	}
	bool addValidOutputRange(const float minVoltage, const float maxVoltage,
			IMPPTOutput* const output=nullptr);
	void update(const float inVoltage, const float inPower, const float outVoltage);
};

#endif /* __SAFE_MPPT_HPP__ */

