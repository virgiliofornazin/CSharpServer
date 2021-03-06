#include "stdafx.h"

#include "UdpResolver.h"

namespace CSharpServer {

    UdpResolver::UdpResolver(CSharpServer::Service^ service) :
        _service(service),
        _resolver(new std::shared_ptr<CppServer::Asio::UDPResolver>(std::make_shared<CppServer::Asio::UDPResolver>(service->_service.Value)))
    {
    }

}
