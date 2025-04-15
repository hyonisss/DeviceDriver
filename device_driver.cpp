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
    // 현재 값 읽기
    int current = static_cast<int>(m_hardware->read(address));

    // 0xFF가 아니면 이미 값이 있음 → 예외 발생
    if (current != 0xFF)
    {
        throw std::runtime_error("Write failed: memory not erased.");
    }

    // 0xFF면 새 값 쓰기
    m_hardware->write(address, static_cast<unsigned char>(data));
    // m_hardware->write(address, (unsigned char)data);
}