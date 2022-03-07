#ifndef __MPPT_SIGMA_DELTA_OUTPUT_HPP__
#define __MPPT_SIGMA_DELTA_OUTPUT_HPP__

#include <IMPPTOutput.hpp>
#include <sigma_delta.h>

class MPPTSigmaDeltaOutput : public IMPPTOutput {
private:
	const uint8_t sigmaDeltaChannel;

protected:
	virtual void writeHw(const uint8_t value) override {
		sigmaDeltaWrite(sigmaDeltaChannel, value);
	}

public:
	MPPTSigmaDeltaOutput(const uint8_t outPin, const bool inverted=false,
			const uint8_t sigmaDeltaChannel = 0) : IMPPTOutput(inverted),
			sigmaDeltaChannel(sigmaDeltaChannel) {
		/* do not use max of 312,5kHz due to ESP freezes near by 128 -> 40MHz */
		sigmaDeltaSetup(sigmaDeltaChannel, 19531);
		write(0);
		sigmaDeltaAttachPin(outPin, sigmaDeltaChannel);
	}
};
#endif /* __MPPT_SIGMA_DELTA_OUTPUT_HPP__ */

