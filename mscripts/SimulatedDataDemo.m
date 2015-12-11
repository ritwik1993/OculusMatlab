close all
clear 

dist = input('Enter the p.dl dist of phantom from gantry in cm: ');
uiopen('*.mat');
t = tcpclient('localhost', 1700);
%use localhost when the server is on same computer
% otherwise use IP adddress of the server
i = 1;
k = length(BS);
ES = abs(ES);
ES = ((ES - min(ES) )/ (max(ES) - min(ES))) * 100;
while (true)
    if t.BytesAvailable && i < k
        data = t.read;
        data = char(data);
        yawData = strread(data);
        yaw = yawData(end - 2);
        xpos = yawData(end -1);
        ypos = yawData(end);
        X = ['The current state (Yaw, X, Y) is: ', num2str(yaw), ' ', num2str(xpos), ' ', num2str(ypos)];
        disp(X);
        t.write([uint8(num2str(0)) 44  uint8(num2str(POS(i))) 44 uint8(num2str(ES(i))) 44 uint8(num2str(dist)) 13]);
        i = i+1;
        pause(1/16);
    end
end
 
