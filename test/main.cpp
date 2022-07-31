#include "MPPTDummyOutput.hpp"
#include <SafeMPPT.hpp>
#include <cstdint>
#include <limits>
#include <unity.h>


const float LOW_INPUT_VOLTAGE = 9.0;
const float INPUT_VOLTAGE = 12.0;
const float INPUT_POWER = 1.0;
const float VOLTAGE_MIN = 10.0;
const float VOLTAGE_MAX = 15.0;
const float VOLTAGE_MIN2 = 20.0;
const float VOLTAGE_MAX2 = 25.0;
const float VOLTAGE_DIFF = std::numeric_limits<float>::epsilon() * 5;

MPPTDummyOutput* output = nullptr;
MPPTDummyOutput* enableOutput = nullptr;
SafeMPPT* mppt = nullptr;

void setUp()
{
	output = new MPPTDummyOutput;
	enableOutput = new MPPTDummyOutput;
	mppt = new SafeMPPT(*output);
	mppt->addValidOutputRange(VOLTAGE_MIN, VOLTAGE_MAX, *enableOutput);
}

void tearDown()
{
	delete mppt;
	delete enableOutput;
	delete output;
}

void test_unselectedOutputRange()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_INVALID, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_INVALID, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
}

void test_lowInputVoltage()
{
	mppt->update(LOW_INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, enableOutput->value);
}

void test_validOutputRange()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, enableOutput->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, enableOutput->value);
}

void test_validOutputRange2()
{
	MPPTDummyOutput enableOutput2;
	TEST_ASSERT_TRUE(mppt->addValidOutputRange(VOLTAGE_MIN2, VOLTAGE_MAX2, enableOutput2));
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN2);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, enableOutput2.value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX2);
	TEST_ASSERT_EQUAL(MPPT::STATE_INCREASING, mppt->getState());
	// TODO add when supported by Unity TEST_ASSERT_GREATER_THAN_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_GREATER_THAN(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
	TEST_ASSERT_EQUAL(IMPPTOutput::maxValue, enableOutput2.value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX-VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
	TEST_ASSERT_EQUAL(0, enableOutput2.value);
}

void test_underVoltage()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_LOW, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
}

void test_overVoltage()
{
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_HIGH, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);

	mppt->update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	TEST_ASSERT_EQUAL(MPPT::STATE_OUTPUT_HIGH, mppt->getState());
	TEST_ASSERT_EQUAL_FLOAT(0.0, mppt->getPwmLevel());
	TEST_ASSERT_EQUAL(0, output->value);
	TEST_ASSERT_EQUAL(0, enableOutput->value);
}

void test_addInvalidRange()
{
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MAX2, VOLTAGE_MIN2));
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MIN-1.0, VOLTAGE_MIN));
	TEST_ASSERT_FALSE(mppt->addValidOutputRange(VOLTAGE_MAX, VOLTAGE_MAX+1.0));
}

int main()
{
	UNITY_BEGIN();
	RUN_TEST(test_unselectedOutputRange);
	RUN_TEST(test_lowInputVoltage);
	RUN_TEST(test_validOutputRange);
	RUN_TEST(test_validOutputRange2);
	RUN_TEST(test_underVoltage);
	RUN_TEST(test_overVoltage);
	RUN_TEST(test_addInvalidRange);
	return UNITY_END();
}

