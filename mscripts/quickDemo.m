close all
clear 

t = tcpclient('localhost', 1700);
i = 0;
k = 1;
while (true)
    if t.BytesAvailable
        data = t.read;
        data = char(data);
        yawData = strread(data,'%f','delimiter',';');
        yaw = yawData(end - 2);
        xpos = yawData(end -1);
        ypos = yawData(end);
        X = ['The current state (Yaw, X, Y) is: ', num2str(yaw), ' ', num2str(xpos), ' ', num2str(ypos)];
        disp(X);
        t.write([uint8(num2str(yaw)) 44  uint8(num2str(0)) 44 uint8(num2str(i)) 44 uint8(num2str(0)) 13]);
        i = i+k;
        if i == 100
            k = -1;
        end
        if i == 0
            k = 1;
        end
    end
end
    