#ifndef __IMPPTOUTPUT_HPP__
#define __IMPPTOUTPUT_HPP__

#include <stdint.h>

class IMPPTOutput {
private:
	const bool inverted;

protected:
	virtual void writeHw(const uint8_t value) = 0;

public:
	static const uint8_t maxValue = 0xFF;

	IMPPTOutput(const bool inverted=false) : inverted(inverted) { }

	void write(const uint8_t value) {
		if (inverted) {
			writeHw(maxValue - value);
		} else {
			writeHw(value);
		}
	}
};
#endif /* __IMPPTOUTPUT_HPP__ */

