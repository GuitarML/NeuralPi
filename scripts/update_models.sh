#############################################################################
# NeuralPi - Update Models - Mac/Linux Script
#
# This script transfers models from a Linux or Mac computer to the NeuralPi,
#     and from the NeuralPi back to the host computer. Edit the Raspberry Pi
#     IP address (after connecting to a local Wifi Network), and run this
#     script from a Linux/Mac computer running the NeuralPi plugin.
#
# Note: Ensure that all models have unique names or they will be overwritten
# Note: If prompted when connecting, type "yes" and hit enter
#############################################################################

# USER INPUTS #

rpi_ip_address=127.0.0.1  # Update this field with the Raspberry Pi's IP address

# Directory paths for host computer (json models and IRs)
host_model_path=~/Documents/GuitarML/NeuralPi/tones   #Typical Mac/Linux Path (shouldn't need to change)
host_ir_path=~/Documents/GuitarML/NeuralPi/irs   #Typical Mac/Linux Path (shouldn't need to change)

# Directory paths for Raspberry Pi (json models and IRs)
rpi_model_path=/home/mind/Documents/GuitarML/NeuralPi/tones # Rpi with Elk OS Path (shouldn't need to change)
rpi_ir_path=/home/mind/Documents/GuitarML/NeuralPi/irs # Rpi with Elk OS Path (shouldn't need to change)

#############################################################################
echo "Copying all models from local computer to Rpi.."
scp $host_model_path/*.json root@$rpi_ip_address:$rpi_model_path/

echo "Copying all IRs from local computer to Rpi.."
scp $host_ir_path/*.wav root@$rpi_ip_address:$rpi_ir_path/


echo "Copying all models from Rpi to local computer.."
scp root@$rpi_ip_address:$rpi_model_path/*.json $host_model_path/

echo "Copying all IRs from Rpi to local computer.."
scp root@$rpi_ip_address:$rpi_ir_path/*.wav $host_ir_path/
