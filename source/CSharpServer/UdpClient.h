#pragma once

#include "Endpoint.h"
#include "UdpResolver.h"

#include <server/asio/udp_client.h>

namespace CSharpServer {

    ref class UdpClient;

    class UdpClientEx : public CppServer::Asio::UDPClient
    {
    public:
        using CppServer::Asio::UDPClient::UDPClient;

        gcroot<UdpClient^> root;

        void onConnected() override;
        void onDisconnected() override;
        void onJoinedMulticastGroup(const std::string& address) override;
        void onLeftMulticastGroup(const std::string& address) override;
        void onReceived(const asio::ip::udp::endpoint& endpoint, const void* buffer, size_t size) override;
        void onSent(const asio::ip::udp::endpoint& endpoint, size_t sent) override;
        void onError(int error, const std::string& category, const std::string& message) override;
    };

    //! UDP client
    /*!
        UDP client is used to read/write data from/into the connected UDP server.
    */
    public ref class UdpClient
    {
    public:
        //! Initialize UDP client with a given service, server address and port number
        /*!
            \param service - Service
            \param address - Server address
            \param port - Server port number
        */
        UdpClient(Service^ service, String^ address, int port);
        //! Initialize UDP client with a given service, server address and scheme name
        /*!
            \param service - Service
            \param address - Server address
            \param scheme - Scheme name
        */
        UdpClient(Service^ service, String^ address, String^ scheme);
        //! Initialize UDP client with a given service and UDP endpoint
        /*!
            \param service - Service
            \param endpoint - Server UDP endpoint
        */
        UdpClient(Service^ service, UdpEndpoint^ endpoint);
        ~UdpClient() { this->!UdpClient(); }

        //! Get the client Id
        property String^ Id { String^ get() { return marshal_as<String^>(_client->get()->id().string()); } }

        //! Get the service
        property Service^ Service { CSharpServer::Service^ get() { return _service; } }

        //! Get the server address
        property String^ Address { String^ get() { return marshal_as<String^>(_client->get()->address()); } }
        //! Get the scheme name
        property String^ Scheme { String^ get() { return marshal_as<String^>(_client->get()->scheme()); } }
        //! Get the server port number
        property int Port { int get() { return _client->get()->port(); } }

        //! Get the number of bytes pending sent by the client
        property long long BytesPending { long long get() { return (long long)_client->get()->bytes_pending(); } }
        //! Get the number of bytes sent by the client
        property long long BytesSent { long long get() { return (long long)_client->get()->bytes_sent(); } }
        //! Get the number of bytes received by the client
        property long long BytesReceived { long long get() { return (long long)_client->get()->bytes_received(); } }
        //! Get the number datagrams sent by the client
        property long long DatagramsSent { long long get() { return (long long)_client->get()->datagrams_sent(); } }
        //! Get the number datagrams received by the client
        property long long DatagramsReceived { long long get() { return (long long)_client->get()->datagrams_received(); } }

        //! Get the option: reuse address
        property bool OptionReuseAddress { bool get() { return _client->get()->option_reuse_address(); } }
        //! Get the option: reuse port
        property bool OptionReusePort { bool get() { return _client->get()->option_reuse_port(); } }
        //! Get the option: bind the socket to the multicast UDP server
        property bool OptionMulticast { bool get() { return _client->get()->option_multicast(); } }
        //! Get the option: receive buffer size
        property long OptionReceiveBufferSize { long get() { return (long)_client->get()->option_receive_buffer_size(); } }
        //! Get the option: send buffer size
        property long OptionSendBufferSize { long get() { return (long)_client->get()->option_send_buffer_size(); } }

        //! Is the client connected?
        property bool IsConnected { bool get() { return _client->get()->IsConnected(); } }

        //! Connect the client (synchronous)
        /*!
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool Connect() { return _client->get()->Connect(); }
        //! Connect the client using the given DNS resolver (synchronous)
        /*!
            \param resolver - DNS resolver
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool Connect(UdpResolver^ resolver) { return _client->get()->Connect(resolver->_resolver.Value); }
        //! Disconnect the client (synchronous)
        /*!
            \return 'true' if the client was successfully disconnected, 'false' if the client is already disconnected
        */
        bool Disconnect() { return _client->get()->Disconnect(); }
        //! Reconnect the client (synchronous)
        /*!
            \return 'true' if the client was successfully reconnected, 'false' if the client is already reconnected
        */
        bool Reconnect() { return _client->get()->Reconnect(); }

        //! Connect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool ConnectAsync() { return _client->get()->ConnectAsync(); }
        //! Connect the client using the given DNS resolver (asynchronous)
        /*!
            \param resolver - DNS resolver
            \return 'true' if the client was successfully connected, 'false' if the client failed to connect
        */
        bool ConnectAsync(UdpResolver^ resolver) { return _client->get()->ConnectAsync(resolver->_resolver.Value); }
        //! Disconnect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully disconnected, 'false' if the client is already disconnected
        */
        bool DisconnectAsync() { return _client->get()->DisconnectAsync(); }
        //! Reconnect the client (asynchronous)
        /*!
            \return 'true' if the client was successfully reconnected, 'false' if the client is already reconnected
        */
        bool ReconnectAsync() { return _client->get()->ReconnectAsync(); }

        //! Join multicast group with a given address (synchronous)
        /*!
            \param address - Multicast group address
        */
        void JoinMulticastGroup(String^ address) { return _client->get()->JoinMulticastGroup(marshal_as<std::string>(address)); }
        //! Leave multicast group with a given address (synchronous)
        /*!
            \param address - Multicast group address
        */
        void LeaveMulticastGroup(String^ address) { return _client->get()->LeaveMulticastGroup(marshal_as<std::string>(address)); }

        //! Join multicast group with a given address (asynchronous)
        /*!
            \param address - Multicast group address
        */
        void JoinMulticastGroupAsync(String^ address) { return _client->get()->JoinMulticastGroupAsync(marshal_as<std::string>(address)); }
        //! Leave multicast group with a given address (asynchronous)
        /*!
            \param address - Multicast group address
        */
        void LeaveMulticastGroupAsync(String^ address) { return _client->get()->LeaveMulticastGroupAsync(marshal_as<std::string>(address)); }

        //! Send datagram to the connected server (synchronous)
        /*!
            \param buffer - Datagram buffer to send
            \return Size of sent datagram
        */
        long long Send(array<Byte>^ buffer) { return Send(buffer, 0, buffer->Length); }
        //! Send datagram to the connected server (synchronous)
        /*!
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return Size of sent datagram
        */
        long long Send(array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Send(ptr, size);
        }
        //! Send text to the connected server (synchronous)
        /*!
            \param text - Text string to send
            \return Size of sent datagram
        */
        long long Send(String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_client->get()->Send(temp.data(), temp.size());
        }
        //! Send datagram to the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, array<Byte>^ buffer) { return Send(endpoint, buffer, 0, buffer->Length); }
        //! Send datagram to the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Send(endpoint->_endpoint.Value, ptr, size);
        }
        //! Send text to the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param text - Text string to send
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_client->get()->Send(endpoint->_endpoint.Value, temp.data(), temp.size());
        }

        //! Send datagram to the connected server with timeout (synchronous)
        /*!
            \param buffer - Datagram buffer to send
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(array<Byte>^ buffer, TimeSpan^ timeout) { return Send(buffer, 0, buffer->Length, timeout); }
        //! Send datagram to the connected server with timeout (synchronous)
        /*!
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(array<Byte>^ buffer, long long offset, long long size, TimeSpan^ timeout)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Send(ptr, size, CppCommon::Timespan::nanoseconds(100 * timeout->Ticks));
        }
        //! Send text to the connected server with timeout (synchronous)
        /*!
            \param text - Text string to send
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(String^ text, TimeSpan^ timeout)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_client->get()->Send(temp.data(), temp.size(), CppCommon::Timespan::nanoseconds(100 * timeout->Ticks));
        }
        //! Send datagram to the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, array<Byte>^ buffer, TimeSpan^ timeout) { return Send(endpoint, buffer, 0, buffer->Length, timeout); }
        //! Send datagram to the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size, TimeSpan^ timeout)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Send(endpoint->_endpoint.Value, ptr, size, CppCommon::Timespan::nanoseconds(100 * timeout->Ticks));
        }
        //! Send text to the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to send
            \param text - Text string to send
            \param timeout - Timeout
            \return Size of sent datagram
        */
        long long Send(UdpEndpoint^ endpoint, String^ text, TimeSpan^ timeout)
        {
            std::string temp = marshal_as<std::string>(text);
            return (long long)_client->get()->Send(endpoint->_endpoint.Value, temp.data(), temp.size(), CppCommon::Timespan::nanoseconds(100 * timeout->Ticks));
        }

        //! Send datagram to the connected server (asynchronous)
        /*!
            \param buffer - Datagram buffer to send
            \return 'true' if the datagram was successfully sent, 'false' if the datagram was not sent
        */
        bool SendAsync(array<Byte>^ buffer) { return SendAsync(buffer, 0, buffer->Length); }
        //! Send datagram to the connected server (asynchronous)
        /*!
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return 'true' if the datagram was successfully sent, 'false' if the datagram was not sent
        */
        bool SendAsync(array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return _client->get()->SendAsync(ptr, size);
        }
        //! Send text to the connected server (asynchronous)
        /*!
            \param text - Text string to send
            \return 'true' if the text was successfully sent, 'false' if the text was not sent
        */
        bool SendAsync(String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return _client->get()->SendAsync(temp.data(), temp.size());
        }
        //! Send datagram to the given endpoint (asynchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \return 'true' if the datagram was successfully sent, 'false' if the datagram was not sent
        */
        bool SendAsync(UdpEndpoint^ endpoint, array<Byte>^ buffer) { return SendAsync(endpoint, buffer, 0, buffer->Length); }
        //! Send datagram to the given endpoint (asynchronous)
        /*!
            \param endpoint - Endpoint to send
            \param buffer - Datagram buffer to send
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return 'true' if the datagram was successfully sent, 'false' if the datagram was not sent
        */
        bool SendAsync(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return _client->get()->SendAsync(endpoint->_endpoint.Value, ptr, size);
        }
        //! Send text to the given endpoint (asynchronous)
        /*!
            \param endpoint - Endpoint to send
            \param text - Text string to send
            \return 'true' if the text was successfully sent, 'false' if the text was not sent
        */
        bool SendAsync(UdpEndpoint^ endpoint, String^ text)
        {
            std::string temp = marshal_as<std::string>(text);
            return _client->get()->SendAsync(endpoint->_endpoint.Value, temp.data(), temp.size());
        }

        //! Receive a new datagram from the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param buffer - Datagram buffer to receive
            \return Size of received datagram
        */
        long long Receive(UdpEndpoint^ endpoint, array<Byte>^ buffer) { return Receive(endpoint, buffer, 0, buffer->Length); }
        //! Receive a new datagram from the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param buffer - Datagram buffer to receive
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \return Size of received datagram
        */
        long long Receive(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Receive(endpoint->_endpoint.Value, ptr, size);
        }
        //! Receive text from the given endpoint (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param size - Text size to receive
            \return Received text
        */
        String^ Receive(UdpEndpoint^ endpoint, long long size)
        {
            return marshal_as<String^>(_client->get()->Receive(endpoint->_endpoint.Value, size));
        }

        //! Receive a new datagram from the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param buffer - Datagram buffer to receive
            \param timeout - Timeout
            \return Size of received datagram
        */
        long long Receive(UdpEndpoint^ endpoint, array<Byte>^ buffer, TimeSpan^ timeout) { return Receive(endpoint, buffer, 0, buffer->Length, timeout); }
        //! Receive a new datagram from the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param buffer - Datagram buffer to receive
            \param offset - Datagram buffer offset
            \param size - Datagram buffer size
            \param timeout - Timeout
            \return Size of received datagram
        */
        long long Receive(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long offset, long long size, TimeSpan^ timeout)
        {
            pin_ptr<Byte> ptr = &buffer[buffer->GetLowerBound(0) + (int)offset];
            return (long long)_client->get()->Receive(endpoint->_endpoint.Value, ptr, size, CppCommon::Timespan::nanoseconds(100 * timeout->Ticks));
        }
        //! Receive text from the given endpoint with timeout (synchronous)
        /*!
            \param endpoint - Endpoint to receive from
            \param size - Text size to receive
            \param timeout - Timeout
            \return Received text
        */
        String^ Receive(UdpEndpoint^ endpoint, long long size, TimeSpan^ timeout)
        {
            return marshal_as<String^>(_client->get()->Receive(endpoint->_endpoint.Value, size, CppCommon::Timespan::nanoseconds(100 * timeout->Ticks)));
        }

        //! Receive datagram from the server (asynchronous)
        void ReceiveAsync() { return _client->get()->ReceiveAsync(); }

        //! Setup option: reuse address
        /*!
            This option will enable/disable SO_REUSEADDR if the OS support this feature.

            \param enable - Enable/disable option
        */
        void SetupReuseAddress(bool enable) { return _client->get()->SetupReuseAddress(enable); }
        //! Setup option: reuse port
        /*!
            This option will enable/disable SO_REUSEPORT if the OS support this feature.

            \param enable - Enable/disable option
        */
        void SetupReusePort(bool enable) { return _client->get()->SetupReusePort(enable); }
        //! Setup option: bind the socket to the multicast UDP server
        /*!
            \param enable - Enable/disable option
        */
        void SetupMulticast(bool enable) { return _client->get()->SetupMulticast(enable); }
        //! Setup option: receive buffer size
        /*!
            This option will setup SO_RCVBUF if the OS support this feature.

            \param size - Receive buffer size
        */
        void SetupReceiveBufferSize(long size) { return _client->get()->SetupReceiveBufferSize(size); }
        //! Setup option: send buffer size
        /*!
            This option will setup SO_SNDBUF if the OS support this feature.

            \param size - Send buffer size
        */
        void SetupSendBufferSize(long size) { return _client->get()->SetupSendBufferSize(size); }

    protected:
        //! Handle client connected notification
        virtual void OnConnected() {}
        //! Handle client disconnected notification
        virtual void OnDisconnected() {}

        //! Handle client joined multicast group notification
        /*!
            \param address - Multicast group address
        */
        virtual void OnJoinedMulticastGroup(String^ address) {}
        //! Handle client left multicast group notification
        /*!
            \param address - Multicast group address
        */
        virtual void OnLeftMulticastGroup(String^ address) {}

        //! Handle datagram received notification
        /*!
            Notification is called when another datagram was received
            from some endpoint.

            \param endpoint - Received endpoint
            \param buffer - Received datagram buffer
            \param size - Received datagram buffer size
        */
        virtual void OnReceived(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long size) {}
        //! Handle datagram sent notification
        /*!
            Notification is called when a datagram was sent to the server.

            This handler could be used to send another datagram to the server
            for instance when the pending size is zero.

            \param endpoint - Endpoint of sent datagram
            \param sent - Size of sent datagram buffer
        */
        virtual void OnSent(UdpEndpoint^ endpoint, long long sent) {}

        //! Handle error notification
        /*!
            \param error - Error code
            \param category - Error category
            \param message - Error message
        */
        virtual void OnError(int error, String^ category, String^ message) {}

    internal:
        void InternalOnConnected() { OnConnected(); }
        void InternalOnDisconnected() { OnDisconnected(); }
        void InternalOnJoinedMulticastGroup(String^ address) { OnJoinedMulticastGroup(address); }
        void InternalOnLeftMulticastGroup(String^ address) { OnLeftMulticastGroup(address); }
        void InternalOnReceived(UdpEndpoint^ endpoint, array<Byte>^ buffer, long long size) { OnReceived(endpoint, buffer, size); }
        void InternalOnSent(UdpEndpoint^ endpoint, long long sent) { OnSent(endpoint, sent); }
        void InternalOnError(int error, String^ category, String^ message) { OnError(error, category, message); }

    protected:
        !UdpClient() { _client.Release(); }

    private:
        CSharpServer::Service^ _service;
        Embedded<std::shared_ptr<UdpClientEx>> _client;

    internal:
        UdpEndpoint^ _receive_endpoint;
        UdpEndpoint^ _send_endpoint;
    };

}
