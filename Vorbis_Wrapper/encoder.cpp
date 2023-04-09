#include "vogg/encoder.h"

namespace vogg
{
    Encoder::Encoder(Stream& in, Stream& out, CodecConfig in_conf, CodecConfig out_conf) : in(in),out(out)
    {
        this->in_conf = in_conf;
        this->out_conf = out_conf;
        init();
    }
    
    Encoder::~Encoder()
    {
        ogg_stream_clear(&ostream);
        vorbis_info_clear(&vinfo);
        vorbis_comment_clear(&vcomment);
        vorbis_block_clear(&vblock);
        vorbis_dsp_clear(&vdsp);
        
    }

	void Encoder::init()
	{
        
        /* init vorbis info */
        vorbis_info_init(&vinfo); 

        /* init vorbis comment */
        vorbis_comment_init(&vcomment); 

        /* setup base configuration for codec */
        if ((last_error = vorbis_encode_init(&vinfo, out_conf.channels,
                                                     out_conf.rate,
                                                     out_conf.max_bitrate,
                                                     out_conf.nominal_bitrate,
                                                     out_conf.min_bitrate)) < 0)
        {
            printf("<VORBIS_ENCODE_INIT ERROR> CODE: %i",last_error);
            throw std::exception("<<VORBIS_ENCODE_INIT ERROR>");
        }

        /* init vorbis analysis */
        vorbis_analysis_init(&vdsp, &vinfo);

        /* init ogg stream */
        if (last_error = ogg_stream_init(&ostream, 1) != 0)
        {
            printf("<OGG_STREAM_INIT ERROR> CODE: %i", last_error);
            throw std::exception("<OGG_STREAM_INIT ERROR>");
        };

        /* init vorbis block */
        vorbis_block_init(&vdsp, &vblock);

 
	}

    void Encoder::encode()
    {
        /* send codec config headers */
        send_headers();

        size_t read;
        /* while there is data in IN-STREAM */
        while ((read = in.stream_read(readbuffer,4096,4096))>0)
        {
            /* get buffer and fill it with raw-samples */
            float** buf = vorbis_analysis_buffer(&vdsp, 1024);
          


            /* from STEREO to STEREO */
            if (in_conf.channels == 2 && out_conf.channels == 2)
                stereo_x_stereo(buf,read);

            /* from STEREO to MONO */
            if (in_conf.channels == 2 && out_conf.channels == 1)
                stereo_x_mono(buf, read);

            /* from MONO to STEREO */
            if (in_conf.channels == 1 && out_conf.channels == 2)
                mono_x_stereo(buf, read);

            /* from MONO to MONO */
            if (in_conf.channels == 1 && out_conf.channels == 1)
                mono_x_mono(buf, read);




            /* break raw-data into blocks */
            while (vorbis_analysis_blockout(&vdsp, &vblock) > 0)
            {
                /* compressing them with VORBIS */
                vorbis_analysis(&vblock, 0);
                vorbis_bitrate_addblock(&vblock);

                /* get compressed packet */
                while (vorbis_bitrate_flushpacket(&vdsp, &opacket))
                {
                    /* push compressed packet in ogg stream */
                    ogg_stream_packetin(&ostream, &opacket);

                    /* send compressed pages to OUT-STREAM */
                    push_page(WRITE_CHUNK);
                }
            }
            
        }
        /* send EOF */
        vorbis_analysis_wrote(&vdsp, 0);

    }

    void Encoder::push_page(int32_t page_size)
    {
        /*
      
        PAGE_SIZE parameter is:

            -1 to get a page of any size
             0 to get default page this reasonable size
            >0 to get a page if reaches PAGE_SIZE

        */

        switch (page_size)
        {
        case -1:
            while (ogg_stream_flush(&ostream, &opage))
            {
                send_page();
                if (ogg_page_eos(&opage)) break;
            }
            break;
        case 0:
            while (ogg_stream_pageout(&ostream, &opage))
            {
                send_page();
                if (ogg_page_eos(&opage)) break;
            }
            break;
        default:
            while (ogg_stream_pageout_fill(&ostream, &opage,page_size))
            {
                send_page();
                if (ogg_page_eos(&opage)) break;
            }
            break;
        }

    }

    void Encoder::send_headers()
    {
   
        /* Load codec configuration to 3 packets and add them to OGG STREAM */
        if ((last_error = vorbis_analysis_headerout(&vdsp, &vcomment, &op, &op_comm, &op_code)) < 0)
        {

            printf("<VORBIS_ANALYSIS_HEADEROUT ERROR> CODE: %i", last_error);
            throw std::exception("<VORBIS_ANALYSIS_HEADEROUT ERROR>");
        }

        ogg_stream_packetin(&ostream, &op);
        ogg_stream_packetin(&ostream, &op_comm);
        ogg_stream_packetin(&ostream, &op_code);

        /* send codec headers to OUT */
        push_page(-1);
    }

    void Encoder::send_page()
    {
        
        memcpy(writebuffer, opage.header, opage.header_len);
        memcpy(writebuffer+opage.header_len, opage.body, opage.body_len);

        out.stream_write(writebuffer, opage.header_len + opage.body_len);

        /*out.stream_write(opage.header, opage.header_len);
        out.stream_write(opage.body, opage.body_len);*/
        
    }

    void Encoder::stereo_x_stereo(float**& buf, size_t datasize)
    {
        for (int i = 0; i < datasize / 4; i++)
        {
            buf[0][i] = ((int16_t*)readbuffer)[i * 2] / 32768.f; // L - channel
            buf[1][i] = ((int16_t*)readbuffer)[i * 2 + 1] / 32768.f; // R - channel
        }


        vorbis_analysis_wrote(&vdsp, (int)(datasize / 4));
    };

    void Encoder::stereo_x_mono(float**& buf, size_t datasize)
    {
        
        for (int i = 0; i < datasize / 4; i++)
        {
            //float val = ((((int16_t*)readbuffer)[i * 2] / 32768.f) + (((int16_t*)readbuffer)[i * 2 + 1] / 32768.f)) * 0.5f;
            buf[0][i] = ((int16_t*)readbuffer)[i * 2] / 32768.f;
        
        }
        vorbis_analysis_wrote(&vdsp, (int)(datasize / 4));
    };

    void Encoder::mono_x_stereo(float**& buf, size_t datasize)
    {
        for (int i = 0; i < datasize / 2; i++)
        {
            float val = (((int16_t*)readbuffer)[i] / 32768.f);
            buf[0][i] = val;
            buf[1][i] = val;

        }
        vorbis_analysis_wrote(&vdsp, (int)(datasize / 2));
    };

    void Encoder::mono_x_mono(float**& buf, size_t datasize)
    {
        for (int i = 0; i < datasize / 2; i++)
        {
            buf[0][i] = (((int16_t*)readbuffer)[i] / 32768.f);
        }
        vorbis_analysis_wrote(&vdsp, (int)(datasize / 2));
    };
};