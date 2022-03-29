#include <Adafruit_INA219.h>
#include <MPPT.hpp>
#include <MPPTSigmaDeltaOutput.hpp>

Adafruit_INA219 ina219;
/* GPIO12 has to be connected to dead time control of TL494 */
const uint8_t TL494_DTC_PIN = 12;
MPPTSigmaDeltaOutput mpptOutput(TL494_DTC_PIN, true);
MPPT mppt(mpptOutput);


void setup()
{
	ina219.begin();
}

void loop()
{
	const unsigned long now = millis();
	static unsigned long next = now;
	if (next < now) {
		next += 250;
    mppt.update(ina219.getBusVoltage_V(), ina219.getPower_mW());
	}
}

