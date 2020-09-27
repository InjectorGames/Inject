#pragma once
#include "asio.hpp"
#include "asio/ssl.hpp"

namespace Injector
{
	class SslTcpSession
	{
	 protected:
		asio::ssl::stream<asio::ip::tcp::socket> stream;

		void endSession();
		virtual void onSession();
	 public:
		SslTcpSession(
			asio::ssl::stream<asio::ip::tcp::socket> stream);
		virtual ~SslTcpSession();

		void onHandshake(const asio::error_code& error);
	};
}