#include "Wrapper.h"


DLLEXPORT SISProtocol * DLLCALLCONV init()
{
	SISProtocol * protocol = new SISProtocol();
	return protocol;
}


DLLEXPORT int32_t DLLCALLCONV open(SISProtocol* ID_ref, const char* ID_comport, uint32_t ID_combaudrate, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref)) 
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		ID_ref->open(ID_comport);
		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_OpenByCOM);
	}
	catch (CSerial::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_OpenByCOM);
	}
}


DLLEXPORT int32_t DLLCALLCONV close(SISProtocol* ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		ID_ref->close();

		delete ID_ref;
		ID_ref = NULL;
		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_Close);
	}
}


DLLEXPORT int32_t DLLCALLCONV sequencer_activate(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Enter parameterization level 1 (S-0-0420) // Command C0400
		ID_ref->execute_command(TGM::SERCOS_Param_S, 420);

		// Primary Operation Mode (S-0-0032)
		UINT64 mode = 0b111011; // Positioning mode lagless, encoder 1
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 32, mode);

		// Leave parameterization level 1 (S-0-0422) // Command C0200
		ID_ref->execute_command(TGM::SERCOS_Param_S, 422);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV sequencer_init(SISProtocol * ID_ref, uint32_t ID_max_accel, uint32_t ID_max_jerk, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Max Acceleration (S-0-0138)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, ID_max_accel);

		// Max Jerk (S-0-0349)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 349, ID_max_jerk);

		// SPS Global Register G1 (P-0-1371) - Reset Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<UINT64>(0));

		// SPS Global Register G2 (P-0-1372) - Reset Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<UINT64>(0));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV sequencer_write(SISProtocol * ID_ref, int32_t ID_speeds[], double_t ID_accels[], double_t ID_jerks[], uint32_t ID_delays[], const uint16_t ID_set_length, uint8_t ID_direction, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
			
		for (uint16_t i = 1; i <= ID_set_length; i++)
		{
			// Speed in min^-1 (P-0-4007)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4007, i, static_cast<UINT32>(abs(ID_speeds[i])));

			// Acceleration in rad/s^2 (P-0-4008)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4008, i, ID_accels[i]);

			// Deceleration in rad/s^2 (P-0-4063)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4063, i, ID_accels[i]);

			// Jerk in rad/s^3 (P-0-4009)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4009, i, ID_jerks[i]);

			// Mode (P-0-4019)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4019, i, static_cast<UINT32>(0b10000000 & ((stde::sgn<int32_t>(ID_speeds[i]) == 1 ? 0b01 : 0b10) << 2)));

			// Pos (P-0-4006)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4006, i, static_cast<UINT64>(0));

			// Wait (P-0-4018)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4018, i, static_cast<UINT64>(0));

			// Delay (P-0-4063)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 4063, i, static_cast<UINT64>(0));

			// Timers in cs (P-0-1389)
			ID_ref->write_listelm(TGM::SERCOS_Param_P, 1389, i, ID_delays[i]);
		}		

		// Time triggers for cam (P-0-1370)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1370, static_cast<UINT32>(ID_set_length));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}	
}

DLLEXPORT int32_t DLLCALLCONV sequencer_softtrigger(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		UINT32 qb0stat;

		/// FEED DATA

		// SPS Global Register G1 (P-0-1371) - Reset Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<UINT64>(0));

		// SPS Global Register G1 (P-0-1371) - Set Read Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1371, static_cast<UINT64>(1));
					
		// Check status (P-0-1410)
		ID_ref->read_parameter(TGM::SERCOS_Param_P, 1410, qb0stat); /// TODO: Check RESULT_READ_OK bit (0b100000)

		/// TRIGGER

		// SPS Global Register G2 (P-0-1372) - Reset Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<UINT64>(0));

		// SPS Global Register G2 (P-0-1372) - Set Sequencer Trigger
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1372, static_cast<UINT64>(1));

		// Check status (P-0-1410)
		ID_ref->read_parameter(TGM::SERCOS_Param_P, 1410, qb0stat); /// TODO: Check Drive started bit (0b1000)

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_SeqWrite);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_activate(SISProtocol * ID_ref, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Enter parameterization level 1 (S-0-0420) // Command C0400
		ID_ref->execute_command(TGM::SERCOS_Param_S, 420);

		// Primary Operation Mode (S-0-0032)
		UINT64 mode = 0b10; // Velocity Control
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 32, mode);

		// Leave parameterization level 1 (S-0-0422) // Command C0200
		ID_ref->execute_command(TGM::SERCOS_Param_S, 422);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_init(SISProtocol * ID_ref, uint32_t ID_max_accel, uint32_t ID_max_jerk, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Max Acceleration (S-0-0138)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 138, ID_max_accel);

		// Max Jerk (S-0-0349)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 349, ID_max_jerk);

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCInit);
	}
}

DLLEXPORT int32_t DLLCALLCONV speedcontrol_write(SISProtocol * ID_ref, int32_t ID_speed, double_t ID_accel, ErrHandle ID_err)
{
	if (!dynamic_cast<SISProtocol*>(ID_ref))
		// Return error for wrong reference
		return set_error(
			ID_err, sformat("Reference pointing to invalid location '%p'.", ID_ref),
			Err_Invalid_Pointer);

	try
	{
		// Rotation direction
		UINT32 rotmode = static_cast<UINT32>((stde::sgn<int32_t>(ID_speed) == 1 ? 1 : 0) << 10);
		// Control Mode (P-0-1200)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1200, rotmode);

		// Acceleration in rad/s^2 (P-0-1203)
		ID_ref->write_parameter(TGM::SERCOS_Param_P, 1203, ID_accel);

		// Speed in rpm (S-0-0036)
		ID_ref->write_parameter(TGM::SERCOS_Param_S, 36, static_cast<UINT32>(abs(ID_speed)));

		return Err_NoError;
	}
	catch (SISProtocol::ExceptionGeneric &ex)
	{
		return set_error(ID_err, char2str(ex.what()), Err_Block_VelCWrite);
	}
}