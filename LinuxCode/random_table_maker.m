clc
scale=50; % 랜덤하게 표현한 최대의 수
n=300; % 랜덤한 숫자 개수
a=round(rand(n,1)*(2*scale))-scale;

fid = fopen('C:\Users\MrJohd\Desktop\Task\4학년\최신DSP\Project\Dodge\random_table.h','w');
fprintf(fid, '//convert by 충남대 장진혁\n');
fprintf(fid, '//Random number maker Range : 0 to %d\n',scale);
fprintf(fid, '//  Number of random number : %d\n',n);
fprintf(fid, 'const unsigned int table[] = {\n');
for i=1:n
    fprintf(fid, '%d, ',a(i));
    if mod(i,10)==0 
        fprintf(fid, '\n');
    end
end
fprintf(fid, '}; //end of table');
fclose(fid);