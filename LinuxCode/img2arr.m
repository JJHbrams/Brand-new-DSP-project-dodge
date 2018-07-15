clc,clear, close all

img=zeros(240,320,3);
% Image 가공
% img(:,:,1) = rgb2gray(uint16(round((double(imresize(imread('D:\DSP\Project\Ver2.2.2\count1.png'),[240,320]))/255)*65535)));
% img(:,:,2) = rgb2gray(uint16(round((double(imresize(imread('D:\DSP\Project\Ver2.2.2\count2.png'),[240,320]))/255)*65535)));
% img(:,:,3) = rgb2gray(uint16(round((double(imresize(imread('D:\DSP\Project\Ver2.2.2\count3.png'),[240,320]))/255)*65535)));

img(:,:,1) = rgb2gray(imresize(imread('D:\DSP\Project\Ver2.2.2\count1.png'),[240,320]));
img(:,:,2) = rgb2gray(imresize(imread('D:\DSP\Project\Ver2.2.2\count2.png'),[240,320]));
img(:,:,3) = rgb2gray(imresize(imread('D:\DSP\Project\Ver2.2.2\count3.png'),[240,320]));

count=zeros(240,320,3);

% Number 3
% Make data
I=img(:,:,3);
I=round(I/7);
r=bitshift(I,11);
g=bitshift(I,5);
b=I;
rgb=bitor(bitor(r,g),b);
count(:,:,3)=rgb;
% display : tftlcd.h에서 처리하는 방식
RGB=zeros(240,320,3);
RGB(:,:,1) = bitshift(bitand(rgb,31),-11);
RGB(:,:,2) = bitshift(bitand(rgb,63),-5);
RGB(:,:,3) = bitand(rgb,31);
figure(1)
image(uint8(RGB))

% Number 2
% Make data 
I=img(:,:,2);
I=round(I/7);
r=bitshift(I,11);
g=bitshift(I,5);
b=I;
rgb=bitor(bitor(r,g),b);
count(:,:,2)=rgb;
% display
RGB=zeros(240,320,3);
RGB(:,:,1) = bitshift(bitand(rgb,31),-11);
RGB(:,:,2) = bitshift(bitand(rgb,63),-5);
RGB(:,:,3) = bitand(rgb,31);
figure(2)
image(uint8(RGB))

% Number 1
% Make data
I=img(:,:,1);
I=round(I/7);
r=bitshift(I,11);
g=bitshift(I,5);
b=I;
rgb=bitor(bitor(r,g),b);
count(:,:,1)=rgb;
% display
RGB=zeros(240,320,3);
RGB(:,:,1) = bitshift(bitand(rgb,31),-11);
RGB(:,:,2) = bitshift(bitand(rgb,63),-5);
RGB(:,:,3) = bitand(rgb,31);
figure(3)
image(uint8(RGB))

% array로 반환
fid = fopen('D:\DSP\Project\Ver2.2.5\count_number.h','w');
fprintf(fid, '//convert by 충남대 장진혁\n');
fprintf(fid, '//Countdown image array\n');
fprintf(fid, 'const unsigned int number[3][240*320] = {\n');
for i=1:3
    fprintf(fid, '\n{');
    for j=1:240
        for k=1:320
            fprintf(fid, ' %3d,',(count(j,k,i)));
        end
        fprintf(fid, '\n');
    end
    fprintf(fid, '},\n');
end
fprintf(fid, '}; //end of table');
fclose(fid);
'done'
