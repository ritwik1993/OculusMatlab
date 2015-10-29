close all
clear 

t = tcpclient('localhost', 1700);

while (true)
    if t.BytesAvailable
        data = t.read;
        data = char(data);
        yawData = strread(data);
        yaw = yawData(end);
        X = ['The current yaw is: ', num2str(yaw)];
        disp(X);
    end
end
    