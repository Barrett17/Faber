
class MediaRecorder {
public:
									MediaRecorder(const char* name,
										media_format format,
										int32 priority = 0);

	virtual							~MediaRecorder();

			status_t				InitCheck() const;

			const media_node&		Node() const;

			status_t				SetHook(void* cookie,
										void (*record_hook)(
										void* cookie,
										void* data,
										size_t size,
										const media_header& header));

			status_t				Connect(const dormant_node_info& info,
										const media_format* format = 0,
										uint32 flags = 0);

			status_t				Connect(const media_node& node,
										const media_output* useOutput = 0,
										const media_format* format = 0,
										uint32 flags = 0);

			bool					IsConnected() const;

			status_t				Disconnect();

			status_t				Start();
			status_t				Stop();

			bool					IsStarted() const;

			const media_output &	Input() const;
			const media_input&		Output() const;

			const media_format&		Format() const;

protected:
	virtual void					BufferReceived(void* data,
										size_t size,
										const media_header& header);
private:

			bool					fStarted;
			
};
