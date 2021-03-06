/*
    Copyright(c) Microsoft Open Technologies, Inc. All rights reserved.

    The MIT License(MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files(the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#pragma once
#include "IStream.h"
#include <mutex>

namespace Microsoft {
namespace Maker {
namespace Serial {

public ref class BluetoothSerial sealed : public IStream
{
public:
    virtual event IStreamConnectionCallback ^ConnectionEstablished;
    virtual event IStreamConnectionCallbackWithMessage ^ConnectionLost;
    virtual event IStreamConnectionCallbackWithMessage ^ConnectionFailed;

    [Windows::Foundation::Metadata::DefaultOverload]
    ///<summary>
    ///A constructor which accepts a string corresponding to a device name or ID to connect to.
    ///</summary>
    BluetoothSerial(
        Platform::String ^device_name_
        );

    ///<summary>
    ///A constructor which accepts a DeviceInformation object to explicitly specify which device to connect to.
    ///</summary>
    BluetoothSerial(
        Windows::Devices::Enumeration::DeviceInformation ^device_
        );

    virtual
    ~BluetoothSerial(
        void
        );

    virtual
    uint16_t
    available(
        void
        );

    virtual
    void
    begin(
        uint32_t baud_,
        SerialConfig config_
        );

    virtual
    bool
    connectionReady(
        void
        );

    virtual
    void
    end(
        void
        );

    virtual
    void
    flush(
        void
        );

    virtual
    void
    lock(
        void
        );

    virtual
    uint16_t
    read(
        void
        );

    virtual
    void
    unlock(
        void
        );

    virtual
    uint32_t
    write(
        uint8_t c_
        );

    ///<summary>
    ///Begins an asyncronous request for all Bluetooth devices that are paired and may be used to attempt a device connection.
    ///</summary>
    static
    Windows::Foundation::IAsyncOperation<Windows::Devices::Enumeration::DeviceInformationCollection ^> ^
    listAvailableDevicesAsync(
        void
        );

private:
    //maximum amount of data that may be read at a time, allows efficient reads
    static const uint8_t MAX_READ_SIZE = 100;

    // Device specific members (set during instantation)
    Windows::Devices::Enumeration::DeviceInformation ^_device;
    Platform::String ^_device_name;

    //thread-safe mechanisms. std::unique_lock used to manage the lifecycle of std::mutex
    std::mutex _blutex;
    std::unique_lock<std::mutex> _bluetooth_lock;

    std::atomic_bool _connection_ready;
    Windows::Storage::Streams::DataReaderLoadOperation ^_current_load_operation;
    Windows::Devices::Enumeration::DeviceInformationCollection ^_device_collection;
    Windows::Devices::Bluetooth::Rfcomm::RfcommDeviceService ^_rfcomm_service;
    Windows::Networking::Sockets::StreamSocket ^_stream_socket;
    Windows::Storage::Streams::DataReader ^_rx;
    Windows::Storage::Streams::DataWriter ^_tx;

    Concurrency::task<void>
    connectToDeviceAsync(
        Windows::Devices::Enumeration::DeviceInformation ^device_
        );

    Windows::Devices::Enumeration::DeviceInformation ^
    identifyDeviceFromCollection(
        Windows::Devices::Enumeration::DeviceInformationCollection ^devices_
        );
};

} // namespace Serial
} // namespace Maker
} // namespace Microsoft
