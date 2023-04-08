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
	
		// ogg_stream_init(); создаем поток контейнера
	

		// ogg_stream_flush(); сбрасывает пакеты в страницу (использовать в цикле, пока не ноль)
		// ogg_stream_pageout_fill(); берет страницу из потока когда >= значению
	

		// ogg_stream_packetin(); кладем пакет в контейнер
	

		// encode
		
		
		// ogg_page_eos(); если страница последн€€ 
		// ogg_stream_clear(); уничтожает поток

		// decode
		
		// ogg_sync_init создаем буффер синхронизации страниц
		// ogg_page_serialno(); получаем номер страницы


		// ogg_sync_pageout(); формируте страницу из данных в синк буффере
		// ogg_sync_buffer(); возвращает указатель на буффер дл€ данных дл€ декодировани€
		// ogg_sync_wrote(); сообщает синк скольок данных мы записали в синк буффер
		
		
		// ogg_stream_pagein(); кладем страницу в поток
		// ogg_stream_packetout(); забираем пакет из потока

}

#endif
