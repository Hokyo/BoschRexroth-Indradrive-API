#include "stdafx.h"
#include "CppUnitTest.h"

#include "SISProtocol.h"
#include "Telegrams.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SISProtocolTest
{		
	TEST_CLASS(utSISProtocol)
	{
	public:


		TEST_METHOD(SIS_SetBaudrate)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				sis.set_baudrate(SISProtocol::Baud_19200);

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Assert::Fail(char2wchar(ex.what()));
			}			
		}

		TEST_METHOD(SIS_SINGLEPARAMETER_READ)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");
				
				UINT32 rcvddata;
				sis.read_parameter(TGM::SERCOS_Param_S, 36, rcvddata);
				Logger::WriteMessage(sformat("S-0-0036=%d", rcvddata).c_str());

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_SINGLEPARAMETER_WRITE)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				DOUBLE value = 5.2;
				sis.write_parameter(TGM::SERCOS_Param_S, 36, value);

				DOUBLE rcvddata;
				sis.read_parameter(TGM::SERCOS_Param_S, 36, rcvddata);
				Logger::WriteMessage(sformat("S-0-0036=%f", rcvddata).c_str());

				sis.close();

				Assert::AreEqual<BYTE>(value, rcvddata);
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_LISTPARAMETER_READ)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				for (int i = 1; i < 10; i++)
				{
					DOUBLE rcvddata;
					sis.read_listelm(TGM::SERCOS_Param_P, 4007, i, rcvddata);
					Logger::WriteMessage(sformat("P-0-4007:%d=%f", i, rcvddata).c_str());
				}

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_LISTPARAMETER_WRITE)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				DOUBLE data = 500.2;
				sis.write_listelm(TGM::SERCOS_Param_P, 4007, 1, data);

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

		TEST_METHOD(SIS_EXEC_COMMAND)
		{
			SISProtocol sis;

			try
			{
				sis.open("COM1");

				// Activate parameterization level 1 // C0400
				sis.execute_command(TGM::SERCOS_Param_S, 420);

				// Leave parameterization level 1 // C0200
				sis.execute_command(TGM::SERCOS_Param_S, 422);

				sis.close();
			}
			catch (SISProtocol::ExceptionTransceiveFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionSISError &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (SISProtocol::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionReceptionFailed &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
			catch (CSerial::ExceptionGeneric &ex)
			{
				Logger::WriteMessage(ex.what());
				Assert::Fail(char2wchar(ex.what()));
			}
		}

	};
}