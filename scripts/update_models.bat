::############################################################################
:: NeuralPi - Update Models - Windows Script
::
:: This script transfers models and impulse responses from a Windows computer
::     to the NeuralPi, and from the NeuralPi back to the host computer. Edit
::     the Raspberry Pi IP address (after connecting to a local Wifi Network),
::     and run this script from a Windows computer running the NeuralPi plugin.
::
:: Note: Ensure OpenSSH is installed. This comes installed with Windows as of 2018.
::############################################################################

:: USER INPUTS 

:: Update this field with the Raspberry Pi's IP address
set "rpi_ip_address=127.0.0.1"


:: Typical Windows 10 Path (shouldn't need to change)
set "host_model_path=%userprofile%/Documents/GuitarML/NeuralPi/tones"
set "host_ir_path=%userprofile%/Documents/GuitarML/NeuralPi/irs"


:: Rpi with Elk OS Path (shouldn't need to change)
set "rpi_model_path=/home/mind/Documents/GuitarML/NeuralPi/tones"
set "rpi_ir_path=/home/mind/Documents/GuitarML/NeuralPi/irs"

:: ############################################################################

:: Copy all models from local computer to Rpi
scp %host_model_path%/*.json  root@%rpi_ip_address%:%rpi_model_path%/

:: Copy all IRs from local computer to Rpi
scp %host_ir_path%/*.wav  root@%rpi_ip_address%:%rpi_ir_path%/


:: Copy all models from Rpi to local computer
scp root@%rpi_ip_address%:%rpi_model_path%/*.json  %host_model_path%/

:: Copy all IRs from Rpi to local computer
scp root@%rpi_ip_address%:%rpi_ir_path%/*.wav  %host_ir_path%/
