# Boost-ASIO-TCP-Server
A TCP server framework implemented using boost::asio and C++ templates. The business logic is implemented in the CClientContext class, and there is no need to deal with any network communication.



SYSTEM_INFO systemInfo;
GetSystemInfo(&systemInfo);


std::size_t num_threads = (int)systemInfo.dwNumberOfProcessors * 2 + 2;

string addr = string("0.0.0.0");
qizhi::boostserver::server<CClientContext> *pBoostServer pBoostServer = new qizhi::boostserver::server<CClientContext>(addr, m_SSvr.nServerPort, num_threads);
pBoostServer->set_buffer_size(m_nSendBufferSize, m_nReceiveBufferSize);
pBoostServer->set_crypt(m_bCrypt ? true : false);
pBoostServer->run();


//////////////////////

All business logic is implemented in the CClientContext class
