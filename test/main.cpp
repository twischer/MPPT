#include "MPPTDummyOutput.hpp"
#include <SafeMPPT.hpp>
#include <cstdint>
#include <limits>
#include <unity.h>


const float INPUT_VOLTAGE = 12.0;
const float INPUT_POWER = 1.0;
const float VOLTAGE_MIN = 10.0;
const float VOLTAGE_MAX = 15.0;
const float VOLTAGE_MIN2 = 20.0;
const float VOLTAGE_MAX2 = 25.0;
const float VOLTAGE_DIFF = std::numeric_limits<float>::epsilon() * 5;
const uint8_t PWM_MAX = 128;

MPPTDummyOutput* output = nullptr;
SafeMPPT* mppt = nullptr;

void setUp()
{
	output = new MPPTDummyOutput;
	mppt = new SafeMPPT(*output);
	mppt->addValidOutputRange(VOLTAGE_MIN, VOLTAGE_MAX, PWM_MAX);
}

void tearDown()
{
	delete mppt;
	delete output;
}

void test_unselectedOutputRange()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_INVALID, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_INVALID, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_validOutputRange()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
}

void test_validOutputRange2()
{
	TEST_ASSERT_TRUE(mppt->addValidOutputRange(VOLTAGE_MIN2, VOLTAGE_MAX2));
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN2);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX2);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX-VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_underVoltage()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_overVoltage()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_HIGH, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_HIGH, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_addInvalidRange()
{
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MAX2, VOLTAGE_MIN2));
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MIN-1.0, VOLTAGE_MIN));
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MAX, VOLTAGE_MAX+1.0));
}

void test_pwmLimit()
{
	uint8_t max = 0;
	for (int i=0; i<PWM_MAX*2; i++) {
		mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
		TEST_ASSERT_LESS_OR_EQUAL(PWM_MAX, output->value);

		if (output->value > max) {
			max = output->value;
		}
	}
	TEST_ASSERT_EQUAL(PWM_MAX, max);
}

int main()
{
	UNITY_BEGIN();
	RUN_TEST(test_unselectedOutputRange);
	RUN_TEST(test_validOutputRange);
	RUN_TEST(test_validOutputRange2);
	RUN_TEST(test_underVoltage);
	RUN_TEST(test_overVoltage);
	RUN_TEST(test_addInvalidRange);
	RUN_TEST(test_pwmLimit);
	return UNITY_END();
}

