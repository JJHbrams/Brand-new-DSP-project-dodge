clc
scale=50; % �����ϰ� ǥ���� �ִ��� ��
n=300; % ������ ���� ����
a=round(rand(n,1)*(2*scale))-scale;

fid = fopen('C:\Users\MrJohd\Desktop\Task\4�г�\�ֽ�DSP\Project\Dodge\random_table.h','w');
fprintf(fid, '//convert by �泲�� ������\n');
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