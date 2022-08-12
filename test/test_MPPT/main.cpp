#include "MPPTDummyOutput.hpp"
#include <MPPT.hpp>
#include <cstdint>
#include <limits>
#include <unity.h>


const float INPUT_VOLTAGE = 12.0;
const float LOW_INPUT_VOLTAGE = 9.0;
const float INPUT_POWER = 1.0;
const float LOW_INPUT_POWER = 0.1;

MPPTDummyOutput* output = nullptr;
MPPT* mppt = nullptr;

void setUp()
{
	output = new MPPTDummyOutput;
	mppt = new MPPT(*output);
}

void tearDown()
{
	delete mppt;
	delete output;
}

void test_increasing()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
}

void test_decreasing()
{
	test_increasing();

	mppt->update(INPUT_VOLTAGE, LOW_INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_DECREASING, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_increasing2decreasingBoundary()
{
	for (int i=0; i<IMPPTOutput::maxValue; i++) {
		test_increasing();
	}
	TEST_ASSERT_EQUAL_FLOAT(100.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_DECREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
}

void test_decreasing2increasingBoundary()
{
	test_increasing();
	test_increasing();

	mppt->update(INPUT_VOLTAGE, LOW_INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_DECREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_DECREASING, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
}

void test_lowInputVoltage()
{
	mppt->update(LOW_INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_INPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
}

void test_lateLowInputVoltage()
{
	test_increasing();
	test_increasing();

	mppt->update(LOW_INPUT_VOLTAGE, INPUT_POWER);
	TEST_ASSERT_EQUAL(MPPT::STATE_INPUT_LOW, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	/* Output shall not be switched off on STATE_INPUT_LOW. It shall be ramped down */
	TEST_ASSERT_EQUAL(1, output->value);
}

int main()
{
	UNITY_BEGIN();
	RUN_TEST(test_increasing);
	RUN_TEST(test_decreasing);
	RUN_TEST(test_increasing2decreasingBoundary);
	RUN_TEST(test_decreasing2increasingBoundary);
	RUN_TEST(test_lowInputVoltage);
	RUN_TEST(test_lateLowInputVoltage);
	return UNITY_END();
}

