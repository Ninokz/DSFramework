#include "Session.h"

#include "SessionManager.h"
#include "EventHandler.h"

namespace DSFramework {
	namespace DSCommunication {
		Session::Session(boost::asio::io_context& ioContext, IEventHandler* eventHandler, uint8_t recvQSize):
			m_uuid(boost::uuids::to_string(boost::uuids::random_generator()())),
			m_lastActiveTime(boost::posix_time::microsec_clock::local_time()),
			m_eventHandler(eventHandler),
			m_socket(ioContext),
			m_sendQueueMaxSize(recvQSize)
		{
		}

		Session::~Session()
		{
			if (!m_closed)
				Close();
			LOG_INFO_CONSOLE("Session " + m_uuid + " closed and destroyed.");
		}

		void Session::Close()
		{
			m_closed = true;
			m_eventHandler->OnClose(shared_from_this());
			m_socket.close();
		}

		void Session::Start()
		{
			try
			{
				this->m_cached_HeadRecvPacket = std::make_shared<DSCRecvPacket>(DSC_PACKET_HEAD_BYTES_SIZE);
				this->m_cached_HeadRecvPacket->Clear();
				boost::asio::async_read(m_socket,
					boost::asio::buffer(this->m_cached_HeadRecvPacket->m_data,
						DSC_PACKET_HEAD_BYTES_SIZE),
					boost::bind(&Session::HandleHeadRead, shared_from_this(),
						std::placeholders::_1, std::placeholders::_2));

				m_eventHandler->OnConnect(shared_from_this());
				m_closed = false;
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				Close();
			}
		}

		void Session::HandleHeadRead(const boost::system::error_code& ec, size_t bytes_tramsferred)
		{
			try {
				if (!ec) {
					/// 头部长度不正确
					if (bytes_tramsferred < DSC_PACKET_HEAD_BYTES_SIZE) {
						LOG_WARN_CONSOLE(this->m_uuid + " Header length is not correct");
						Close();
						return;
					}

					/// 将头部数据转换为数据长度
					unsigned short data_size = 0;
					memcpy(&data_size, m_cached_HeadRecvPacket->m_data, DSC_PACKET_HEAD_BYTES_SIZE);
					/// 网络字节序转换为主机字节序
					data_size = boost::asio::detail::socket_ops::network_to_host_short(data_size);

					/// 头部长度异常
					if (data_size > DSC_PACKET_BODY_MAX_SIZE)
					{
						LOG_WARN_CONSOLE(this->m_uuid + " Data size is too large");
						Close();
						return;
					}

					/// 读取数据
					m_cached_BodyRecvPacket = std::make_shared<DSCRecvPacket>(data_size);
					boost::asio::async_read(m_socket,
						boost::asio::buffer(m_cached_BodyRecvPacket->m_data,
							m_cached_BodyRecvPacket->m_dataSize),
						std::bind(&Session::HandleBodyRead, shared_from_this(),
							std::placeholders::_1, std::placeholders::_2));
				}
				else 
				{
					LOG_WARN_CONSOLE(this->m_uuid + " " + std::to_string(ec.value()) + " " + ec.message() + " in HandleHeadRead");
					Close();
				}
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				Close();
			}
		}

		void Session::HandleBodyRead(const boost::system::error_code& ec, size_t bytes_tramsferred)
		{

			try {
				if (!ec) {
					/// 测试
					std::cout << "============================" << std::endl;
					std::cout << "Session Layer" << std::endl;
					m_cached_BodyRecvPacket->PrintStringFormat();
					std::cout << "============================" << std::endl;
					/// 发送回去
					this->Send(m_cached_BodyRecvPacket->m_data, m_cached_BodyRecvPacket->m_dataSize);
					/// 测试

					//// Deep copy
					auto recvPacketShawdow = std::make_shared<DSCRecvPacket>(*m_cached_BodyRecvPacket);

					/// 消息接收触发, 使用OnMessage将消息传递给上层
					this->m_eventHandler->OnData(shared_from_this(), std::move(recvPacketShawdow));
					
					/// 更新最后活动时间
					this->m_lastActiveTime = boost::posix_time::microsec_clock::local_time();

					/// 重新开始读取消息头
					this->m_cached_HeadRecvPacket->Clear();
					boost::asio::async_read(m_socket,
						boost::asio::buffer(m_cached_HeadRecvPacket->m_data,
							DSC_PACKET_HEAD_BYTES_SIZE),
						std::bind(&Session::HandleHeadRead, shared_from_this(),
							std::placeholders::_1, std::placeholders::_2));
				}
				else {
					LOG_WARN_CONSOLE(this->m_uuid + " " + std::to_string(ec.value()) + " " + ec.message() + " in HandleBodyRead");
					Close();
				}
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				Close();
			}
		}

		void Session::Send(const char* data, int dataLength)
		{
			try {
				/// 队列是否存在数据
				bool queue_pending = false;
				std::lock_guard<std::mutex> lock(m_sendQueueMutex);
				if (this->m_sendQueue.size() >= this->m_sendQueueMaxSize)
				{
					/// 队列数据过多
					LOG_WARN_CONSOLE(this->m_uuid + " Queue is full, send fail");
					return;
				}
				if (!this->m_sendQueue.empty())
				{
					/// 在本次发送数据前队列存在待发送或者正在发送数据
					queue_pending = true;
				}
				/// 将数据加入队列
				this->m_sendQueue.push(std::make_shared<DSCSendPacket>(data, dataLength));
				if (queue_pending) {
					/// 在本次发送数据前队列已经有数据 直接返回
					return;
				}
				/// 挂起发送事件与发送成功回调
				auto& nxtdata = m_sendQueue.front();
				boost::asio::async_write(this->m_socket, boost::asio::buffer(nxtdata->m_data, nxtdata->m_dataSize),
					std::bind(&Session::HandleWrite, shared_from_this(), std::placeholders::_1));
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				Close();
			}
		}

		void Session::HandleWrite(const boost::system::error_code& ec)
		{
			try {
				if (!ec) {
					/// 数据发送成功
					std::lock_guard<std::mutex> lock(m_sendQueueMutex);
					this->m_sendQueue.pop();
					/// 队列是否存在数据
					if (!this->m_sendQueue.empty())
					{
						/// 取队列中的首部数据
						auto& nxtdata = m_sendQueue.front();
						/// 挂起发送事件与发送成功回调
						boost::asio::async_write(this->m_socket,
							boost::asio::buffer(nxtdata->m_data, nxtdata->m_dataSize),
							std::bind(&Session::HandleWrite, shared_from_this(), std::placeholders::_1));
					}
				}
				else {
					LOG_WARN_CONSOLE(this->m_uuid + " " + std::to_string(ec.value()) + " " + ec.message() + " in HandleWrite");
					Close();
				}
			}
			catch (std::exception& e) {
				LOG_ERROR_CONSOLE_DETAIL(e.what());
				Close();
			}	
		}
	}
}