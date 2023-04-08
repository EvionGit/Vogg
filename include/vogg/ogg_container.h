#ifndef OGG_CONTAINER_HEADER_H
#define OGG_CONTAINER_HEADER_H

#include "base_definition.h"
#include <ogg/ogg.h>
#include <ogg/os_types.h>

namespace vogg
{
	struct Ogg
	{

		ogg_sync_state osync;
		ogg_stream_state ostream;
		ogg_page opage;
		ogg_packet opacket, op, op_comm, op_code;
	};
	
		// ogg_stream_init(); ������� ����� ����������
	

		// ogg_stream_flush(); ���������� ������ � �������� (������������ � �����, ���� �� ����)
		// ogg_stream_pageout_fill(); ����� �������� �� ������ ����� >= ��������
	

		// ogg_stream_packetin(); ������ ����� � ���������
	

		// encode
		
		
		// ogg_page_eos(); ���� �������� ��������� 
		// ogg_stream_clear(); ���������� �����

		// decode
		
		// ogg_sync_init ������� ������ ������������� �������
		// ogg_page_serialno(); �������� ����� ��������


		// ogg_sync_pageout(); ��������� �������� �� ������ � ���� �������
		// ogg_sync_buffer(); ���������� ��������� �� ������ ��� ������ ��� �������������
		// ogg_sync_wrote(); �������� ���� ������� ������ �� �������� � ���� ������
		
		
		// ogg_stream_pagein(); ������ �������� � �����
		// ogg_stream_packetout(); �������� ����� �� ������

}

#endif
