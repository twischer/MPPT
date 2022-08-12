#ifndef __MPPT_DUMMY_OUTPUT_HPP__
#define __MPPT_DUMMY_OUTPUT_HPP__

#include <IMPPTOutput.hpp>
#include <gmock/gmock.h>

class MPPTDummyOutput : public IMPPTOutput {
public:
	MOCK_METHOD(void, writeHw, (const uint8_t));
};
#endif  /* __MPPT_DUMMY_OUTPUT_HPP__ */

