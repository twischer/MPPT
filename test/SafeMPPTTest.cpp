#include "MPPTDummyOutput.hpp"
#include <SafeMPPT.hpp>
#include <cstdint>
#include <limits>
#include <gtest/gtest.h>

using namespace ::testing;

class SafeMPPTTest : public ::testing::Test {
protected:
	const float LOW_INPUT_VOLTAGE = 9.0;
	const float INPUT_VOLTAGE = 12.0;
	const float INPUT_POWER = 1.0;
	const float VOLTAGE_MIN = 10.0;
	const float VOLTAGE_MAX = 15.0;
	const float VOLTAGE_MIN2 = 20.0;
	const float VOLTAGE_MAX2 = 25.0;
	const float VOLTAGE_DIFF = std::numeric_limits<float>::epsilon() * 5;

	MPPTDummyOutput output;
	MPPTDummyOutput enableOutput;
	SafeMPPT mppt;

	SafeMPPTTest() : mppt(output)
	{
		EXPECT_TRUE(mppt.addValidOutputRange(VOLTAGE_MIN, VOLTAGE_MAX, enableOutput));
	}

};

TEST_F(SafeMPPTTest, unselectedOutputRangeTooLow)
{
	EXPECT_CALL(output, writeHw(0));
	EXPECT_CALL(enableOutput, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	EXPECT_EQ(MPPT::STATE_OUTPUT_INVALID, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, unselectedOutputRangeTooHigh)
{
	EXPECT_CALL(output, writeHw(0));
	EXPECT_CALL(enableOutput, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	EXPECT_EQ(MPPT::STATE_OUTPUT_INVALID, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, lowInputVoltage)
{
	EXPECT_CALL(enableOutput, writeHw(IMPPTOutput::maxValue));
	mppt.update(LOW_INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
}

TEST_F(SafeMPPTTest, validOutputRange)
{
	EXPECT_CALL(output, writeHw(Gt(0)));
	EXPECT_CALL(enableOutput, writeHw(IMPPTOutput::maxValue));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(Gt(0)));
	EXPECT_CALL(enableOutput, writeHw(IMPPTOutput::maxValue)).Times(AnyNumber());
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, validOutputRange2)
{
	MPPTDummyOutput enableOutput2;
	EXPECT_TRUE(mppt.addValidOutputRange(VOLTAGE_MIN2, VOLTAGE_MAX2, enableOutput2));

	EXPECT_CALL(output, writeHw(Gt(0)));
	/* SafeMPPT shall not care about init of outputs on startup. This has to be done by the
	 * output implementations itself
	 */
	EXPECT_CALL(enableOutput, writeHw(0)).Times(AnyNumber());
	EXPECT_CALL(enableOutput2, writeHw(IMPPTOutput::maxValue)).Times(AtLeast(1));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN2);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(Gt(0)));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX2);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(0));
	EXPECT_CALL(enableOutput2, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX-VOLTAGE_DIFF);
	EXPECT_EQ(MPPT::STATE_OUTPUT_LOW, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, underVoltage)
{
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);

	EXPECT_CALL(output, writeHw(0));
	EXPECT_CALL(enableOutput, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN - VOLTAGE_DIFF);
	EXPECT_EQ(MPPT::STATE_OUTPUT_LOW, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(_)).Times(0);
	EXPECT_CALL(enableOutput, writeHw(_)).Times(0);
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	EXPECT_EQ(MPPT::STATE_OUTPUT_LOW, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, overVoltage)
{
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);

	EXPECT_CALL(output, writeHw(0));
	EXPECT_CALL(enableOutput, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MAX + VOLTAGE_DIFF);
	EXPECT_EQ(MPPT::STATE_OUTPUT_HIGH, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(_)).Times(0);
	EXPECT_CALL(enableOutput, writeHw(_)).Times(0);
	mppt.update(INPUT_VOLTAGE, INPUT_POWER, VOLTAGE_MIN);
	EXPECT_EQ(MPPT::STATE_OUTPUT_HIGH, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(SafeMPPTTest, addInvalidRange)
{
	EXPECT_FALSE(mppt.addValidOutputRange(VOLTAGE_MAX2, VOLTAGE_MIN2));
	EXPECT_FALSE(mppt.addValidOutputRange(VOLTAGE_MIN-1.0, VOLTAGE_MIN));
	EXPECT_FALSE(mppt.addValidOutputRange(VOLTAGE_MAX, VOLTAGE_MAX+1.0));
}

