#pragma once
#include <array>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "..\shares\vfsdefine.h"

//#include "CClientContext.h"

namespace qizhi {
	namespace boostserver {
		 
		/// Represents a single connection from a client.
		template<typename T>
		class connection
			: public std::enable_shared_from_this<connection<T>>,
			  private boost::noncopyable

		{
		public:
			connection(const connection&) = delete;
			connection& operator=(const connection&) = delete;

			/// Construct a connection with the given socket.
			explicit connection(boost::asio::io_service& io_service);

			/// Start the first asynchronous operation for the connection.
			void start(size_t send_buff_size, size_t recv_buff_size,bool crypt);

			/// Stop all asynchronous operations associated with the connection.
			void stop();

			/// Get the socket associated with the connection.
			boost::asio::ip::tcp::socket& socket();
		private:
			/// Perform an asynchronous read operation.
			void do_read();

			/// Perform an asynchronous write operation.
			//void do_write();

			void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
			void handle_write(const boost::system::error_code& error, size_t bytes_transferred);

			void close();
		  

			/// Socket for the connection.
			boost::asio::ip::tcp::socket socket_;

		
			/// Buffer for incoming data.
			std::array<char, SESSION_BUFF_SIZE> buffer_;

			T   context_;
		
		};

		template <typename T>
		using connection_ptr = std::tr1::shared_ptr<connection<T>>;
		//typedef boost::shared_ptr<connection> connection_ptr;
 

	} // namespace server
} // namespace http

#include "connection.cpp"