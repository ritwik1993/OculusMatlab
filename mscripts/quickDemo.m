close all
clear 

t = tcpclient('localhost', 1700);
i = 0;
k = 1;
while (true)
    if t.BytesAvailable
        data = t.read;
        data = char(data);
        yawData = strread(data);
        yaw = yawData(end);
        X = ['The current yaw is: ', num2str(yaw)];
        disp(X);
        t.write([uint8(num2str(yaw)) 10]);
        i = i+k;
        if i == 100
            k = -1;
        end
        if i == 0
            k = 1;
        end
    end
end
    