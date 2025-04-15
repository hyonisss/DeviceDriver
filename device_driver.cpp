#include "device_driver.h"
#include <stdexcept> // 예외 사용 시 필요

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    // TODO: implement this method properly
    const int readCount = 5;
    int values[readCount];

    for (int i = 0; i < readCount; ++i)
    {
        values[i] = static_cast<int>(m_hardware->read(address));
    }

    for (int i = 1; i < readCount; ++i)
    {
        if (values[i] != values[0])
        {
            throw std::runtime_error("Read failed: values are not consistent.");
        }
    }

    return values[0]; // 모든 값이 같을 때만 반환
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}