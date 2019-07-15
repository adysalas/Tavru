#Readme TAVRU
Criar um arede ethernet manualmente com IPV4: 172.45.100.210 // ping 172.45.100.211 // ping 172.45.100.212 (Ricardo/Amado/Xastre respetivamente) com NetMask de 16 bits.

ifconfig
sudo ifconfig enp2s0 192.168.35.120 netmask 255.255.0.0 (para fazer trigger às cameras)
ping 172.45.100.210 // ping 172.45.100.211 // ping 172.45.100.212 (Ricardo/Amado/Xastre respetivamente) para confirmar conecção

Abrir browser com Wifi desligado e Ethernet CameraField ligada e escrever no browser: 192.168.35.127 (janela para trigger das cameras)
Confirgurar por defeito as cameras 0 e 1 a 10Hz e dutycycle a 200 PPM
Send Configurations
Start


## Se ainda nao isntalou o Software Basler:
To install the pylon 5 Camera Software Suite in /opt/pylon5
follow these steps:

  * Change to the directory which contains this INSTALL file, e.g.:
       cd ~/pylon-5.2.x.y-<ARCH>

  * Extract the corresponding SDK into /opt
       sudo tar -C /opt -xzf pylonSDK*.tar.gz

  * Install udev-rules to set up permissions for Basler USB cameras
     (Not required if installing using the Debian packages (*.deb))
       ./setup-usb.sh

  * Unplug and replug all USB cameras to get the udev rules applied.

  * Execute /opt/pylon5/bin/PylonViewerApp to test your cameras.

## fim da instalacao do software Basler


##Executar:
/opt/pylon5/bin/PylonViewerApp

Deslidar Rede das cameras (CameraField) e voltar a ligar
Voltar ao ambiente do software Basler

Já aparecerao as cameras 0 e 1.
Clicar numa delas, fazer Open e de seguida capturar uma imagem ou video
(a repreduçao de video de ambas as cameras em simultaneo causa problemas)





// Comando matlab para obter dados caracteristicos das cameras 0 e 1
>> gigecamlist
