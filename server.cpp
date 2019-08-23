#include "pch.h"
#include "server.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace qizhi {
	namespace boostserver {

		template <typename T>
		server<T>::server(const std::string& address, int port, size_t io_service_pool_size)
			:io_service_pool_(io_service_pool_size),
			signals_(io_service_pool_.get_io_service()),
			acceptor_(io_service_pool_.get_io_service()),
			new_connection_()
		{
			// Register to handle the signals that indicate when the server should exit.
  // It is safe to register for the same signal multiple times in a program,
  // provided all registration for the specified signal is made through Asio.
			signals_.add(SIGINT);
			signals_.add(SIGTERM);
#if defined(SIGQUIT)
			signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
			signals_.async_wait(boost::bind(&server::handle_stop, this));


			boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);

			acceptor_.open(endpoint.protocol());
			acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

		//	typedef boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reuse_port;
		//	acceptor_.set_option(reuse_port(true));

			acceptor_.bind(endpoint);
			acceptor_.listen();
			 
			start_accept();
		}

		template <typename T>
		void server<T>::run()
		{
			io_service_pool_.run();

		}
		template <typename T>
		void server<T>::set_buffer_size(size_t send, size_t receive)
		{
			send_buffer_size = send;
			recv_buffer_size = receive;
		}

		template <typename T>
		void server<T>::set_crypt(bool crypt)
		{
			is_crypt = crypt;
		}

		template <typename T>
		void server<T>::start_accept()
		{  
			new_connection_.reset(new connection<T>(io_service_pool_.get_io_service()));

			acceptor_.async_accept(new_connection_->socket(),
				boost::bind(&server::handle_accept, this,
					boost::asio::placeholders::error));
 
		}
		 
		template <typename T>
		void server<T>::handle_accept(const boost::system::error_code& e)
		{
			if (!e)
			{
				new_connection_->start(send_buffer_size, recv_buffer_size, is_crypt);
			}

			start_accept();
		}

		template <typename T>
		void server<T>::handle_stop()
		{
			io_service_pool_.stop();
		}

		template <typename T>
		void server<T>::stop()
		{
			io_service_pool_.stop();
		}

	} // namespace server
} // namespace http