#ifndef __MPPT_DUMMY_OUTPUT_HPP__
#define __MPPT_DUMMY_OUTPUT_HPP__

#include <IMPPTOutput.hpp>
#include <cstdint>

class MPPTDummyOutput : public IMPPTOutput {
public:
	uint8_t value;

	virtual void writeHw(const uint8_t v) {
		value = v;
	}
};
#endif  /* __MPPT_DUMMY_OUTPUT_HPP__ */

