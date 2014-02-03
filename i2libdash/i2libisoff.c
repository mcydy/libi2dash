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

//trex for audio and video files is the same
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

//mdhd for audio and video files is the same
uint32_t write_dinf(byte *data, uint32_t media_type);

//mdhd for audio and video files is the same
uint32_t write_dref(byte *data, uint32_t media_type);

uint32_t write_url(byte * data);

uint32_t write_stbl(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_stsd(byte *data, uint32_t media_type, i2ctx *context);

//only video
uint32_t write_avc1(byte *data, i2ctx_video ctxVideo);

//only video
uint32_t write_avc3(byte *data, i2ctx_video ctxVideo);

//only video
uint32_t write_avcc(byte *data, i2ctx_video ctxVideo);

//only audio
uint32_t write_mp4a(byte *data, i2ctx_audio ctxAudio);

//only audio
uint32_t write_esds(byte *data, i2ctx_audio ctxAudio);

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

uint32_t write_moof(byte *data, uint32_t media_type, i2ctx *context);

//mfhd for audio and video files is the same
uint32_t write_mfhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_traf(byte *data, uint32_t media_type, i2ctx *context);

//tfhd for audio and video files is the same
uint32_t write_tfhd(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_tfdt(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_trun(byte *data, uint32_t media_type, i2ctx *context);

uint32_t write_mdat(byte *data, uint32_t media_type, i2ctx *context);

i2Err initVideoGenerator(byte *sps, byte *pps, i2ctx *context){}

i2Err initAudioGenerator(byte *aac_data, i2ctx *context){}
//TODO mirar que es lo que nos importa de aac

i2Err segmentGenerator(byte *data, uint32_t media_type, i2ctx *context){}

uint32_t write_matrix(byte *data, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t tx, uint32_t ty);

uint32_t write_ftyp(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, version, hton_version, zero;

	count = 4;
	version = 1;
	zero = 0;
	
	//Box type
	memcpy(data + count, "ftyp", 4);
	count = count + 4;

	//Major brand
    memcpy(data + count, "iso6", 4);
	count = count + 4;

    //Version
	hton_version = htonl(version);
    memcpy(data + count, hton_version, 4);
	count = count + 4;

    //Compatible brands
    memcpy(data + count, "isom", 4);
	count = count + 4;
    memcpy(data + count, "iso6", 4);
	count = count + 4;
    memcpy(data + count, "dash", 4);
	count = count + 4;
	
	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, hton_size, 4);
	return count;
}

uint32_t write_moov(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, size_mvhd, size_mvex, size_trak, hton_size;
	count = 4;
	
	//Box type
	memcpy(data + count, "moov", 4);
	count = count + 4;

    size_mvhd = write_mvhd(data + count, media_type, context);
	count = count + size_mvhd;
	size_mvex = write_mvex(data + count, media_type, context);
	count = count + size_mvex;
	size_trak = write_trak(data + count, media_type, context);
	count = count + size_trak;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, hton_size, 4);
	return count;
}

uint32_t write_mvhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, hton_size, size_matrix, flag32, hton_flag32;
	uint16_t flag16, hton_flag16;
	byte *matrix;
	count = 4;
	
	//Box type
	memcpy(data + count, "mvhd", 4);
	count = count + 4;

	//Version
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Creation time
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Modification time
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Timescale
	flag32 = 1000;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Duration
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Reserved
	flag32 = 0x00010000;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, hton_flag32, 4);
	count = count + 4;
	flag16 = 0x0100;
	hton_flag16 = htons(flag16);
    memcpy(data + count, flag16, 2);
	count = count + 2;
	flag16 = 0x0;
    memcpy(data + count, flag16, 2);
	count = count + 2;
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;
	
	//transformation matrix
    size_matrix = write_matrix(data, 1, 0, 0, 1, 0, 0);
	count = count + size_matrix;

    //Reserved
	flag32 = 0x0;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;
    memcpy(data + count, flag32, 4);
	count = count + 4;

    //Next track id
	flag32 = 1;
	hton_flag32 = htonl(flag32);
    memcpy(data + count, flag32, 4);
	count = count + 4;

	//Box size
	size = count;
	hton_size = htonl(size);
	memcpy(data, hton_size, 4);
	return count;

}

uint32_t write_mvex(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_trex(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_trak(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_tkhd(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_mdia(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_mdhd(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_hdlr(byte *data, uint32_t media_type, i2ctx *context) {

}

uint32_t write_minf(byte *data, uint32_t media_type, i2ctx *context) {

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
	memcpy(data + count, hton_size, 4);
	count = count + 4;
	//Box type
	memcpy(data + count, "vmhd", 4);
	count = count + 4;

    //Version and flags
	hton_flag32 = htonl(flag32);
    memcpy(data + count, hton_flag32, 4);

	count = count + 4;

    //reserved (graphics mode=copy)
    memcpy(data + count, zero, 4);
	count = count + 4;
	memcpy(data + count, zero, 4);
	count = count + 4;
	
	return count;
}

uint32_t write_smhd(byte *data) {
	uint32_t count, size;
	uint64_t zero;
	// smhd size is always 16, apparently
	size = 16;
	count = 0;
	zero = 0;
	// Box size
	memcpy(data + count, htonl(size), 4);
	count = count + 4;
	// Box type
	memcpy(data + count, "smhd", 4);
	count = count + 4;
	// Version and flags and reserved (balance normally = 0)
	memcpy(data + count, zero, 8);
	count = count + 8;

	return count;
}

uint32_t write_dinf(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, dref;
	count = 0;
	zero = 0;
	memcpy(data + count, zero, 4);
	count = count + 4;
	memcpy(data + count, "dinf", 4);
	count = count + 4;
	dref = write_dref(data + count, media_type, context);
	if !(dref > 0)
		return I2ERROR
	count = count + dref;

	return count;
}

//mdhd for audio and video files is the same
uint32_t write_dref(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, one, url_size;
	count = 0;
	zero = 0;
	one = 1;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "dref", 4);
	count = count + 4;
	// version and flags
	memcpy(data + count, zero, 4);
	count = count + 4;
	// entries
	memcpy(data + count, one, 4);
	count = count + 4;

	return count;
}

uint32_t write_url(byte * data) {
	uint32_t count, size, flags;
	count = 0;
	size = 0xc;
	flags = 0x00000001;
	// size
	memcpy(data + count, htonl(size), 4);
	count = count + 4;
	// box type
	memcpy(data + count, "url ", 4);
	count = count + 4;
	// version and flags
	memcpy(data + count, htonl(flags), 4);
	count = count + 4;

	return count;
}

uint32_t write_stbl(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, stsd, stts, stsc, stsz, stco;
	count = 0;
	
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "stbl", 4);
	count = count + 4;

	// write subBoxes and update count value
	stsd = write_stsd(data + count, media_type, context);
	if !(stsd > 0 && stsd != I2ERROR)
		return I2ERROR
	count = count + stsd;
	
	stts = write_stts(data + count, media_type, context);	
	if !(stts > 0 && stts != I2ERROR)
		return I2ERROR
	count = count + stts;

	stsc = write_stsc(data + count, media_type, context);
	if !(stsc > 0 && stsc != I2ERROR)
		return I2ERROR
	count = count + stsc;

	stsz = write_stsz(data + count, media_type, context);
	if !(stsz > 0 && stsz != I2ERROR)
		return I2ERROR
	count = count + stsz;
	
	stco = write_stco(data + count, media_type, context);
	if !(stco > 0 && stco != I2ERROR)
		return I2ERROR
	count = count + stco;
	
	return count;
}

uint32_t write_stsd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, one, avc1, mp4a;
	i2ctx_audio *ctxaudio;
	i2ctx_video *ctxvideo;
	count = 0;
	zero = 0;
	one = 1;
	ctxvideo = context->ctxvideo;
	ctxaudio = context->ctxaudio;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "stsd", 4);
	count = count + 4;
	// version and flags
	memcpy(data + count, zero, 4);
	count = count + 4;
	// entries
	memcpy(data + count, one, 4);
	count = count + 4;
	/*
	* media_type = 0 none
	* media_type = 1 video
	* media_type = 2 audio
	* media_type = 3 video_audio
	*/
	// write avcX or AAC boxes
	if(media_type == 1) {
		// write avc1
		avc1 = write_avc1(data + count, ctxvideo);
		if !(avc1 > 0)
			return I2ERROR
		count = count + avc1;
	} else if(media_type == 2) {
		// write mp4a
		mp4a = write_mp4a(data + count, ctxaudio);
		if !(mp4a > 0)
			return I2ERROR
		count = count + mp4a;
	} else if(media_type == 3) {
		// TODO
		return I2ERROR;
	} else {
		return I2ERROR;
	}

	return count;
}


uint32_t write_avc1(byte *data, i2ctx_video ctxVideo) {
	uint32_t count, zero_32, one, hv_resolution, avcc;
	uint64_t zero_64;
	uint16_t zero_16, width, height;
	count = 0;
	zero_16 = 0;
	zero_32 = 0;
	zero_64 = 0;
	one = 1;
	hv_resolution = 0x00480000;
	width = ctxVideo.width;
	height = ctxVideo.height;
	// size
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "avc1", 4);
	count + count + 4;
	// reserved
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	memcpy(data + count, zero_16, 2);
	count = count + 2;
	// data reference index
	memcpy(data + count, one, 2);
	count = count + 2;
	// codec stream version + revision + reserved
	memcpy(data + count, zero_64, 8);
	count = count + 8;
	memcpy(data + count, zero_64, 8);
	count = count + 8;
	// width
	memcpy(data + count, htonl(width), 2);
	count = count + 2;
	// height
	memcpy(data + count, htonl(height), 2);
	count = count + 2;
	// horitzonal and vertical resolution 72dpi
	memcpy(data + count, htonl(hv_resolution), 4);
	count = count + 4;
	memcpy(data + count, htonl(hv_resolution), 4);
	count = count + 4;
	// data size
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// frame count
	memcpy(data + count, one, 2);
	count = count + 2;
	// compressor name
	memcpy(data + count, zero_64, 8);
	count = count + 8;
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// reserved
	memcpy(data + count, zero_64, 8);
	count = count + 8;
	memcpy(data + count, zero_64, 8);
	count = count + 8;
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	memcpy(data + count, htonl(0x18), 2);
	count = count + 2;
	memcpy(data + count, htonl(0xffff), 2);
	count = count + 2;
	// write avcC
	avcc = write_avcc(data + count, ctxVideo);
	if !(avcc > 0)
		return I2ERROR
	count = count + avcc;

	return count;
}

uint32_t write_avc3(byte *data, i2ctx_video ctxVideo) {
	// TODO
}

uint32_t write_avcc(byte *data, i2ctx_video ctxVideo) {
	uint32_t count, zero, size;
	u_char avc_header;
	count = 0;
	zero = 0;
	avc_header = i2ctx_video.avc_header;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "avcC", 4);
	count = count + 4;
	// avc header, includes version, profile, level, sps and pps
	size = sizeof(avc_header);
	memcpy(data + count, htonl(avc_header), size);
	count = count + size;

	return count;
}

uint32_t write_mp4a(byte *data, i2ctx_audio ctxAudio) {
	uint32_t count, zero_32, esds;
	uint16_t zero_16, audio_channels, sample_size, sample_rate;
	uint64_t zero_64;
	count = 0;
	zero_16 = 0;
	zero_32 = 0;
	zero_64 = 0;
	audio_channels = ctxAudio.channels;
	sample_size = ctxAudio.sample_size;
	sample_rate = ctxAudio.sample_rate;
	// size
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "mp4a", 4);
	count = count + 4;
	// reserved
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	memcpy(data + count, zero_16, 2);
	count = count + 2;
	// channel count
	memcpy(data + count, htonl(audio_channels), 2);
	count = count + 2;
	// sample size
	memcpy(data + count, (htonl(sample_size) * 8), 2);
	count = count + 2;
	// reserved
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// timescale
	memcpy(data + count, htonl(1000), 2);
	count = count + 2;
	// sample rate
	memcpy(data + count, htonl(sample_rate), 2);
	count = count + 2;
	// write esds
	esds = write_esds(data + count, ctxAudio);
	if !(esds > 0)
		return I2ERROR
	count = count + esds;

	return count;
}

uint32_t write_esds(byte *data, i2ctx_audio ctxAudio) {
	// TODO
}

uint32_t write_stts(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "stts", 4);
	count = count + 4;
	// version
	memcpy(data + count, zero, 4);
	count = count + 4;
	// entries
	memcpy(data + count, zero, 4);
	count = count + 4;

	return count;
}

uint32_t write_stsc(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "stsc", 4);
	count = count + 4;
	// version
	memcpy(data + count, zero, 4);
	count = count + 4;
	// entries
	memcpy(data + count, zero, 4);
	count = count + 4;

	return count;
}

uint32_t write_stsz(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// Box type
	memcpy(data + count, "stsz", 4);
	count = count + 4;
	// Version
	memcpy(data + count, zero, 4);
	count = count + 4;
	// Flags
	memcpy(data + count, zero, 4);
	count = count + 4;
	// Entries
	memcpy(data + count, zero, 4);
	count = count + 4;

	return count;
}

uint32_t write_stco(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "stco", 4);
	count = count + 4;
	// version
	memcpy(data + count, zero, 4);
	count = count + 4;
	// entries
	memcpy(data + count, zero, 4);
	count = count + 4;

	return count;
}

uint32_t write_styp(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "styp", 4);
	count = count + 4;
	// major brand
	memcpy(data + count, "iso6", 4);
	count = count + 4;
	// compatible brands
	memcpy(data + count, "isom", 4);
	count = count + 4;
	memcpy(data + count, "iso6", 4);
	count = count + 4;
	memcpy(data + count, "dash", 4);
	count = count + 4;

	return count;
}

uint32_t write_sidx(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero_32, duration_ms, decode_time_ms, one_32, reference_size, subseg_duration_ms;
	uint8_t zero_8;
	uint16_t zero_16, one_16;
	count = 0;
	zero_8 = 0;
	zero_16 = 0;
	zero_32 = 0;
	one_16 = 1;
	one_32 = 1;
	subseg_duration_ms = context->i2ctx_sample.duration_ms;
	decode_time_ms = context->i2ctx_sample.decode_time_ms;
	reference_size = context.reference_size;
	duration_ms = context.duration_ms;
	// size
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "sidx", 4);
	count = count + 4;
	// version
	memcpy(data + count, zero_32, 4);
	count = count + 4;
	// reference id
	memcpy(data + count, one_32, 4);
	count = count + 4;
	// timescale
	memcpy(data + count, htonl(1000), 4);
	count = count + 4;
	// decode time
	memcpy(data + count, htonl(decode_time_ms), 4);
	count = count + 4;
	// duration
	memcpy(data + count, htonl(duration_ms), 4);
	count = count + 4;
	// reserved
	memcpy(data + count, zero_16, 2);
	count = count + 2;	
	// reference count = 1
	memcpy(data + count, one_16, 2);
	count = count + 2;
	// reference size
	memcpy(data + count, htonl(reference_size), 4);
	count = count + 4;
	// subsegment_duration 
	memcpy(data + count, htonl(subseg_duration_ms), 4);
	count = count + 4;
	// 1st bit is reference type, the rest is reference size
	memcpy(data + count, htonl(0x90), 1);
	count = count + 1;
	// SAP delta time
	memcpy(data + count, zero_16, 2);
	count = count + 2;
	memcpy(data + count, zero_8, 1);
	count = count + 1;

	return count;
}

uint32_t write_moof(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "moof", 4);
	count = count + 4;
	// write mfhd
	mfhd = write_mfhd(data + count, media_type, context);
	if !(mfhd > 0)
		return I2ERROR
	count = count + mfhd;
	// write traf
	traf = write_traf(data + count, media_type, context);
	if !(traf > 0)
		return I2ERROR
	count = count + traf;

	return count;
}

// Esta función precisa ctxsample. TODO check que respeta desde arriba
uint32_t write_mfhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, seqnum;
	count = 0;
	zero = 0;
	seqnum = context->ctxsample.index;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "mfhd", 4);
	count = count + 4;
	// version and flags
	memcpy(data + count, zero, 4);
	count = count + 4;
	// sequence number
	memcpy(data + count, htonl(seqnum), 4);
	count = count + 4;

	return count;
}

uint32_t write_traf(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t zero, count, tfhd, tfdt, trun;
	count = 0;
	zero = 0;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "traf", 4);
	count = count + 4;
	// write tfhd
	tfhd = write_tfhd(data + count, media_type, context);
	if !(tfhd > 0)
		return I2ERROR
	count = count + tfhd;
	// write tfdt
	tfdt = write_tfdt(data + count, media_type, context);
	if !(tfdt > 0)
		return I2ERROR
	count = count + tfdt;
	// write trun
	trun = write_trun(data + count, media_type, context);
	if !(trun > 0)
		return I2ERROR
	count = count + trun;

	return count;
}

uint32_t write_tfhd(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, size, zero, one, flags;
	count = 0;
	zero = 0;
	one = 1;
	flags = 0x0002000;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "tfhd", 4);
	count = count + 4;
	// version and flags
	memcpy(data + count, htonl(flags), 4);
	count = count + 4;
	// track id
	memcpy(data + count, one, 4);
	count = count + 4;

	return count;
}

uint32_t write_tfdt(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, decode_time_ms;
	count = 0;
	zero = 0;
	decode_time_ms = context->i2ctx_sample.decode_time_ms;
	// size
	memcpy(data + count, zero, 4);
	count = count + 4;
	// box type
	memcpy(data + count, "tfdt", 4);
	count = count + 4;
	// flags
	memcpy(data + count, zero, 4);
	count = count + 4;
	// decode time
	memcpy(data + count, htonl(decode_time_ms), 4);
	count = count + 4;

	return count;
}

uint32_t write_trun(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, zero, num_samples;
	count = 0;
	zero = 0;
	num_samples = 0;
}

uint32_t write_mdat(byte *data, uint32_t media_type, i2ctx *context) {
	uint32_t count, mdat_size;
	count = 0;
	mdat_size = context->i2ctx_sample.mdat_size;
	memcpy(data + count, htonl(mdat_size), 4);
	count = count + 4;
	memcpy(data + count, "mdat", 4);
	count = count + 4;

	return count;
}

uint32_t write_matrix(byte *data, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t tx, uint32_t ty) {
	uint32_t count, value, hton_value;
	count = 0;
	value = 0x0;

	// transformation matrix
	// |a  b  u|
	// |c  d  v|
	// |tx ty w|

	//a in 16.16 format
	value = a << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//b in 16.16 format
	value = b << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//u in 2.30 format
	value = 0
	memcpy(data + count, value, 4);
	count = count + 4;
	//c in 16.16 format
	value = c << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//d in 16.16 format
	value = d << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//v in 2.30 format
	value = 0
	memcpy(data + count, value, 4);
	count = count + 4;
	//tx in 16.16 format
	value = tx << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//ty in 16.16 format
	value = ty << 16;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;
	//w in 2.30 format
	value = 1;
	hton_value = htonl(value);
	memcpy(data + count, hton_value, 4);
	count = count + 4;

    return count;
}
