#pragma once
 

class CClientContext
{
public:
	CClientContext();
	~CClientContext();
public:
	void  set_secure(bool s);
	bool  get_secure();

	void  initiate(PBYTE pkey,int key_len);

	bool  need_receive(const PVOID pdata, int len,bool &malloc_error);
	const char* need_send(int &len);
	
	DWORD   recv_data_size();
	DWORD   send_data_size();

	void  reset();
	void  free();

	virtual void  process_data();

	void  add_handle(HANDLE h);
	void  remove_handle(HANDLE h);
private:

	DABuff   m_buff_recv;
	DABuff   m_buff_send;
	DWORD    m_current;
	map<HANDLE, bool> m_handles;
 
	PCR_CIPHER     m_pcipher;
	CR_CTR_IV      m_iv;

	BYTE           m_com_key[16];
	bool           m_secure;
	bool           m_key_sent;
};

using context_ptr = std::shared_ptr<CClientContext>;