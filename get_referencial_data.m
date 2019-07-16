clear all
clc
close all

'A configurar'

g0=gigecam(1);%('172.45.100.0')
g0.PixelFormat='BayerBG8';
g1=gigecam(2);%('172.45.100.1')
g1.PixelFormat='BayerBG8';

'Configuracao Terminada'
pause
%% Capturar origem do referencial

'A capturar origem do referencial'

[frames0x,t0x] = snapshot(g0);
[frames1x,t1x] = snapshot(g1);

dt = ((t1x.Hour*3600+t1x.Minute*60+t1x.Second) - (t0x.Hour*3600+t0x.Minute*60+t0x.Second))

figure (1)
subplot(1,2,1)
imshow(frames0x)
subplot(1,2,2)
imshow(frames1x)

name0=sprintf('camera0_Origem_dark.jpeg',i);
imwrite(frames0x,name0,'JPEG');
name1=sprintf('camera1_Oorigem_dark.jpeg',i);
imwrite(frames1x,name1,'JPEG');

'Capturado origem do referencial'
pause

%% Capturar segundo eixo X

'A capturar segundo eixo X'

[frames0x,t0x] = snapshot(g0);
[frames1x,t1x] = snapshot(g1);

dt = ((t1x.Hour*3600+t1x.Minute*60+t1x.Second) - (t0x.Hour*3600+t0x.Minute*60+t0x.Second))

figure (1)
subplot(1,2,1)
imshow(frames0x)
subplot(1,2,2)
imshow(frames1x)

name0=sprintf('camera0_Ox_dark.jpeg',i);
imwrite(frames0x,name0,'JPEG');
name1=sprintf('camera1_Ox_dark.jpeg',i);
imwrite(frames1x,name1,'JPEG');

'A capturar segundo eixo X'
pause
%% Capturar segundo eixo Y

'A capturar segundo eixo Y'

[frames0x,t0x] = snapshot(g0);
[frames1x,t1x] = snapshot(g1);

dt = ((t1x.Hour*3600+t1x.Minute*60+t1x.Second) - (t0x.Hour*3600+t0x.Minute*60+t0x.Second))

figure (1)
subplot(1,2,1)
imshow(frames0x)
subplot(1,2,2)
imshow(frames1x)

name0=sprintf('camera0_Oy_dark.jpeg',i);
imwrite(frames0x,name0,'JPEG');
name1=sprintf('camera1_Oy_dark.jpeg',i);
imwrite(frames1x,name1,'JPEG');

'A capturar segundo eixo Y'
'Concluido'