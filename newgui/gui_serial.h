/*! \file GUI_Serial.h
	Defines the class GUI_Serial
*/
/************************************************************************************************
Copyright 2008 Gregory W Heckler

This file is part of the GPS Software Defined Radio (GPS-SDR)

The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GPS-SDR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with GPS-SDR; if not,
write to the:

Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
************************************************************************************************/

#ifndef GUI_SERIAL_H
#define GUI_SERIAL_H

#include "gui.h"

/*! \ingroup CLASSES
 *
 */
class GUI_Serial
{

	private:

		/* Default GUI_Serial variables */
		uint32 				execution_tic;					//!< Execution counter
		uint32 				start_tic;						//!< OS tic at start of function
		uint32 				stop_tic;						//!< OS tic at end of function
		pthread_t 			thread;							//!< For the thread
		pthread_mutex_t		mutex;							//!< Protect the following variable

	public:

		/* Nondefault variables */
		int					message_sync;					//!< Are we synched to the packets
		int					pipe_open;
		char 				syncword[9];

		/* Headers for CCSDC packets */
		CCSDS_PH 			pheader;						//!< CCSDS Packet header
		CCSDS_CH 			cheader;						//!< CCSDS Command header
		CCSDS_Header		ccsds_header;					//!< Decoded header
		int 				packet_count[LAST_M_ID+1];		//!< Count the packets
		int					byte_count;						//!< Count the bytes

		/* Data gets stored here! */
		Board_Health_M 		board_health;					//!< Board health message
		Task_Health_M		task_health;					//!< Task health message
		Channel_Health_M 	channel_health[MAX_CHANNELS+1]; //!< Channel health message, last element is used as a buffer

		SPS_M				sps;							//!< SPS message
		Clock_M				clock;							//!< Clock message
		SV_Position_M		sv_positions[MAX_CHANNELS+1];	//!< SV Positions, last element is used as a buffer
		Pseudorange_M 		pseudoranges[MAX_CHANNELS+1];	//!< Pseudoranges, last element is used as a buffer
		Measurement_M 		measurements[MAX_CHANNELS+1];	//!< Measurements, last element is used as a buffer

		Ephemeris_M			ephemeris[NUM_CODES+1];			//!< Ephemeris message, last element is used as a buffer
		Almanac_M			almanac[NUM_CODES+1];			//!< Almanac message, last element is used as a buffer
		Ephemeris_Status_M	ephemeris_status;				//!< Status of ephemeris
		FIFO_M				fifo_status;					//!< FIFO status

	public:

		/* Default GUI_Serial methods */
		GUI_Serial();										//!< Constructor
		~GUI_Serial();										//!< Destructor
		void Start();										//!< Start the thread
		void Stop();										//!< Stop the thread
		void Import();										//!< Get data into the thread
		void Export();										//!< Get data out of the thread
		void Lock(){pthread_mutex_lock(&mutex);};			//!< Lock the object's mutex
		void Unlock(){pthread_mutex_unlock(&mutex);};		//!< Unlock the object's mutex
		uint32 GetExecTic(){return(execution_tic);};		//!< Get the execution counter
		uint32 GetStartTic(){return(start_tic);};			//!< Get the Nucleus tic at start of function
		uint32 GetStopTic(){return(execution_tic);};		//!< Get the Nucleus tic at end of function
		uint32 GetPipeOpen(){return(pipe_open);};
		uint32 GetByteCount(){return(byte_count);};

		/* Nondefault methods */
		void readPipe();
		void pipeRead(void *_b, int32 _bytes);
	    void DecodeCCSDSPacketHeader(CCSDS_Header *_h, CCSDS_PH *_p);

};

#endif /* GUI_Serial_H */