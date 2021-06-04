::############################################################################
:: NeuralPi - Update Models - Windows Script
::
:: This script transfers models from a Windows computer to the NeuralPi,
::     and from the NeuralPi back to the host computer. Edit the Raspberry Pi
::     IP address (after connecting to a local Wifi Network), and run this
::     script from a Windows computer running the NeuralPi plugin.
::
:: Note: Ensure OpenSSH is installed. This comes installed with Windows as of 2018.
::############################################################################

:: USER INPUTS 

:: Update this field with the Raspberry Pi's IP address
set "rpi_ip_address=127.0.0.1"


:: Typical Windows 10 Path, edit <YOUR_USERNAME> with your Windows Username
set "host_model_path=C:/Users/<YOUR_USERNAME>/AppData/Roaming/GuitarML/NeuralPi/tones"


:: Rpi with Elk OS Path (shouldn't need to change)
set "rpi_model_path=/home/mind/.config/GuitarML/NeuralPi/tones"

#############################################################################

:: Copy all models from local computer to Rpi
scp %host_model_path%/*.json  root@%rpi_ip_address%:%rpi_model_path%/

:: Copy all models from Rpi to local computer
scp root@%rpi_ip_address%:%rpi_model_path%/*.json  %host_model_path%/
