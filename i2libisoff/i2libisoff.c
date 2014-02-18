#include "i2libisoff.h"

//ftyp for audio and video files is the same
uint32_t write_ftyp(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_moov(byte *data, uint32_t media_type, i2ctx *context);

//mvhd for audio and video files is the same
uint32_t write_mvhd(byte *data, uint32_t media_type, i2ctx *context);

//mvex for audio and video files is the same
uint32_t write_mvex(byte *data, uint32_t media_type, i2ctx *context);

//trex for audio and video files is the same
uint32_t write_trex(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_trak(byte *data, uint32_t media_type, i2ctx *context);

//tkhd for audio and video files is the same
uint32_t write_tkhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_mdia(byte *data, uint32_t media_type, i2ctx *context);

//mdhd for audio and video files is the same
uint32_t write_mdhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_hdlr(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_minf(byte *data, uint32_t media_type, i2ctx *context);

//only video
uint32_t write_vmhd(byte *data);

//only audio
uint32_t write_smhd(byte *data);

//dinf for audio and video files is the same
uint32_t write_dinf(byte *data, uint32_t media_type, i2ctx *context);

//dref for audio and video files is the same
uint32_t write_dref(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_url(byte * data);

uint32_t write_stbl(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_stsd(byte *data, uint32_t media_type, i2ctx *context);

//only video
uint32_t write_avc1(byte *data, i2ctx_video *ctxVideo);

//only video
uint32_t write_avc3(byte *data, i2ctx_video *ctxVideo);

//only video
uint32_t write_avcc(byte *data, i2ctx_video *ctxVideo);

//only audio
uint32_t write_mp4a(byte *data, i2ctx_audio *ctxAudio);

//only audio
uint32_t write_esds(byte *data, i2ctx_audio *ctxAudio);

//stts for audio and video files is the same
uint32_t write_stts(byte *data, uint32_t media_type, i2ctx *context);

//stsc for audio and video files is the same
uint32_t write_stsc(byte *data, uint32_t media_type, i2ctx *context);

//stsz for audio and video files is the same
uint32_t write_stsz(byte *data, uint32_t media_type, i2ctx *context);

//stco for audio and video files is the same
uint32_t write_stco(byte *data, uint32_t media_type, i2ctx *context);

//styp for audio and video files is the same
uint32_t write_styp(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_sidx(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_moof(byte *data, uint32_t media_type, i2ctx **context);

//mfhd for audio and video files is the same
uint32_t write_mfhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_traf(byte *data, uint32_t media_type, i2ctx *context);

//tfhd for audio and video files is the same
uint32_t write_tfhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_tfdt(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_trun(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_mdat(byte* source_data, uint32_t size_source_data, byte *data, uint32_t media_type, i2ctx *context);

// source_data equals to pps_sps_data
uint32_t initVideoGenerator(byte *source_data, uint32_t size_source_data, byte *destination_data, i2ctx **context) {
	uint32_t count, size_ftyp, size_moov;
	if(size_source_data <= 0) {
		return I2ERROR;
	}
	count = 0;
	(*context)->ctxvideo->pps_sps_data = (byte*) malloc (size_source_data);
	memcpy((*context)->ctxvideo->pps_sps_data, source_data, size_source_data);
	(*context)->ctxvideo->pps_sps_data_length = size_source_data;
	size_ftyp = write_ftyp(destination_data + count, VIDEO_TYPE, (*context));

	if (size_ftyp < 8)
		return I2ERROR;

	count+= size_ftyp;
	size_moov = write_moov(destination_data + count, VIDEO_TYPE, (*context));

	if (size_moov < 8)
		return I2ERROR;

	count+= size_moov;

	return count;
}
// source_data equals to aac_data
uint32_t initAudioGenerator(byte *source_data, uint32_t size_source_data, byte *destination_data, i2ctx **context) {
	uint32_t count, size_ftyp, size_moov;
	if(size_source_data <= 0) {
		return I2ERROR;
	}
	count = 0;
	(*context)->ctxaudio->aac_data = (byte*) malloc(size_source_data);
	memcpy((*context)->ctxaudio->aac_data, source_data, size_source_data);
	(*context)->ctxaudio->aac_data_length = size_source_data;
	size_ftyp = write_ftyp(destination_data + count, AUDIO_TYPE, (*context));
	
	if (size_ftyp < 8)
		return I2ERROR;

	count+= size_ftyp;
	size_moov = write_moov(destination_data + count, AUDIO_TYPE, (*context));

	if (size_moov < 8)
		return I2ERROR;

	count+= size_moov;

	return count;
}

uint32_t segmentGenerator(byte *source_data, uint32_t size_source_data, byte *destination_data, uint32_t media_type, i2ctx **context) {
	uint32_t count, size_styp, size_sidx, size_moof, size_mdat, mdat_length;
	if(size_source_data <= 0) {
		return I2ERROR;
	}
	//TODO AUDIOVIDEO_TYPE
	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;
	
	count = 0;
	size_styp = write_styp(destination_data + count, media_type, (*context));
	
	if (size_styp < 8)
		return I2ERROR;
	
	count+= size_styp;
	
	size_sidx = 44;

	size_moof = write_moof(destination_data + count + size_sidx, media_type, context);

	if (size_moof < 8)
		return I2ERROR;
	/*
	*referenced_size: the distance in bytes from the first byte
	* of the referenced item to the first byte of the next
	* referenced item, or in the case of the last entry,
	* the end of the referenced material;
	*/

	(*context)->reference_size = size_moof + 8 + size_source_data;
	size_sidx = write_sidx(destination_data + count, media_type, (*context));
	
	if (size_sidx < 8)
		return I2ERROR;

	printf("SIZE_SIDX: %u\n", size_sidx);

	count+= size_moof + size_sidx;
	size_mdat = write_mdat(source_data , size_source_data, destination_data + count, media_type, (*context));

	if (size_mdat < 8)
		return I2ERROR;

	count+= size_mdat;

	return count;
}

uint32_t write_matrix(byte *data, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t tx, uint32_t ty);

uint32_t write_ftyp(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, version, hton_version;
	
	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	version = 1;
	
	//Box type
	memcpy(data + count, "ftyp", 4);
	count+= 4;

	//Major brand
    memcpy(data + count, "iso6", 4);
	count+= 4;

    //Version
	hton_version = htonl(version);
    memcpy(data + count, &hton_version, 4);
	count+= 4;

    //Compatible brands
    memcpy(data + count, "isom", 4);
	count+= 4;
    memcpy(data + count, "iso6", 4);
	count+= 4;
    memcpy(data + count, "dash", 4);
	count+= 4;
	
	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_moov(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, size_mvhd, size_mvex, size_trak, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	
	//Box type
	memcpy(data + count, "moov", 4);
	count+= 4;

    size_mvhd = write_mvhd(data + count, media_type, context);
	if (size_mvhd < 8)
		return I2ERROR;

	count+= size_mvhd;
	size_mvex = write_mvex(data + count, media_type, context);
	if (size_mvex < 8)
		return I2ERROR;

	count+= size_mvex;
	size_trak = write_trak(data + count, media_type, context);
	if (size_trak < 8)
		return I2ERROR;

	count+= size_trak;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_mvhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_matrix, flag32, hton_flag32;
	uint16_t flag16, hton_flag16;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;	
	//Box type
	memcpy(data + count, "mvhd", 4);
	count+= 4;

	//Version
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;

    //Creation time
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;

    //Modification time
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;

    //Timescale
	flag32 = 1000;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, &hton_flag32, 4);
	count+= 4;

    //Duration
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;

    //Reserved
	flag32 = 0x00010000;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, &hton_flag32, 4);
	count+= 4;
	flag16 = 0x0100;
	hton_flag16 = htons(flag16);
    memcpy(data + count, &hton_flag16, 2);
	count+= 2;
	flag16 = 0;
    memcpy(data + count, &flag16, 2);
	count+= 2;
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;
	
	//transformation matrix
    size_matrix = write_matrix(data + count, 1, 0, 0, 1, 0, 0);
	count+= size_matrix;

    //Reserved
	flag32 = 0;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;
    memcpy(data + count, &flag32, 4);
	count+= 4;

    //Next track id
	flag32 = 1;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, &hton_flag32, 4);
	count+= 4;

	//Box size
	size = count;

	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;

}

uint32_t write_mvex(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_trex;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "mvex", 4);
	count+= 4;

	size_trex = write_trex(data + count, media_type, context);
	if (size_trex < 8)
		return I2ERROR;
	count+= size_trex;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_trex(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, flag32, hton_flag32;
	
	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	//Size is always 32, apparently	
	//Box size
	size = 32; //0x20
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	count+= 4;

	//Box type
	memcpy(data + count, "trex", 4);
	count+= 4;

    //Version & flags
	flag32 = 0x0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Track id
	flag32 = 1;
	hton_flag32 = htonl(flag32);
	memcpy(data + count, &hton_flag32, 4);
	count+= 4;

    //Default sample description index
	flag32 = 1;
	hton_flag32 = htonl(flag32);
	memcpy(data + count, &hton_flag32, 4);
	count+= 4;

    //Default sample duration
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Default sample size, 1024 for AAC
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Default sample flags, key on
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	return count;
}

uint32_t write_trak(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_tkhd, size_mdia;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "trak", 4);
	count+= 4;

	size_tkhd = write_tkhd(data + count, media_type, context);
	if (size_tkhd < 8)
		return I2ERROR;
	count+= size_tkhd;
	size_mdia = write_mdia(data + count, media_type, context);
	if (size_mdia < 8)
		return I2ERROR;
	count+= size_mdia;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);

	return count;
}

uint32_t write_tkhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_matrix, flag32, hton_flag32;
	uint16_t flag16, hton_flag16;
	uint8_t flag8;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;	
	//Box type
	memcpy(data + count, "tkhd", 4);
	count+= 4;

    /* version */
	flag8 = 0;
	memcpy(data + count, &flag8, 1);
	count+= 1;

    //Flags: TrackEnabled
	flag8 = 0;
	memcpy(data + count, &flag8, 1);
	count+= 1;
	flag16 = 0x000f;
	hton_flag16 = htons(flag16);
	memcpy(data + count, &hton_flag16, 2);
	count+= 2;

    //Creation time
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Modification time
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Track id
	flag32 = 1;
	hton_flag32 = htonl(flag32);
	memcpy(data + count, &hton_flag32, 4);
	count+= 4;

    //Reserved
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Duration
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Reserved
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;
	memcpy(data + count, &flag32, 4);
	count+= 4;
	memcpy(data + count, &flag32, 4);
	count+= 4;

    //Reserved
	if (media_type == VIDEO_TYPE)
		flag16 = 0;
	else
		flag16 = 0x0100;
	hton_flag16 = htons(flag16);
	memcpy(data + count, &hton_flag16, 2);
	count+= 2;

    //Reserved
	flag16 = 0;
	memcpy(data + count, &flag16, 2);
	count+= 2;

    size_matrix = write_matrix(data + count, 1, 0, 0, 1, 0, 0);
	count+= size_matrix;

    if (media_type == VIDEO_TYPE) {
		flag32 = 0;
		flag32 = context->ctxvideo->width << 16;
		hton_flag32 = htonl(flag32);
		memcpy(data + count, &hton_flag32, 4);
		count+= 4;
		flag32 = 0;
		flag32 = context->ctxvideo->height << 16;
		hton_flag32 = htonl(flag32);
		memcpy(data + count, &hton_flag32, 4);
		count+= 4;	
    } else {
		flag32 = 0;
		memcpy(data + count, &flag32, 4);
		count+= 4;	
		memcpy(data + count, &flag32, 4);
		count+= 4;	
    }

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_mdia(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_mdhd, size_hdlr, size_minf;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "mdia", 4);
	count+= 4;

	size_mdhd = write_mdhd(data + count, media_type, context);
	if (size_mdhd < 8)
		return I2ERROR;
	count+= size_mdhd;
	size_hdlr = write_hdlr(data + count, media_type, context);
	if (size_hdlr < 8)
		return I2ERROR;
	count+= size_hdlr;
	size_minf = write_minf(data + count, media_type, context);
	if (size_minf < 8)
		return I2ERROR;
	count+= size_minf;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;

}

uint32_t write_mdhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, flag32, hton_flag32;
	uint16_t flag16, hton_flag16;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "mdhd", 4);
	count+= 4;

	//Version
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	//Creation time
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	//Modification time
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	//Time scale
	flag32 = 1000;
	hton_flag32 = htonl(flag32);
	memcpy(data + count, &hton_flag32, 4);
	count+= 4;

	//Duration
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	//Lanuguage
	flag16 = 0x15C7;
	hton_flag16 = htons(flag16);
	memcpy(data + count, &hton_flag16, 2);
	count+= 2;

	//Reserved
	flag16 = 0;
	memcpy(data + count, &flag16, 2);
	count+= 2;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_hdlr(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, flag32;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "hdlr", 4);
	count+= 4;
	//Version and flags
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	//Pre defined
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	if (media_type == VIDEO_TYPE) {
		memcpy(data + count, "vide", 4);
		count+= 4;
	} else {
		memcpy(data + count, "soun", 4);
		count+= 4;
	}

	//Reserved
	flag32 = 0;
	memcpy(data + count, &flag32, 4);
	count+= 4;
	memcpy(data + count, &flag32, 4);
	count+= 4;
	memcpy(data + count, &flag32, 4);
	count+= 4;

	if (media_type == VIDEO_TYPE) {
		//Video handler string, NULL-terminated
		memcpy(data + count, "VideoHandler", sizeof("VideoHandler"));
		count+= sizeof("VideoHandler");
    } else {
		//Sound handler string, NULL-terminated
		memcpy(data + count, "SoundHandler", sizeof("SoundHandler"));
		count+= sizeof("SoundHandler");
	}

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_minf(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_vmhd, size_smhd, size_dinf, size_stbl;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	//Box type
	memcpy(data + count, "minf", 4);
	count+= 4;
	if (media_type == VIDEO_TYPE) {
		size_vmhd = write_vmhd(data + count);
		if (size_vmhd < 8)
			return I2ERROR;
		count+= size_vmhd;
	} else {
		size_smhd = write_smhd(data + count);
		if (size_smhd < 8)
			return I2ERROR;
		count+= size_smhd;
	}
	
	size_dinf = write_dinf(data + count, media_type, context);
	if (size_dinf < 8)
		return I2ERROR;
	count+= size_dinf;
	size_stbl = write_stbl(data + count, media_type, context);
	if (size_stbl < 8)
		return I2ERROR;
	count+= size_stbl;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_vmhd(byte *data) {
	uint32_t count, size, hton_size, flag32, hton_flag32, zero;

	//Size is always 20, apparently
	size = 20;
	count = 0;
	flag32 = 0x01;
	zero = 0;
	//Box size
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;
	//Box type
	memcpy(data + count, "vmhd", 4);
	count+= 4;

    //Version and flags
	hton_flag32 = htonl(flag32);
    memcpy(data + count, &hton_flag32, 4);

	count+= 4;

    //reserved (graphics mode=copy)
    memcpy(data + count, &zero, 4);
	count+= 4;
	memcpy(data + count, &zero, 4);
	count+= 4;
	
	return count;
}

uint32_t write_smhd(byte *data) {
	uint32_t count, size, hton_size;
	uint64_t zero;

	// smhd size is always 16, apparently
	size = 16;
	count = 0;
	zero = 0;
	// Box size
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;
	// Box type
	memcpy(data + count, "smhd", 4);
	count+= 4;
	// Version and flags and reserved (balance normally = 0)
	memcpy(data + count, &zero, 8);
	count+= 8;

	return count;
}

uint32_t write_dinf(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_dref;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;
	count = 4;
	memcpy(data + count, "dinf", 4);
	count+= 4;
	size_dref = write_dref(data + count, media_type, context);
	if (size_dref < 8)
		return I2ERROR;
	count+= size_dref;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

//dref for audio and video files is the same
uint32_t write_dref(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, zero, one, size_url, hton_one;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	zero = 0;
	one = 1;

	// box type
	memcpy(data + count, "dref", 4);
	count+= 4;
	// version and flags
	memcpy(data + count, &zero, 4);
	count+= 4;
	// entries
	hton_one = htonl(one);
	memcpy(data + count, &hton_one, 4);
	count+= 4;
	size_url = write_url(data + count);
	if(size_url < 8)
		return I2ERROR;
	count+= size_url;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_url(byte * data) {
	uint32_t count, size, flags, hton_size, hton_flags;

	count = 0;
	size = 12;
	flags = 0x00000001;
	// size
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;
	// box type
	memcpy(data + count, "url ", 4);
	count+= 4;
	// version and flags
	hton_flags = htonl(flags);
	memcpy(data + count, &hton_flags, 4);
	count+= 4;

	return count;
}

uint32_t write_stbl(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_stsd, size_stts, size_stsc, size_stsz, size_stco;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	
	// box type
	memcpy(data + count, "stbl", 4);
	count+= 4;

	// write subBoxes and update count value
	size_stsd = write_stsd(data + count, media_type, context);
	if (size_stsd < 8)
		return I2ERROR;
	count+= size_stsd;
	
	size_stts = write_stts(data + count, media_type, context);	
	if (size_stts < 8)
		return I2ERROR;
	count+= size_stts;

	size_stsc = write_stsc(data + count, media_type, context);
	if (size_stsc < 8)
		return I2ERROR;
	count+= size_stsc;

	size_stsz = write_stsz(data + count, media_type, context);
	if (size_stsz < 8)
		return I2ERROR;
	count+= size_stsz;
	
	size_stco = write_stco(data + count, media_type, context);
	if (size_stco < 8)
		return I2ERROR;
	count+= size_stco;
	
	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_stsd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, one, size, hton_size, size_avc1, size_mp4a, hton_one;
	i2ctx_audio *ctxaudio;
	i2ctx_video *ctxvideo;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	zero = 0;
	one = 1;
	ctxvideo = context->ctxvideo;
	ctxaudio = context->ctxaudio;

	// box type
	memcpy(data + count, "stsd", 4);
	count+= 4;
	// version and flags
	memcpy(data + count, &zero, 4);
	count+= 4;
	// entries
	hton_one = htonl(one);
	memcpy(data + count, &hton_one, 4);
	count+= 4;
	/*
	* media_type = 0 none
	* media_type = 1 video
	* media_type = 2 audio
	* media_type = 3 video_audio
	*/
	// write avcX or AAC boxes
	if(media_type == VIDEO_TYPE) {
		// write avc1
		size_avc1 = write_avc1(data + count, ctxvideo);
		if (size_avc1 < 8)
			return I2ERROR;
		count+= size_avc1;
	} else if(media_type == AUDIO_TYPE) {
		// write mp4a
		size_mp4a = write_mp4a(data + count, ctxaudio);
		if (size_mp4a < 8)
			return I2ERROR;
		count+= size_mp4a;
	} else {
		return I2ERROR;
	}

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}


uint32_t write_avc1(byte *data, i2ctx_video *ctxVideo) {
	uint32_t count, size, hton_size, zero_32, hv_resolution, hton_hv_resolution, size_avcc;
	uint64_t zero_64;
	uint16_t zero_16, width, height, flag_one, flag16, hton_flag16, hton_flag_one, hton_width, hton_height;

	count = 4;
	zero_16 = 0;
	zero_32 = 0;
	zero_64 = 0;
	flag_one = 1;
	flag16 = 0;
	hv_resolution = 0x00480000;
	width = ctxVideo->width;
	height = ctxVideo->height;

	// box type
	memcpy(data + count, "avc1", 4);
	count+= 4;
	// reserved
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	memcpy(data + count, &zero_16, 2);
	count+= 2;
	// data reference index
	hton_flag_one = htons(flag_one);
	memcpy(data + count, &hton_flag_one, 2);
	count+= 2;
	// codec stream version + revision + reserved
	memcpy(data + count, &zero_64, 8);
	count+= 8;
	memcpy(data + count, &zero_64, 8);
	count+= 8;
	// width
	hton_width = htons(width);
	memcpy(data + count, &hton_width, 2);
	count+= 2;
	// height
	hton_height = htons(height);
	memcpy(data + count, &hton_height, 2);
	count+= 2;
	// horitzonal and vertical resolution 72dpi
	hton_hv_resolution = htonl(hv_resolution);
	memcpy(data + count, &hton_hv_resolution, 4);
	count+= 4;
	memcpy(data + count, &hton_hv_resolution, 4);
	count+= 4;
	// data size
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	// frame count
	memcpy(data + count, &hton_flag_one, 2);
	count+= 2;
	// compressor name
	memcpy(data + count, &zero_64, 8);
	count+= 8;
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	// reserved
	memcpy(data + count, &zero_64, 8);
	count+= 8;
	memcpy(data + count, &zero_64, 8);
	count+= 8;
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	flag16 = 0x18;
	hton_flag16 = htons(flag16);
	memcpy(data + count, &hton_flag16, 2);
	count+= 2;
	flag16 = 0xffff;
	hton_flag16 = htons(flag16);
	memcpy(data + count, &hton_flag16, 2);
	count+= 2;
	// write avcC
	size_avcc = write_avcc(data + count, ctxVideo);
	if (size_avcc < 8)
		return I2ERROR;
	count+= size_avcc;
	
	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_avc3(byte *data, i2ctx_video *ctxVideo) {
	return 1;
	// TODO
}

uint32_t write_avcc(byte *data, i2ctx_video *ctxVideo) {
	uint32_t count, size, hton_size;
	
	count = 0;

	size = ctxVideo->pps_sps_data_length + 8;
	hton_size = htonl(size);
	// box size
	memcpy(data + count, &hton_size, 4);
	count+= 4;
	// box type
	memcpy(data + count, "avcC", 4);
	count+= 4;
	// avc header, includes version, profile, level, sps and pps
	memcpy(data + count, ctxVideo->pps_sps_data, ctxVideo->pps_sps_data_length);
	
	count+= ctxVideo->pps_sps_data_length;

	return count;
}

uint32_t write_mp4a(byte *data, i2ctx_audio *ctxAudio) {
	uint32_t count, zero_32, size_esds, size, hton_size;
	uint16_t zero_16, audio_channels, hton_audio_channels, sample_size, one_16, hton_one_16;
	uint16_t hton_sample_size, sample_rate, hton_sample_rate, hton_timescale;

	count = 4;
	zero_16 = 0;
	zero_32 = 0;
	one_16 = 1;
	audio_channels = ctxAudio->channels;
	sample_size = ctxAudio->sample_size;
	sample_rate = ctxAudio->sample_rate;

	// box type
	memcpy(data + count, "mp4a", 4);
	count+= 4;
	// reserved
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	memcpy(data + count, &zero_16, 2);
	count+= 2;
	// data referennce index
	hton_one_16 = htons(one_16);
	memcpy(data + count, &hton_one_16, 2);
	count+= 2;
	// reserved
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	memcpy(data + count, &zero_32, 4);
	count+= 4;	
	// channel count
	hton_audio_channels = htons(audio_channels); 
	memcpy(data + count, &hton_audio_channels, 2);
	count+= 2;
	// sample size
	hton_sample_size = htons(sample_size);
	memcpy(data + count, &hton_sample_size, 2);
	count+= 2;
	// reserved
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	// timescale
	hton_timescale = htons(1000);
	memcpy(data + count, &hton_timescale, 2);
	count+= 2;
	// sample rate
	hton_sample_rate = htons(sample_rate);
	memcpy(data + count, &hton_sample_rate, 2);
	count+= 2;
	// write esds
	size_esds = write_esds(data + count, ctxAudio);
	if (size_esds < 8)
		return I2ERROR;
	count+= size_esds;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);

	return count;
}

uint32_t write_esds(byte *data, i2ctx_audio *ctxAudio) {
	uint32_t count, zero_32, aac_data_length, max_bitrate, hton_max_bitrate, avg_bitrate, hton_avg_bitrate, size, hton_size;
	uint16_t one_16, zero_16, hton_one_16;
	uint8_t zero_8, tag_esd, tag_dcd, flag_oti, flag_st, tag_dsid, tag_sld, size_sld, flag_sld;
	byte *dsi, *aac_data, flag8;
	size_t dsi_len, size_esd, size_dcd;

	count = 4;
	zero_32 = 0;
	zero_16 = 0;
	zero_8 = 0;
	one_16 = 1;
	aac_data = ctxAudio->aac_data;
	aac_data_length = ctxAudio->aac_data_length;
	dsi = aac_data + 2; //TODO check
	dsi_len = aac_data_length -2; // TODO check
	tag_esd = 0x03;
	size_esd = 23 + dsi_len;
	tag_dcd = 0x04;
	size_dcd = 15 + dsi_len;
	flag_oti = 0x40;
	flag_st = 0x15;
	max_bitrate = 0x0001F151;
	avg_bitrate = 0x0001F14D;
	tag_dsid = 0x05;
	tag_sld = 0x06;
	size_sld = 1;
	flag_sld = 0x02;

	// box type
	memcpy(data + count, "esds", 4);
	count+= 4;
	
	// box version
	memcpy(data + count, &zero_32, 4);
	count+= 4;

	// ES descriptor
	memcpy(data + count, &tag_esd, 1);
	count+= 1;
	flag8 = size_esd & 0x7F;
	memcpy(data + count, &flag8, 1);
	count+= 1;

	// Es id
	hton_one_16 = htons(one_16);
	memcpy(data + count, &hton_one_16, 2);
	count+= 2;

	// flags
	memcpy(data + count, &zero_8, 1);
	count+= 1;

	// decoder config descriptor
	memcpy(data + count, &tag_dcd, 1);
	count+= 1;
	flag8 = size_dcd & 0x7F;
	memcpy(data + count, &flag8, 1);
	count+= 1;

	// objectTypeIndication: Audio ISO/IEC 14496-3 (AAC)
	memcpy(data + count, &flag_oti, 1);
	count+= 1;

	// streamType: AudioStream
	memcpy(data + count, &flag_st, 1);
	count+= 1;

	// bufferSizeDB
	memcpy(data + count, &zero_16, 2);
	count+= 2;
	memcpy(data + count, &zero_8, 1);
	count+= 1;

	// maxBitrate
	hton_max_bitrate = htonl(max_bitrate);
	memcpy(data + count, &hton_max_bitrate, 4);
	count+= 4;

	// avgBitrate
	hton_avg_bitrate = htonl(avg_bitrate);
	memcpy(data + count, &hton_avg_bitrate, 4);
	count+= 4;

	// DecoderSpecificInfo Descriptor
	memcpy(data + count, &tag_dsid, 1);
	count+= 1;
	flag8 = dsi_len & 0x7F;
	memcpy(data + count, &flag8, 1);
	count+= 1;
	memcpy(data + count, dsi, dsi_len);
	count+= dsi_len;

	// SL Descriptor
	memcpy(data + count, &tag_sld, 1);
	count+= 1;
	flag8 = size_sld & 0x7F;
	memcpy(data + count, &flag8, 1);
	count+= 1;
	memcpy(data + count, &flag_sld, 1);
	count+= 1;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);

	return count;
}

uint32_t write_stts(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	zero = 0;
	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "stts", 4);
	count+= 4;

	// version
	memcpy(data + count, &zero, 4);
	count+= 4;

	// entries
	memcpy(data + count, &zero, 4);
	count+= 4;

	return count;
}

uint32_t write_stsc(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	zero = 0;
	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "stsc", 4);
	count+= 4;

	// version
	memcpy(data + count, &zero, 4);
	count+= 4;

	// entries
	memcpy(data + count, &zero, 4);
	count+= 4;

	return count;
}

uint32_t write_stsz(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	zero = 0;
	//Size is always 20, apparently
	size = 20;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// Box type
	memcpy(data + count, "stsz", 4);
	count+= 4;

	// Version
	memcpy(data + count, &zero, 4);
	count+= 4;

	// Flags
	memcpy(data + count, &zero, 4);
	count+= 4;

	// Entries
	memcpy(data + count, &zero, 4);
	count+= 4;

	return count;
}

uint32_t write_stco(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	zero = 0;
	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "stco", 4);
	count+= 4;

	// version
	memcpy(data + count, &zero, 4);
	count+= 4;

	// entries
	memcpy(data + count, &zero, 4);
	count+= 4;

	return count;
}

uint32_t write_styp(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, version, hton_version, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	version = 1;
	//Size is always 28, apparently
	size = 28;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "styp", 4);
	count+= 4;

	// major brand
	memcpy(data + count, "iso6", 4);
	count+= 4;

    //Version
	hton_version = htonl(version);
    memcpy(data + count, &hton_version, 4);
	count+= 4;

	// compatible brands
	memcpy(data + count, "isom", 4);
	count+= 4;
	memcpy(data + count, "iso6", 4);
	count+= 4;
	memcpy(data + count, "dash", 4);
	count+= 4;

	return count;
}

uint32_t write_sidx(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero_32, duration_ms, hton_duration_ms, one_32, hton_one_32;
	uint32_t reference_size, hton_reference_size, size, hton_size, hton_timescale;
	uint32_t earliest_presentation_time, latest_presentation_time, hton_earliest_presentation_time;
	uint8_t zero_8;
	uint16_t zero_16, one_16, hton_one_16;
	byte flag8;
	i2ctx_video *ctxVideo = context->ctxvideo;
	i2ctx_audio *ctxAudio = context->ctxaudio;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	if (media_type == VIDEO_TYPE) {
		earliest_presentation_time = ctxVideo->earliest_presentation_time;
		latest_presentation_time = ctxVideo->latest_presentation_time;
	}
	else if (media_type == AUDIO_TYPE) {
		earliest_presentation_time = ctxAudio->earliest_presentation_time;
		latest_presentation_time = ctxAudio->latest_presentation_time;
	}

	count = 4;
	zero_8 = 0;
	zero_16 = 0;
	zero_32 = 0;
	one_16 = 1;
	one_32 = 1;
	duration_ms = latest_presentation_time - earliest_presentation_time;
	reference_size = context->reference_size;

	// box type
	memcpy(data + count, "sidx", 4);
	count+= 4;
	// version
	memcpy(data + count, &zero_32, 4);
	count+= 4;
	// reference id
	hton_one_32 = htonl(one_32);
	memcpy(data + count, &hton_one_32, 4);
	count+= 4;
	// timescale
	hton_timescale = htonl(1000);
	memcpy(data + count, &hton_timescale, 4);
	count+= 4;
	// earliest_presentation_time
	hton_earliest_presentation_time = htonl(earliest_presentation_time);
	memcpy(data + count, &hton_earliest_presentation_time, 4);
	count+= 4;
	// first offset
	hton_duration_ms = htonl(duration_ms); //TODO
	memcpy(data + count, &hton_duration_ms, 4);
	count+= 4;
	// reserved
	memcpy(data + count, &zero_16, 2);
	count+= 2;	
	// reference count = 1
	hton_one_16 = htons(one_16);
	memcpy(data + count, &hton_one_16, 2);
	count+= 2;
	// reference size
	hton_reference_size = htonl(reference_size);
	memcpy(data + count, &hton_reference_size, 4);
	count+= 4;
	// subsegment_duration
	hton_duration_ms =  htonl(duration_ms);
	memcpy(data + count, &hton_duration_ms, 4);
	count+= 4;
	// 1st bit is reference type, the rest is reference size
	flag8 = 0x90;
	memcpy(data + count, &flag8, 1);
	count+= 1;
	// SAP delta time
	memcpy(data + count, &zero_16, 2);
	count+= 2;
	memcpy(data + count, &zero_8, 1);
	count+= 1;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_moof(byte *data, uint32_t media_type, i2ctx **context) {
	uint32_t count, size, hton_size, size_traf, size_mfhd;
	i2ctx_sample *samples; 

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	
	if(media_type == VIDEO_TYPE)
		samples = (*context)->ctxvideo->ctxsample;
	else if (media_type == AUDIO_TYPE)
		samples = (*context)->ctxaudio->ctxsample;

	// box type
	memcpy(data + count, "moof", 4);
	count+= 4;
	samples->moof_pos = count;

	// write mfhd
	size_mfhd = write_mfhd(data + count, media_type, (*context));
	if (size_mfhd < 8)
		return I2ERROR;

	count+=size_mfhd;
	// write traf
	size_traf = write_traf(data + count, media_type, (*context));
	if (size_traf < 8)
		return I2ERROR;

	count+= size_traf;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_mfhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, seqnum, hton_seqnum, size, hton_size;
	i2ctx_video *ctxVideo = context->ctxvideo;
	i2ctx_audio *ctxAudio = context->ctxaudio;
	
	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;
	
	if (media_type == VIDEO_TYPE) {
		seqnum = ctxVideo->sequence_number;
	}
	else if (media_type == AUDIO_TYPE) {
		seqnum = ctxAudio->sequence_number;
	}
	count = 0;
	zero = 0;

	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;
	// box type
	memcpy(data + count, "mfhd", 4);
	count+= 4;
	// version and flags
	memcpy(data + count, &zero, 4);
	count+= 4;
	// sequence number
	hton_seqnum = htonl(seqnum);
	memcpy(data + count, &hton_seqnum, 4);
	count+= 4;

	return count;
}

uint32_t write_traf(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size_tfhd, size_tfdt, size_trun, size, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;

	// box type
	memcpy(data + count, "traf", 4);
	count+= 4;
	
	// write tfhd
	size_tfhd = write_tfhd(data + count, media_type, context);
	if (size_tfhd < 8)
		return I2ERROR;
	count+= size_tfhd;
	
	// write tfdt
	size_tfdt = write_tfdt(data + count, media_type, context);
	if (size_tfdt < 8)
		return I2ERROR;
	count+= size_tfdt;
	
	// write trun
	size_trun = write_trun(data + count, media_type, context);
	if (size_trun < 8)
		return I2ERROR;
	count+= size_trun;

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_tfhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, one, hton_one, flags, hton_flags, hton_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	one = 1;
	flags = 0x00020000;
	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count,&hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "tfhd", 4);
	count+= 4;

	// version and flags
	hton_flags = htonl(flags);
	memcpy(data + count, &hton_flags, 4);
	count+= 4;

	// track id
	hton_one = htonl(one);
	memcpy(data + count, &hton_one, 4);
	count+= 4;

	return count;
}

uint32_t write_tfdt(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, size, hton_size;
	uint32_t earliest_presentation_time, hton_earliest_presentation_time;

	i2ctx_video *ctxVideo = context->ctxvideo;
	i2ctx_audio *ctxAudio = context->ctxaudio;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	if (media_type == VIDEO_TYPE) {
		earliest_presentation_time = ctxVideo->earliest_presentation_time;
	}
	else if (media_type == AUDIO_TYPE) {
		earliest_presentation_time = ctxAudio->earliest_presentation_time;
	}

	count = 0;
	zero = 0;

	//Size is always 16, apparently
	size = 16;
	hton_size = htonl(size);
	memcpy(data + count, &hton_size, 4);
	count+= 4;

	// box type
	memcpy(data + count, "tfdt", 4);
	count+= 4;
	
	// flags
	memcpy(data + count, &zero, 4);
	count+= 4;
	
	// baseMediaDecodeTime //TODO check standard
	hton_earliest_presentation_time = htonl(earliest_presentation_time);
	memcpy(data + count, &hton_earliest_presentation_time, 4);
	count+= 4;

	return count;
}

uint32_t write_trun(byte *data, uint32_t media_type, i2ctx *context) {

	uint32_t count, nitems, flags, hton_flags, hton_sample_size, hton_sample_duration;
	uint32_t hton_sample_delay, sample_num, hton_sample_num, offset, hton_offset, moof_pos, size, hton_size;
	
	i2ctx_sample *samples;
	//unsigned sample_key;
	int i  = 0;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 4;
	nitems = 0;

	if(media_type == VIDEO_TYPE) {
		samples = context->ctxvideo->ctxsample;
		nitems = 4;
	} else if (media_type == AUDIO_TYPE) {
		samples = context->ctxaudio->ctxsample;
		nitems = 2;
	}

	flags = samples->box_flags;
	sample_num = samples->mdat_sample_length;
	moof_pos = samples->moof_pos;


	// box type
	memcpy(data + count, "trun", 4);
	count+= 4;
	offset = (count - moof_pos) + 20 + (sample_num * nitems * 4) + 8;

	// flags
	hton_flags = htonl(flags);
	memcpy(data + count, &hton_flags, 4);
	count+= 4;

	// sample count
	hton_sample_num = htonl(sample_num);
	memcpy(data + count, &hton_sample_num, 4);
	count+= 4;

	// offset
	hton_offset = htonl(offset);
	memcpy(data + count, &hton_offset, 4);
	count+= 4;
	samples->moof_pos = 0;

	for (i = 0; i < sample_num; i++)
	{
		// sample duration
		hton_sample_duration = htonl(samples->mdat[i].duration_ms);
		memcpy(data + count, &hton_sample_duration, 4);
		count+= 4;
		// sample size
		hton_sample_size = htonl(samples->mdat[i].size);
		memcpy(data + count, &hton_sample_size, 4);
		count+= 4;
		// video exclusive
		if(media_type == VIDEO_TYPE) {
			// sample flags
			flags = samples->mdat[i].key ? 0x00000000 : 0x00010000;
			hton_flags = htonl(flags);
			memcpy(data + count, &hton_flags, 4);
			count+= 4;
			// sample composition time offsets
			hton_sample_delay = htonl(samples->mdat[i].delay);
			memcpy(data + count, &hton_sample_delay, 4);
			count+= 4;
		}
	}

	// box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, &hton_size, 4);
	return count;
}

uint32_t write_mdat(byte* source_data, uint32_t size_source_data, byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, mdat_size, hton_mdat_size;

	if ((media_type == NO_TYPE) || (media_type == AUDIOVIDEO_TYPE))
		return I2ERROR;

	count = 0;
	mdat_size = size_source_data + 8;
	hton_mdat_size = htonl(mdat_size);
	memcpy(data + count, &hton_mdat_size, 4);
	count+= 4;
	memcpy(data + count, "mdat", 4);
	count+= 4;
	memcpy(data + count, source_data, size_source_data);
	count+= size_source_data;

	return count;
}

uint32_t write_matrix(byte *data, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t tx, uint32_t ty) {
	uint32_t count, value, hton_value;
	count = 0;
	value = 0;

	// transformation matrix
	// |a  b  u|
	// |c  d  v|
	// |tx ty w|

	//a in 16.16 format
	value = a << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//b in 16.16 format
	value = b << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//u in 2.30 format
	value = 0;
	memcpy(data + count, &value, 4);
	count+= 4;
	//c in 16.16 format
	value = c << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//d in 16.16 format
	value = d << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//v in 2.30 format
	value = 0;
	memcpy(data + count, &value, 4);
	count+= 4;
	//tx in 16.16 format
	value = tx << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//ty in 16.16 format
	value = ty << 16;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;
	//w in 2.30 format
	value = 1 << 30;
	hton_value = htonl(value);
	memcpy(data + count, &hton_value, 4);
	count+= 4;

    return count;
}
