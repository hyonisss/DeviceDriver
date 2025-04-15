#include "gmock/gmock.h"
#include "device_driver.h"

using namespace testing;

class FlashMock : public FlashMemoryDevice
{
public:
    MOCK_METHOD(unsigned char, read, (long address), (override));
    MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, ReadSucceedsWhenAllValuesAreSame)
{
    // TODO : replace hardware with a Test Double
    FlashMock mock;
    EXPECT_CALL(mock, read(0xBB))
        .Times(5)
        .WillRepeatedly(Return(0xAB));

    DeviceDriver driver{&mock};
    int result = driver.read(0xBB);
    EXPECT_EQ(result, 0xAB);
}

// 값이 달라서 예외 발생하는 경우
TEST(DeviceDriver, ReadFailsWhenValuesDiffer)
{
    // TODO : replace hardware with a Test Double
    FlashMock mock;
    EXPECT_CALL(mock, read(0xBB))
        .WillOnce(Return(0xAA))
        .WillOnce(Return(0xAA))
        .WillOnce(Return(0xAB)) // 중간에 다름!
        .WillOnce(Return(0xAA))
        .WillOnce(Return(0xAA));

    DeviceDriver driver{&mock};
    EXPECT_THROW({ driver.read(0xBB); }, std::runtime_error); // 또는 ReadFailException (정의한 경우)
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}