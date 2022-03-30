#include <iostream>
#include <MPPT.hpp>
#include <MPPTPWMOutput.hpp>

/* Connected to pin 12 on pin header (PWM0) */
#define GPIO18	1

int main(void)
{
	if (wiringPiSetup() == -1)
		exit(1);

	MPPTPWMOutput mpptOutput(GPIO18, true);
	MPPT mppt(mpptOutput);

	for (;;)
	{
		/* Accepts input lines of "N.M O.P" where N,M,O,P are numbers */
		std::string voltageStr;
		std::getline(std::cin, voltageStr, ' ');
		const float voltage = std::stof(voltageStr);

		std::string powerStr;
		std::getline(std::cin, powerStr);
		const float power = std::stof(powerStr);

		mppt.update(voltage, power);
		std::cout << "PWM: " << mppt.getPwmLevel() << " %" << std::endl;
	}
	return 0;
}

