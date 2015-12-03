close all
clear 

dist = input('Enter the p.dl dist of phantom from gantry in cm: ');
uiopen('*.mat');
t = tcpclient('localhost', 1700);
i = 1;
k = length(BS);
ES = abs(ES);
ES = ((ES - min(ES) )/ (max(ES) - min(ES))) * 100;
while (true)
    if t.BytesAvailable && i < k
        data = t.read;
        data = char(data);
        yawData = strread(data);
        yaw = yawData(end);
        X = ['The current yaw is: ', num2str(yaw)];
        disp(X);
        t.write([uint8(num2str(0)) 44  uint8(num2str(POS(i))) 44 uint8(num2str(ES(i))) 44 uint8(num2str(dist)) 13]);
        i = i+1;
        pause(1/16);
    end
end
 