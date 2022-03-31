#include <Adafruit_INA219.h>
#include <MPPT.hpp>
#include <MPPTPWMOutput.hpp>

Adafruit_INA219 ina219;
/* PB1/OC1A has to be connected to dead time control of TL494 */
const uint8_t TL494_DTC_PIN = 9;
MPPTPWMOutput mpptOutput(TL494_DTC_PIN, true);
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
    mppt.update(ina219.getBusVoltage_V(), ina219.getPower_mW() / 1000.0);
	}
}

