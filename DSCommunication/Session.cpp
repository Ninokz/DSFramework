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
					/// ͷ�����Ȳ���ȷ
					if (bytes_tramsferred < DSC_PACKET_HEAD_BYTES_SIZE) {
						LOG_WARN_CONSOLE(this->m_uuid + " Header length is not correct");
						Close();
						return;
					}

					/// ��ͷ������ת��Ϊ���ݳ���
					unsigned short data_size = 0;
					memcpy(&data_size, m_cached_HeadRecvPacket->m_data, DSC_PACKET_HEAD_BYTES_SIZE);
					/// �����ֽ���ת��Ϊ�����ֽ���
					data_size = boost::asio::detail::socket_ops::network_to_host_short(data_size);

					/// ͷ�������쳣
					if (data_size > DSC_PACKET_BODY_MAX_SIZE)
					{
						LOG_WARN_CONSOLE(this->m_uuid + " Data size is too large");
						Close();
						return;
					}

					/// ��ȡ����
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
					/// ����
					std::cout << "============================" << std::endl;
					std::cout << "Session Layer" << std::endl;
					m_cached_BodyRecvPacket->PrintStringFormat();
					std::cout << "============================" << std::endl;
					/// ���ͻ�ȥ
					this->Send(m_cached_BodyRecvPacket->m_data, m_cached_BodyRecvPacket->m_dataSize);
					/// ����

					//// Deep copy
					auto recvPacketShawdow = std::make_shared<DSCRecvPacket>(*m_cached_BodyRecvPacket);

					/// ��Ϣ���մ���, ʹ��OnMessage����Ϣ���ݸ��ϲ�
					this->m_eventHandler->OnData(shared_from_this(), std::move(recvPacketShawdow));
					
					/// �������ʱ��
					this->m_lastActiveTime = boost::posix_time::microsec_clock::local_time();

					/// ���¿�ʼ��ȡ��Ϣͷ
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
				/// �����Ƿ��������
				bool queue_pending = false;
				std::lock_guard<std::mutex> lock(m_sendQueueMutex);
				if (this->m_sendQueue.size() >= this->m_sendQueueMaxSize)
				{
					/// �������ݹ���
					LOG_WARN_CONSOLE(this->m_uuid + " Queue is full, send fail");
					return;
				}
				if (!this->m_sendQueue.empty())
				{
					/// �ڱ��η�������ǰ���д��ڴ����ͻ������ڷ�������
					queue_pending = true;
				}
				/// �����ݼ������
				this->m_sendQueue.push(std::make_shared<DSCSendPacket>(data, dataLength));
				if (queue_pending) {
					/// �ڱ��η�������ǰ�����Ѿ������� ֱ�ӷ���
					return;
				}
				/// �������¼��뷢�ͳɹ��ص�
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
					/// ���ݷ��ͳɹ�
					std::lock_guard<std::mutex> lock(m_sendQueueMutex);
					this->m_sendQueue.pop();
					/// �����Ƿ��������
					if (!this->m_sendQueue.empty())
					{
						/// ȡ�����е��ײ�����
						auto& nxtdata = m_sendQueue.front();
						/// �������¼��뷢�ͳɹ��ص�
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