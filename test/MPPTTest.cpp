#include "MPPTDummyOutput.hpp"
#include <MPPT.hpp>
#include <cstdint>
#include <gtest/gtest.h>

using namespace testing;

class MPPTTest : public ::testing::Test {
protected:
	const float INPUT_VOLTAGE = 12.0;
	const float LOW_INPUT_VOLTAGE = 9.0;
	const float INPUT_POWER = 1.0;
	const float LOW_INPUT_POWER = 0.1;

	MPPTDummyOutput output;
	MPPT mppt;

	MPPTTest() : mppt(output) {}
};

TEST_F(MPPTTest, increasing)
{
	EXPECT_CALL(output, writeHw(Ge(0)));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());
}

TEST_F(MPPTTest, decreasing)
{
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());

	EXPECT_CALL(output, writeHw(0));
	mppt.update(INPUT_VOLTAGE, LOW_INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_DECREASING, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(MPPTTest, increasing2decreasingBoundary)
{
	EXPECT_CALL(output, writeHw(_)).Times(IMPPTOutput::maxValue-1);
	for (int i=0; i<IMPPTOutput::maxValue-1; i++) {
		mppt.update(INPUT_VOLTAGE, INPUT_POWER);
		EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	}

	EXPECT_CALL(output, writeHw(IMPPTOutput::maxValue));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_FLOAT_EQ(100.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(Ge(0)));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_DECREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());
}

TEST_F(MPPTTest, decreasing2increasingBoundary)
{
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());

	EXPECT_CALL(output, writeHw(Ge(0)));
	mppt.update(INPUT_VOLTAGE, LOW_INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_DECREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(0));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_DECREASING, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());

	EXPECT_CALL(output, writeHw(Ge(0)));
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());
}

TEST_F(MPPTTest, lowInputVoltage)
{
	EXPECT_CALL(output, writeHw(0));
	mppt.update(LOW_INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INPUT_LOW, mppt.getState());
	EXPECT_FLOAT_EQ(0.0, mppt.getPwmLevel());
}

TEST_F(MPPTTest, lateLowInputVoltage)
{
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());
	mppt.update(INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INCREASING, mppt.getState());

	/* Output shall not be switched off on STATE_INPUT_LOW. It shall be ramped down */
	EXPECT_CALL(output, writeHw(Ge(0)));
	mppt.update(LOW_INPUT_VOLTAGE, INPUT_POWER);
	EXPECT_EQ(MPPT::STATE_INPUT_LOW, mppt.getState());
	EXPECT_LT(0.0, mppt.getPwmLevel());
}

