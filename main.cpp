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

TEST(DeviceDriver, WriteSuccess)
{
    FlashMock mock;
    EXPECT_CALL(mock, read(0xAB))
        .Times(1)
        .WillOnce(Return(0xFF)); // 메모리가 비어 있음

    EXPECT_CALL(mock, write(0xAB, 0x42))
        .Times(1); // write가 정확히 한 번 호출되어야 함

    DeviceDriver driver{&mock};
    driver.write(0xAB, 0x42);
}

TEST(DeviceDriver, WriteFailsWhenNotErased)
{
    FlashMock mock;

    EXPECT_CALL(mock, read(0xAB))
        .Times(1)
        .WillOnce(Return(0x00)); // 이미 값이 있음

    // write는 호출되지 않아야 하므로 기대 설정 안 함

    DeviceDriver driver{&mock};

    EXPECT_THROW({ driver.write(0xAB, 0x42); }, std::runtime_error);
}

int main()
{
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}