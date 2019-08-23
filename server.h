#pragma once

#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.h"
#include "io_service_pool.h"

namespace qizhi {
	namespace boostserver {

		/// The top-level class of the HTTP server.
		
		template<typename T>
		class server : private boost::noncopyable
		{
		public:
			//server(const server&) = delete;
			//server& operator=(const server&) = delete;

			/// Construct the server to listen on the specified TCP address and port, and
			/// serve up files from the given directory.
			server(const std::string& address, int port, size_t io_service_pool_size);

			/// Run the server's io_context loop.
			void run();
 
			void set_buffer_size(size_t send,size_t receive);
			void set_crypt(bool crypt);

			void stop();
		private:
			/// Perform an asynchronous accept operation.
			void start_accept();
 
			/// Handle completion of an asynchronous accept operation.
			void handle_accept(const boost::system::error_code& e);
 
			/// Handle a request to stop the server.
			void handle_stop();

			io_service_pool io_service_pool_;

			/// The signal_set is used to register for process termination notifications.
			boost::asio::signal_set signals_;

			size_t    thread_pool_size_;
			/// The io_context used to perform asynchronous operations.
			//boost::asio::io_context io_context_;

		 
			/// Acceptor used to listen for incoming connections.
			boost::asio::ip::tcp::acceptor acceptor_;

			connection_ptr<T>  new_connection_;

			size_t  send_buffer_size;
			size_t  recv_buffer_size;
			bool    is_crypt;
			 
		};

	} // namespace server
} // namespace http

#include "server.cpp"