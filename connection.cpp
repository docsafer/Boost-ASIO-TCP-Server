#include "pch.h"

#include "connection.h"
#include <vector>
#include <boost/bind.hpp>
 

namespace qizhi {
	 
	namespace boostserver {
		 

		template<typename T>
		connection<T>::connection(boost::asio::io_service& io_service)
			: socket_(io_service)
		{
			
		}

		template<typename T>
		boost::asio::ip::tcp::socket& connection<T>::socket()
		{
			return socket_;
		}
		
		template<typename T>
		void connection<T>::start(size_t send_buff_size,size_t recv_buff_size, bool crypt)
		{ 
			socket_.set_option(boost::asio::ip::tcp::no_delay(true));
			socket_.set_option(boost::asio::socket_base::send_buffer_size(send_buff_size));
			socket_.set_option(boost::asio::socket_base::receive_buffer_size(recv_buff_size));
			socket_.set_option(boost::asio::socket_base::reuse_address(true));

            if(crypt)
			  context_.set_secure(false);
			else
			  context_.set_secure(true);
 

			do_read();
		}
 

		template<typename T>
		void connection<T>::stop()
		{
			boost::system::error_code    ignored_ec;

			socket_.close();
			socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

			
		}

		template<typename T>
		void connection<T>::do_read()
		{
			//auto self(shared_from_this());

			socket_.async_read_some(boost::asio::buffer(buffer_),
				boost::bind(&connection::handle_read, this->shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
 
 
		}
 
		template<typename T>
		void connection<T>::handle_read(const boost::system::error_code& error,size_t bytes_transferred)
		{
			//auto self(shared_from_this());

			if (!error)
			{

				bool malloc_error = false;

				if (context_.need_receive((const PVOID)buffer_.data(), bytes_transferred, malloc_error))
				{
					socket_.async_read_some(boost::asio::buffer(buffer_),
						boost::bind(&connection::handle_read, this->shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
				}
				else
				{
					if (malloc_error)
					{
						close();
					}
					else
					{
						context_.process_data();

						int len = 0;
						const char *pdata = context_.need_send(len);
						if (pdata != NULL)
						{
							boost::asio::async_write(socket_, boost::asio::buffer(pdata, len),
								boost::bind(&connection::handle_write, this->shared_from_this(),
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));

						}
						else
						{

							socket_.async_read_some(boost::asio::buffer(buffer_),
								boost::bind(&connection::handle_read, this->shared_from_this(),
									boost::asio::placeholders::error,
									boost::asio::placeholders::bytes_transferred));
						}
					}

				}


			}
			 
		}

		template<typename T>
		void connection<T>::handle_write(const boost::system::error_code& error, size_t bytes_transferred)
		{
			//auto self(shared_from_this());
			if (!error)
			{
				int len = 0;
				const char *pdata = context_.need_send(len);
				if (pdata != NULL)
				{
					boost::asio::async_write(socket_, boost::asio::buffer(pdata, len),
						boost::bind(&connection::handle_write, this->shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
				}
				else
				{
					context_.reset();
 					do_read();
 					 
				}
			}
			 
 
		}

		template<typename T>
		void connection<T>::close()
		{
			CDADebugLog::FormatLogToFile(TRUE, _T("connection closed\r\n"));
			context_.free();
			socket_.close();
		}

	} // namespace server
} // namespace http